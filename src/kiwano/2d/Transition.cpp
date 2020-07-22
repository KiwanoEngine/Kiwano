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
#include <kiwano/2d/Transition.h>
#include <kiwano/utils/Logger.h>
#include <kiwano/platform/Window.h>
#include <kiwano/render/Renderer.h>

namespace kiwano
{
//-------------------------------------------------------
// Transition
//-------------------------------------------------------

Transition::Transition()
    : done_(false)
    , duration_()
    , delta_()
    , process_(0)
    , window_size_()
    , out_stage_(nullptr)
    , in_stage_(nullptr)
    , out_layer_()
    , in_layer_()
{
}

Transition::~Transition() {}

bool Transition::IsDone()
{
    return done_;
}

void Transition::Init(StagePtr prev, StagePtr next)
{
    process_ = 0;
    delta_   = Duration{};

    out_stage_   = prev;
    in_stage_    = next;
    window_size_ = Renderer::GetInstance().GetOutputSize();

    if (in_stage_)
    {
        in_layer_.SetClipRect(Rect{ Point(), window_size_ });
    }

    if (out_stage_)
    {
        out_layer_.SetClipRect(Rect{ Point(), window_size_ });
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

    if (process_ >= 1)
    {
        this->Stop();
    }
}

void Transition::Render(RenderContext& ctx)
{
    if (out_stage_)
    {
        out_stage_->PrepareToRender(ctx);
        ctx.PushClipRect(Rect{ Point{}, window_size_ });
        ctx.PushLayer(out_layer_);

        out_stage_->Render(ctx);

        ctx.PopLayer();
        ctx.PopClipRect();
    }

    if (in_stage_)
    {
        in_stage_->PrepareToRender(ctx);
        ctx.PushClipRect(Rect{ Point{}, window_size_ });
        ctx.PushLayer(in_layer_);

        in_stage_->Render(ctx);

        ctx.PopLayer();
        ctx.PopClipRect();
    }
}

void Transition::Stop()
{
    done_ = true;
    Reset();
}

//-------------------------------------------------------
// BoxTransition
//-------------------------------------------------------

BoxTransitionPtr BoxTransition::Create(Duration duration)
{
    BoxTransitionPtr ptr = new (autogc) BoxTransition;
    if (ptr)
    {
        ptr->SetDuration(duration);
    }
    return ptr;
}

BoxTransition::BoxTransition() {}

void BoxTransition::Init(StagePtr prev, StagePtr next)
{
    Transition::Init(prev, next);

    in_layer_.SetOpacity(0.f);
}

void BoxTransition::Update(Duration dt)
{
    Transition::Update(dt);

    if (process_ < .5f)
    {
        out_layer_.SetClipRect(Rect(window_size_.x * process_, window_size_.y * process_,
                                    window_size_.x * (1 - process_), window_size_.y * (1 - process_)));
    }
    else
    {
        out_layer_.SetOpacity(0.f);
        in_layer_.SetOpacity(1.f);
        in_layer_.SetClipRect(Rect(window_size_.x * (1 - process_), window_size_.y * (1 - process_),
                                   window_size_.x * process_, window_size_.y * process_));
    }
}

//-------------------------------------------------------
// EmergeTransition
//-------------------------------------------------------

EmergeTransitionPtr EmergeTransition::Create(Duration duration)
{
    EmergeTransitionPtr ptr = new (autogc) EmergeTransition;
    if (ptr)
    {
        ptr->SetDuration(duration);
    }
    return ptr;
}

EmergeTransition::EmergeTransition() {}

void EmergeTransition::Init(StagePtr prev, StagePtr next)
{
    Transition::Init(prev, next);

    out_layer_.SetOpacity(1.f);
    in_layer_.SetOpacity(0.f);
}

void EmergeTransition::Update(Duration dt)
{
    Transition::Update(dt);

    out_layer_.SetOpacity(1 - process_);
    in_layer_.SetOpacity(process_);
}

//-------------------------------------------------------
// FadeTransition
//-------------------------------------------------------

FadeTransitionPtr FadeTransition::Create(Duration duration)
{
    FadeTransitionPtr ptr = new (autogc) FadeTransition;
    if (ptr)
    {
        ptr->SetDuration(duration);
    }
    return ptr;
}

FadeTransition::FadeTransition() {}

void FadeTransition::Init(StagePtr prev, StagePtr next)
{
    Transition::Init(prev, next);

    out_layer_.SetOpacity(1.f);
    in_layer_.SetOpacity(0.f);
}

void FadeTransition::Update(Duration dt)
{
    Transition::Update(dt);

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

//-------------------------------------------------------
// MoveTransition
//-------------------------------------------------------

MoveTransitionPtr MoveTransition::Create(Duration duration, Type type)
{
    MoveTransitionPtr ptr = new (autogc) MoveTransition;
    if (ptr)
    {
        ptr->type_ = type;
        ptr->SetDuration(duration);
    }
    return ptr;
}

MoveTransition::MoveTransition()
    : type_(Type::Left)
{
}

void MoveTransition::Init(StagePtr prev, StagePtr next)
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

void MoveTransition::Update(Duration dt)
{
    Transition::Update(dt);

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

//-------------------------------------------------------
// RotationTransition
//-------------------------------------------------------

RotationTransitionPtr RotationTransition::Create(Duration duration, float rotation)
{
    RotationTransitionPtr ptr = new (autogc) RotationTransition;
    if (ptr)
    {
        ptr->rotation_ = rotation;
        ptr->SetDuration(duration);
    }
    return ptr;
}

RotationTransition::RotationTransition()
    : rotation_(0.0f)
{
}

void RotationTransition::Init(StagePtr prev, StagePtr next)
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

    in_layer_.SetOpacity(0.f);
}

void RotationTransition::Update(Duration dt)
{
    Transition::Update(dt);

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
            out_layer_.SetOpacity(0.f);
            in_layer_.SetOpacity(1.f);

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
