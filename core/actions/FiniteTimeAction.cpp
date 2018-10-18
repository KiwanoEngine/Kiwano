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

easy2d::FiniteTimeAction::FiniteTimeAction(float duration)
	: delta_(0)
	, duration_(std::max(duration, 0.f))
{
}

void easy2d::FiniteTimeAction::Reset()
{
	Action::Reset();
	delta_ = 0;
}

void easy2d::FiniteTimeAction::Init()
{
	Action::Init();
}

void easy2d::FiniteTimeAction::Update()
{
	Action::Update();

	if (duration_ == 0)
	{
		delta_ = 1;
		this->Stop();
	}
	else
	{
		delta_ = std::min((Time::Now() - started_).Seconds() / duration_, 1.f);

		if (delta_ >= 1)
		{
			this->Stop();
		}
	}
}

void easy2d::FiniteTimeAction::ResetTime()
{
	Action::ResetTime();
	started_ = Time::Now() - Duration::Second * (delta_ * duration_);
}
