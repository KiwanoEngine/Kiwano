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
#include <kiwano/utils/Logger.h>
#include <kiwano/render/Renderer.h>

namespace kiwano
{
namespace
{

float default_anchor_x = 0.f;
float default_anchor_y = 0.f;

}  // namespace

void Actor::SetDefaultAnchor(float anchor_x, float anchor_y)
{
    default_anchor_x = anchor_x;
    default_anchor_y = anchor_y;
}

Actor::Actor()
    : ComponentManager(this)
    , visible_(true)
    , visible_in_rt_(true)
    , update_pausing_(false)
    , hover_(false)
    , pressed_(false)
    , responsible_(false)
    , dirty_visibility_(true)
    , dirty_transform_(false)
    , dirty_transform_inverse_(false)
    , cascade_opacity_(true)
    , show_border_(false)
    , is_fast_transform_(true)
    , evt_dispatch_enabled_(true)
    , parent_(nullptr)
    , stage_(nullptr)
    , physic_body_(nullptr)
    , hash_name_(0)
    , z_order_(0)
    , opacity_(1.f)
    , displayed_opacity_(1.f)
    , anchor_(default_anchor_x, default_anchor_y)
{
}

Actor::~Actor()
{
    RemoveAllComponents();
    RemoveAllChildren();
}

void Actor::Update(Duration dt)
{
    ActionScheduler::Update(this, dt);
    TaskScheduler::Update(dt);
    ComponentManager::Update(dt);

    if (!update_pausing_)
    {
        if (cb_update_)
            cb_update_(dt);

        OnUpdate(dt);
    }

    if (!children_.IsEmpty())
    {
        ActorPtr next;
        for (auto child = children_.GetFirst(); child; child = next)
        {
            next = child->GetNext();
            child->Update(dt);
        }
    }
}

void Actor::Render(RenderContext& ctx)
{
    if (!visible_)
        return;

    UpdateTransform();

    if (children_.IsEmpty())
    {
        if (CheckVisibility(ctx))
        {
            PrepareToRender(ctx);
            ComponentManager::Render(ctx);
            OnRender(ctx);
        }
    }
    else
    {
        // render children those are less than 0 in Z-Order
        ActorPtr child = children_.GetFirst();
        while (child)
        {
            if (child->GetZOrder() >= 0)
                break;

            child->Render(ctx);
            child = child->GetNext();
        }

        if (CheckVisibility(ctx))
        {
            PrepareToRender(ctx);
            ComponentManager::Render(ctx);
            OnRender(ctx);
        }

        while (child)
        {
            child->Render(ctx);
            child = child->GetNext();
        }
    }
}

void Actor::PrepareToRender(RenderContext& ctx)
{
    ctx.SetTransform(transform_matrix_);
    ctx.SetBrushOpacity(GetDisplayedOpacity());
}

void Actor::RenderBorder(RenderContext& ctx)
{
    if (show_border_ && !size_.IsOrigin())
    {
        Rect bounds = GetBounds();

        ctx.SetTransform(transform_matrix_);

        ctx.SetCurrentBrush(GetStage()->GetBorderFillBrush());
        ctx.FillRectangle(bounds);

        ctx.SetCurrentBrush(GetStage()->GetBorderStrokeBrush());
        ctx.DrawRectangle(bounds);
    }

    for (auto& child : children_)
    {
        child->RenderBorder(ctx);
    }
}

bool Actor::CheckVisibility(RenderContext& ctx) const
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
            visible_in_rt_ = ctx.CheckVisibility(GetBounds(), GetTransformMatrix());
        }
    }
    return visible_in_rt_;
}

bool Actor::DispatchEvent(Event* evt)
{
    if (!visible_ || !evt_dispatch_enabled_)
        return true;

    // Dispatch to children those are greater than 0 in Z-Order
    ActorPtr child = children_.GetLast();
    while (child)
    {
        if (child->GetZOrder() < 0)
            break;

        if (!child->DispatchEvent(evt))
            return false;

        child = child->GetPrev();
    }

    if (!HandleEvent(evt))
        return false;

    while (child)
    {
        if (!child->DispatchEvent(evt))
            return false;

        child = child->GetPrev();
    }
    return true;
}

void Actor::SetEventDispatchEnabled(bool enabled)
{
    evt_dispatch_enabled_ = enabled;
}

