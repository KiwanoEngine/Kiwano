#include "..\e2dobject.h"
#include "..\e2devent.h"
#include "..\e2dmanager.h"
#include "..\e2daction.h"

const e2d::Node::Property e2d::Node::Property::Origin = { };

e2d::Node::Property e2d::Node::Property::operator+(Property const & prop) const
{
	Property result;
	result.pos = this->pos + prop.pos;
	result.size = this->size + prop.size;
	result.anchor = this->anchor + prop.anchor;
	result.scale = this->scale + prop.scale;
	result.skew = this->skew + prop.skew;
	result.rotation = this->rotation + prop.rotation;
	return std::move(result);
}

e2d::Node::Property e2d::Node::Property::operator-(Property const & prop) const
{
	Property result;
	result.pos = this->pos - prop.pos;
	result.size = this->size - prop.size;
	result.anchor = this->anchor - prop.anchor;
	result.scale = this->scale - prop.scale;
	result.skew = this->skew - prop.skew;
	result.rotation = this->rotation - prop.rotation;
	return std::move(result);
}


e2d::Node::Node()
	: visible_(true)
	, parent_(nullptr)
	, parent_scene_(nullptr)
	, hash_name_(0)
	, clip_enabled_(false)
	, need_sort_(false)
	, need_transform_(false)
	, fixed_position_(false)
	, collider_(this)
	, border_(nullptr)
	, order_(0)
	, pos_()
	, size_()
	, scale_(1.f, 1.f)
	, rotation_(0)
	, skew_(0, 0)
	, display_opacity_(1.f)
	, real_opacity_(1.f)
	, anchor_()
	, children_()
	, actions_()
	, tasks_()
	, initial_matrix_(D2D1::Matrix3x2F::Identity())
	, final_matrix_(D2D1::Matrix3x2F::Identity())
	, border_color_(Color::Red, 0.6f)
	, extrapolate_(Property::Origin)
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

	if (!Game::GetInstance()->IsPaused())
	{
		UpdateActions();
		UpdateTasks();

		auto updatableNode = dynamic_cast<Updatable*>(this);
		if (updatableNode)
		{
			updatableNode->Update();
		}
	}

	UpdateTransform();
	extrapolate_ = this->GetProperty();

	auto render_target = Renderer::GetInstance()->GetRenderTarget();
	if (clip_enabled_)
	{
		render_target->SetTransform(final_matrix_);
		render_target->PushAxisAlignedClip(
			D2D1::RectF(0, 0, size_.width, size_.height),
			D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
		);
	}

	if (children_.empty())
	{
		auto drawableNode = dynamic_cast<Drawable*>(this);
		if (drawableNode)
		{
			render_target->SetTransform(final_matrix_);
			drawableNode->Draw();
		}
	}
	else
	{
		// 子节点排序
		SortChildren();

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
		
		auto drawableNode = dynamic_cast<Drawable*>(this);
		if (drawableNode)
		{
			render_target->SetTransform(final_matrix_);
			drawableNode->Draw();
		}

		// 访问剩余节点
		for (; i < children_.size(); ++i)
			children_[i]->Visit();
	}

	if (clip_enabled_)
	{
		render_target->PopAxisAlignedClip();
	}
}

