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

#include "Node.h"
#include "Scene.h"
#include "Task.h"
#include "Action.hpp"
#include "render.h"

namespace easy2d
{
	Node::Node()
		: visible_(true)
		, parent_(nullptr)
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
		, initial_matrix_()
		, final_matrix_()
		, border_color_(Color::Red, 0.6f)
	{
	}

	Node::~Node()
	{
		SafeRelease(border_);
	}

	void Node::Visit()
	{
		if (!visible_)
			return;

		auto& graphics = devices::Graphics::Instance();

		if (clip_enabled_)
		{
			graphics.PushClip(final_matrix_, transform_.size);
		}

		if (children_.empty())
		{
			graphics.SetTransform(final_matrix_);
			OnDraw();
		}
		else
		{
			// �ӽڵ�����
			if (dirty_sort_)
			{
				std::sort(
					children_.begin(),
					children_.end(),
					[](spNode const& n1, spNode const& n2) { return n1->GetOrder() < n2->GetOrder(); }
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

			graphics.SetTransform(final_matrix_);
			OnDraw();

			// ����ʣ��ڵ�
			for (; i < children_.size(); ++i)
				children_[i]->Visit();
		}

		if (clip_enabled_)
		{
			graphics.PopClip();
		}
	}

	void Node::Update(Duration const& dt)
	{
		if (children_.empty())
		{
			OnUpdate(dt);
			UpdateActions(this, dt);
			UpdateTasks(dt);
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
					child->Update(dt);
				}
				else
				{
					break;
				}
			}

			OnUpdate(dt);
			UpdateActions(this, dt);
			UpdateTasks(dt);
			UpdateTransform();

			// ����ʣ��ڵ�
			for (; i < children_.size(); ++i)
				children_[i]->Update(dt);
		}
	}

	void Node::DrawBorder()
	{
		if (visible_)
		{
			if (border_)
			{
				devices::Graphics::Instance().DrawGeometry(border_, border_color_, 1.f, 1.5f);
			}

			for (const auto& child : children_)
			{
				child->DrawBorder();
			}
		}
	}

	void Node::UpdateTransform()
	{
		if (!dirty_transform_)
			return;

		dirty_transform_ = false;

		final_matrix_ = transform_.ToMatrix();
		initial_matrix_ = final_matrix_ * math::Matrix::Translation(
			Point{ transform_.size.width * transform_.pivot.x, transform_.size.height * transform_.pivot.y }
		);

		if (parent_)
		{
			initial_matrix_ = initial_matrix_ * parent_->initial_matrix_;
			final_matrix_ = final_matrix_ * parent_->initial_matrix_;
		}

		// ���¹�������
		SafeRelease(border_);
		ThrowIfFailed(
			devices::Graphics::Instance().CreateRectGeometry(final_matrix_, transform_.size, &border_)
		);

		for (auto& child : children_)
		{
			child->dirty_transform_ = true;
		}
	}

	bool Node::Dispatch(const MouseEvent & e, bool handled)
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

	bool Node::Dispatch(const KeyEvent & e, bool handled)
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

	void Node::UpdateOpacity()
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

	bool Node::IsVisible() const
	{
		return visible_;
	}

	const String& Node::GetName() const
	{
		return name_;
	}

	size_t Node::GetHashName() const
	{
		return hash_name_;
	}

	const Point& Node::GetPosition() const
	{
		return transform_.position;
	}

	float Node::GetWidth() const
	{
		return transform_.size.width * transform_.scale.x;
	}

	float Node::GetHeight() const
	{
		return transform_.size.height * transform_.scale.y;
	}

	float Node::GetRealWidth() const
	{
		return transform_.size.width;
	}

	float Node::GetRealHeight() const
	{
		return transform_.size.height;
	}

	const Size& Node::GetRealSize() const
	{
		return transform_.size;
	}

	float Node::GetPivotX() const
	{
		return transform_.pivot.x;
	}

	float Node::GetPivotY() const
	{
		return transform_.pivot.y;
	}

