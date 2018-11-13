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
#include "Sprite.h"

namespace easy2d
{
	//-------------------------------------------------------
	// Animate
	//-------------------------------------------------------

	Animate::Animate()
		: frame_index_(0)
		, animation_(nullptr)
	{
	}

	Animate::Animate(spAnimation const& animation)
		: frame_index_(0)
		, animation_(nullptr)
	{
		this->SetAnimation(animation);
	}

	Animate::~Animate()
	{
	}

	spAnimation Animate::GetAnimation() const
	{
		return animation_;
	}

	void Animate::SetAnimation(spAnimation const& animation)
	{
		if (animation && animation != animation_)
		{
			animation_ = animation;
			frame_index_ = 0;
		}
	}

	void Animate::Init(Node* target)
	{
		Action::Init(target);

		auto sprite_target = dynamic_cast<Sprite*>(target);
		if (sprite_target && animation_)
		{
			sprite_target->Load(animation_->GetFrames()[frame_index_]);
			++frame_index_;
		}
	}

	void Animate::Update(Node* target)
	{
		Action::Update(target);

		if (!animation_)
		{
			this->Stop();
			return;
		}

		while ((time::Now() - started_).Seconds() >= animation_->GetInterval())
		{
			auto& frames = animation_->GetFrames();
			auto sprite_target = dynamic_cast<Sprite*>(target);

			if (sprite_target)
			{
				sprite_target->Load(frames[frame_index_]);
			}

			started_ += time::Second * animation_->GetInterval();
			++frame_index_;

			if (frame_index_ == frames.size())
			{
				this->Stop();
				break;
			}
		}
	}

	void Animate::ResetTime()
	{
		Action::ResetTime();
	}

	void Animate::Reset()
	{
		Action::Reset();
		frame_index_ = 0;
	}

	spAction Animate::Clone() const
	{
		if (animation_)
		{
			return new (std::nothrow) Animate(animation_);
		}
		return nullptr;
	}

	spAction Animate::Reverse() const
	{
		if (animation_)
		{
			auto animation = animation_->Reverse();
			if (animation)
			{
				return new (std::nothrow) Animate(animation);
			}
		}
		return nullptr;
	}


	//-------------------------------------------------------
	// Animation
	//-------------------------------------------------------

	Animation::Animation()
		: interval_(1)
	{
	}

	Animation::Animation(const Images& frames)
		: interval_(1)
	{
		this->Add(frames);
	}

	Animation::Animation(float interval)
		: interval_(interval)
	{
	}

	Animation::Animation(float interval, const Images& frames)
		: interval_(interval)
	{
		this->Add(frames);
	}

	Animation::~Animation()
	{
	}

	void Animation::SetInterval(float interval)
	{
		interval_ = std::max(interval, 0.f);
	}

	void Animation::Add(spImage const& frame)
	{
		E2D_WARNING_IF(!frame, "Animation::Add failed, frame Is nullptr.");
		if (frame)
		{
			frames_.push_back(frame);
		}
	}

	void Animation::Add(const Images& frames)
	{
		for (const auto &image : frames)
		{
			this->Add(image);
		}
	}

	float Animation::GetInterval() const
	{
		return interval_;
	}

	const Images& Animation::GetFrames() const
	{
		return frames_;
	}

	spAnimation Animation::Clone() const
	{
		auto animation = new (std::nothrow) Animation(interval_);
		if (animation)
		{
			for (const auto& frame : frames_)
			{
				animation->Add(frame);
			}
		}
		return animation;
	}

	spAnimation Animation::Reverse() const
	{
		auto animation = new (std::nothrow) Animation(interval_);
		if (!frames_.empty())
		{
			for (auto iter = frames_.crbegin(), crend = frames_.crend(); iter != crend; ++iter)
			{
				if (*iter)
					animation->Add(*iter);
			}
		}
		return animation;
	}
}