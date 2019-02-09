// Copyright (c) 2016-2018 Easy2D - Nomango
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

namespace easy2d
{
	Animation::Animation()
		: frames_(nullptr)
	{
	}

	Animation::Animation(Duration duration, FramesPtr const& animation, EaseFunc func)
		: ActionTween(duration, func)
		, frames_(nullptr)
	{
		this->SetAnimation(animation);
	}

	Animation::~Animation()
	{
	}

	FramesPtr Animation::GetAnimation() const
	{
		return frames_;
	}

	void Animation::SetAnimation(FramesPtr const& animation)
	{
		if (animation && animation != frames_)
		{
			frames_ = animation;
		}
	}

	void Animation::Init(Node* target)
	{
		if (!frames_ || frames_->GetFrames().empty())
		{
			this->Stop();
			return;
		}

		auto sprite_target = dynamic_cast<Sprite*>(target);
		if (sprite_target && frames_)
		{
			sprite_target->Load(frames_->GetFrames()[0]);
		}
	}

	void Animation::UpdateStep(Node * target, float step)
	{
		E2D_ASSERT(dynamic_cast<Sprite*>(target) && "Animation only supports Sprites");

		const auto& frames = frames_->GetFrames();
		int size = frames.size();
		int index = std::min(static_cast<int>(math::Floor(size * step)), size - 1);

		static_cast<Sprite*>(target)->Load(frames[index]);
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