	Size Node::GetSize() const
	{
		return Size{ GetWidth(), GetHeight() };
	}

	float Node::GetScaleX() const
	{
		return transform_.scale.x;
	}

	float Node::GetScaleY() const
	{
		return transform_.scale.y;
	}

	float Node::GetSkewX() const
	{
		return transform_.skew.x;
	}

	float Node::GetSkewY() const
	{
		return transform_.skew.y;
	}

	float Node::GetRotation() const
	{
		return transform_.rotation;
	}

	float Node::GetOpacity() const
	{
		return real_opacity_;
	}

	float Node::GetDisplayOpacity() const
	{
		return display_opacity_;
	}

	int Node::GetOrder() const
	{
		return order_;
	}

	void Node::SetOrder(int order)
	{
		if (order_ == order)
			return;

		order_ = order;
		if (parent_)
		{
			parent_->dirty_sort_ = true;
		}
	}

	void Node::SetPositionX(float x)
	{
		this->SetPosition(x, transform_.position.y);
	}

	void Node::SetPositionY(float y)
	{
		this->SetPosition(transform_.position.x, y);
	}

	void Node::SetPosition(const Point & p)
	{
		this->SetPosition(p.x, p.y);
	}

	void Node::SetPosition(float x, float y)
	{
		if (transform_.position.x == x && transform_.position.y == y)
			return;

		transform_.position.x = x;
		transform_.position.y = y;
		dirty_transform_ = true;
	}

	void Node::MoveBy(float x, float y)
	{
		this->SetPosition(transform_.position.x + x, transform_.position.y + y);
	}

	void Node::MoveBy(const Point & v)
	{
		this->MoveBy(v.x, v.y);
	}

	void Node::SetScaleX(float scale_x)
	{
		this->SetScale(scale_x, transform_.scale.y);
	}

	void Node::SetScaleY(float scale_y)
	{
		this->SetScale(transform_.scale.x, scale_y);
	}

	void Node::SetScale(float scale)
	{
		this->SetScale(scale, scale);
	}

	void Node::SetScale(float scale_x, float scale_y)
	{
		if (transform_.scale.x == scale_x && transform_.scale.y == scale_y)
			return;

		transform_.scale.x = scale_x;
		transform_.scale.y = scale_y;
		dirty_transform_ = true;
	}

	void Node::SetSkewX(float skew_x)
	{
		this->SetSkew(skew_x, transform_.skew.y);
	}

	void Node::SetSkewY(float skew_y)
	{
		this->SetSkew(transform_.skew.x, skew_y);
	}

	void Node::SetSkew(float skew_x, float skew_y)
	{
		if (transform_.skew.x == skew_x && transform_.skew.y == skew_y)
			return;

		transform_.skew.x = skew_x;
		transform_.skew.y = skew_y;
		dirty_transform_ = true;
	}

	void Node::SetRotation(float angle)
	{
		if (transform_.rotation == angle)
			return;

		transform_.rotation = angle;
		dirty_transform_ = true;
	}

	void Node::SetOpacity(float opacity)
	{
		if (real_opacity_ == opacity)
			return;

		display_opacity_ = real_opacity_ = std::min(std::max(opacity, 0.f), 1.f);
		// ���½ڵ�͸����
		UpdateOpacity();
	}

	void Node::SetPivotX(float pivot_x)
	{
		this->SetPivot(pivot_x, transform_.pivot.y);
	}

	void Node::SetPivotY(float pivot_y)
	{
		this->SetPivot(transform_.pivot.x, pivot_y);
	}

	void Node::SetPivot(float pivot_x, float pivot_y)
	{
		if (transform_.pivot.x == pivot_x && transform_.pivot.y == pivot_y)
			return;

		transform_.pivot.x = pivot_x;
		transform_.pivot.y = pivot_y;
		dirty_transform_ = true;
	}

	void Node::SetWidth(float width)
	{
		this->SetSize(width, transform_.size.height);
	}

