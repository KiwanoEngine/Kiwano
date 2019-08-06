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
#include <iterator>


easy2d::Node::Node()
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

easy2d::Node::~Node()
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

void easy2d::Node::Visit()
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
		OnDraw();
	}
	else
	{
		// �ӽڵ�����
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
			// ���� Order С����Ľڵ�
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
		OnDraw();

		// ����ʣ��ڵ�
		for (; i < children_.size(); ++i)
			children_[i]->Visit();
	}

	if (clip_enabled_)
	{
		render_target->PopAxisAlignedClip();
	}
}

void easy2d::Node::UpdateChildren(float dt)
{
	if (children_.empty())
	{
		OnUpdate(dt);
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
			// ���� Order С����Ľڵ�
			if (child->GetOrder() < 0)
			{
				child->UpdateChildren(dt);
			}
			else
			{
				break;
			}
		}

		OnUpdate(dt);
		UpdateActions();
		UpdateTasks();
		UpdateTransform();

		// ����ʣ��ڵ�
		for (; i < children_.size(); ++i)
			children_[i]->UpdateChildren(dt);
	}
}

void easy2d::Node::DrawBorder()
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

void easy2d::Node::UpdateTransform()
{
	if (!dirty_transform_)
		return;

	dirty_transform_ = false;

	final_matrix_ = static_cast<D2D1::Matrix3x2F>(transform_);

	// ��������֧����� Initial �����ӽڵ㽫�������������б任
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

	// ���¹�������
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

	// ֪ͨ�ӽڵ����ת��
	for (const auto& child : children_)
	{
		child->dirty_transform_ = true;
	}
}

bool easy2d::Node::Dispatch(const MouseEvent & e, bool handled)
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

bool easy2d::Node::Dispatch(const KeyEvent & e, bool handled)
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

void easy2d::Node::UpdateOpacity()
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

void easy2d::Node::UpdateActions()
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

	// ���������������еĶ���
	for (const auto& action : currActions)
		action->Update();

	// �����ɵĶ���
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

bool easy2d::Node::IsVisible() const
{
	return visible_;
}

const std::wstring& easy2d::Node::GetName() const
{
	return name_;
}

size_t easy2d::Node::GetHashName() const
{
	return hash_name_;
}

const easy2d::Point& easy2d::Node::GetPosition() const
{
	return transform_.position;
}

float easy2d::Node::GetWidth() const
{
	return transform_.size.width * transform_.scale_x;
}

float easy2d::Node::GetHeight() const
{
	return transform_.size.height * transform_.scale_y;
}

float easy2d::Node::GetRealWidth() const
{
	return transform_.size.width;
}

float easy2d::Node::GetRealHeight() const
{
	return transform_.size.height;
}

const easy2d::Size& easy2d::Node::GetRealSize() const
{
	return transform_.size;
}

float easy2d::Node::GetPivotX() const
{
	return transform_.pivot_x;
}

float easy2d::Node::GetPivotY() const
{
	return transform_.pivot_y;
}

easy2d::Size easy2d::Node::GetSize() const
{
	return std::move(Size(GetWidth(), GetHeight()));
}

float easy2d::Node::GetScaleX() const
{
	return transform_.scale_x;
}

float easy2d::Node::GetScaleY() const
{
	return transform_.scale_y;
}

float easy2d::Node::GetSkewX() const
{
	return transform_.skew_x;
}

float easy2d::Node::GetSkewY() const
{
	return transform_.skew_y;
}

float easy2d::Node::GetRotation() const
{
	return transform_.rotation;
}

const easy2d::Transform & easy2d::Node::GetTransform() const
{
	return transform_;
}

float easy2d::Node::GetOpacity() const
{
	return real_opacity_;
}

float easy2d::Node::GetDisplayOpacity() const
{
	return display_opacity_;
}

int easy2d::Node::GetOrder() const
{
	return order_;
}

void easy2d::Node::SetOrder(int order)
{
	if (order_ == order)
		return;

	order_ = order;
	if (parent_)
	{
		parent_->dirty_sort_ = true;
	}
}

void easy2d::Node::SetPositionX(float x)
{
	this->SetPosition(x, transform_.position.y);
}

void easy2d::Node::SetPositionY(float y)
{
	this->SetPosition(transform_.position.x, y);
}

void easy2d::Node::SetPosition(const Point & p)
{
	this->SetPosition(p.x, p.y);
}

void easy2d::Node::SetPosition(float x, float y)
{
	if (transform_.position.x == x && transform_.position.y == y)
		return;

	transform_.position.x = x;
	transform_.position.y = y;
	dirty_transform_ = true;
}

void easy2d::Node::MoveBy(float x, float y)
{
	this->SetPosition(transform_.position.x + x, transform_.position.y + y);
}

void easy2d::Node::MoveBy(const Point & v)
{
	this->MoveBy(v.x, v.y);
}

