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

#include <kiwano/2d/FrameSequence.h>
#include <kiwano/2d/Sprite.h>
#include <kiwano/2d/action/Animation.h>

namespace kiwano
{

AnimationPtr Animation::Create(Duration duration, FrameSequencePtr frame_seq)
{
    AnimationPtr ptr = memory::New<Animation>();
    if (ptr)
    {
        ptr->SetDuration(duration);
        ptr->SetFrameSequence(frame_seq);
    }
    return ptr;
}

Animation::Animation()
    : frame_seq_(nullptr)
{
}

Animation::~Animation() {}

FrameSequencePtr Animation::GetFrameSequence() const
{
    return frame_seq_;
}

void Animation::SetFrameSequence(FrameSequencePtr frame_seq)
{
    frame_seq_ = frame_seq;
}

void Animation::Init(Actor* target)
{
    KGE_ASSERT(frame_seq_ && "Animation::Init() failed: FrameSequence is NULL!");
    if (!frame_seq_ || frame_seq_->GetFrames().empty())
    {
        Done();
        return;
    }

    auto sprite_target = dynamic_cast<Sprite*>(target);
    KGE_ASSERT(sprite_target && "Animation only supports Sprites!");

    if (sprite_target && frame_seq_)
    {
        sprite_target->SetFrame(frame_seq_->GetFrames()[0]);
    }
}

void Animation::UpdateTween(Actor* target, float percent)
{
    auto sprite_target = dynamic_cast<Sprite*>(target);

    if (sprite_target && frame_seq_)
    {
        const auto& frames = frame_seq_->GetFrames();
        auto        size   = frames.size();
        auto        index  = std::min(static_cast<size_t>(math::Floor(size * percent)), size - 1);

        sprite_target->SetFrame(frames[index]);
    }
}

ActionPtr Animation::Clone() const
{
    if (frame_seq_)
    {
        return InnerClone(Animation::Create(GetDuration(), frame_seq_));
    }
    return nullptr;
}

ActionPtr Animation::Reverse() const
{
    if (frame_seq_)
    {
        FrameSequencePtr frames = frame_seq_->Reverse();
        if (frames)
        {
            return InnerClone(Animation::Create(GetDuration(), frames));
        }
    }
    return nullptr;
}

}  // namespace kiwano
