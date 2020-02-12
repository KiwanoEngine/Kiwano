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
// ActionTween
//-------------------------------------------------------

ActionTween::ActionTween()
    : dur_()
    , ease_func_(nullptr)
{
}

ActionTween::ActionTween(Duration duration, EaseFunc func)
    : dur_(duration)
    , ease_func_(func)
{
}

void ActionTween::Update(Actor* target, Duration dt)
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

ActionPtr ActionTween::InnerClone(ActionTweenPtr to) const
{
    if (to)
    {
        (void)Action::InnerClone(to);
        to->SetDuration(this->GetDuration());
        to->SetEaseFunc(this->GetEaseFunc());
    }
    return to;
}

//-------------------------------------------------------
// Move Action
//-------------------------------------------------------

ActionMoveByPtr ActionMoveBy::Create(Duration duration, Vec2 const& displacement)
{
    ActionMoveByPtr ptr = new (std::nothrow) ActionMoveBy;
    if (ptr)
    {
        ptr->SetDuration(duration);
        ptr->SetDisplacement(displacement);
    }
    return ptr;
}

ActionMoveBy::ActionMoveBy() {}

void ActionMoveBy::Init(Actor* target)
{
    if (target)
    {
        prev_pos_ = start_pos_ = target->GetPosition();
    }
}

void ActionMoveBy::UpdateTween(Actor* target, float percent)
{
    Point diff = target->GetPosition() - prev_pos_;
    start_pos_ = start_pos_ + diff;

    Point new_pos = start_pos_ + (displacement_ * percent);
    target->SetPosition(new_pos);

    prev_pos_ = new_pos;
}

ActionPtr ActionMoveBy::Clone() const
{
    return InnerClone(ActionMoveBy::Create(GetDuration(), displacement_));
}

ActionPtr ActionMoveBy::Reverse() const
{
    return InnerClone(ActionMoveBy::Create(GetDuration(), -displacement_));
}

ActionMoveToPtr ActionMoveTo::Create(Duration duration, Point const& distination)
{
    ActionMoveToPtr ptr = new (std::nothrow) ActionMoveTo;
    if (ptr)
    {
        ptr->SetDuration(duration);
        ptr->SetDistination(distination);
    }
    return ptr;
}

ActionMoveTo::ActionMoveTo() {}

ActionPtr ActionMoveTo::Clone() const
{
    return InnerClone(ActionMoveTo::Create(GetDuration(), distination_));
}

void ActionMoveTo::Init(Actor* target)
{
    ActionMoveBy::Init(target);
    displacement_ = distination_ - start_pos_;
}

//-------------------------------------------------------
// Jump Action
//-------------------------------------------------------

ActionJumpByPtr ActionJumpBy::Create(Duration duration, Vec2 const& displacement, float height, int count,
                                     EaseFunc ease)
{
    ActionJumpByPtr ptr = new (std::nothrow) ActionJumpBy;
    if (ptr)
    {
        ptr->SetDuration(duration);
        ptr->SetEaseFunc(ease);
        ptr->SetJumpHeight(height);
        ptr->SetJumpCount(count);
        ptr->SetDisplacement(displacement);
    }
    return ptr;
}

ActionJumpBy::ActionJumpBy()
    : height_(0.0f)
    , jump_count_(0)
{
}

ActionPtr ActionJumpBy::Clone() const
{
    return InnerClone(ActionJumpBy::Create(GetDuration(), displacement_, height_, jump_count_));
}

ActionPtr ActionJumpBy::Reverse() const
{
    return InnerClone(ActionJumpBy::Create(GetDuration(), -displacement_, height_, jump_count_));
}

void ActionJumpBy::Init(Actor* target)
{
    if (target)
    {
        prev_pos_ = start_pos_ = target->GetPosition();
    }
}

void ActionJumpBy::UpdateTween(Actor* target, float percent)
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

ActionJumpToPtr ActionJumpTo::Create(Duration duration, Point const& distination, float height, int count,
                                     EaseFunc ease)
{
    ActionJumpToPtr ptr = new (std::nothrow) ActionJumpTo;
    if (ptr)
    {
        ptr->SetDuration(duration);
        ptr->SetEaseFunc(ease);
        ptr->SetJumpHeight(height);
        ptr->SetJumpCount(count);
        ptr->SetDistination(distination);
    }
    return ptr;
}

ActionJumpTo::ActionJumpTo() {}

ActionPtr ActionJumpTo::Clone() const
{
    return InnerClone(ActionJumpTo::Create(GetDuration(), distination_, height_, jump_count_));
}

void ActionJumpTo::Init(Actor* target)
{
    ActionJumpBy::Init(target);
    displacement_ = distination_ - start_pos_;
}

//-------------------------------------------------------
// Scale Action
//-------------------------------------------------------

ActionScaleByPtr ActionScaleBy::Create(Duration duration, float scale_x, float scale_y)
{
    ActionScaleByPtr ptr = new (std::nothrow) ActionScaleBy;
    if (ptr)
    {
        ptr->SetDuration(duration);
        ptr->SetScaleX(scale_x);
        ptr->SetScaleY(scale_y);
    }
    return ptr;
}

ActionScaleBy::ActionScaleBy()
    : delta_x_(0.0f)
    , delta_y_(0.0f)
    , start_scale_x_(0.f)
    , start_scale_y_(0.f)
{
}

void ActionScaleBy::Init(Actor* target)
{
    if (target)
    {
        start_scale_x_ = target->GetScaleX();
        start_scale_y_ = target->GetScaleY();
    }
}

void ActionScaleBy::UpdateTween(Actor* target, float percent)
{
    target->SetScale(Vec2{ start_scale_x_ + delta_x_ * percent, start_scale_y_ + delta_y_ * percent });
}

