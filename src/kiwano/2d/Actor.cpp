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

#include "Actor.h"
#include "Stage.h"
#include "../base/Logger.h"
#include "../renderer/Renderer.h"

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
		, update_pausing_(false)
		, hover_(false)
		, pressed_(false)
		, responsible_(false)
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
		if (!update_pausing_)
		{
			UpdateActions(this, dt);
			UpdateTimers(dt);

			if (cb_update_)
				cb_update_(dt);

			OnUpdate(dt);
		}

		if (!children_.item_empty())
		{
			ActorPtr next;
			for (auto child = children_.first_item(); child; child = next)
			{
				next = child->next_item();
				child->Update(dt);
			}
		}
	}

	void Actor::Render(Renderer* renderer)
	{
		if (!visible_)
			return;

		UpdateTransform();

		if (children_.item_empty())
		{
			OnRender(renderer);
		}
		else
		{
			// render children those are less than 0 in Z-Order
			Actor* child = children_.first_item().get();
			while (child)
			{
				if (child->GetZOrder() >= 0)
					break;

				child->Render(renderer);
				child = child->next_item().get();
			}

			OnRender(renderer);

			while (child)
			{
				child->Render(renderer);
				child = child->next_item().get();
			}
		}
	}

	void Actor::PrepareRender(Renderer* renderer)
	{
		renderer->SetTransform(transform_matrix_);
		renderer->SetOpacity(displayed_opacity_);
	}

	void Actor::RenderBorder()
	{
		if (show_border_)
		{
            Rect bounds = GetBounds();

            auto renderer = Renderer::GetInstance();
            renderer->SetTransform(transform_matrix_);
            renderer->FillRectangle(bounds, Color(Color::Red, .4f));
            renderer->DrawRectangle(bounds, Color(Color::Red, .8f), 2.f);
		}

		for (auto child = children_.first_item(); child; child = child->next_item())
		{
			child->RenderBorder();
		}
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

		if (responsible_ && MouseEvent::Check(evt.type))
		{
			if (evt.type == Event::MouseMove)
			{
				if (!evt.target && ContainsPoint(Point{ evt.mouse.x, evt.mouse.y }))
				{
					evt.target = this;

					if (!hover_)
					{
						hover_ = true;

						Event hover = evt;
						hover.type = Event::MouseHover;
						EventDispatcher::Dispatch(hover);
					}
				}
				else if (hover_)
				{
					hover_ = false;
					pressed_ = false;

					Event out = evt;
					out.target = this;
					out.type = Event::MouseOut;
					EventDispatcher::Dispatch(out);
				}
			}

			if (evt.type == Event::MouseBtnDown && hover_)
			{
				pressed_ = true;
				evt.target = this;
			}

			if (evt.type == Event::MouseBtnUp && pressed_)
			{
				pressed_ = false;
				evt.target = this;

				Event click = evt;
				click.type = Event::Click;
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

		if (is_fast_transform_)
		{
			transform_matrix_ = Matrix3x2::Translation(transform_.position);
		}
		else
		{
			// matrix multiplication is optimized by expression template
            transform_matrix_ = Matrix3x2::SRT(transform_.position, transform_.scale, transform_.rotation);

            if (!transform_.skew.IsOrigin())
            {
                transform_matrix_ = Matrix3x2::Skewing(transform_.skew) * transform_matrix_;
            }
		}

		transform_matrix_.Translate(Point{ -size_.x * anchor_.x, -size_.y * anchor_.y });

		if (parent_)
		{
			transform_matrix_ *= parent_->transform_matrix_;
		}

		// update children's transform
		for (Actor* child = children_.first_item().get(); child; child = child->next_item().get())
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

	void Actor::SetStage(Stage* scene)
	{
		if (scene && stage_ != scene)
		{
			stage_ = scene;
			for (Actor* child = children_.first_item().get(); child; child = child->next_item().get())
			{
				child->stage_ = scene;
			}
		}
	}

	void Actor::Reorder()
	{
		if (parent_)
		{
			ActorPtr me = this;

			parent_->children_.remove_item(me);

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
				parent_->children_.push_front_item(me);
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

	void Actor::SetAnchorX(float anchor_x)
	{
		this->SetAnchor(anchor_x, anchor_.y);
	}

	void Actor::SetAnchorY(float anchor_y)
	{
		this->SetAnchor(anchor_.x, anchor_y);
	}

	void Actor::SetAnchor(float anchor_x, float anchor_y)
	{
		if (anchor_.x == anchor_x && anchor_.y == anchor_y)
			return;

		anchor_.x = anchor_x;
		anchor_.y = anchor_y;
		dirty_transform_ = true;
	}

	void Actor::SetAnchor(Point const& anchor)
	{
		this->SetAnchor(anchor.x, anchor.y);
	}

	void Actor::SetWidth(float width)
	{
		this->SetSize(width, size_.y);
	}

	void Actor::SetHeight(float height)
	{
		this->SetSize(size_.x, height);
	}

	void Actor::SetSize(const Size& size)
	{
		this->SetSize(size.x, size.y);
	}

	void Actor::SetSize(float width, float height)
	{
		if (size_.x == width && size_.y == height)
			return;

		size_.x = width;
		size_.y = height;
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

	void Actor::SetPositionX(float x)
	{
		this->SetPosition(x, transform_.position.y);
	}

	void Actor::SetPositionY(float y)
	{
		this->SetPosition(transform_.position.x, y);
	}

	void Actor::SetPosition(const Point & p)
	{
		this->SetPosition(p.x, p.y);
	}

	void Actor::SetPosition(float x, float y)
	{
		if (transform_.position.x == x && transform_.position.y == y)
			return;

		transform_.position.x = x;
		transform_.position.y = y;
		dirty_transform_ = true;
	}

	void Actor::Move(float x, float y)
	{
		this->SetPosition(transform_.position.x + x, transform_.position.y + y);
	}

	void Actor::Move(const Point & v)
	{
		this->Move(v.x, v.y);
	}

	void Actor::SetScaleX(float scale_x)
	{
		this->SetScale(scale_x, transform_.scale.y);
	}

	void Actor::SetScaleY(float scale_y)
	{
		this->SetScale(transform_.scale.x, scale_y);
	}

	void Actor::SetScale(float scale)
	{
		this->SetScale(scale, scale);
	}

	void Actor::SetScale(float scale_x, float scale_y)
	{
		if (transform_.scale.x == scale_x && transform_.scale.y == scale_y)
			return;

		transform_.scale.x = scale_x;
		transform_.scale.y = scale_y;
		dirty_transform_ = true;
		is_fast_transform_ = false;
	}

	void Actor::SetScale(Point const& scale)
	{
		this->SetScale(scale.x, scale.y);
	}

	void Actor::SetSkewX(float skew_x)
	{
		this->SetSkew(skew_x, transform_.skew.y);
	}

	void Actor::SetSkewY(float skew_y)
	{
		this->SetSkew(transform_.skew.x, skew_y);
	}

	void Actor::SetSkew(float skew_x, float skew_y)
	{
		if (transform_.skew.x == skew_x && transform_.skew.y == skew_y)
			return;

		transform_.skew.x = skew_x;
		transform_.skew.y = skew_y;
		dirty_transform_ = true;
		is_fast_transform_ = false;
	}

	void Actor::SetSkew(Point const& skew)
	{
		this->SetSkew(skew.x, skew.y);
	}

	void Actor::SetRotation(float angle)
	{
		if (transform_.rotation == angle)
			return;

		transform_.rotation = angle;
		dirty_transform_ = true;
		is_fast_transform_ = false;
	}

	void Actor::AddChild(ActorPtr child)
	{
		KGE_ASSERT(child && "Actor::AddChild failed, NULL pointer exception");

		if (child)
		{
#ifdef KGE_DEBUG

			if (child->parent_)
				KGE_ERROR_LOG(L"The actor to be added already has a parent");

			for (Actor* parent = parent_; parent; parent = parent->parent_)
				if (parent == child)
					KGE_ERROR_LOG(L"A actor cannot be its own parent");

#endif // KGE_DEBUG

			children_.push_back_item(child);
			child->parent_ = this;
			child->SetStage(this->stage_);
			child->dirty_transform_ = true;
			child->UpdateOpacity();
			child->Reorder();
		}
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
		return Rect(Point{}, size_);
	}

	Rect Actor::GetBoundingBox() const
	{
		return GetTransformMatrix().Transform(GetBounds());
	}

	Vector<ActorPtr> Actor::GetChildren(String const& name) const
	{
		Vector<ActorPtr> children;
		size_t hash_code = std::hash<String>{}(name);

		for (Actor* child = children_.first_item().get(); child; child = child->next_item().get())
		{
			if (child->hash_name_ == hash_code && child->IsName(name))
			{
				children.push_back(child);
			}
		}
		return children;
	}

	ActorPtr Actor::GetChild(String const& name) const
	{
		size_t hash_code = std::hash<String>{}(name);

		for (Actor* child = children_.first_item().get(); child; child = child->next_item().get())
		{
			if (child->hash_name_ == hash_code && child->IsName(name))
			{
				return child;
			}
		}
		return nullptr;
	}

	Actor::Children const & Actor::GetChildren() const
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

		if (children_.item_empty())
			return;

		if (child)
		{
			child->parent_ = nullptr;
			if (child->stage_) child->SetStage(nullptr);
			children_.remove_item(ActorPtr(child));
		}
	}

	void Actor::RemoveChildren(String const& child_name)
	{
		if (children_.item_empty())
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
		children_.clear_items();
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
