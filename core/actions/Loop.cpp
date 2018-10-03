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

e2d::Loop::Loop(Action * action, int times /* = -1 */)
	: action_(action)
	, times_(0)
	, total_times_(times)
{
	WARN_IF(action == nullptr, "Loop NULL pointer exception!");

	if (action)
	{
		action_ = action;
		action_->Retain();
	}
}

e2d::Loop::~Loop()
{
	SafeRelease(action_);
}

e2d::Loop * e2d::Loop::Clone() const
{
	if (action_)
	{
		return new Loop(action_->Clone());
	}
	else
	{
		return nullptr;
	}
}

e2d::Loop * e2d::Loop::Reverse() const
{
	if (action_)
	{
		return new Loop(action_->Clone());
	}
	else
	{
		return nullptr;
	}
}

void e2d::Loop::Init()
{
	Action::Init();

	if (action_)
	{
		action_->target_ = target_;
		action_->Init();
	}
}

void e2d::Loop::Update()
{
	Action::Update();

	if (times_ == total_times_)
	{
		this->Stop();
		return;
	}

	if (action_)
	{
		action_->Update();

		if (action_->IsDone())
		{
			++times_;

			Action::Reset();
			action_->Reset();
		}
	}
	else
	{
		this->Stop();
	}
}

void e2d::Loop::Reset()
{
	Action::Reset();

	if (action_) action_->Reset();
	times_ = 0;
}

void e2d::Loop::ResetTime()
{
	if (action_) action_->ResetTime();
}