ActionPtr ActionScaleBy::Clone() const
{
    return InnerClone(ActionScaleBy::Create(GetDuration(), delta_x_, delta_y_));
}

ActionPtr ActionScaleBy::Reverse() const
{
    return InnerClone(ActionScaleBy::Create(GetDuration(), -delta_x_, -delta_y_));
}

ActionScaleToPtr ActionScaleTo::Create(Duration duration, float scale_x, float scale_y)
{
    ActionScaleToPtr ptr = new (std::nothrow) ActionScaleTo;
    if (ptr)
    {
        ptr->SetDuration(duration);
        ptr->SetTargetScaleX(scale_x);
        ptr->SetTargetScaleY(scale_y);
    }
    return ptr;
}

ActionScaleTo::ActionScaleTo()
    : end_scale_x_(0.0f)
    , end_scale_y_(0.0f)
{
}

ActionPtr ActionScaleTo::Clone() const
{
    return InnerClone(ActionScaleTo::Create(GetDuration(), end_scale_x_, end_scale_y_));
}

void ActionScaleTo::Init(Actor* target)
{
    ActionScaleBy::Init(target);
    delta_x_ = end_scale_x_ - start_scale_x_;
    delta_y_ = end_scale_y_ - start_scale_y_;
}

//-------------------------------------------------------
// Opacity Action
//-------------------------------------------------------

ActionFadeToPtr ActionFadeTo::Create(Duration duration, float opacity)
{
    ActionFadeToPtr ptr = new (std::nothrow) ActionFadeTo;
    if (ptr)
    {
        ptr->SetDuration(duration);
        ptr->SetTargetOpacity(opacity);
    }
    return ptr;
}

ActionFadeTo::ActionFadeTo()
    : delta_val_(0.0f)
    , start_val_(0.f)
    , end_val_(0.0f)
{
}

void ActionFadeTo::Init(Actor* target)
{
    if (target)
    {
        start_val_ = target->GetOpacity();
        delta_val_ = end_val_ - start_val_;
    }
}

void ActionFadeTo::UpdateTween(Actor* target, float percent)
{
    target->SetOpacity(start_val_ + delta_val_ * percent);
}

ActionPtr ActionFadeTo::Clone() const
{
    return InnerClone(ActionFadeTo::Create(GetDuration(), end_val_));
}

ActionFadeInPtr ActionFadeIn::Create(Duration duration)
{
    ActionFadeInPtr ptr = new (std::nothrow) ActionFadeIn;
    if (ptr)
    {
        ptr->SetDuration(duration);
        ptr->SetTargetOpacity(1.0f);
    }
    return ptr;
}

ActionFadeOutPtr ActionFadeOut::Create(Duration duration)
{
    ActionFadeOutPtr ptr = new (std::nothrow) ActionFadeOut;
    if (ptr)
    {
        ptr->SetDuration(duration);
        ptr->SetTargetOpacity(0.0f);
    }
    return ptr;
}

//-------------------------------------------------------
// Rotate Action
//-------------------------------------------------------

ActionRotateByPtr ActionRotateBy::Create(Duration duration, float rotation)
{
    ActionRotateByPtr ptr = new (std::nothrow) ActionRotateBy;
    if (ptr)
    {
        ptr->SetDuration(duration);
        ptr->SetRotation(rotation);
    }
    return ptr;
}

ActionRotateBy::ActionRotateBy()
    : start_val_(0.0f)
    , delta_val_(0.0f)
{
}

void ActionRotateBy::Init(Actor* target)
{
    if (target)
    {
        start_val_ = target->GetRotation();
    }
}

void ActionRotateBy::UpdateTween(Actor* target, float percent)
{
    float rotation = start_val_ + delta_val_ * percent;
    if (rotation > 360.f)
        rotation -= 360.f;

    target->SetRotation(rotation);
}

ActionPtr ActionRotateBy::Clone() const
{
    return InnerClone(ActionRotateBy::Create(GetDuration(), delta_val_));
}

ActionPtr ActionRotateBy::Reverse() const
{
    return InnerClone(ActionRotateBy::Create(GetDuration(), -delta_val_));
}

ActionRotateToPtr ActionRotateTo::Create(Duration duration, float rotation)
{
    ActionRotateToPtr ptr = new (std::nothrow) ActionRotateTo;
    if (ptr)
    {
        ptr->SetDuration(duration);
        ptr->SetTargetRotation(rotation);
    }
    return ptr;
}

ActionRotateTo::ActionRotateTo()
    : end_val_(0.0f)
{
}

ActionPtr ActionRotateTo::Clone() const
{
    return InnerClone(ActionRotateTo::Create(GetDuration(), end_val_));
}

void ActionRotateTo::Init(Actor* target)
{
    ActionRotateBy::Init(target);
    delta_val_ = end_val_ - start_val_;
}

//-------------------------------------------------------
// ActionCustom
//-------------------------------------------------------

ActionCustomPtr ActionCustom::Create(Duration duration, TweenFunc tween_func)
{
    ActionCustomPtr ptr = new (std::nothrow) ActionCustom;
    if (ptr)
    {
        ptr->SetDuration(duration);
        ptr->SetTweenFunc(tween_func);
    }
    return ptr;
}

ActionCustom::ActionCustom() {}

ActionPtr ActionCustom::Clone() const
{
    return InnerClone(ActionCustom::Create(GetDuration(), tween_func_));
}

void ActionCustom::Init(Actor* target)
{
    if (!tween_func_)
        this->Done();
}

void ActionCustom::UpdateTween(Actor* target, float percent)
{
    if (tween_func_)
        tween_func_(target, percent);
}

}  // namespace kiwano
