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

#include <kiwano/2d/transition/BoxTransition.h>

namespace kiwano
{

BoxTransition::BoxTransition(Duration duration)
{
    SetDuration(duration);
}

void BoxTransition::Init(Stage* prev, Stage* next)
{
    Transition::Init(prev, next);

    in_layer_.opacity = 0.f;
}

void BoxTransition::Update(Duration dt)
{
    Transition::Update(dt);

    if (process_ < .5f)
    {
        out_layer_.bounds = Rect(window_size_.x * process_, window_size_.y * process_, window_size_.x * (1 - process_),
                                 window_size_.y * (1 - process_));
    }
    else
    {
        out_layer_.opacity = 0.f;
        in_layer_.opacity  = 1.f;
        in_layer_.bounds   = Rect(window_size_.x * (1 - process_), window_size_.y * (1 - process_),
                                  window_size_.x * process_, window_size_.y * process_);
    }
}

}  // namespace kiwano
