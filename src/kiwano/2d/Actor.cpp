// Copyright (c) 2016-2018 Kiwano - Nomango
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

#include <kiwano/2d/Actor.h>
#include <kiwano/2d/Stage.h>
#include <kiwano/core/Logger.h>
#include <kiwano/renderer/Renderer.h>

namespace kiwano
{
	namespace
	{
		float default_anchor_x = 0.f;
		float default_anchor_y = 0.f;
	}

	void Actor::SetDefaultAnchor(float anchor_x, float anchor_y)
	{
		default_anchor_x = anchor_x;
		default_anchor_y = anchor_y;
	}

	Actor::Actor()
		: visible_(true)
		, visible_in_rt_(true)
		, update_pausing_(false)
		, hover_(false)
		, pressed_(false)
		, responsible_(false)
		, dirty_visibility_(true)
		, dirty_transform_(false)
		, dirty_transform_inverse_(false)
		, cascade_opacity_(false)
		, show_border_(false)
		, is_fast_transform_(true)
		, parent_(nullptr)
		, stage_(nullptr)
		, hash_name_(0)
		, z_order_(0)
		, opacity_(1.f)
		, displayed_opacity_(1.f)
		, anchor_(default_anchor_x, default_anchor_y)
	{
	}

	void Actor::Update(Duration dt)
	{
		UpdateActions(this, dt);
		UpdateTimers(dt);

		if (!update_pausing_)
		{
			if (cb_update_)
				cb_update_(dt);

			OnUpdate(dt);
		}

		if (!children_.empty())
		{
			ActorPtr next;
			for (auto child = children_.first_item(); child; child = next)
			{
				next = child->next_item();
				child->Update(dt);
			}
		}
	}

	void Actor::Render(RenderTarget* rt)
	{
		if (!visible_)
			return;

		UpdateTransform();

		if (children_.empty())
		{
			if (CheckVisibilty(rt))
			{
				PrepareToRender(rt);
				OnRender(rt);
			}
		}
		else
		{
			// render children those are less than 0 in Z-Order
			Actor* child = children_.first_item().get();
			while (child)
			{
				if (child->GetZOrder() >= 0)
					break;

				child->Render(rt);
				child = child->next_item().get();
			}

			if (CheckVisibilty(rt))
			{
				PrepareToRender(rt);
				OnRender(rt);
			}

			while (child)
			{
				child->Render(rt);
				child = child->next_item().get();
			}
		}
	}

	void Actor::PrepareToRender(RenderTarget* rt)
	{
		rt->SetTransform(transform_matrix_);
		rt->SetBrushOpacity(GetDisplayedOpacity());
	}

	void Actor::RenderBorder(RenderTarget* rt)
	{
		if (show_border_ && !size_.IsOrigin())
		{
			Rect bounds = GetBounds();

			rt->SetTransform(transform_matrix_);

			rt->SetCurrentBrush(GetStage()->GetBorderFillBrush());
			rt->FillRectangle(bounds);

			rt->SetCurrentBrush(GetStage()->GetBorderStrokeBrush());
			rt->DrawRectangle(bounds, 2.f);
		}

		for (auto child = children_.first_item(); child; child = child->next_item())
		{
			child->RenderBorder(rt);
		}
	}

	bool Actor::CheckVisibilty(RenderTarget* rt) const
	{
		if (dirty_visibility_)
		{
			dirty_visibility_ = false;

			if (size_.IsOrigin())
			{
				visible_in_rt_ = false;
			}
			else
			{
				visible_in_rt_ = rt->CheckVisibility(GetBounds(), GetTransformMatrix());
			}
		}
		return visible_in_rt_;
	}

	void Actor::Dispatch(Event& evt)
	{
		if (!visible_)
			return;

		ActorPtr prev;
		for (auto child = children_.last_item(); child; child = prev)
		{
			prev = child->prev_item();
			child->Dispatch(evt);
		}

		if (responsible_)
		{
			if (evt.IsType<MouseMoveEvent>())
			{
				auto& mouse_evt = evt.SafeCast<MouseMoveEvent>();
				if (!mouse_evt.target && ContainsPoint(mouse_evt.pos))
				{
					mouse_evt.target = this;

					if (!hover_)
					{
						hover_ = true;

						MouseHoverEvent hover;
						hover.pos = mouse_evt.pos;
						hover.left_btn_down = mouse_evt.left_btn_down;
						hover.right_btn_down = mouse_evt.right_btn_down;
						hover.target = this;
						EventDispatcher::Dispatch(hover);
					}
				}
				else if (hover_)
				{
					hover_ = false;
					pressed_ = false;

					MouseOutEvent out;
					out.pos = mouse_evt.pos;
					out.left_btn_down = mouse_evt.left_btn_down;
					out.right_btn_down = mouse_evt.right_btn_down;
					out.target = this;
					EventDispatcher::Dispatch(out);
				}
			}

			if (evt.IsType<MouseDownEvent>() && hover_)
			{
				pressed_ = true;
				evt.SafeCast<MouseDownEvent>().target = this;
			}

			if (evt.IsType<MouseUpEvent>() && pressed_)
			{
				pressed_ = false;

				auto mouse_up_evt = evt.SafeCast<MouseUpEvent>();
				mouse_up_evt.target = this;

				MouseClickEvent click;
				click.pos = mouse_up_evt.pos;
				click.left_btn_down = mouse_up_evt.left_btn_down;
				click.right_btn_down = mouse_up_evt.right_btn_down;
				click.target = this;
				click.button = mouse_up_evt.button;
				EventDispatcher::Dispatch(click);
			}
		}

		EventDispatcher::Dispatch(evt);
	}

