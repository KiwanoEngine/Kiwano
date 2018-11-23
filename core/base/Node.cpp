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
#include "Factory.h"
#include "Scene.h"
#include "Task.h"
#include "Action.hpp"
#include "render.h"
#include "logs.h"

namespace easy2d
{
	namespace
	{
		float default_pivot_x = 0.f;
		float default_pivot_y = 0.f;
		bool border_enabled = false;
	}

	void Node::SetDefaultPivot(float pivot_x, float pivot_y)
	{
		default_pivot_x = pivot_x;
		default_pivot_y = pivot_y;
	}

	void easy2d::Node::EnableBorder()
	{
		border_enabled = true;
	}

	void easy2d::Node::DisableBorder()
	{
		border_enabled = false;
	}

	Node::Node()
		: inited_(false)
		, visible_(true)
		, parent_(nullptr)
		, hash_name_(0)
		, z_order_(0)
		, opacity_(1.f)
		, display_opacity_(1.f)
		, children_()
		, border_color_(Color::Red, 0.6f)
		, initial_matrix_()
		, final_matrix_()
		, pivot_(default_pivot_x, default_pivot_y)
		, size_()
	{
	}

	void Node::Init()
	{
		inited_ = true;
	}

	void Node::OnRender()
	{
		// normal node renders nothing
	}

