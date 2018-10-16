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

easy2d::JumpBy::JumpBy(float duration, const Point & vec, float height, int jumps)
	: FiniteTimeAction(duration)
	, delta_pos_(vec)
	, height_(height)
	, jumps_(jumps)
{
}

easy2d::JumpBy * easy2d::JumpBy::Clone() const
{
	return new JumpBy(duration_, delta_pos_, height_, jumps_);
}

easy2d::JumpBy * easy2d::JumpBy::Reverse() const
{
	return new JumpBy(duration_, -delta_pos_, height_, jumps_);
}

void easy2d::JumpBy::Init()
{
	FiniteTimeAction::Init();

	if (target_)
	{
		prev_pos_ = start_pos_ = target_->GetPosition();
	}
}

void easy2d::JumpBy::Update()
{
	FiniteTimeAction::Update();

	if (target_)
	{
		float frac = fmod(delta_ * jumps_, 1.f);
		float x = delta_pos_.x * delta_;
		float y = height_ * 4 * frac * (1 - frac);
		y += delta_pos_.y * delta_;

		Point currentPos = target_->GetPosition();

		Point diff = currentPos - prev_pos_;
		start_pos_ = diff + start_pos_;

		Point newPos = start_pos_ + Point(x, y);
		target_->SetPosition(newPos);

		prev_pos_ = newPos;
	}
}