	Matrix3x2 const & Actor::GetTransformMatrix()  const
	{
		UpdateTransform();
		return transform_matrix_;
	}

	Matrix3x2 const & Actor::GetTransformInverseMatrix()  const
	{
		UpdateTransform();
		if (dirty_transform_inverse_)
		{
			transform_matrix_inverse_ = transform_matrix_.Invert();
			dirty_transform_inverse_ = false;
		}
		return transform_matrix_inverse_;
	}

	void Actor::UpdateTransform() const
	{
		if (!dirty_transform_)
			return;

		dirty_transform_ = false;
		dirty_transform_inverse_ = true;
		dirty_visibility_ = true;

		if (is_fast_transform_)
		{
			transform_matrix_ = Matrix3x2::Translation(transform_.position);
		}
		else
		{
			// matrix multiplication is optimized by expression template
			transform_matrix_ = transform_.ToMatrix();
		}

		transform_matrix_.Translate(Point{ -size_.x * anchor_.x, -size_.y * anchor_.y });

		if (parent_)
		{
			transform_matrix_ *= parent_->transform_matrix_;
		}

		// update children's transform
		for (auto child = children_.first_item().get(); child; child = child->next_item().get())
			child->dirty_transform_ = true;
	}

	void Actor::UpdateOpacity()
	{
		if (parent_ && parent_->IsCascadeOpacityEnabled())
		{
			displayed_opacity_ = opacity_ * parent_->displayed_opacity_;
		}
		else
		{
			displayed_opacity_ = opacity_;
		}

		for (Actor* child = children_.first_item().get(); child; child = child->next_item().get())
		{
			child->UpdateOpacity();
		}
	}

	void Actor::SetStage(Stage* stage)
	{
		if (stage_ != stage)
		{
			stage_ = stage;
			for (Actor* child = children_.first_item().get(); child; child = child->next_item().get())
			{
				child->stage_ = stage;
			}
		}
	}

	void Actor::Reorder()
	{
		if (parent_)
		{
			ActorPtr me = this;

			parent_->children_.remove(me);

			Actor* sibling = parent_->children_.last_item().get();

			if (sibling && sibling->GetZOrder() > z_order_)
			{
				sibling = sibling->prev_item().get();
				while (sibling)
				{
					if (sibling->GetZOrder() <= z_order_)
						break;
					sibling = sibling->prev_item().get();
				}
			}

			if (sibling)
			{
				parent_->children_.insert_after(me, sibling);
			}
			else
			{
				parent_->children_.push_front(me);
			}
		}
	}

	void Actor::SetZOrder(int zorder)
	{
		if (z_order_ != zorder)
		{
			z_order_ = zorder;
			Reorder();
		}
	}

	void Actor::SetOpacity(float opacity)
	{
		if (opacity_ == opacity)
			return;

		displayed_opacity_ = opacity_ = std::min(std::max(opacity, 0.f), 1.f);
		UpdateOpacity();
	}

	void Actor::SetCascadeOpacityEnabled(bool enabled)
	{
		if (cascade_opacity_ == enabled)
			return;

		cascade_opacity_ = enabled;
		UpdateOpacity();
	}

	void Actor::SetAnchor(Vec2 const& anchor)
	{
		if (anchor_ == anchor)
			return;

		anchor_ = anchor;
		dirty_transform_ = true;
	}

	void Actor::SetWidth(float width)
	{
		SetSize(Size{ width, size_.y });
	}

	void Actor::SetHeight(float height)
	{
		SetSize(Size{ size_.x, height });
	}

	void Actor::SetSize(Size const& size)
	{
		if (size_ == size)
			return;

		size_ = size;
		dirty_transform_ = true;
	}

	void Actor::SetTransform(Transform const& transform)
	{
		transform_ = transform;
		dirty_transform_ = true;
		is_fast_transform_ = false;
	}

	void Actor::SetVisible(bool val)
	{
		visible_ = val;
	}

	void Actor::SetName(String const& name)
	{
		if (!IsName(name))
		{
			ObjectBase::SetName(name);
			hash_name_ = std::hash<String>{}(name);
		}
	}

