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
    , cascade_opacity_(true)
    , show_border_(false)
    , evt_dispatch_enabled_(true)
    , dirty_flag_(DirtyFlag::DirtyVisibility)
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
    Animator::Update(this, dt);
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
        RefPtr<Actor> next;
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
    UpdateOpacity();

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
        RefPtr<Actor> child = children_.GetFirst();
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
    if (!visible_)
        return;

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
    if (dirty_flag_.Has(DirtyFlag::DirtyVisibility))
    {
        dirty_flag_.Unset(DirtyFlag::DirtyVisibility);

        if (size_.IsOrigin())
        {
            visible_in_rt_ = false;
        }
        else
        {
            visible_in_rt_ = ctx.CheckVisibility(GetBounds(), transform_matrix_ /* GetTransformMatrix() */);
        }
    }
    return visible_in_rt_;
}

bool Actor::DispatchEvent(Event* evt)
{
    if (!visible_ || !evt_dispatch_enabled_)
        return true;

    // Dispatch to children those are greater than 0 in Z-Order
    RefPtr<Actor> child = children_.GetLast();
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
    (*serializer) << visible_ << update_pausing_ << cascade_opacity_ << z_order_ << opacity_ << anchor_ << size_
                  << transform_;
}

void Actor::DoDeserialize(Deserializer* deserializer)
{
    ObjectBase::DoDeserialize(deserializer);

    float opacity = 1.0f;
    Transform transform;
    (*deserializer) >> visible_ >> update_pausing_ >> cascade_opacity_ >> z_order_ >> opacity >> anchor_ >> size_
        >> transform;

    SetOpacity(opacity);
    SetTransform(transform);
}

bool Actor::HandleEvent(Event* evt)
{
    ComponentManager::DispatchToComponents(evt);

    if (!EventDispatcher::DispatchEvent(evt))
        return false;
    return true;
}

const Matrix3x2& Actor::GetTransformMatrix() const
{
    UpdateTransformUpwards();
    return transform_matrix_;
}

const Matrix3x2& Actor::GetTransformInverseMatrix() const
{
    UpdateTransformUpwards();
    if (dirty_flag_.Has(DirtyFlag::DirtyTransformInverse))
    {
        dirty_flag_.Unset(DirtyFlag::DirtyTransformInverse);
        transform_matrix_inverse_ = transform_matrix_.Invert();
    }
    return transform_matrix_inverse_;
}

const Matrix3x2& Actor::GetTransformMatrixToParent() const
{
    UpdateTransformUpwards();
    return transform_matrix_to_parent_;
}

void Actor::UpdateTransform() const
{
    if (!dirty_flag_.Has(DirtyFlag::DirtyTransform))
        return;

    dirty_flag_.Unset(DirtyFlag::DirtyTransform);
    dirty_flag_.Set(DirtyFlag::DirtyTransformInverse);
    dirty_flag_.Set(DirtyFlag::DirtyVisibility);

    if (transform_.IsFast())
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
        child->dirty_flag_.Set(DirtyFlag::DirtyTransform);
}

void Actor::UpdateTransformUpwards() const
{
    if (parent_)
    {
        parent_->UpdateTransformUpwards();

        if (parent_->dirty_flag_.Has(DirtyFlag::DirtyTransform))
        {
            dirty_flag_.Set(DirtyFlag::DirtyTransform);
        }
    }

    UpdateTransform();
}

void Actor::UpdateOpacity()
{
    if (!dirty_flag_.Has(DirtyFlag::DirtyOpacity))
        return;

    dirty_flag_.Unset(DirtyFlag::DirtyOpacity);

    if (parent_ && parent_->IsCascadeOpacityEnabled())
    {
        displayed_opacity_ = opacity_ * parent_->displayed_opacity_;
    }
    else
    {
        displayed_opacity_ = opacity_;
    }

    for (const auto& child : children_)
        child->dirty_flag_.Set(DirtyFlag::DirtyOpacity);
}