void easy2d::Node::SetScaleX(float scale_x)
{
	this->SetScale(scale_x, transform_.scale_y);
}

void easy2d::Node::SetScaleY(float scale_y)
{
	this->SetScale(transform_.scale_x, scale_y);
}

void easy2d::Node::SetScale(float scale)
{
	this->SetScale(scale, scale);
}

void easy2d::Node::SetScale(float scale_x, float scale_y)
{
	if (transform_.scale_x == scale_x && transform_.scale_y == scale_y)
		return;

	transform_.scale_x = scale_x;
	transform_.scale_y = scale_y;
	dirty_transform_ = true;
}

void easy2d::Node::SetSkewX(float skew_x)
{
	this->SetSkew(skew_x, transform_.skew_y);
}

void easy2d::Node::SetSkewY(float skew_y)
{
	this->SetSkew(transform_.skew_x, skew_y);
}

void easy2d::Node::SetSkew(float skew_x, float skew_y)
{
	if (transform_.skew_x == skew_x && transform_.skew_y == skew_y)
		return;

	transform_.skew_x = skew_x;
	transform_.skew_y = skew_y;
	dirty_transform_ = true;
}

void easy2d::Node::SetRotation(float angle)
{
	if (transform_.rotation == angle)
		return;

	transform_.rotation = angle;
	dirty_transform_ = true;
}

void easy2d::Node::SetOpacity(float opacity)
{
	if (real_opacity_ == opacity)
		return;

	display_opacity_ = real_opacity_ = std::min(std::max(opacity, 0.f), 1.f);
	// ���½ڵ�͸����
	UpdateOpacity();
}

void easy2d::Node::SetPivotX(float pivot_x)
{
	this->SetPivot(pivot_x, transform_.pivot_y);
}

void easy2d::Node::SetPivotY(float pivot_y)
{
	this->SetPivot(transform_.pivot_x, pivot_y);
}

void easy2d::Node::SetPivot(float pivot_x, float pivot_y)
{
	if (transform_.pivot_x == pivot_x && transform_.pivot_y == pivot_y)
		return;

	transform_.pivot_x = pivot_x;
	transform_.pivot_y = pivot_y;
	dirty_transform_ = true;
}

void easy2d::Node::SetWidth(float width)
{
	this->SetSize(width, transform_.size.height);
}

void easy2d::Node::SetHeight(float height)
{
	this->SetSize(transform_.size.width, height);
}

void easy2d::Node::SetSize(float width, float height)
{
	if (transform_.size.width == width && transform_.size.height == height)
		return;

	transform_.size.width = width;
	transform_.size.height = height;
	dirty_transform_ = true;
}

void easy2d::Node::SetSize(const Size& size)
{
	this->SetSize(size.width, size.height);
}

void easy2d::Node::SetTransform(const Transform & transform)
{
	transform_ = transform;
	dirty_transform_ = true;
}

void easy2d::Node::SetClipEnabled(bool enabled)
{
	clip_enabled_ = enabled;
}

void easy2d::Node::SetBorderColor(const Color & color)
{
	border_color_ = color;
}