	void Node::Update(Duration const & dt)
	{
		if (!inited_)
		{
			Init();
		}

		UpdateActions(this, dt);
		UpdateTasks(dt);

		if (!children_.IsEmpty())
		{
			spNode next;
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

		auto graphics = Graphics::Instance();

		if (children_.IsEmpty())
		{
			graphics->SetTransform(final_matrix_);
			graphics->SetOpacity(display_opacity_);

			OnRender();
		}
		else
		{
			// render children those are less than 0 in Z-Order
			spNode child = children_.First();
			for (spNode next; child; child = next)
			{
				next = child->NextItem();
				if (child->GetZOrder() < 0)
				{
					child->Render();
				}
				else
				{
					break;
				}
			}

			graphics->SetTransform(final_matrix_);
			graphics->SetOpacity(display_opacity_);

			OnRender();

			for (spNode next; child; child = next)
			{
				next = child->NextItem();
				child->Render();
			}
		}
	}

	void Node::DispatchEvent(Event * e)
	{
		if (!visible_)
			return;

		this->HandleEvent(e);

		if (!e->has_target)
		{
			EventDispatcher::DispatchEvent(e);
		}
	}

	void Node::HandleEvent(Event * e)
	{
		spNode prev;
		for (auto child = children_.Last(); child; child = prev)
		{
			prev = child->PrevItem();
			child->HandleEvent(e);
		}
	}

	void Node::DrawBorder()
	{
		if (visible_)
		{
			if (border_)
			{
				Graphics::Instance()->DrawGeometry(border_, border_color_, 1.5f);
			}

			for (Node* child = children_.First().Get(); child; child = child->NextItem().Get())
			{
				child->DrawBorder();
			}
		}
	}

	math::Matrix const & Node::GetTransformMatrix()
	{
		UpdateTransform();
		return final_matrix_;
	}

	spNode Node::GetParent() const
	{
		return parent_;
	}

	spScene Node::GetScene() const
	{
		return scene_;
	}

	void Node::UpdateTransform()
	{
		if (!dirty_transform_)
			return;

		dirty_transform_ = false;

		Point center{ size_.width * pivot_.x, size_.height * pivot_.y };
		final_matrix_ = math::Matrix::Scaling(transform_.scale, center)
			* math::Matrix::Skewing(transform_.skew.x, transform_.skew.y, center)
			* math::Matrix::Rotation(transform_.rotation, center)
			* math::Matrix::Translation(transform_.position - center);

		initial_matrix_ = final_matrix_ * math::Matrix::Translation(
			Point{ size_.width * pivot_.x, size_.height * pivot_.y }
		);

		if (parent_)
		{
			initial_matrix_ = initial_matrix_ * parent_->initial_matrix_;
			final_matrix_ = final_matrix_ * parent_->initial_matrix_;
		}

		// update children's transform
		for (Node* child = children_.First().Get(); child; child = child->NextItem().Get())
		{
			child->dirty_transform_ = true;
		}

		// update border
		if (border_enabled)
		{
			UpdateBorder();
		}
	}

	void Node::UpdateBorder()
	{
		cpRectangleGeometry rect;
		cpTransformedGeometry transformed;

		HRESULT hr = Factory::Instance()->CreateRectangleGeometry(
			rect,
			Rect(Point{}, size_)
		);

		if (SUCCEEDED(hr))
		{
			hr = Factory::Instance()->CreateTransformedGeometry(
				transformed,
				final_matrix_,
				rect
			);
		}

		if (SUCCEEDED(hr))
		{
			border_ = transformed;
		}
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
			spNode me = this;

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
				parent_->children_.InsertAfter(me, spNode(sibling));
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

	void Node::SetPivotX(float pivot_x)
	{
		this->SetPivot(pivot_x, pivot_.y);
	}

	void Node::SetPivotY(float pivot_y)
	{
		this->SetPivot(pivot_.x, pivot_y);
	}

	void Node::SetPivot(float pivot_x, float pivot_y)
	{
		if (pivot_.x == pivot_x && pivot_.y == pivot_y)
			return;

		pivot_.x = pivot_x;
		pivot_.y = pivot_y;
		dirty_transform_ = true;
	}

	void Node::SetWidth(float width)
	{
		this->SetSize(width, size_.height);
	}

	void Node::SetHeight(float height)
	{
		this->SetSize(size_.width, height);
	}

	void Node::SetSize(const Size& size)
	{
		this->SetSize(size.width, size.height);
	}

	void Node::SetSize(float width, float height)
	{
		if (size_.width == width && size_.height == height)
			return;

		size_.width = width;
		size_.height = height;
		dirty_transform_ = true;
	}

	void Node::SetTransform(Transform const& transform)
	{
		transform_ = transform;
		dirty_transform_ = true;
	}

	void Node::SetBorderColor(Color const& color)
	{
		border_color_ = color;
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

	void Node::AddChild(spNode const& child)
	{
		E2D_ASSERT(child && "Node::AddChild failed, NULL pointer exception");

		if (child)
		{
#ifdef E2D_DEBUG

			if (child->parent_)
				logs::Errorln("The node to be added already has a parent");

			for (Node* parent = parent_; parent; parent = parent->parent_)
				if (parent == child)
					logs::Errorln("A node cannot be its own parent");

#endif // E2D_DEBUG

			children_.PushBack(Node::ItemType(child));
			child->parent_ = this;
			child->SetScene(this->scene_);
			child->dirty_transform_ = true;
			child->UpdateOpacity();
			child->SetZOrder(child->GetZOrder());
		}
	}

	void Node::AddChildren(const Nodes& children)
	{
		for (const auto& node : children)
		{
			this->AddChild(node);
		}
	}

	Rect Node::GetBounds()
	{
		return Rect(Point{}, size_);
	}

	Node::Nodes Node::GetChildren(String const& name) const
	{
		Nodes children;
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

	spNode Node::GetChild(String const& name) const
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

	bool Node::RemoveChild(spNode const& child)
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
			children_.Remove(spNode(child));
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

	bool Node::ContainsPoint(const Point& point)
	{
		if (size_.width == 0.f || size_.height == 0.f)
			return false;

		UpdateTransform();

		cpRectangleGeometry border;

		ThrowIfFailed(
			Factory::Instance()->CreateRectangleGeometry(
				border,
				Rect(Point{}, size_)
			)
		);

		BOOL ret = 0;
		// no matter it failed or not
		border->FillContainsPoint(
			point,
			ConvertToD2DMatrix(final_matrix_),
			&ret
		);
		return !!ret;
	}
}
