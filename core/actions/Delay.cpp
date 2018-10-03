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

e2d::Delay::Delay(float duration)
	: delta_(0)
	, delay_(std::max(duration, 0.f))
{
}

e2d::Delay * e2d::Delay::Clone() const
{
	return new Delay(delay_);
}

e2d::Delay * e2d::Delay::Reverse() const
{
	return new Delay(delay_);
}

void e2d::Delay::Reset()
{
	Action::Reset();
	delta_ = 0;
}

void e2d::Delay::Init()
{
	Action::Init();
}

void e2d::Delay::Update()
{
	Action::Update();

	delta_ = (Time::Now() - started_).Seconds();

	if (delta_ >= delay_)
	{
		this->Stop();
	}
}

void e2d::Delay::ResetTime()
{
	Action::ResetTime();
	started_ = Time::Now() - Duration(delta_);
}
