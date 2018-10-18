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

#include "..\e2daction.h"
#include "..\e2dobject.h"

easy2d::Animate::Animate() 
	: frame_index_(0)
	, animation_(nullptr)
{
}

easy2d::Animate::Animate(Animation * animation)
	: frame_index_(0)
	, animation_(nullptr)
{
	this->SetAnimation(animation);
}

easy2d::Animate::~Animate()
{
	SafeRelease(animation_);
}

easy2d::Animation * easy2d::Animate::GetAnimation() const
{
	return animation_;
}

void easy2d::Animate::SetAnimation(Animation * animation)
{
	if (animation && animation != animation_)
	{
		if (animation_)
		{
			animation_->Release();
		}
		animation_ = animation;
		animation_->Retain();
		frame_index_ = 0;
	}
}

void easy2d::Animate::Init()
{
	Action::Init();

	auto target = dynamic_cast<Sprite*>(target_);
	if (target && animation_)
	{
		target->Load(animation_->GetFrames()[frame_index_]);
		++frame_index_;
	}
}

void easy2d::Animate::Update()
{
	Action::Update();

	if (!animation_)
	{
		this->Stop();
		return;
	}

	while ((Time::Now() - started_).Seconds() >= animation_->GetInterval())
	{
		auto& frames = animation_->GetFrames();
		auto target = dynamic_cast<Sprite*>(target_);

		if (target)
		{
			target->Load(frames[frame_index_]);
		}

		started_ += Duration::Second * animation_->GetInterval();
		++frame_index_;

		if (frame_index_ == frames.size())
		{
			this->Stop();
			break;
		}
	}
}

void easy2d::Animate::ResetTime()
{
	Action::ResetTime();
}

void easy2d::Animate::Reset()
{
	Action::Reset();
	frame_index_ = 0;
}

easy2d::Animate * easy2d::Animate::Clone() const
{
	if (animation_)
	{
		return new Animate(animation_);
	}
	return nullptr;
}

easy2d::Animate * easy2d::Animate::Reverse() const
{
	if (animation_)
	{
		auto animation = animation_->Reverse();
		if (animation)
		{
			return new Animate(animation);
		}
	}
	return nullptr;
}
