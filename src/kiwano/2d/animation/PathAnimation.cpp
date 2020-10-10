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

#include <kiwano/2d/animation/PathAnimation.h>
#include <kiwano/2d/Actor.h>

namespace kiwano
{

PathAnimation::PathAnimation(Duration duration, ShapePtr path, bool rotating, float start, float end)
    : TweenAnimation(duration)
    , start_(start)
    , end_(end)
    , rotating_(rotating)
    , length_(0.f)
    , path_(path)
{
}

PathAnimation* PathAnimation::Clone() const
{
    PathAnimation* ptr = new PathAnimation(GetDuration(), path_, rotating_, start_, end_);
    DoClone(ptr);
    return ptr;
}

PathAnimation* PathAnimation::Reverse() const
{
    PathAnimation* ptr = new PathAnimation(GetDuration(), path_, rotating_, end_, start_);
    DoClone(ptr);
    return ptr;
}

void PathAnimation::Init(Actor* target)
{
    if (!path_ || !path_->IsValid())
    {
        Done();
        return;
    }

    start_pos_ = target->GetPosition();
    length_    = path_->GetLength();
}

void PathAnimation::UpdateTween(Actor* target, float percent)
{
    float distance = length_ * std::min(std::max((end_ - start_) * percent + start_, 0.f), 1.f);

    Point point, tangent;
    if (path_->ComputePointAtLength(distance, point, tangent))
    {
        target->SetPosition(start_pos_ + point);

        if (rotating_)
        {
            float ac       = math::Acos(tangent.x);
            float rotation = (tangent.y < 0.f) ? 360.f - ac : ac;
            target->SetRotation(rotation);
        }
    }
}

}  // namespace kiwano
