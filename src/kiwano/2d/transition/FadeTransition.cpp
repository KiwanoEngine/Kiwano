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

#include <kiwano/2d/transition/FadeTransition.h>

namespace kiwano
{

FadeTransition::FadeTransition(Duration duration, bool parallel)
    : parallel_(parallel)
{
    SetDuration(duration);
}

FadeTransition::FadeTransition() {}

void FadeTransition::Init(Stage* prev, Stage* next)
{
    Transition::Init(prev, next);

    out_layer_.SetOpacity(1.f);
    in_layer_.SetOpacity(0.f);
}

void FadeTransition::Update(Duration dt)
{
    Transition::Update(dt);

    if (parallel_)
    {
        out_layer_.SetOpacity(1 - process_);
        in_layer_.SetOpacity(process_);
    }
    else
    {
        if (process_ < 0.5)
        {
            out_layer_.SetOpacity(1 - process_ * 2);
            in_layer_.SetOpacity(0.f);
        }
        else
        {
            out_layer_.SetOpacity(0.f);
            in_layer_.SetOpacity((process_ - 0.5f) * 2);
        }
    }
}

}  // namespace kiwano