void Actor::SetStage(Stage* stage)
{
    if (stage_ != stage)
    {
        stage_ = stage;
        for (auto& child : children_)
        {
            child->SetStage(stage);
        }
    }
}

void Actor::Reorder()
{
    if (parent_)
    {
        RefPtr<Actor> me = this;

        parent_->children_.Remove(me);

        RefPtr<Actor> sibling = parent_->children_.GetLast();

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
    dirty_flag_.Set(DirtyFlag::DirtyOpacity);
}

void Actor::SetCascadeOpacityEnabled(bool enabled)
{
    if (cascade_opacity_ == enabled)
        return;

    cascade_opacity_ = enabled;
    dirty_flag_.Set(DirtyFlag::DirtyOpacity);
}

void Actor::SetAnchor(const Vec2& anchor)
{
    if (anchor_ == anchor)
        return;

    anchor_ = anchor;
    dirty_flag_.Set(DirtyFlag::DirtyTransform);
}

void Actor::SetSize(const Size& size)
{
    if (size_ == size)
        return;

    size_ = size;
    dirty_flag_.Set(DirtyFlag::DirtyTransform);
}

void Actor::SetTransform(const Transform& transform)
{
    transform_ = transform;
    dirty_flag_.Set(DirtyFlag::DirtyTransform);
}

void Actor::SetVisible(bool val)
{
    visible_ = val;
}

void Actor::SetName(StringView name)
{
    ObjectBase::SetName(name);
    hash_name_ = std::hash<StringView>{}(name);
}

void Actor::SetPosition(const Point& pos)
{
    if (transform_.position == pos)
        return;

    transform_.position = pos;
    dirty_flag_.Set(DirtyFlag::DirtyTransform);
}

void Actor::SetScale(const Vec2& scale)
{
    if (transform_.scale == scale)
        return;

    transform_.scale = scale;
    dirty_flag_.Set(DirtyFlag::DirtyTransform);
}

void Actor::SetSkew(const Vec2& skew)
{
    if (transform_.skew == skew)
        return;

    transform_.skew = skew;
    dirty_flag_.Set(DirtyFlag::DirtyTransform);
}

void Actor::SetRotation(float angle)
{
    if (transform_.rotation == angle)
        return;

    transform_.rotation = angle;
    dirty_flag_.Set(DirtyFlag::DirtyTransform);
}

void Actor::AddChild(RefPtr<Actor> child)
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

        child->dirty_flag_.Set(DirtyFlag::DirtyTransform);
        child->dirty_flag_.Set(DirtyFlag::DirtyOpacity);
        child->Reorder();
    }
    else
    {
        Fail("Actor::AddChild failed, NULL pointer exception");
    }
}

void Actor::AddChild(RefPtr<Actor> child, int zorder)
{
    child->z_order_ = zorder;
    this->AddChild(child);
}

void Actor::AddChildren(const Vector<RefPtr<Actor>>& children)
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

Vector<RefPtr<Actor>> Actor::GetChildren(StringView name) const
{
    Vector<RefPtr<Actor>> children;
    size_t           hash_code = std::hash<StringView>{}(name);

    for (const auto& child : children_)
    {
        if (child->hash_name_ == hash_code && child->IsName(name))
        {
            children.push_back(child);
        }
    }
    return children;
}

RefPtr<Actor> Actor::GetChild(StringView name) const
{
    size_t hash_code = std::hash<StringView>{}(name);

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

void Actor::RemoveChild(RefPtr<Actor> child)
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

void Actor::RemoveChildren(StringView child_name)
{
    if (children_.IsEmpty())
    {
        return;
    }

    size_t hash_code = std::hash<StringView>{}(child_name);

    RefPtr<Actor> next;
    for (RefPtr<Actor> child = children_.GetFirst(); child; child = next)
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
    RefPtr<Actor> next;
    for (RefPtr<Actor> child = children_.GetFirst(); child; child = next)
    {
        next = child->GetNext();
        RemoveChild(child);
    }
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