void e2d::Node::DrawBorder()
{
	if (visible_)
	{
		if (border_)
		{
			auto renderer = Renderer::GetInstance();
			auto brush = renderer->GetSolidBrush();
			brush->SetColor(D2D1_COLOR_F(border_color_));
			renderer->GetRenderTarget()->DrawGeometry(
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

void e2d::Node::DrawCollider()
{
	if (visible_)
	{
		collider_.Draw();

		for (const auto& child : children_)
		{
			child->DrawCollider();
		}
	}
}

void e2d::Node::UpdateTransform()
{
	if (!need_transform_)
		return;

	need_transform_ = false;

	// 计算锚点坐标
	D2D1_POINT_2F anchor = { size_.width * anchor_.x, size_.height * anchor_.y };
	// 变换 Initial 矩阵，子节点将根据这个矩阵进行变换
	initial_matrix_ = D2D1::Matrix3x2F::Scale(
		scale_.x,
		scale_.y,
		anchor
	) * D2D1::Matrix3x2F::Skew(
		skew_.x,
		skew_.y,
		anchor
	) * D2D1::Matrix3x2F::Rotation(
		rotation_,
		anchor
	) * D2D1::Matrix3x2F::Translation(
		pos_.x,
		pos_.y
	);
	// 根据自身锚点变换 Final 矩阵
	final_matrix_ = initial_matrix_ * D2D1::Matrix3x2F::Translation(-anchor.x, -anchor.y);
	// 和父节点矩阵相乘
	if (!fixed_position_ && parent_)
	{
		initial_matrix_ = initial_matrix_ * parent_->initial_matrix_;
		final_matrix_ = final_matrix_ * parent_->initial_matrix_;
	}

	// 重新构造轮廓
	SafeRelease(border_);

	ID2D1Factory * factory = Renderer::GetFactory();
	ID2D1RectangleGeometry * rectangle = nullptr;
	ID2D1TransformedGeometry * transformed = nullptr;
	ThrowIfFailed(
		factory->CreateRectangleGeometry(
			D2D1::RectF(0, 0, size_.width, size_.height),
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
		child->need_transform_ = true;
	}

	// 更新碰撞体
	collider_.Recreate();

	if (collider_.IsEnabled() &&
		collider_.IsCollisionNotify() &&
		collider_.GetShape() != Collider::Shape::None)
	{
		CollisionManager::GetInstance()->UpdateCollider(&collider_);
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

void e2d::Node::SortChildren()
{
	if (need_sort_)
	{
		std::sort(
			std::begin(children_),
			std::end(children_),
			[](Node * n1, Node * n2) { return n1->GetOrder() < n2->GetOrder(); }
		);

		need_sort_ = false;
	}
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

float e2d::Node::GetPosX() const
{
	return pos_.x;
}

float e2d::Node::GetPosY() const
{
	return pos_.y;
}

const e2d::Point& e2d::Node::GetPos() const
{
	return pos_;
}

float e2d::Node::GetWidth() const
{
	return size_.width * scale_.x;
}

float e2d::Node::GetHeight() const
{
	return size_.height * scale_.y;
}

float e2d::Node::GetRealWidth() const
{
	return size_.width;
}

float e2d::Node::GetRealHeight() const
{
	return size_.height;
}

const e2d::Size& e2d::Node::GetRealSize() const
{
	return size_;
}

float e2d::Node::GetAnchorX() const
{
	return anchor_.x;
}

float e2d::Node::GetAnchorY() const
{
	return anchor_.y;
}

e2d::Size e2d::Node::GetSize() const
{
	return Size(GetWidth(), GetHeight());
}

float e2d::Node::GetScaleX() const
{
	return scale_.x;
}

float e2d::Node::GetScaleY() const
{
	return scale_.y;
}

float e2d::Node::GetSkewX() const
{
	return skew_.x;
}

float e2d::Node::GetSkewY() const
{
	return skew_.y;
}

float e2d::Node::GetRotation() const
{
	return rotation_;
}

float e2d::Node::GetOpacity() const
{
	return real_opacity_;
}

e2d::Node::Property e2d::Node::GetProperty() const
{
	Property prop;
	prop.pos = pos_;
	prop.size = size_;
	prop.anchor = anchor_;
	prop.scale = scale_;
	prop.rotation = rotation_;
	prop.skew = skew_;
	return std::move(prop);
}

e2d::Node::Property e2d::Node::GetExtrapolate() const
{
	return this->GetProperty() - extrapolate_;
}

e2d::Collider* e2d::Node::GetCollider()
{
	return &collider_;
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
		parent_->need_sort_ = true;
	}
}

void e2d::Node::SetPosX(float x)
{
	this->SetPos(x, pos_.y);
}

void e2d::Node::SetPosY(float y)
{
	this->SetPos(pos_.x, y);
}

void e2d::Node::SetPos(const Point & p)
{
	this->SetPos(p.x, p.y);
}

void e2d::Node::SetPos(float x, float y)
{
	if (pos_.x == x && pos_.y == y)
		return;

	pos_.x = x;
	pos_.y = y;
	need_transform_ = true;
}

void e2d::Node::SetPosFixed(bool fixed)
{
	if (fixed_position_ == fixed)
		return;

	fixed_position_ = fixed;
	need_transform_ = true;
}

void e2d::Node::Move(float x, float y)
{
	this->SetPos(pos_.x + x, pos_.y + y);
}

void e2d::Node::Move(const Point & v)
{
	this->Move(v.x, v.y);
}

void e2d::Node::SetScaleX(float scale_x)
{
	this->SetScale(scale_x, scale_.y);
}

void e2d::Node::SetScaleY(float scale_y)
{
	this->SetScale(scale_.x, scale_y);
}

void e2d::Node::SetScale(float scale)
{
	this->SetScale(scale, scale);
}

void e2d::Node::SetScale(float scale_x, float scale_y)
{
	if (scale_.x == scale_x && scale_.y == scale_y)
		return;

	scale_.x = scale_x;
	scale_.y = scale_y;
	need_transform_ = true;
}

void e2d::Node::SetSkewX(float skew_x)
{
	this->SetSkew(skew_x, skew_.y);
}

void e2d::Node::SetSkewY(float skew_y)
{
	this->SetSkew(skew_.x, skew_y);
}

void e2d::Node::SetSkew(float skew_x, float skew_y)
{
	if (skew_.x == skew_x && skew_.y == skew_y)
		return;

	skew_.x = skew_x;
	skew_.y = skew_y;
	need_transform_ = true;
}

void e2d::Node::SetRotation(float angle)
{
	if (rotation_ == angle)
		return;

	rotation_ = angle;
	need_transform_ = true;
}

void e2d::Node::SetOpacity(float opacity)
{
	if (real_opacity_ == opacity)
		return;

	display_opacity_ = real_opacity_ = std::min(std::max(opacity, 0.f), 1.f);
	// 更新节点透明度
	UpdateOpacity();
}

void e2d::Node::SetAnchorX(float anchor_x)
{
	this->SetAnchor(anchor_x, anchor_.y);
}

void e2d::Node::SetAnchorY(float anchor_y)
{
	this->SetAnchor(anchor_.x, anchor_y);
}

void e2d::Node::SetAnchor(float anchor_x, float anchor_y)
{
	if (anchor_.x == anchor_x && anchor_.y == anchor_y)
		return;

	anchor_.x = std::min(std::max(anchor_x, 0.f), 1.f);
	anchor_.y = std::min(std::max(anchor_y, 0.f), 1.f);
	need_transform_ = true;
}

void e2d::Node::SetWidth(float width)
{
	this->SetSize(width, size_.height);
}

void e2d::Node::SetHeight(float height)
{
	this->SetSize(size_.width, height);
}

void e2d::Node::SetSize(float width, float height)
{
	if (size_.width == width && size_.height == height)
		return;

	size_.width = width;
	size_.height = height;
	need_transform_ = true;
}

void e2d::Node::SetSize(Size size)
{
	this->SetSize(size.width, size.height);
}

void e2d::Node::SetProperty(Property prop)
{
	this->SetPos(prop.pos.x, prop.pos.y);
	this->SetSize(prop.size.width, prop.size.height);
	this->SetAnchor(prop.anchor.x, prop.anchor.y);
	this->SetScale(prop.scale.x, prop.scale.y);
	this->SetRotation(prop.rotation);
	this->SetSkew(prop.skew.x, prop.skew.y);
}

void e2d::Node::SetClipEnabled(bool enabled)
{
	clip_enabled_ = enabled;
}

void e2d::Node::SetBorderColor(const Color & color)
{
	border_color_ = color;
}

void e2d::Node::AddChild(Node * child, int order  /* = 0 */)
{
	WARN_IF(child == nullptr, "Node::AddChild NULL pointer exception.");

	if (child)
	{
		if (child->parent_ != nullptr)
		{
			throw Exception("节点已有父节点, 不能再添加到其他节点");
		}

		for (Node * parent = this; parent != nullptr; parent = parent->GetParent())
		{
			if (child == parent)
			{
				throw Exception("一个节点不能同时是另一个节点的父节点和子节点");
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
		child->need_transform_ = true;
		// 更新子节点排序
		need_sort_ = true;
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
	WARN_IF(child_name.IsEmpty(), "Invalid Node name.");

	if (children_.empty())
	{
		return;
	}

	// 计算名称 Hash 值
	size_t hash = child_name.GetHash();

	auto iter = std::find_if(
		children_.begin(),
		children_.end(),
		[child_name, hash](Node* child) ->bool { return child->hash_name_ == hash && child->name_ == child_name; }
	);

	if (iter != children_.end())
	{
		(*iter)->parent_ = nullptr;
		if ((*iter)->parent_scene_)
		{
			(*iter)->SetParentScene(nullptr);
		}
		(*iter)->Release();
		children_.erase(iter);
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
			throw Exception("该 Action 已有执行目标");
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
	if (size_.width == 0.f || size_.height == 0.f)
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
	if (size_.width == 0.f || size_.height == 0.f || node->size_.width == 0.f || node->size_.height == 0.f)
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