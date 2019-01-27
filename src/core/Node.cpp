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
#include "Action.hpp"
#include "Factory.h"
#include "Scene.h"
#include "Task.h"
#include "render.h"
#include "logs.h"

namespace easy2d
{
	namespace
	{
		float default_anchor_x = 0.f;
		float default_anchor_y = 0.f;
	}

	void Node::SetDefaultAnchor(float anchor_x, float anchor_y)
	{
		default_anchor_x = anchor_x;
		default_anchor_y = anchor_y;
	}

	Node::Node()
		: visible_(true)
		, pause_(false)
		, hover_(false)
		, pressed_(false)
		, responsible_(false)
		, dirty_transform_(false)
		, dirty_transform_inverse_(false)
		, parent_(nullptr)
		, hash_name_(0)
		, z_order_(0)
		, opacity_(1.f)
		, display_opacity_(1.f)
		, anchor_(default_anchor_x, default_anchor_y)
	{
	}

	void Node::Update(Duration dt)
	{
		if (pause_)
			return;

		OnUpdate(dt);
		UpdateActions(this, dt);
		UpdateTasks(dt);

		if (!children_.IsEmpty())
		{
			NodePtr next;
			for (auto child = children_.First(); child; child = next)
			{
				next = child->NextItem();
				child->Update(dt);
			}
		}
	}

	void Node::Render()
	{
		if (!visible_)
			return;

		UpdateTransform();

		auto rt = RenderSystem::Instance();

		if (children_.IsEmpty())
		{
			rt->SetTransform(transform_matrix_);
			rt->SetOpacity(display_opacity_);

			OnRender();
		}
		else
		{
			// render children those are less than 0 in Z-Order
			Node* child = children_.First().Get();
			while (child)
			{
				if (child->GetZOrder() >= 0)
					break;

				child->Render();
				child = child->NextItem().Get();
			}

			rt->SetTransform(transform_matrix_);
			rt->SetOpacity(display_opacity_);

			OnRender();

			while (child)
			{
				child->Render();
				child = child->NextItem().Get();
			}
		}
	}

	void Node::Dispatch(Event& evt)
	{
		if (!visible_)
			return;

		NodePtr prev;
		for (auto child = children_.Last(); child; child = prev)
		{
			prev = child->PrevItem();
			child->Dispatch(evt);
		}

		if (responsible_ && MouseEvent::Check(evt.type))
		{
			if (evt.type == MouseEvent::Move)
			{
				if (!evt.target && ContainsPoint(Point{ evt.mouse.x, evt.mouse.y }))
				{
					evt.target = this;

					if (!hover_)
					{
						hover_ = true;

						Event hover = evt;
						hover.type = MouseEvent::Hover;
						EventDispatcher::Dispatch(hover);
					}
				}
				else if (hover_)
				{
					hover_ = false;
					pressed_ = false;

					Event out = evt;
					out.target = this;
					out.type = MouseEvent::Out;
					EventDispatcher::Dispatch(out);
				}
			}

			if (evt.type == MouseEvent::Down && hover_)
			{
				pressed_ = true;
				evt.target = this;
			}

			if (evt.type == MouseEvent::Up && pressed_)
			{
				pressed_ = false;
				evt.target = this;

				Event click = evt;
				click.type = MouseEvent::Click;
				EventDispatcher::Dispatch(click);
			}
		}

		EventDispatcher::Dispatch(evt);
	}

	void Node::PauseUpdating()
	{
		pause_ = true;
	}

	void Node::ResumeUpdating()
	{
		pause_ = false;
	}

	Matrix const & Node::GetTransformMatrix()  const
	{
		UpdateTransform();
		return transform_matrix_;
	}

	Matrix const & Node::GetTransformInverseMatrix()  const
	{
		UpdateTransform();
		if (dirty_transform_inverse_)
		{
			transform_matrix_inverse_ = Matrix::Invert(transform_matrix_);
			dirty_transform_inverse_ = false;
		}
		return transform_matrix_inverse_;
	}

	Node* Node::GetParent() const
	{
		return parent_;
	}

	Scene* Node::GetScene() const
	{
		return scene_;
	}

	void Node::UpdateTransform() const
	{
		if (!dirty_transform_)
			return;

		dirty_transform_ = false;
		dirty_transform_inverse_ = true;

		// matrix multiplication is optimized by expression template
		transform_matrix_ = Matrix::Scaling(transform_.scale)
			* Matrix::Skewing(transform_.skew.x, transform_.skew.y)
			* Matrix::Rotation(transform_.rotation)
			* Matrix::Translation(transform_.position);

		Point offset{ -size_.x * anchor_.x, -size_.y * anchor_.y };
		transform_matrix_.Translate(offset);

		if (parent_)
			transform_matrix_ = transform_matrix_ * parent_->transform_matrix_;

		// update children's transform
		for (Node* child = children_.First().Get(); child; child = child->NextItem().Get())
			child->dirty_transform_ = true;
	}

	void Node::UpdateOpacity()
	{
		if (parent_)
		{
			display_opacity_ = opacity_ * parent_->display_opacity_;
		}
		for (Node* child = children_.First().Get(); child; child = child->NextItem().Get())
		{
			child->UpdateOpacity();
		}
	}

	void Node::SetScene(Scene * scene)
	{
		scene_ = scene;
		for (Node* child = children_.First().Get(); child; child = child->NextItem().Get())
		{
			child->scene_ = scene;
		}
	}