void easy2d::Node::AddChild(Node * child, int order)
{
	E2D_WARNING_IF(child == nullptr, "Node::AddChild NULL pointer exception.");

	if (child)
	{
		if (child->parent_ != nullptr)
		{
			throw std::runtime_error("�ڵ����и��ڵ�, ��������ӵ������ڵ�");
		}

		for (Node * parent = this; parent != nullptr; parent = parent->GetParent())
		{
			if (child == parent)
			{
				throw std::runtime_error("һ���ڵ㲻��ͬʱ����һ���ڵ�ĸ��ڵ���ӽڵ�");
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

		// �����ӽڵ�͸����
		child->UpdateOpacity();
		// ���½ڵ�ת��
		child->dirty_transform_ = true;
		// �����ӽڵ�����
		dirty_sort_ = true;
	}
}

void easy2d::Node::AddChild(const Nodes& nodes, int order)
{
	for (const auto& node : nodes)
	{
		this->AddChild(node, order);
	}
}

easy2d::Node * easy2d::Node::GetParent() const
{
	return parent_;
}

easy2d::Scene * easy2d::Node::GetParentScene() const
{
	return parent_scene_;
}

easy2d::Node::Nodes easy2d::Node::GetChildren(const std::wstring& name) const
{
	Nodes children;
	size_t hash_code = std::hash<std::wstring>{}(name);

	for (const auto& child : children_)
	{
		// ��ͬ�����ƿ��ܻ�����ͬ�� Hash ֵ�������ȱȽ� Hash �������������ٶ�
		if (child->hash_name_ == hash_code && child->name_ == name)
		{
			children.push_back(child);
		}
	}
	return std::move(children);
}

easy2d::Node * easy2d::Node::GetChild(const std::wstring& name) const
{
	size_t hash_code = std::hash<std::wstring>{}(name);

	for (const auto& child : children_)
	{
		// ��ͬ�����ƿ��ܻ�����ͬ�� Hash ֵ�������ȱȽ� Hash �������������ٶ�
		if (child->hash_name_ == hash_code && child->name_ == name)
		{
			return child;
		}
	}
	return nullptr;
}

const std::vector<easy2d::Node*>& easy2d::Node::GetAllChildren() const
{
	return children_;
}

int easy2d::Node::GetChildrenCount() const
{
	return static_cast<int>(children_.size());
}

void easy2d::Node::RemoveFromParent()
{
	if (parent_)
	{
		parent_->RemoveChild(this);
	}
}

bool easy2d::Node::RemoveChild(Node * child)
{
	E2D_WARNING_IF(child == nullptr, "Node::RemoveChildren NULL pointer exception.");

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

void easy2d::Node::RemoveChildren(const std::wstring& child_name)
{
	if (children_.empty())
	{
		return;
	}

	size_t hash_code = std::hash<std::wstring>{}(child_name);
	for (auto iter = children_.begin(); iter != children_.end();)
	{
		if ((*iter)->hash_name_ == hash_code && (*iter)->name_ == child_name)
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

void easy2d::Node::RemoveAllChildren()
{
	// ���нڵ�����ü�����һ
	for (const auto& child : children_)
	{
		child->Release();
	}
	// ��մ���ڵ������
	children_.clear();
}

void easy2d::Node::RunAction(Action * action)
{
	E2D_WARNING_IF(action == nullptr, "Action NULL pointer exception!");

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
			throw std::runtime_error("�� Action ����ִ��Ŀ��");
		}
	}
}

void easy2d::Node::ResumeAction(const std::wstring& name)
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

void easy2d::Node::PauseAction(const std::wstring& name)
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

void easy2d::Node::StopAction(const std::wstring& name)
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

bool easy2d::Node::ContainsPoint(const Point& point)
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

bool easy2d::Node::Intersects(Node * node)
{
	if (transform_.size.width == 0.f || transform_.size.height == 0.f || node->transform_.size.width == 0.f || node->transform_.size.height == 0.f)
		return false;

	// ����ת������
	UpdateTransform();
	node->UpdateTransform();

	// ��ȡ�ཻ״̬
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

void easy2d::Node::ResumeAllActions()
{
	if (actions_.empty())
		return;

	for (const auto& action : actions_)
	{
		action->Resume();
	}
}

void easy2d::Node::PauseAllActions()
{
	if (actions_.empty())
		return;

	for (const auto& action : actions_)
	{
		action->Pause();
	}
}

void easy2d::Node::StopAllActions()
{
	if (actions_.empty())
		return;

	for (const auto& action : actions_)
	{
		action->Stop();
	}
}

const easy2d::Node::Actions & easy2d::Node::GetAllActions() const
{
	return actions_;
}

void easy2d::Node::AddTask(Task * task)
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

void easy2d::Node::StopTasks(const std::wstring& name)
{
	for (const auto& task : tasks_)
	{
		if (task->GetName() == name)
		{
			task->Stop();
		}
	}
}

void easy2d::Node::StartTasks(const std::wstring& name)
{
	for (const auto& task : tasks_)
	{
		if (task->GetName() == name)
		{
			task->Start();
		}
	}
}

void easy2d::Node::RemoveTasks(const std::wstring& name)
{
	for (const auto& task : tasks_)
	{
		if (task->GetName() == name)
		{
			task->stopped_ = true;
		}
	}
}

void easy2d::Node::StopAllTasks()
{
	for (const auto& task : tasks_)
	{
		task->Stop();
	}
}

void easy2d::Node::StartAllTasks()
{
	for (const auto& task : tasks_)
	{
		task->Start();
	}
}

void easy2d::Node::RemoveAllTasks()
{
	for (const auto& task : tasks_)
	{
		task->stopped_ = true;
	}
}

const easy2d::Node::Tasks & easy2d::Node::GetAllTasks() const
{
	return tasks_;
}

void easy2d::Node::UpdateTasks()
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

	// ��������������
	for (const auto& task : currTasks)
		task->Update();

	// �������������
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

void easy2d::Node::UpdateTime()
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

void easy2d::Node::SetVisible(bool value)
{
	visible_ = value;
}

void easy2d::Node::SetName(const std::wstring& name)
{
	if (name_ != name)
	{
		// ����ڵ���
		name_ = name;
		// ����ڵ� Hash ��
		hash_name_ = std::hash<std::wstring>{}(name);
	}
}

void easy2d::Node::SetParentScene(Scene * scene)
{
	parent_scene_ = scene;
	for (const auto& child : children_)
	{
		child->SetParentScene(scene);
	}
}