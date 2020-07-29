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

#include <functional>
#include <kiwano/2d/Actor.h>
#include <kiwano/2d/action/ActionTween.h>

namespace kiwano
{
//-------------------------------------------------------
// Ease Functions
//-------------------------------------------------------

inline EaseFunc MakeEaseIn(float rate)
{
    return std::bind(math::EaseIn, std::placeholders::_1, rate);
}
inline EaseFunc MakeEaseOut(float rate)
{
    return std::bind(math::EaseOut, std::placeholders::_1, rate);
}
inline EaseFunc MakeEaseInOut(float rate)
{
    return std::bind(math::EaseInOut, std::placeholders::_1, rate);
}
inline EaseFunc MakeEaseElasticIn(float period)
{
    return std::bind(math::EaseElasticIn, std::placeholders::_1, period);
}
inline EaseFunc MakeEaseElasticOut(float period)
{
    return std::bind(math::EaseElasticOut, std::placeholders::_1, period);
}
inline EaseFunc MakeEaseElasticInOut(float period)
{
    return std::bind(math::EaseElasticInOut, std::placeholders::_1, period);
}

KGE_API EaseFunc Ease::Linear       = math::Linear;
KGE_API EaseFunc Ease::EaseIn       = MakeEaseIn(2.f);
KGE_API EaseFunc Ease::EaseOut      = MakeEaseOut(2.f);
KGE_API EaseFunc Ease::EaseInOut    = MakeEaseInOut(2.f);
KGE_API EaseFunc Ease::ExpoIn       = math::EaseExponentialIn;
KGE_API EaseFunc Ease::ExpoOut      = math::EaseExponentialOut;
KGE_API EaseFunc Ease::ExpoInOut    = math::EaseExponentialInOut;
KGE_API EaseFunc Ease::BounceIn     = math::EaseBounceIn;
KGE_API EaseFunc Ease::BounceOut    = math::EaseBounceOut;
KGE_API EaseFunc Ease::BounceInOut  = math::EaseBounceInOut;
KGE_API EaseFunc Ease::ElasticIn    = MakeEaseElasticIn(0.3f);
KGE_API EaseFunc Ease::ElasticOut   = MakeEaseElasticOut(0.3f);
KGE_API EaseFunc Ease::ElasticInOut = MakeEaseElasticInOut(0.3f);
KGE_API EaseFunc Ease::SineIn       = math::EaseSineIn;
KGE_API EaseFunc Ease::SineOut      = math::EaseSineOut;
KGE_API EaseFunc Ease::SineInOut    = math::EaseSineInOut;
KGE_API EaseFunc Ease::BackIn       = math::EaseBackIn;
KGE_API EaseFunc Ease::BackOut      = math::EaseBackOut;
KGE_API EaseFunc Ease::BackInOut    = math::EaseBackInOut;
KGE_API EaseFunc Ease::QuadIn       = math::EaseQuadIn;
KGE_API EaseFunc Ease::QuadOut      = math::EaseQuadOut;
KGE_API EaseFunc Ease::QuadInOut    = math::EaseQuadInOut;
KGE_API EaseFunc Ease::CubicIn      = math::EaseCubicIn;
KGE_API EaseFunc Ease::CubicOut     = math::EaseCubicOut;
KGE_API EaseFunc Ease::CubicInOut   = math::EaseCubicInOut;
KGE_API EaseFunc Ease::QuartIn      = math::EaseQuartIn;
KGE_API EaseFunc Ease::QuartOut     = math::EaseQuartOut;
KGE_API EaseFunc Ease::QuartInOut   = math::EaseQuartInOut;
KGE_API EaseFunc Ease::QuintIn      = math::EaseQuintIn;
KGE_API EaseFunc Ease::QuintOut     = math::EaseQuintOut;
KGE_API EaseFunc Ease::QuintInOut   = math::EaseQuintInOut;

//-------------------------------------------------------
// ActionTweenEntity
//-------------------------------------------------------

ActionTweenEntity::ActionTweenEntity()
    : dur_()
    , ease_func_(nullptr)
{
}

ActionTweenEntity::ActionTweenEntity(Duration duration)
    : dur_(duration)
    , ease_func_(nullptr)
{
}

void ActionTweenEntity::Update(Actor* target, Duration dt)
{
    float percent;

    if (dur_.IsZero())
    {
        percent = 1.f;
        Complete(target);
    }
    else
    {
        Duration elapsed    = GetElapsed() - GetDelay();
        float    loops_done = elapsed / dur_;

        while (GetLoopsDone() < static_cast<int>(loops_done))
        {
            Complete(target);  // loops_done_++
        }

        percent = (GetStatus() == Status::Done) ? 1.f : (loops_done - static_cast<float>(GetLoopsDone()));
    }

    if (ease_func_)
        percent = ease_func_(percent);

    UpdateTween(target, percent);
}

void ActionTweenEntity::DoClone(ActionTweenEntity* to) const
{
    if (to)
    {
        ActionEntity::DoClone(to);
        to->SetDuration(this->GetDuration());
        to->SetEaseFunc(this->GetEaseFunc());
    }
}

//-------------------------------------------------------
// Move Action
//-------------------------------------------------------

ActionMoveByEntity::ActionMoveByEntity(Duration duration, const Vec2& displacement)
    : ActionTweenEntity(duration)
    , displacement_(displacement)
{
}

void ActionMoveByEntity::Init(Actor* target)
{
    if (target)
    {
        prev_pos_ = start_pos_ = target->GetPosition();
    }
}

void ActionMoveByEntity::UpdateTween(Actor* target, float percent)
{
    Point diff = target->GetPosition() - prev_pos_;
    start_pos_ = start_pos_ + diff;

    Point new_pos = start_pos_ + (displacement_ * percent);
    target->SetPosition(new_pos);

    prev_pos_ = new_pos;
}

ActionMoveByEntity* ActionMoveByEntity::Clone() const
{
    ActionMoveByEntity* ptr = new ActionMoveByEntity(GetDuration(), displacement_);
    DoClone(ptr);
    return ptr;
}

ActionMoveByEntity* ActionMoveByEntity::Reverse() const
{
    ActionMoveByEntity* ptr = new ActionMoveByEntity(GetDuration(), -displacement_);
    DoClone(ptr);
    return ptr;
}

ActionMoveToEntity::ActionMoveToEntity(Duration duration, const Point& distination)
    : ActionMoveByEntity(duration, Vec2())
    , distination_(distination)
{
}

ActionMoveToEntity* ActionMoveToEntity::Clone() const
{
    ActionMoveToEntity* ptr = new ActionMoveToEntity(GetDuration(), distination_);
    DoClone(ptr);
    return ptr;
}

void ActionMoveToEntity::Init(Actor* target)
{
    ActionMoveByEntity::Init(target);
    displacement_ = distination_ - start_pos_;
}

//-------------------------------------------------------
// Jump Action
//-------------------------------------------------------

ActionJumpByEntity::ActionJumpByEntity(Duration duration, const Vec2& displacement, float height, int count)
    : ActionTweenEntity(duration)
    , height_(height)
    , jump_count_(count)
    , displacement_(displacement)
{
}

ActionJumpByEntity* ActionJumpByEntity::Clone() const
{
    ActionJumpByEntity* ptr = new ActionJumpByEntity(GetDuration(), displacement_, height_, jump_count_);
    DoClone(ptr);
    return ptr;
}

ActionJumpByEntity* ActionJumpByEntity::Reverse() const
{
    ActionJumpByEntity* ptr = new ActionJumpByEntity(GetDuration(), -displacement_, height_, jump_count_);
    DoClone(ptr);
    return ptr;
}

void ActionJumpByEntity::Init(Actor* target)
{
    if (target)
    {
        prev_pos_ = start_pos_ = target->GetPosition();
    }
}

void ActionJumpByEntity::UpdateTween(Actor* target, float percent)
{
    float frac = fmod(percent * jump_count_, 1.f);
    float x    = displacement_.x * percent;
    float y    = height_ * 4 * frac * (1 - frac);
    y += displacement_.y * percent;

    Point diff = target->GetPosition() - prev_pos_;
    start_pos_ = diff + start_pos_;

    Point new_pos = start_pos_ + Point(x, y);
    target->SetPosition(new_pos);

    prev_pos_ = new_pos;
}

ActionJumpToEntity::ActionJumpToEntity(Duration duration, const Point& distination, float height, int count)
    : ActionJumpByEntity(duration, Vec2(), height, count)
    , distination_(distination)
{
}

ActionJumpToEntity* ActionJumpToEntity::Clone() const
{
    ActionJumpToEntity* ptr = new ActionJumpToEntity(GetDuration(), distination_, height_, jump_count_);
    DoClone(ptr);
    return ptr;
}

void ActionJumpToEntity::Init(Actor* target)
{
    ActionJumpByEntity::Init(target);
    displacement_ = distination_ - start_pos_;
}

//-------------------------------------------------------
// Scale Action
//-------------------------------------------------------

ActionScaleByEntity::ActionScaleByEntity(Duration duration, float scale_x, float scale_y)
    : ActionTweenEntity(duration)
    , delta_x_(scale_x)
    , delta_y_(scale_y)
    , start_scale_x_(0.f)
    , start_scale_y_(0.f)
{
}

void ActionScaleByEntity::Init(Actor* target)
{
    if (target)
    {
        start_scale_x_ = target->GetScaleX();
        start_scale_y_ = target->GetScaleY();
    }
}

void ActionScaleByEntity::UpdateTween(Actor* target, float percent)
{
    target->SetScale(Vec2{ start_scale_x_ + delta_x_ * percent, start_scale_y_ + delta_y_ * percent });
}

ActionScaleByEntity* ActionScaleByEntity::Clone() const
{
    ActionScaleByEntity* ptr = new ActionScaleByEntity(GetDuration(), delta_x_, delta_y_);
    DoClone(ptr);
    return ptr;
}

ActionScaleByEntity* ActionScaleByEntity::Reverse() const
{
    ActionScaleByEntity* ptr = new ActionScaleByEntity(GetDuration(), -delta_x_, -delta_y_);
    DoClone(ptr);
    return ptr;
}

ActionScaleToEntity::ActionScaleToEntity(Duration duration, float scale_x, float scale_y)
    : ActionScaleByEntity(duration, 0, 0)
    , end_scale_x_(scale_x)
    , end_scale_y_(scale_y)
{
}

ActionScaleToEntity* ActionScaleToEntity::Clone() const
{
    ActionScaleToEntity* ptr = new ActionScaleToEntity(GetDuration(), end_scale_x_, end_scale_y_);
    DoClone(ptr);
    return ptr;
}

void ActionScaleToEntity::Init(Actor* target)
{
    ActionScaleByEntity::Init(target);
    delta_x_ = end_scale_x_ - start_scale_x_;
    delta_y_ = end_scale_y_ - start_scale_y_;
}

//-------------------------------------------------------
// Opacity Action
//-------------------------------------------------------

ActionFadeToEntity::ActionFadeToEntity(Duration duration, float opacity)
    : ActionTweenEntity(duration)
    , delta_val_(0.0f)
    , start_val_(0.f)
    , end_val_(opacity)
{
}

void ActionFadeToEntity::Init(Actor* target)
{
    if (target)
    {
        start_val_ = target->GetOpacity();
        delta_val_ = end_val_ - start_val_;
    }
}

void ActionFadeToEntity::UpdateTween(Actor* target, float percent)
{
    target->SetOpacity(start_val_ + delta_val_ * percent);
}

ActionFadeToEntity* ActionFadeToEntity::Clone() const
{
    ActionFadeToEntity* ptr = new ActionFadeToEntity(GetDuration(), end_val_);
    DoClone(ptr);
    return ptr;
}

//-------------------------------------------------------
// Rotate Action
//-------------------------------------------------------

ActionRotateByEntity::ActionRotateByEntity(Duration duration, float rotation)
    : ActionTweenEntity(duration)
    , delta_val_(rotation)
    , start_val_(0.0f)
{
}

void ActionRotateByEntity::Init(Actor* target)
{
    if (target)
    {
        start_val_ = target->GetRotation();
    }
}

void ActionRotateByEntity::UpdateTween(Actor* target, float percent)
{
    float rotation = start_val_ + delta_val_ * percent;
    if (rotation > 360.f)
        rotation -= 360.f;

    target->SetRotation(rotation);
}

ActionRotateByEntity* ActionRotateByEntity::Clone() const
{
    ActionRotateByEntity* ptr = new ActionRotateByEntity(GetDuration(), delta_val_);
    DoClone(ptr);
    return ptr;
}

ActionRotateByEntity* ActionRotateByEntity::Reverse() const
{
    ActionRotateByEntity* ptr = new ActionRotateByEntity(GetDuration(), -delta_val_);
    DoClone(ptr);
    return ptr;
}

ActionRotateToEntity::ActionRotateToEntity(Duration duration, float rotation)
    : ActionRotateByEntity(duration, 0)
    , end_val_(rotation)
{
}

ActionRotateToEntity* ActionRotateToEntity::Clone() const
{
    ActionRotateToEntity* ptr = new ActionRotateToEntity(GetDuration(), end_val_);
    DoClone(ptr);
    return ptr;
}

void ActionRotateToEntity::Init(Actor* target)
{
    ActionRotateByEntity::Init(target);
    delta_val_ = end_val_ - start_val_;
}

//-------------------------------------------------------
// ActionCustomEntity
//-------------------------------------------------------

ActionCustomEntity::ActionCustomEntity(Duration duration, ActionCustom::TweenFunc tween_func)
    : ActionTweenEntity(duration)
    , tween_func_(tween_func)
{
}

ActionCustomEntity* ActionCustomEntity::Clone() const
{
    ActionCustomEntity* ptr = new ActionCustomEntity(GetDuration(), tween_func_);
    DoClone(ptr);
    return ptr;
}

void ActionCustomEntity::Init(Actor* target)
{
    if (!tween_func_)
        this->Done();
}

void ActionCustomEntity::UpdateTween(Actor* target, float percent)
{
    if (tween_func_)
        tween_func_(target, percent);
}

ActionMoveBy::ActionMoveBy(Duration duration, const Vec2& displacement)
{
    SetEntity(MakePtr<ActionMoveByEntity>(duration, displacement));
}

ActionMoveTo::ActionMoveTo(Duration duration, const Point& distination)
{
    SetEntity(MakePtr<ActionMoveToEntity>(duration, distination));
}

ActionJumpBy::ActionJumpBy(Duration duration, const Vec2& displacement, float height, int count)
{
    SetEntity(MakePtr<ActionJumpByEntity>(duration, displacement, height, count));
}

ActionJumpTo::ActionJumpTo(Duration duration, const Point& distination, float height, int count)
{
    SetEntity(MakePtr<ActionJumpToEntity>(duration, distination, height, count));
}

ActionScaleBy::ActionScaleBy(Duration duration, float scale_x, float scale_y)
{
    SetEntity(MakePtr<ActionScaleByEntity>(duration, scale_x, scale_y));
}

ActionScaleBy::ActionScaleBy(Duration duration, Vec2 scale)
{
    SetEntity(MakePtr<ActionScaleByEntity>(duration, scale.x, scale.y));
}

ActionScaleTo::ActionScaleTo(Duration duration, float scale_x, float scale_y)
{
    SetEntity(MakePtr<ActionScaleToEntity>(duration, scale_x, scale_y));
}

ActionScaleTo::ActionScaleTo(Duration duration, Vec2 scale)
{
    SetEntity(MakePtr<ActionScaleToEntity>(duration, scale.x, scale.y));
}

ActionFadeTo::ActionFadeTo(Duration duration, float opacity)
{
    SetEntity(MakePtr<ActionFadeToEntity>(duration, opacity));
}

ActionFadeIn::ActionFadeIn(Duration duration)
{
    SetEntity(MakePtr<ActionFadeToEntity>(duration, 1.0f));
}

ActionFadeOut::ActionFadeOut(Duration duration)
{
    SetEntity(MakePtr<ActionFadeToEntity>(duration, 0.0f));
}

ActionRotateBy::ActionRotateBy(Duration duration, float rotation)
{
    SetEntity(MakePtr<ActionRotateByEntity>(duration, rotation));
}

ActionRotateTo::ActionRotateTo(Duration duration, float rotation)
{
    SetEntity(MakePtr<ActionRotateToEntity>(duration, rotation));
}

ActionCustom::ActionCustom(Duration duration, TweenFunc tween_func)
{
    SetEntity(MakePtr<ActionCustomEntity>(duration, tween_func));
}

}  // namespace kiwano