	void Node::SetHeight(float height)
	{
		this->SetSize(transform_.size.width, height);
	}

	void Node::SetSize(float width, float height)
	{
		if (transform_.size.width == width && transform_.size.height == height)
			return;

		transform_.size.width = width;
		transform_.size.height = height;
		dirty_transform_ = true;
	}

	void Node::SetSize(const Size& size)
	{
		this->SetSize(size.width, size.height);
	}

	math::Transform const& Node::GetTransform() const
	{
		return transform_;
	}

	void Node::SetTransform(math::Transform const& transform)
	{
		transform_ = transform;
		dirty_transform_ = true;
	}

	void Node::SetClipEnabled(bool enabled)
	{
		clip_enabled_ = enabled;
	}

	void Node::SetBorderColor(const Color & color)
	{
		border_color_ = color;
	}

	void Node::AddChild(spNode const& child, int order)
	{
		E2D_WARNING_IF(!child, "Node::AddChild NULL pointer exception.");

		if (child)
		{
			if (child->parent_)
			{
				throw std::runtime_error("�ڵ����и��ڵ�, ��������ӵ������ڵ�");
			}

			for (Node * parent = this; parent; parent = parent->GetParent().Get())
			{
				if (child == parent)
				{
					throw std::runtime_error("һ���ڵ㲻��ͬʱ����һ���ڵ�ĸ��ڵ���ӽڵ�");
				}
			}

			children_.push_back(child);
			child->SetOrder(order);
			child->parent_ = this;

			// �����ӽڵ�͸����
			child->UpdateOpacity();
			// ���½ڵ�ת��
			child->dirty_transform_ = true;
			// �����ӽڵ�����
			dirty_sort_ = true;
		}
	}

	void Node::AddChild(const Nodes& nodes, int order)
	{
		for (const auto& node : nodes)
		{
			this->AddChild(node, order);
		}
	}

	spNode Node::GetParent() const
	{
		return parent_;
	}

	Nodes Node::GetChildren(const String& name) const
	{
		Nodes children;
		size_t hash_code = std::hash<String>{}(name);

		for (const auto& child : children_)
		{
			// ��ͬ�����ƿ��ܻ�����ͬ�� Hash ֵ�������ȱȽ� Hash �������������ٶ�
			if (child->hash_name_ == hash_code && child->name_ == name)
			{
				children.push_back(child);
			}
		}
		return children;
	}

	spNode Node::GetChild(const String& name) const
	{
		size_t hash_code = std::hash<String>{}(name);

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

	const std::vector<spNode>& Node::GetAllChildren() const
	{
		return children_;
	}

	int Node::GetChildrenCount() const
	{
		return static_cast<int>(children_.size());
	}

	void Node::RemoveFromParent()
	{
		if (parent_)
		{
			parent_->RemoveChild(this);
		}
	}

	bool Node::RemoveChild(spNode const& child)
	{
		E2D_WARNING_IF(!child, "Node::RemoveChildren NULL pointer exception.");

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
				return true;
			}
		}
		return false;
	}

	void Node::RemoveChildren(const String& child_name)
	{
		if (children_.empty())
		{
			return;
		}

		size_t hash_code = std::hash<String>{}(child_name);
		for (auto iter = children_.begin(); iter != children_.end();)
		{
			if ((*iter)->hash_name_ == hash_code && (*iter)->name_ == child_name)
			{
				(*iter)->parent_ = nullptr;
				iter = children_.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}

	void Node::RemoveAllChildren()
	{
		children_.clear();
	}

	bool Node::ContainsPoint(const Point& point)
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

	bool Node::Intersects(spNode const& node)
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

	void Node::SetVisible(bool val)
	{
		visible_ = val;
	}

	void Node::SetName(const String& name)
	{
		if (name_ != name)
		{
			// ����ڵ���
			name_ = name;
			// ����ڵ� Hash ��
			hash_name_ = std::hash<String>{}(name);
		}
	}
}