	void Actor::SetPosition(const Point & pos)
	{
		if (transform_.position == pos)
			return;

		transform_.position = pos;
		dirty_transform_ = true;
	}

	void Actor::SetPositionX(float x)
	{
		SetPosition(Point{ x, transform_.position.y });
	}

	void Actor::SetPositionY(float y)
	{
		SetPosition(Point{ transform_.position.x, y });
	}

	void Actor::Move(Vec2 const& v)
	{
		this->SetPosition(Point{ transform_.position.x + v.x, transform_.position.y + v.y });
	}

	void Actor::SetScale(Vec2 const& scale)
	{
		if (transform_.scale == scale)
			return;

		transform_.scale = scale;
		dirty_transform_ = true;
		is_fast_transform_ = false;
	}

	void Actor::SetSkew(Vec2 const& skew)
	{
		if (transform_.skew == skew)
			return;

		transform_.skew = skew;
		dirty_transform_ = true;
		is_fast_transform_ = false;
	}

	void Actor::SetRotation(float angle)
	{
		if (transform_.rotation == angle)
			return;

		transform_.rotation = angle;
		dirty_transform_ = true;
		is_fast_transform_ = false;
	}

	void Actor::AddChild(Actor* child, int zorder)
	{
		KGE_ASSERT(child && "Actor::AddChild failed, NULL pointer exception");

		if (child)
		{
			KGE_ASSERT(!child->parent_ && "Actor::AddChild failed, the actor to be added already has a parent");

#ifdef KGE_DEBUG

			for (Actor* parent = parent_; parent; parent = parent->parent_)
			{
				if (parent == child)
				{
					KGE_ERROR(L"A actor cannot be its own parent");
					return;
				}
			}

#endif // KGE_DEBUG

			children_.push_back(child);
			child->parent_ = this;
			child->SetStage(this->stage_);

			child->dirty_transform_ = true;
			child->z_order_ = zorder;
			child->Reorder();
			child->UpdateOpacity();
		}
	}

	void Actor::AddChild(ActorPtr child, int zorder)
	{
		AddChild(child.get());
	}

	void Actor::AddChildren(Vector<ActorPtr> const& children)
	{
		for (const auto& actor : children)
		{
			this->AddChild(actor);
		}
	}

	Rect Actor::GetBounds() const
	{
		return Rect{ Point{}, size_ };
	}

	Rect Actor::GetBoundingBox() const
	{
		return GetTransformMatrix().Transform(GetBounds());
	}

	Vector<ActorPtr> Actor::GetChildren(String const& name) const
	{
		Vector<ActorPtr> children;
		size_t hash_code = std::hash<String>{}(name);

		for (auto child = children_.first_item().get(); child; child = child->next_item().get())
		{
			if (child->hash_name_ == hash_code && child->IsName(name))
			{
				children.push_back(const_cast<Actor*>(child));
			}
		}
		return children;
	}

	Actor* Actor::GetChild(String const& name) const
	{
		size_t hash_code = std::hash<String>{}(name);

		for (auto child = children_.first_item().get(); child; child = child->next_item().get())
		{
			if (child->hash_name_ == hash_code && child->IsName(name))
			{
				return const_cast<Actor*>(child);
			}
		}
		return nullptr;
	}

	Actor::Children& Actor::GetAllChildren()
	{
		return children_;
	}

	Actor::Children const & Actor::GetAllChildren() const
	{
		return children_;
	}

	void Actor::RemoveFromParent()
	{
		if (parent_)
		{
			parent_->RemoveChild(this);
		}
	}

	void Actor::RemoveChild(ActorPtr child)
	{
		RemoveChild(child.get());
	}

	void Actor::RemoveChild(Actor * child)
	{
		KGE_ASSERT(child && "Actor::RemoveChild failed, NULL pointer exception");

		if (children_.empty())
			return;

		if (child)
		{
			child->parent_ = nullptr;
			if (child->stage_) child->SetStage(nullptr);
			children_.remove(ActorPtr(child));
		}
	}

	void Actor::RemoveChildren(String const& child_name)
	{
		if (children_.empty())
		{
			return;
		}

		size_t hash_code = std::hash<String>{}(child_name);

		Actor* next;
		for (Actor* child = children_.first_item().get(); child; child = next)
		{
			next = child->next_item().get();

			if (child->hash_name_ == hash_code && child->IsName(child_name))
			{
				RemoveChild(child);
			}
		}
	}

	void Actor::RemoveAllChildren()
	{
		children_.clear();
	}

	void Actor::SetResponsible(bool enable)
	{
		responsible_ = enable;
	}

	bool Actor::ContainsPoint(const Point& point) const
	{
		if (size_.x == 0.f || size_.y == 0.f)
			return false;

		Point local = GetTransformInverseMatrix().Transform(point);
		return GetBounds().ContainsPoint(local);
	}

}
