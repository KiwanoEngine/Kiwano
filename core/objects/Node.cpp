// Copyright (c) 2016-2018 Easy2D - Nomango
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "..\e2dobject.h"
#include "..\e2devent.h"
#include "..\e2daction.h"
#include "..\e2dmodule.h"


e2d::Node::Node()
	: visible_(true)
	, parent_(nullptr)
	, parent_scene_(nullptr)
	, hash_name_(0)
	, clip_enabled_(false)
	, dirty_sort_(false)
	, dirty_transform_(false)
	, border_(nullptr)
	, order_(0)
	, transform_()
	, display_opacity_(1.f)
	, real_opacity_(1.f)
	, children_()
	, actions_()
	, tasks_()
	, initial_matrix_(D2D1::Matrix3x2F::Identity())
	, final_matrix_(D2D1::Matrix3x2F::Identity())
	, border_color_(Color::Red, 0.6f)
{
}

e2d::Node::~Node()
{
	SafeRelease(border_);

	for (auto action : actions_)
	{
		SafeRelease(action);
	}

	for (auto task : tasks_)
	{
		SafeRelease(task);
	}

	for (auto child : children_)
	{
		SafeRelease(child);
	}
}

void e2d::Node::Visit()
{
	if (!visible_)
		return;

	auto render_target = Device::GetGraphics()->GetRenderTarget();
	if (clip_enabled_)
	{
		render_target->SetTransform(final_matrix_);
		render_target->PushAxisAlignedClip(
			D2D1::RectF(0, 0, transform_.size.width, transform_.size.height),
			D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
		);
	}

	if (children_.empty())
	{
		render_target->SetTransform(final_matrix_);
		Draw();
	}
	else
	{
		// 子节点排序
		if (dirty_sort_)
		{
			std::sort(
				std::begin(children_),
				std::end(children_),
				[](Node * n1, Node * n2) { return n1->GetOrder() < n2->GetOrder(); }
			);

			dirty_sort_ = false;
		}

		size_t i;
		for (i = 0; i < children_.size(); ++i)
		{
			auto child = children_[i];
			// 访问 Order 小于零的节点
			if (child->GetOrder() < 0)
			{
				child->Visit();
			}
			else
			{
				break;
			}
		}
		
		render_target->SetTransform(final_matrix_);
		Draw();

		// 访问剩余节点
		for (; i < children_.size(); ++i)
			children_[i]->Visit();
	}

	if (clip_enabled_)
	{
		render_target->PopAxisAlignedClip();
	}
}

void e2d::Node::UpdateChildren(float dt)
{
	if (children_.empty())
	{
		Update(dt);
		UpdateActions();
		UpdateTasks();
		UpdateTransform();
	}
	else
	{
		size_t i;
		for (i = 0; i < children_.size(); ++i)
		{
			auto child = children_[i];
			// 访问 Order 小于零的节点
			if (child->GetOrder() < 0)
			{
				child->UpdateChildren(dt);
			}
			else
			{
				break;
			}
		}

		Update(dt);
		UpdateActions();
		UpdateTasks();
		UpdateTransform();

		// 访问剩余节点
		for (; i < children_.size(); ++i)
			children_[i]->UpdateChildren(dt);
	}
}

void e2d::Node::DrawBorder()
{
	if (visible_)
	{
		if (border_)
		{
			auto graphics = Device::GetGraphics();
			auto brush = graphics->GetSolidBrush();
			brush->SetColor(D2D1_COLOR_F(border_color_));
			graphics->GetRenderTarget()->DrawGeometry(
				border_,
				brush,
				1.5f
			);
		}

		for (const auto& child : children_)
		{
			child->DrawBorder();
		}
	}
}

