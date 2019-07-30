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
#include "Frames.h"
#include "Image.h"
#include "Sprite.h"

namespace kiwano
{
	Animation::Animation()
		: frames_(nullptr)
	{
	}

	Animation::Animation(Duration duration, FramesPtr animation, EaseFunc func)
		: ActionTween(duration, func)
		, frames_(nullptr)
	{
		this->SetFrames(animation);
	}

	Animation::~Animation()
	{
	}

	FramesPtr Animation::GetFrames() const
	{
		return frames_;
	}

	void Animation::SetFrames(FramesPtr frames)
	{
		frames_ = frames;
	}

	void Animation::Init(NodePtr target)
	{
		if (!frames_ || frames_->GetFrames().empty())
		{
			Done();
			return;
		}

		auto sprite_target = dynamic_cast<Sprite*>(target.Get());
		if (sprite_target && frames_)
		{
			sprite_target->Load(frames_->GetFrames()[0]);
		}
	}

	void Animation::UpdateTween(NodePtr target, float percent)
	{
		auto sprite_target = dynamic_cast<Sprite*>(target.Get());

		KGE_ASSERT(sprite_target && "Animation only supports Sprites");

		const auto& frames = frames_->GetFrames();
		auto size = frames.size();
		auto index = std::min(static_cast<size_t>(math::Floor(size * percent)), size - 1);

		sprite_target->Load(frames[index]);
	}

	ActionPtr Animation::Clone() const
	{
		if (frames_)
		{
			return new (std::nothrow) Animation(dur_, frames_, ease_func_);
		}
		return nullptr;
	}

	ActionPtr Animation::Reverse() const
	{
		if (frames_)
		{
			FramesPtr frames = frames_->Reverse();
			if (frames)
			{
				return new (std::nothrow) Animation(dur_, frames, ease_func_);
			}
		}
		return nullptr;
	}

}