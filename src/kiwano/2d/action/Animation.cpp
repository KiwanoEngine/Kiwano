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

#include "Animation.h"
#include "../FrameSequence.h"
#include "../Sprite.h"

namespace kiwano
{
	Animation::Animation()
		: frame_seq_(nullptr)
	{
	}

	Animation::Animation(Duration duration, FrameSequencePtr frame_seq, EaseFunc func)
		: ActionTween(duration, func)
		, frame_seq_(nullptr)
	{
		this->SetFrameSequence(frame_seq);
	}

	Animation::~Animation()
	{
	}

	FrameSequencePtr Animation::GetFrameSequence() const
	{
		return frame_seq_;
	}

	void Animation::SetFrameSequence(FrameSequencePtr frames)
	{
		frame_seq_ = frames;
	}

	void Animation::Init(ActorPtr target)
	{
		KGE_ASSERT(frame_seq_ && "Animation::Init() failed: FrameSequence is NULL!");
		if (!frame_seq_ || frame_seq_->GetFrames().empty())
		{
			Done();
			return;
		}

		auto sprite_target = dynamic_cast<Sprite*>(target.get());
		KGE_ASSERT(sprite_target && "Animation only supports Sprites!");

		if (sprite_target && frame_seq_)
		{
			sprite_target->SetFrame(frame_seq_->GetFrames()[0]);
		}
	}

	void Animation::UpdateTween(ActorPtr target, Float32 percent)
	{
		auto sprite_target = dynamic_cast<Sprite*>(target.get());

		if (sprite_target && frame_seq_)
		{
			const auto& frames = frame_seq_->GetFrames();
			auto size = frames.size();
			auto index = std::min(static_cast<UInt32>(math::Floor(size * percent)), size - 1);

			sprite_target->SetFrame(frames[index]);
		}
	}

	ActionPtr Animation::Clone() const
	{
		if (frame_seq_)
		{
			return new (std::nothrow) Animation(dur_, frame_seq_, ease_func_);
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
				return new (std::nothrow) Animation(dur_, frames, ease_func_);
			}
		}
		return nullptr;
	}

}
