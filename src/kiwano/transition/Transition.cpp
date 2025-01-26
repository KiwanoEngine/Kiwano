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

#include <kiwano/transition/Transition.h>
#include <kiwano/render/Renderer.h>

namespace kiwano
{

Transition::Transition()
    : done_(false)
    , duration_()
    , delta_()
    , process_(0)
    , window_size_()
    , out_stage_(nullptr)
    , in_stage_(nullptr)
    , out_layer_(nullptr)
    , in_layer_(nullptr)
{
}

Transition::~Transition() {}

bool Transition::IsDone() const
{
    return done_;
}

void Transition::Init(Stage* prev, Stage* next)
{
    process_ = 0;
    delta_   = Duration{};

    out_stage_   = prev;
    in_stage_    = next;
    window_size_ = Renderer::GetInstance().GetOutputSize();

    if (in_stage_)
    {
        in_layer_.bounds = Rect{ Point(), window_size_ };
    }

    if (out_stage_)
    {
        out_layer_.bounds = Rect{ Point(), window_size_ };
    }
}

void Transition::Update(Duration dt)
{
    if (duration_.IsZero())
    {
        process_ = 1;
    }
    else
    {
        delta_ += dt;
        process_ = std::min(delta_ / duration_, 1.f);
    }

    UpdateSelf(dt);

    if (process_ >= 1)
    {
        this->Stop();
        return;
    }

    if (out_stage_)
    {
        Renderer::GetInstance().PushRenderGroup(out_layer_);
        out_stage_->Update(dt);
        Renderer::GetInstance().PopRenderGroup();
    }

    if (in_stage_)
    {
        Renderer::GetInstance().PushRenderGroup(in_layer_);
        in_stage_->Update(dt);
        Renderer::GetInstance().PopRenderGroup();
    }
}

void Transition::UpdateSelf(Duration dt) {}

void Transition::Stop()
{
    done_ = true;
    Reset();
}

}  // namespace kiwano