	void Node::SetZOrder(int zorder)
	{
		if (z_order_ == zorder)
			return;

		z_order_ = zorder;

		if (parent_)
		{
			NodePtr me = this;

			parent_->children_.Remove(me);

			Node* sibling = parent_->children_.Last().Get();

			if (sibling && sibling->GetZOrder() > zorder)
			{
				while (sibling = sibling->PrevItem().Get())
				{
					if (sibling->GetZOrder() <= zorder)
						break;
				}
			}

			if (sibling)
			{
				parent_->children_.InsertAfter(me, NodePtr(sibling));
			}
			else
			{
				parent_->children_.PushFront(me);
			}
		}
	}

	void Node::SetOpacity(float opacity)
	{
		if (opacity_ == opacity)
			return;

		display_opacity_ = opacity_ = std::min(std::max(opacity, 0.f), 1.f);
		UpdateOpacity();
	}

	void Node::SetAnchorX(float anchor_x)
	{
		this->SetAnchor(anchor_x, anchor_.y);
	}

	void Node::SetAnchorY(float anchor_y)
	{
		this->SetAnchor(anchor_.x, anchor_y);
	}

	void Node::SetAnchor(float anchor_x, float anchor_y)
	{
		if (anchor_.x == anchor_x && anchor_.y == anchor_y)
			return;

		anchor_.x = anchor_x;
		anchor_.y = anchor_y;
		dirty_transform_ = true;
	}

	void Node::SetWidth(float width)
	{
		this->SetSize(width, size_.y);
	}

	void Node::SetHeight(float height)
	{
		this->SetSize(size_.x, height);
	}

	void Node::SetSize(const Size& size)
	{
		this->SetSize(size.x, size.y);
	}

	void Node::SetSize(float width, float height)
	{
		if (size_.x == width && size_.y == height)
			return;

		size_.x = width;
		size_.y = height;
		dirty_transform_ = true;
	}

	void Node::SetTransform(Transform const& transform)
	{
		transform_ = transform;
		dirty_transform_ = true;
	}

	void Node::SetVisible(bool val)
	{
		visible_ = val;
	}

	void Node::SetName(String const& name)
	{
		if (name_ != name)
		{
			name_ = name;
			hash_name_ = std::hash<String>{}(name);
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

	void Node::Move(float x, float y)
	{
		this->SetPosition(transform_.position.x + x, transform_.position.y + y);
	}

	void Node::Move(const Point & v)
	{
		this->Move(v.x, v.y);
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

	void Node::AddChild(NodePtr const& child)
	{
		E2D_ASSERT(child && "Node::AddChild failed, NULL pointer exception");

		if (child)
		{
#ifdef E2D_DEBUG

			if (child->parent_)
				logs::Errorln(L"The node to be added already has a parent");

			for (Node* parent = parent_; parent; parent = parent->parent_)
				if (parent == child)
					logs::Errorln(L"A node cannot be its own parent");

#endif // E2D_DEBUG

			children_.PushBack(child);
			child->parent_ = this;
			child->SetScene(this->scene_);
			child->dirty_transform_ = true;
			child->UpdateOpacity();
			child->SetZOrder(child->GetZOrder());
		}
	}

	void Node::AddChildren(Array<NodePtr> const& children)
	{
		for (const auto& node : children)
		{
			this->AddChild(node);
		}
	}

	Rect Node::GetBounds() const
	{
		return Rect(Point{}, size_);
	}

	Rect Node::GetBoundingBox() const
	{
		return RectApplyTransform(GetBounds(), GetTransformMatrix());
	}

	Array<NodePtr> Node::GetChildren(String const& name) const
	{
		Array<NodePtr> children;
		size_t hash_code = std::hash<String>{}(name);

		for (Node* child = children_.First().Get(); child; child = child->NextItem().Get())
		{
			if (child->hash_name_ == hash_code && child->name_ == name)
			{
				children.push_back(child);
			}
		}
		return children;
	}

	NodePtr Node::GetChild(String const& name) const
	{
		size_t hash_code = std::hash<String>{}(name);

		for (Node* child = children_.First().Get(); child; child = child->NextItem().Get())
		{
			if (child->hash_name_ == hash_code && child->name_ == name)
			{
				return child;
			}
		}
		return nullptr;
	}

	Node::Children const & Node::GetChildren() const
	{
		return children_;
	}

	void Node::RemoveFromParent()
	{
		if (parent_)
		{
			parent_->RemoveChild(this);
		}
	}

	bool Node::RemoveChild(NodePtr const& child)
	{
		return RemoveChild(child.Get());
	}

	bool Node::RemoveChild(Node * child)
	{
		E2D_ASSERT(child && "Node::RemoveChild failed, NULL pointer exception");

		if (children_.IsEmpty())
			return false;

		if (child)
		{
			child->parent_ = nullptr;
			if (child->scene_) child->SetScene(nullptr);
			children_.Remove(NodePtr(child));
			return true;
		}
		return false;
	}

	void Node::RemoveChildren(String const& child_name)
	{
		if (children_.IsEmpty())
		{
			return;
		}

		size_t hash_code = std::hash<String>{}(child_name);

		Node* next;
		for (Node* child = children_.First().Get(); child; child = next)
		{
			next = child->NextItem().Get();

			if (child->hash_name_ == hash_code && child->name_ == child_name)
			{
				RemoveChild(child);
			}
		}
	}

	void Node::RemoveAllChildren()
	{
		children_.Clear();
	}

	void Node::SetResponsible(bool enable)
	{
		responsible_ = enable;
	}

	bool Node::ContainsPoint(const Point& point) const
	{
		if (size_.x == 0.f || size_.y == 0.f)
			return false;

		Point local = GetTransformInverseMatrix().Transform(point);
		return GetBounds().ContainsPoint(local);
	}
}
