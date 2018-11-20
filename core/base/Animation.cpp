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
		: frame_index_(0)
		, frames_(nullptr)
	{
	}

	Animation::Animation(spFrames const& animation)
		: frame_index_(0)
		, frames_(nullptr)
	{
		this->SetAnimation(animation);
	}

	Animation::~Animation()
	{
	}

	spFrames Animation::GetAnimation() const
	{
		return frames_;
	}

	void Animation::SetAnimation(spFrames const& animation)
	{
		if (animation && animation != frames_)
		{
			frames_ = animation;
			frame_index_ = 0;
		}
	}

	void Animation::Init(Node* target)
	{
		Action::Init(target);

		auto sprite_target = dynamic_cast<Sprite*>(target);
		if (sprite_target && frames_)
		{
			sprite_target->Load(frames_->GetFrames()[frame_index_]);
			++frame_index_;
		}
	}

	void Animation::Update(Node* target, Duration const& dt)
	{
		Action::Update(target, dt);

		if (!frames_)
		{
			this->Stop();
			return;
		}

		delta_ += dt;
		while (delta_ >= frames_->GetInterval())
		{
			auto& frames = frames_->GetFrames();
			auto sprite_target = dynamic_cast<Sprite*>(target);

			if (sprite_target)
			{
				sprite_target->Load(frames[frame_index_]);
			}

			delta_ -= frames_->GetInterval();
			++frame_index_;

			if (frame_index_ == frames.size())
			{
				this->Stop();
				break;
			}
		}
	}

	void Animation::Reset()
	{
		Action::Reset();
		frame_index_ = 0;
	}

	spAction Animation::Clone() const
	{
		if (frames_)
		{
			return new (std::nothrow) Animation(frames_);
		}
		return nullptr;
	}

	spAction Animation::Reverse() const
	{
		if (frames_)
		{
			auto animation = frames_->Reverse();
			if (animation)
			{
				return new (std::nothrow) Animation(animation);
			}
		}
		return nullptr;
	}

}