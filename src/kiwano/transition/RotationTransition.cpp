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

#include <kiwano/transition/RotationTransition.h>

namespace kiwano
{

RotationTransition::RotationTransition(Duration duration, float rotation)
    : rotation_(rotation)
{
    SetDuration(duration);
}

void RotationTransition::Init(Stage* prev, Stage* next)
{
    Transition::Init(prev, next);

    auto transform     = Transform{};
    transform.position = Point{ window_size_.x / 2, window_size_.y / 2 };

    if (out_stage_)
    {
        out_stage_->SetTransform(transform);
        out_stage_->SetAnchor(Vec2{ 0.5f, 0.5f });
    }

    if (in_stage_)
    {
        in_stage_->SetTransform(transform);
        in_stage_->SetAnchor(Vec2{ 0.5f, 0.5f });
    }

    in_layer_.opacity = 0.f;
}

void RotationTransition::UpdateSelf(Duration dt)
{
    if (process_ < .5f)
    {
        if (out_stage_)
        {
            auto transform     = out_stage_->GetTransform();
            transform.scale    = Point{ (.5f - process_) * 2, (.5f - process_) * 2 };
            transform.rotation = rotation_ * (.5f - process_) * 2;
            out_stage_->SetTransform(transform);
        }
    }
    else
    {
        if (in_stage_)
        {
            out_layer_.opacity = 0.f;
            in_layer_.opacity  = 1.f;

            auto transform     = in_stage_->GetTransform();
            transform.scale    = Point{ (process_ - .5f) * 2, (process_ - .5f) * 2 };
            transform.rotation = rotation_ * (process_ - .5f) * 2;

            in_stage_->SetTransform(transform);
        }
    }
}

void RotationTransition::Reset()
{
    if (out_stage_)
    {
        out_stage_->SetTransform(Transform{});
        out_stage_->SetAnchor(Vec2{ 0.f, 0.f });
    }

    if (in_stage_)
    {
        in_stage_->SetTransform(Transform{});
        in_stage_->SetAnchor(Vec2{ 0.f, 0.f });
    }
}

}  // namespace kiwano
