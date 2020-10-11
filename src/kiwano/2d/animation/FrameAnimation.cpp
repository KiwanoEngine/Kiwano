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

#include <kiwano/2d/animation/FrameAnimation.h>
#include <kiwano/2d/Sprite.h>

namespace kiwano
{

FrameAnimation::FrameAnimation()
    : frame_seq_(nullptr)
    , current_index_(0)
{
}

FrameAnimation::FrameAnimation(Duration dur, FrameSequencePtr frame_seq)
    : TweenAnimation(dur)
    , frame_seq_(frame_seq)
    , current_index_(0)
{
}

FrameAnimation::~FrameAnimation() {}

FrameSequencePtr FrameAnimation::GetFrameSequence() const
{
    return frame_seq_;
}

void FrameAnimation::SetFrameSequence(FrameSequencePtr frame_seq)
{
    frame_seq_ = frame_seq;
}

void FrameAnimation::Init(Actor* target)
{
    KGE_ASSERT(frame_seq_ && "FrameAnimation::Init() failed: FrameSequence is NULL!");
    if (!frame_seq_ || frame_seq_->GetFrames().empty())
    {
        Done();
        return;
    }

    auto sprite_target = dynamic_cast<Sprite*>(target);
    KGE_ASSERT(sprite_target && "FrameAnimation only supports Sprites!");

    if (sprite_target && frame_seq_)
    {
        sprite_target->SetFrame(frame_seq_->GetFrames()[0]);
        current_index_ = 0;
    }
}

void FrameAnimation::UpdateTween(Actor* target, float percent)
{
    auto sprite_target = dynamic_cast<Sprite*>(target);

    if (sprite_target && frame_seq_)
    {
        const auto& frames = frame_seq_->GetFrames();
        auto        size   = frames.size();
        auto        index  = std::min(static_cast<size_t>(math::Floor(size * percent)), size - 1);

        if (index != current_index_)
        {
            current_index_ = index;
            sprite_target->SetFrame(frames[index]);
        }
    }
}

FrameAnimation* FrameAnimation::Clone() const
{
    FrameAnimation* ptr = new FrameAnimation(GetDuration(), frame_seq_);
    DoClone(ptr);
    return ptr;
}

FrameAnimation* FrameAnimation::Reverse() const
{
    FrameAnimation* ptr = new FrameAnimation(GetDuration(), nullptr);
    DoClone(ptr);

    if (frame_seq_)
    {
        FrameSequencePtr frames = frame_seq_->Reverse();
        ptr->SetFrameSequence(frames);
    }
    return ptr;
}

}  // namespace kiwano