void Actor::DoSerialize(Serializer* serializer) const
{
    ObjectBase::DoSerialize(serializer);
    (*serializer) << visible_ << update_pausing_ << cascade_opacity_ << responsible_ << z_order_ << opacity_ << anchor_
                  << size_ << transform_;
}

void Actor::DoDeserialize(Deserializer* deserializer)
{
    ObjectBase::DoDeserialize(deserializer);

    float opacity = 1.0f;
    Transform transform;
    (*deserializer) >> visible_ >> update_pausing_ >> cascade_opacity_ >> responsible_ >> z_order_ >> opacity >> anchor_
        >> size_ >> transform;

    SetOpacity(opacity);
    SetTransform(transform);
}

bool Actor::HandleEvent(Event* evt)
{
    ComponentManager::DispatchToComponents(evt);

    if (!EventDispatcher::DispatchEvent(evt))
        return false;

    if (responsible_)
    {
        if (evt->IsType<MouseMoveEvent>())
        {
            auto mouse_evt = dynamic_cast<MouseMoveEvent*>(evt);
            bool contains  = ContainsPoint(mouse_evt->pos);
            if (!hover_ && contains)
            {
                hover_ = true;

                MouseHoverEventPtr hover = new MouseHoverEvent;
                hover->pos = mouse_evt->pos;
                HandleEvent(hover.Get());
            }
            else if (hover_ && !contains)
            {
                hover_   = false;
                pressed_ = false;

                MouseOutEventPtr out = new MouseOutEvent;
                out->pos = mouse_evt->pos;
                HandleEvent(out.Get());
            }
        }

        if (evt->IsType<MouseDownEvent>() && hover_)
        {
            pressed_ = true;
        }

        if (evt->IsType<MouseUpEvent>() && pressed_)
        {
            pressed_ = false;

            auto mouse_up_evt = dynamic_cast<MouseUpEvent*>(evt);

            MouseClickEventPtr click = new MouseClickEvent;
            click->pos               = mouse_up_evt->pos;
            click->button            = mouse_up_evt->button;
            HandleEvent(click.Get());
        }
    }
    return true;
}

const Matrix3x2& Actor::GetTransformMatrix() const
{
    UpdateTransform();
    return transform_matrix_;
}

const Matrix3x2& Actor::GetTransformInverseMatrix() const
{
    UpdateTransform();
    if (dirty_transform_inverse_)
    {
        transform_matrix_inverse_ = transform_matrix_.Invert();
        dirty_transform_inverse_  = false;
    }
    return transform_matrix_inverse_;
}

const Matrix3x2& Actor::GetTransformMatrixToParent() const
{
    UpdateTransform();
    return transform_matrix_to_parent_;
}

void Actor::UpdateTransform() const
{
    if (!dirty_transform_)
        return;

    dirty_transform_         = false;
    dirty_transform_inverse_ = true;
    dirty_visibility_        = true;

    if (is_fast_transform_)
    {
        transform_matrix_to_parent_ = Matrix3x2::Translation(transform_.position);
    }
    else
    {
        // matrix multiplication is optimized by expression template
        transform_matrix_to_parent_ = transform_.ToMatrix();
    }

    Point anchor_offset(-size_.x * anchor_.x, -size_.y * anchor_.y);
    transform_matrix_to_parent_.Translate(anchor_offset);

    transform_matrix_ = transform_matrix_to_parent_;
    if (parent_)
    {
        transform_matrix_ *= parent_->transform_matrix_;
    }

    // update children's transform
    for (const auto& child : children_)
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

    for (auto& child : children_)
    {
        child->UpdateOpacity();
    }
}