void e2d::Node::UpdateTransform()
{
	if (!dirty_transform_)
		return;

	dirty_transform_ = false;

	final_matrix_ = static_cast<D2D1::Matrix3x2F>(transform_);

	// 根据自身支点计算 Initial 矩阵，子节点将根据这个矩阵进行变换
	auto pivot = Point(
		transform_.size.width * transform_.pivot_x,
		transform_.size.height * transform_.pivot_y
	);
	initial_matrix_ = final_matrix_ * D2D1::Matrix3x2F::Translation(pivot.x, pivot.y);

	if (parent_)
	{
		initial_matrix_ = initial_matrix_ * parent_->initial_matrix_;
		final_matrix_ = final_matrix_ * parent_->initial_matrix_;
	}
	else if (parent_scene_)
	{
		initial_matrix_ = initial_matrix_ * parent_scene_->GetTransform();
		final_matrix_ = final_matrix_ * parent_scene_->GetTransform();
	}

	// 重新构造轮廓
	SafeRelease(border_);

	ID2D1Factory * factory = Device::GetGraphics()->GetFactory();
	ID2D1RectangleGeometry * rectangle = nullptr;
	ID2D1TransformedGeometry * transformed = nullptr;
	ThrowIfFailed(
		factory->CreateRectangleGeometry(
			D2D1::RectF(0, 0, transform_.size.width, transform_.size.height),
			&rectangle
		)
	);
	ThrowIfFailed(
		factory->CreateTransformedGeometry(
			rectangle,
			final_matrix_,
			&transformed
		)
	);
	border_ = transformed;

	SafeRelease(rectangle);

	// 通知子节点进行转换
	for (const auto& child : children_)
	{
		child->dirty_transform_ = true;
	}
}

bool e2d::Node::Dispatch(const MouseEvent & e, bool handled)
{
	if (visible_)
	{
		for (auto riter = children_.crbegin(); riter != children_.crend(); ++riter)
			handled = (*riter)->Dispatch(e, handled);

		auto handler = dynamic_cast<MouseEventHandler*>(this);
		if (handler)
			handler->Handle(e);
	}

	return handled;
}

bool e2d::Node::Dispatch(const KeyEvent & e, bool handled)
{
	if (visible_)
	{
		for (auto riter = children_.crbegin(); riter != children_.crend(); ++riter)
			handled = (*riter)->Dispatch(e, handled);

		auto handler = dynamic_cast<KeyEventHandler*>(this);
		if (handler)
			handler->Handle(e);
	}

	return handled;
}

void e2d::Node::UpdateOpacity()
{
	if (parent_)
	{
		display_opacity_ = real_opacity_ * parent_->display_opacity_;
	}
	for (const auto& child : children_)
	{
		child->UpdateOpacity();
	}
}

