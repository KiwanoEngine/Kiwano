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
#include <kiwano/2d/animation/Animation.h>

namespace kiwano
{
Animation::Animation()
    : running_(true)
    , detach_target_(false)
    , loops_done_(0)
    , loops_(0)
    , status_(Status::NotStarted)
{
}

Animation::~Animation() {}

void Animation::Init(Actor* target) {}

void Animation::Update(Actor* target, Duration dt)
{
    Complete(target);
}

void Animation::UpdateStep(Actor* target, Duration dt)
{
    KGE_ASSERT(target != nullptr && "Animation target should NOT be nullptr!");

    elapsed_ += dt;

    if (status_ == Status::NotStarted)
    {
        if (delay_.IsZero())
        {
            status_ = Status::Started;
            EmitEvent(target, AnimationEvent::Started);
        }
        else
        {
            status_ = Status::Delayed;
        }
        Init(target);
    }

    switch (status_)
    {
    case Status::Delayed:
        if (elapsed_ >= delay_)
        {
            status_ = Status::Started;
            EmitEvent(target, AnimationEvent::Started);
        }
        break;

    case Status::Started:
        Update(target, dt);
        break;

    default:
        break;
    }

    if (status_ == Status::Done)
    {
        EmitEvent(target, AnimationEvent::Done);

        if (detach_target_)
            target->RemoveFromParent();

        status_ = Status::Removeable;
    }
}

void Animation::Complete(Actor* target)
{
    EmitEvent(target, AnimationEvent::LoopDone);

    if (loops_ >= 0 && loops_done_ >= loops_)
    {
        Done();
    }
    else
    {
        Init(target);  // reinit when a loop is done
    }

    ++loops_done_;
}

void Animation::Reset()
{
    status_     = Status::NotStarted;
    elapsed_    = 0;
    loops_done_ = 0;
}

void Animation::DoClone(Animation* to) const
{
    if (to)
    {
        to->SetDelay(this->GetDelay());
        to->SetHandler(this->GetHandler());
        to->SetLoops(this->GetLoops());
        to->SetName(this->GetName());
    }
}

}  // namespace kiwano
