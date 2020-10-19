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
#include <kiwano/2d/animation/TweenAnimation.h>

namespace kiwano
{

//-------------------------------------------------------
// TweenAnimation
//-------------------------------------------------------

TweenAnimation::TweenAnimation()
    : dur_()
    , ease_func_(nullptr)
{
}

TweenAnimation::TweenAnimation(Duration duration)
    : dur_(duration)
    , ease_func_(nullptr)
{
}

float TweenAnimation::Interpolate(float frac)
{
    if (frac == 1)
        return 1;
    if (ease_func_)
        frac = ease_func_(frac);
    return frac;
}

void TweenAnimation::Update(Actor* target, Duration dt)
{
    float frac;

    if (dur_.IsZero())
    {
        frac = 1.f;
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

        frac = (GetStatus() == Status::Done) ? 1.f : (loops_done - static_cast<float>(GetLoopsDone()));
    }

    frac = Interpolate(frac);

    UpdateTween(target, frac);
}

void TweenAnimation::DoClone(TweenAnimation* to) const
{
    if (to)
    {
        Animation::DoClone(to);
        to->SetDuration(this->GetDuration());
        to->SetEaseFunc(this->GetEaseFunc());
    }
}

//-------------------------------------------------------
// Move Animation
//-------------------------------------------------------

MoveByAnimation::MoveByAnimation(Duration duration, const Vec2& displacement)
    : TweenAnimation(duration)
    , displacement_(displacement)
{
}

void MoveByAnimation::Init(Actor* target)
{
    if (target)
    {
        prev_pos_ = start_pos_ = target->GetPosition();
    }
}

void MoveByAnimation::UpdateTween(Actor* target, float frac)
{
    Point diff = target->GetPosition() - prev_pos_;
    start_pos_ = start_pos_ + diff;

    Point new_pos = start_pos_ + (displacement_ * frac);
    target->SetPosition(new_pos);

    prev_pos_ = new_pos;
}

MoveByAnimation* MoveByAnimation::Clone() const
{
    MoveByAnimation* ptr = new MoveByAnimation(GetDuration(), displacement_);
    DoClone(ptr);
    return ptr;
}

MoveByAnimation* MoveByAnimation::Reverse() const
{
    MoveByAnimation* ptr = new MoveByAnimation(GetDuration(), -displacement_);
    DoClone(ptr);
    return ptr;
}

MoveToAnimation::MoveToAnimation(Duration duration, const Point& distination)
    : MoveByAnimation(duration, Vec2())
    , distination_(distination)
{
}

MoveToAnimation* MoveToAnimation::Clone() const
{
    MoveToAnimation* ptr = new MoveToAnimation(GetDuration(), distination_);
    DoClone(ptr);
    return ptr;
}

void MoveToAnimation::Init(Actor* target)
{
    MoveByAnimation::Init(target);
    displacement_ = distination_ - start_pos_;
}

//-------------------------------------------------------
// Jump Animation
//-------------------------------------------------------

JumpByAnimation::JumpByAnimation(Duration duration, const Vec2& displacement, float height, int count)
    : TweenAnimation(duration)
    , height_(height)
    , jump_count_(count)
    , displacement_(displacement)
{
}

JumpByAnimation* JumpByAnimation::Clone() const
{
    JumpByAnimation* ptr = new JumpByAnimation(GetDuration(), displacement_, height_, jump_count_);
    DoClone(ptr);
    return ptr;
}

JumpByAnimation* JumpByAnimation::Reverse() const
{
    JumpByAnimation* ptr = new JumpByAnimation(GetDuration(), -displacement_, height_, jump_count_);
    DoClone(ptr);
    return ptr;
}

void JumpByAnimation::Init(Actor* target)
{
    if (target)
    {
        prev_pos_ = start_pos_ = target->GetPosition();
    }
}

void JumpByAnimation::UpdateTween(Actor* target, float frac)
{
    float f = ::fmod(frac * jump_count_, 1.f);
    float x = displacement_.x * frac;
    float y = height_ * 4 * f * (1 - f);
    y += displacement_.y * frac;

    Point diff = target->GetPosition() - prev_pos_;
    start_pos_ = diff + start_pos_;

    Point new_pos = start_pos_ + Point(x, y);
    target->SetPosition(new_pos);

    prev_pos_ = new_pos;
}

JumpToAnimation::JumpToAnimation(Duration duration, const Point& distination, float height, int count)
    : JumpByAnimation(duration, Vec2(), height, count)
    , distination_(distination)
{
}

JumpToAnimation* JumpToAnimation::Clone() const
{
    JumpToAnimation* ptr = new JumpToAnimation(GetDuration(), distination_, height_, jump_count_);
    DoClone(ptr);
    return ptr;
}

void JumpToAnimation::Init(Actor* target)
{
    JumpByAnimation::Init(target);
    displacement_ = distination_ - start_pos_;
}

//-------------------------------------------------------
// Scale Animation
//-------------------------------------------------------

ScaleByAnimation::ScaleByAnimation(Duration duration, const Vec2& scale)
    : TweenAnimation(duration)
    , delta_(scale)
    , start_val_()
{
}

void ScaleByAnimation::Init(Actor* target)
{
    if (target)
    {
        start_val_ = target->GetScale();
    }
}

void ScaleByAnimation::UpdateTween(Actor* target, float frac)
{
    target->SetScale(Vec2{ start_val_.x + delta_.x * frac, start_val_.y + delta_.y * frac });
}

ScaleByAnimation* ScaleByAnimation::Clone() const
{
    ScaleByAnimation* ptr = new ScaleByAnimation(GetDuration(), delta_);
    DoClone(ptr);
    return ptr;
}

ScaleByAnimation* ScaleByAnimation::Reverse() const
{
    ScaleByAnimation* ptr = new ScaleByAnimation(GetDuration(), -delta_);
    DoClone(ptr);
    return ptr;
}

ScaleToAnimation::ScaleToAnimation(Duration duration, const Vec2& scale)
    : ScaleByAnimation(duration, Vec2())
    , end_val_(scale)
{
}

ScaleToAnimation* ScaleToAnimation::Clone() const
{
    ScaleToAnimation* ptr = new ScaleToAnimation(GetDuration(), end_val_);
    DoClone(ptr);
    return ptr;
}

void ScaleToAnimation::Init(Actor* target)
{
    ScaleByAnimation::Init(target);
    delta_ = end_val_ - start_val_;
}

//-------------------------------------------------------
// Opacity Animation
//-------------------------------------------------------

FadeToAnimation::FadeToAnimation(Duration duration, float opacity)
    : TweenAnimation(duration)
    , delta_val_(0.0f)
    , start_val_(0.f)
    , end_val_(opacity)
{
}

void FadeToAnimation::Init(Actor* target)
{
    if (target)
    {
        start_val_ = target->GetOpacity();
        delta_val_ = end_val_ - start_val_;
    }
}

void FadeToAnimation::UpdateTween(Actor* target, float frac)
{
    target->SetOpacity(start_val_ + delta_val_ * frac);
}

FadeToAnimation* FadeToAnimation::Clone() const
{
    FadeToAnimation* ptr = new FadeToAnimation(GetDuration(), end_val_);
    DoClone(ptr);
    return ptr;
}

//-------------------------------------------------------
// Rotate Animation
//-------------------------------------------------------

RotateByAnimation::RotateByAnimation(Duration duration, float rotation)
    : TweenAnimation(duration)
    , delta_val_(rotation)
    , start_val_(0.0f)
{
}

void RotateByAnimation::Init(Actor* target)
{
    if (target)
    {
        start_val_ = target->GetRotation();
    }
}

void RotateByAnimation::UpdateTween(Actor* target, float frac)
{
    float rotation = start_val_ + delta_val_ * frac;
    if (rotation > 360.f)
        rotation -= 360.f;

    target->SetRotation(rotation);
}

RotateByAnimation* RotateByAnimation::Clone() const
{
    RotateByAnimation* ptr = new RotateByAnimation(GetDuration(), delta_val_);
    DoClone(ptr);
    return ptr;
}

RotateByAnimation* RotateByAnimation::Reverse() const
{
    RotateByAnimation* ptr = new RotateByAnimation(GetDuration(), -delta_val_);
    DoClone(ptr);
    return ptr;
}

RotateToAnimation::RotateToAnimation(Duration duration, float rotation)
    : RotateByAnimation(duration, 0)
    , end_val_(rotation)
{
}

RotateToAnimation* RotateToAnimation::Clone() const
{
    RotateToAnimation* ptr = new RotateToAnimation(GetDuration(), end_val_);
    DoClone(ptr);
    return ptr;
}

void RotateToAnimation::Init(Actor* target)
{
    RotateByAnimation::Init(target);
    delta_val_ = end_val_ - start_val_;
}

}  // namespace kiwano