void Actor::SetStage(Stage* stage)
{
    if (stage_ != stage)
    {
        stage_ = stage;
        for (auto& child : children_)
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

        parent_->children_.Remove(me);

        ActorPtr sibling = parent_->children_.GetLast();

        if (sibling && sibling->GetZOrder() > z_order_)
        {
            sibling = sibling->GetPrev();
            while (sibling)
            {
                if (sibling->GetZOrder() <= z_order_)
                    break;
                sibling = sibling->GetPrev();
            }
        }

        if (sibling)
        {
            parent_->children_.InsertAfter(me, sibling);
        }
        else
        {
            parent_->children_.PushFront(me);
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

void Actor::SetAnchor(const Vec2& anchor)
{
    if (anchor_ == anchor)
        return;

    anchor_          = anchor;
    dirty_transform_ = true;
}

void Actor::SetSize(const Size& size)
{
    if (size_ == size)
        return;

    size_            = size;
    dirty_transform_ = true;
}

void Actor::SetTransform(const Transform& transform)
{
    transform_         = transform;
    dirty_transform_   = true;
    is_fast_transform_ = false;
}

void Actor::SetVisible(bool val)
{
    visible_ = val;
}

void Actor::SetName(const String& name)
{
    if (!IsName(name))
    {
        ObjectBase::SetName(name);
        hash_name_ = std::hash<String>{}(name);
    }
}

void Actor::SetPosition(const Point& pos)
{
    if (transform_.position == pos)
        return;

    transform_.position = pos;
    dirty_transform_    = true;
}

void Actor::SetScale(const Vec2& scale)
{
    if (transform_.scale == scale)
        return;

    transform_.scale   = scale;
    dirty_transform_   = true;
    is_fast_transform_ = false;
}

void Actor::SetSkew(const Vec2& skew)
{
    if (transform_.skew == skew)
        return;

    transform_.skew    = skew;
    dirty_transform_   = true;
    is_fast_transform_ = false;
}

void Actor::SetRotation(float angle)
{
    if (transform_.rotation == angle)
        return;

    transform_.rotation = angle;
    dirty_transform_    = true;
    is_fast_transform_  = false;
}

void Actor::AddChild(ActorPtr child)
{
    if (child)
    {
        KGE_ASSERT(!child->parent_ && "Actor::AddChild failed, the actor to be added already has a parent");

#ifdef KGE_DEBUG

        for (Actor* parent = parent_; parent; parent = parent->parent_)
        {
            if (parent == child)
            {
                Fail("Actor::AddChild failed, A actor cannot be its own parent");
                return;
            }
        }

#endif  // KGE_DEBUG

        children_.PushBack(child);
        child->parent_ = this;
        child->SetStage(this->stage_);

        child->dirty_transform_ = true;
        child->Reorder();
        child->UpdateOpacity();
    }
    else
    {
        Fail("Actor::AddChild failed, NULL pointer exception");
    }
}

void Actor::AddChild(ActorPtr child, int zorder)
{
    child->z_order_ = zorder;
    this->AddChild(child);
}

void Actor::AddChildren(const Vector<ActorPtr>& children)
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

Vector<ActorPtr> Actor::GetChildren(const String& name) const
{
    Vector<ActorPtr> children;
    size_t           hash_code = std::hash<String>{}(name);

    for (const auto& child : children_)
    {
        if (child->hash_name_ == hash_code && child->IsName(name))
        {
            children.push_back(child);
        }
    }
    return children;
}

ActorPtr Actor::GetChild(const String& name) const
{
    size_t hash_code = std::hash<String>{}(name);

    for (const auto& child : children_)
    {
        if (child->hash_name_ == hash_code && child->IsName(name))
        {
            return child;
        }
    }
    return nullptr;
}

ActorList& Actor::GetAllChildren()
{
    return children_;
}

const ActorList& Actor::GetAllChildren() const
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
    if (children_.IsEmpty())
        return;

    if (child)
    {
        child->parent_ = nullptr;
        if (child->stage_)
            child->SetStage(nullptr);
        children_.Remove(child);
    }
    else
    {
        Fail("Actor::RemoveChild failed, NULL pointer exception");
    }
}

void Actor::RemoveChildren(const String& child_name)
{
    if (children_.IsEmpty())
    {
        return;
    }

    size_t hash_code = std::hash<String>{}(child_name);

    ActorPtr next;
    for (ActorPtr child = children_.GetFirst(); child; child = next)
    {
        next = child->GetNext();

        if (child->hash_name_ == hash_code && child->IsName(child_name))
        {
            RemoveChild(child);
        }
    }
}

void Actor::RemoveAllChildren()
{
    children_.Clear();
}

void Actor::SetResponsible(bool enable)
{
    responsible_ = enable;
}

bool Actor::ContainsPoint(const Point& point) const
{
    if (size_.x == 0.f || size_.y == 0.f)
        return false;

    Point local = ConvertToLocal(point);
    return local.x >= 0 && local.y >= 0 && local.x <= size_.x && local.y <= size_.y;
}

Point Actor::ConvertToLocal(const Point& point) const
{
    Point local = GetTransformInverseMatrix().Transform(point);
    return local;
}

Point Actor::ConvertToWorld(const Point& point) const
{
    Point world = GetTransformMatrix().Transform(point);
    return world;
}

}  // namespace kiwano