void e2d::Node::UpdateActions()
{
	if (actions_.empty())
		return;

	std::vector<Action*> currActions;
	currActions.reserve(actions_.size());
	std::copy_if(
		actions_.begin(),
		actions_.end(),
		std::back_inserter(currActions),
		[](Action* action) { return action->IsRunning() && !action->IsDone(); }
	);

	// 遍历所有正在运行的动作
	for (const auto& action : currActions)
		action->Update();

	// 清除完成的动作
	for (auto iter = actions_.begin(); iter != actions_.end();)
	{
		if ((*iter)->IsDone())
		{
			(*iter)->Release();
			iter = actions_.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

bool e2d::Node::IsVisible() const
{
	return visible_;
}

const e2d::String& e2d::Node::GetName() const
{
	return name_;
}

size_t e2d::Node::GetHashName() const
{
	return hash_name_;
}

const e2d::Point& e2d::Node::GetPosition() const
{
	return transform_.position;
}

float e2d::Node::GetWidth() const
{
	return transform_.size.width * transform_.scale_x;
}

float e2d::Node::GetHeight() const
{
	return transform_.size.height * transform_.scale_y;
}

float e2d::Node::GetRealWidth() const
{
	return transform_.size.width;
}

float e2d::Node::GetRealHeight() const
{
	return transform_.size.height;
}

const e2d::Size& e2d::Node::GetRealSize() const
{
	return transform_.size;
}

float e2d::Node::GetPivotX() const
{
	return transform_.pivot_x;
}

float e2d::Node::GetPivotY() const
{
	return transform_.pivot_y;
}

e2d::Size e2d::Node::GetSize() const
{
	return std::move(Size(GetWidth(), GetHeight()));
}

float e2d::Node::GetScaleX() const
{
	return transform_.scale_x;
}

float e2d::Node::GetScaleY() const
{
	return transform_.scale_y;
}

float e2d::Node::GetSkewX() const
{
	return transform_.skew_x;
}

float e2d::Node::GetSkewY() const
{
	return transform_.skew_y;
}

float e2d::Node::GetRotation() const
{
	return transform_.rotation;
}

const e2d::Transform & e2d::Node::GetTransform() const
{
	return transform_;
}

float e2d::Node::GetOpacity() const
{
	return real_opacity_;
}

int e2d::Node::GetOrder() const
{
	return order_;
}

void e2d::Node::SetOrder(int order)
{
	if (order_ == order)
		return;

	order_ = order;
	if (parent_)
	{
		parent_->dirty_sort_ = true;
	}
}

void e2d::Node::SetPositionX(float x)
{
	this->SetPosition(x, transform_.position.y);
}

void e2d::Node::SetPositionY(float y)
{
	this->SetPosition(transform_.position.x, y);
}

void e2d::Node::SetPosition(const Point & p)
{
	this->SetPosition(p.x, p.y);
}

void e2d::Node::SetPosition(float x, float y)
{
	if (transform_.position.x == x && transform_.position.y == y)
		return;

	transform_.position.x = x;
	transform_.position.y = y;
	dirty_transform_ = true;
}

void e2d::Node::MoveBy(float x, float y)
{
	this->SetPosition(transform_.position.x + x, transform_.position.y + y);
}

void e2d::Node::MoveBy(const Point & v)
{
	this->MoveBy(v.x, v.y);
}

void e2d::Node::SetScaleX(float scale_x)
{
	this->SetScale(scale_x, transform_.scale_y);
}

void e2d::Node::SetScaleY(float scale_y)
{
	this->SetScale(transform_.scale_x, scale_y);
}

void e2d::Node::SetScale(float scale)
{
	this->SetScale(scale, scale);
}

void e2d::Node::SetScale(float scale_x, float scale_y)
{
	if (transform_.scale_x == scale_x && transform_.scale_y == scale_y)
		return;

	transform_.scale_x = scale_x;
	transform_.scale_y = scale_y;
	dirty_transform_ = true;
}

void e2d::Node::SetSkewX(float skew_x)
{
	this->SetSkew(skew_x, transform_.skew_y);
}

void e2d::Node::SetSkewY(float skew_y)
{
	this->SetSkew(transform_.skew_x, skew_y);
}

void e2d::Node::SetSkew(float skew_x, float skew_y)
{
	if (transform_.skew_x == skew_x && transform_.skew_y == skew_y)
		return;

	transform_.skew_x = skew_x;
	transform_.skew_y = skew_y;
	dirty_transform_ = true;
}

void e2d::Node::SetRotation(float angle)
{
	if (transform_.rotation == angle)
		return;

	transform_.rotation = angle;
	dirty_transform_ = true;
}

void e2d::Node::SetOpacity(float opacity)
{
	if (real_opacity_ == opacity)
		return;

	display_opacity_ = real_opacity_ = std::min(std::max(opacity, 0.f), 1.f);
	// 更新节点透明度
	UpdateOpacity();
}

void e2d::Node::SetPivotX(float pivot_x)
{
	this->SetPivot(pivot_x, transform_.pivot_y);
}

void e2d::Node::SetPivotY(float pivot_y)
{
	this->SetPivot(transform_.pivot_x, pivot_y);
}

void e2d::Node::SetPivot(float pivot_x, float pivot_y)
{
	if (transform_.pivot_x == pivot_x && transform_.pivot_y == pivot_y)
		return;

	transform_.pivot_x = pivot_x;
	transform_.pivot_y = pivot_y;
	dirty_transform_ = true;
}

void e2d::Node::SetWidth(float width)
{
	this->SetSize(width, transform_.size.height);
}

void e2d::Node::SetHeight(float height)
{
	this->SetSize(transform_.size.width, height);
}

void e2d::Node::SetSize(float width, float height)
{
	if (transform_.size.width == width && transform_.size.height == height)
		return;

	transform_.size.width = width;
	transform_.size.height = height;
	dirty_transform_ = true;
}

void e2d::Node::SetSize(const Size& size)
{
	this->SetSize(size.width, size.height);
}

void e2d::Node::SetTransform(const Transform & transform)
{
	transform_ = transform;
	dirty_transform_ = true;
}

void e2d::Node::SetClipEnabled(bool enabled)
{
	clip_enabled_ = enabled;
}

void e2d::Node::SetBorderColor(const Color & color)
{
	border_color_ = color;
}

void e2d::Node::AddChild(Node * child, int order)
{
	WARN_IF(child == nullptr, "Node::AddChild NULL pointer exception.");

	if (child)
	{
		if (child->parent_ != nullptr)
		{
			throw RuntimeException("节点已有父节点, 不能再添加到其他节点");
		}

		for (Node * parent = this; parent != nullptr; parent = parent->GetParent())
		{
			if (child == parent)
			{
				throw RuntimeException("一个节点不能同时是另一个节点的父节点和子节点");
			}
		}

		child->Retain();
		children_.push_back(child);
		child->SetOrder(order);
		child->parent_ = this;
		if (this->parent_scene_)
		{
			child->SetParentScene(this->parent_scene_);
		}

		// 更新子节点透明度
		child->UpdateOpacity();
		// 更新节点转换
		child->dirty_transform_ = true;
		// 更新子节点排序
		dirty_sort_ = true;
	}
}

void e2d::Node::AddChild(const Nodes& nodes, int order)
{
	for (const auto& node : nodes)
	{
		this->AddChild(node, order);
	}
}

e2d::Node * e2d::Node::GetParent() const
{
	return parent_;
}

e2d::Scene * e2d::Node::GetParentScene() const
{
	return parent_scene_;
}

e2d::Node::Nodes e2d::Node::GetChildren(const String& name) const
{
	Nodes children;
	size_t hash = name.GetHash();

	for (const auto& child : children_)
	{
		// 不同的名称可能会有相同的 Hash 值，但是先比较 Hash 可以提升搜索速度
		if (child->hash_name_ == hash && child->name_ == name)
		{
			children.push_back(child);
		}
	}
	return std::move(children);
}

e2d::Node * e2d::Node::GetChild(const String& name) const
{
	size_t hash = name.GetHash();

	for (const auto& child : children_)
	{
		// 不同的名称可能会有相同的 Hash 值，但是先比较 Hash 可以提升搜索速度
		if (child->hash_name_ == hash && child->name_ == name)
		{
			return child;
		}
	}
	return nullptr;
}

const std::vector<e2d::Node*>& e2d::Node::GetAllChildren() const
{
	return children_;
}

int e2d::Node::GetChildrenCount() const
{
	return static_cast<int>(children_.size());
}

void e2d::Node::RemoveFromParent()
{
	if (parent_)
	{
		parent_->RemoveChild(this);
	}
}

bool e2d::Node::RemoveChild(Node * child)
{
	WARN_IF(child == nullptr, "Node::RemoveChildren NULL pointer exception.");

	if (children_.empty())
	{
		return false;
	}

	if (child)
	{
		auto iter = std::find(children_.begin(), children_.end(), child);
		if (iter != children_.end())
		{
			children_.erase(iter);
			child->parent_ = nullptr;

			if (child->parent_scene_)
			{
				child->SetParentScene(nullptr);
			}

			child->Release();
			return true;
		}
	}
	return false;
}

void e2d::Node::RemoveChildren(const String& child_name)
{
	if (children_.empty())
	{
		return;
	}

	size_t hash = child_name.GetHash();
	for (auto iter = children_.begin(); iter != children_.end();)
	{
		if ((*iter)->hash_name_ == hash && (*iter)->name_ == child_name)
		{
			(*iter)->parent_ = nullptr;
			if ((*iter)->parent_scene_)
			{
				(*iter)->SetParentScene(nullptr);
			}
			(*iter)->Release();
			iter = children_.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void e2d::Node::RemoveAllChildren()
{
	// 所有节点的引用计数减一
	for (const auto& child : children_)
	{
		child->Release();
	}
	// 清空储存节点的容器
	children_.clear();
}

void e2d::Node::RunAction(Action * action)
{
	WARN_IF(action == nullptr, "Action NULL pointer exception!");

	if (action)
	{
		if (action->GetTarget() == nullptr)
		{
			auto iter = std::find(actions_.begin(), actions_.end(), action);
			if (iter == actions_.end())
			{
				action->Retain();
				action->StartWithTarget(this);
				actions_.push_back(action);
			}
		}
		else
		{
			throw RuntimeException("该 Action 已有执行目标");
		}
	}
}

void e2d::Node::ResumeAction(const String& name)
{
	if (actions_.empty())
		return;

	for (const auto& action : actions_)
	{
		if (action->GetName() == name)
		{
			action->Resume();
		}
	}
}

void e2d::Node::PauseAction(const String& name)
{
	if (actions_.empty())
		return;

	for (const auto& action : actions_)
	{
		if (action->GetName() == name)
		{
			action->Pause();
		}
	}
}

void e2d::Node::StopAction(const String& name)
{
	if (actions_.empty())
		return;

	for (const auto& action : actions_)
	{
		if (action->GetName() == name)
		{
			action->Stop();
		}
	}
}

bool e2d::Node::ContainsPoint(const Point& point)
{
	if (transform_.size.width == 0.f || transform_.size.height == 0.f)
		return false;

	UpdateTransform();

	BOOL ret = 0;
	ThrowIfFailed(
		border_->FillContainsPoint(
			D2D1::Point2F(point.x, point.y),
			D2D1::Matrix3x2F::Identity(),
			&ret
		)
	);
	return ret != 0;
}

bool e2d::Node::Intersects(Node * node)
{
	if (transform_.size.width == 0.f || transform_.size.height == 0.f || node->transform_.size.width == 0.f || node->transform_.size.height == 0.f)
		return false;

	// 更新转换矩阵
	UpdateTransform();
	node->UpdateTransform();

	// 获取相交状态
	D2D1_GEOMETRY_RELATION relation = D2D1_GEOMETRY_RELATION_UNKNOWN;
	ThrowIfFailed(
		border_->CompareWithGeometry(
			node->border_,
			D2D1::Matrix3x2F::Identity(),
			&relation
		)
	);
	return relation != D2D1_GEOMETRY_RELATION_UNKNOWN &&
		relation != D2D1_GEOMETRY_RELATION_DISJOINT;
}

void e2d::Node::ResumeAllActions()
{
	if (actions_.empty())
		return;

	for (const auto& action : actions_)
	{
		action->Resume();
	}
}

void e2d::Node::PauseAllActions()
{
	if (actions_.empty())
		return;

	for (const auto& action : actions_)
	{
		action->Pause();
	}
}

void e2d::Node::StopAllActions()
{
	if (actions_.empty())
		return;

	for (const auto& action : actions_)
	{
		action->Stop();
	}
}

const e2d::Node::Actions & e2d::Node::GetAllActions() const
{
	return actions_;
}

void e2d::Node::AddTask(Task * task)
{
	if (task)
	{
		auto iter = std::find(tasks_.begin(), tasks_.end(), task);
		if (iter == tasks_.end())
		{
			task->Retain();
			task->last_time_ = Time::Now();
			tasks_.push_back(task);
		}
	}
}

void e2d::Node::StopTasks(const String& name)
{
	for (const auto& task : tasks_)
	{
		if (task->GetName() == name)
		{
			task->Stop();
		}
	}
}

void e2d::Node::StartTasks(const String& name)
{
	for (const auto& task : tasks_)
	{
		if (task->GetName() == name)
		{
			task->Start();
		}
	}
}

void e2d::Node::RemoveTasks(const String& name)
{
	for (const auto& task : tasks_)
	{
		if (task->GetName() == name)
		{
			task->stopped_ = true;
		}
	}
}

void e2d::Node::StopAllTasks()
{
	for (const auto& task : tasks_)
	{
		task->Stop();
	}
}

void e2d::Node::StartAllTasks()
{
	for (const auto& task : tasks_)
	{
		task->Start();
	}
}

void e2d::Node::RemoveAllTasks()
{
	for (const auto& task : tasks_)
	{
		task->stopped_ = true;
	}
}

const e2d::Node::Tasks & e2d::Node::GetAllTasks() const
{
	return tasks_;
}

void e2d::Node::UpdateTasks()
{
	if (tasks_.empty())
		return;

	std::vector<Task*> currTasks;
	currTasks.reserve(tasks_.size());
	std::copy_if(
		tasks_.begin(),
		tasks_.end(),
		std::back_inserter(currTasks),
		[](Task* task) { return task->IsReady() && !task->stopped_; }
	);

	// 遍历就绪的任务
	for (const auto& task : currTasks)
		task->Update();

	// 清除结束的任务
	for (auto iter = tasks_.begin(); iter != tasks_.end();)
	{
		if ((*iter)->stopped_)
		{
			(*iter)->Release();
			iter = tasks_.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void e2d::Node::UpdateTime()
{
	for (const auto& action : actions_)
	{
		action->ResetTime();
	}

	for (const auto& task : tasks_)
	{
		task->ResetTime();
	}

	for (const auto& child : children_)
	{
		child->UpdateTime();
	}
}

void e2d::Node::SetVisible(bool value)
{
	visible_ = value;
}

void e2d::Node::SetName(const String& name)
{
	WARN_IF(name.IsEmpty(), "Invalid Node name.");

	if (!name.IsEmpty() && name_ != name)
	{
		// 保存节点名
		name_ = name;
		// 保存节点 Hash 名
		hash_name_ = name.GetHash();
	}
}

void e2d::Node::SetParentScene(Scene * scene)
{
	parent_scene_ = scene;
	for (const auto& child : children_)
	{
		child->SetParentScene(scene);
	}
}