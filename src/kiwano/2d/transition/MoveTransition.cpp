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

#include <kiwano/2d/transition/MoveTransition.h>

namespace kiwano
{

MoveTransition::MoveTransition(Duration duration, Type type)
    : type_(type)
{
    SetDuration(duration);
}

void MoveTransition::Init(Stage* prev, Stage* next)
{
    Transition::Init(prev, next);

    switch (type_)
    {
    case Type::Up:
        pos_delta_ = Point(0, -window_size_.y);
        start_pos_ = Point(0, window_size_.y);
        break;
    case Type::Down:
        pos_delta_ = Point(0, window_size_.y);
        start_pos_ = Point(0, -window_size_.y);
        break;
    case Type::Left:
        pos_delta_ = Point(-window_size_.x, 0);
        start_pos_ = Point(window_size_.x, 0);
        break;
    case Type::Right:
        pos_delta_ = Point(window_size_.x, 0);
        start_pos_ = Point(-window_size_.x, 0);
        break;
    }

    if (out_stage_)
    {
        out_stage_->SetTransform(Transform{});
    }

    if (in_stage_)
    {
        auto transform     = Transform{};
        transform.position = start_pos_;
        in_stage_->SetTransform(transform);
    }
}

void MoveTransition::UpdateSelf(Duration dt)
{
    if (out_stage_)
    {
        auto transform     = Transform{};
        transform.position = pos_delta_ * process_;
        out_stage_->SetTransform(transform);
    }

    if (in_stage_)
    {
        auto transform     = Transform{};
        transform.position = start_pos_ + pos_delta_ * process_;
        in_stage_->SetTransform(transform);
    }
}

void MoveTransition::Reset()
{
    if (out_stage_)
    {
        out_stage_->SetTransform(Transform{});
    }

    if (in_stage_)
    {
        in_stage_->SetTransform(Transform{});
    }
}

}  // namespace kiwano
