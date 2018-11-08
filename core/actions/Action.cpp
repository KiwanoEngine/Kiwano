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


namespace easy2d
{
	Action::Action()
		: running_(false)
		, done_(false)
		, initialized_(false)
		, target_(nullptr)
	{
	}

	Action::~Action()
	{
	}

	bool Action::IsRunning()
	{
		return running_;
	}

	void Action::Resume()
	{
		running_ = true;
	}

	void Action::Pause()
	{
		running_ = false;
	}

	void Action::Stop()
	{
		done_ = true;
	}

	const std::wstring& Action::GetName() const
	{
		return name_;
	}

	void Action::SetName(const std::wstring& name)
	{
		name_ = name;
	}

	Node * Action::GetTarget()
	{
		return target_;
	}

	void Action::Reset()
	{
		initialized_ = false;
		done_ = false;
		started_ = Time::Now();
	}

	bool Action::IsDone() const
	{
		return done_;
	}

	void Action::StartWithTarget(Node* target)
	{
		target_ = target;
		running_ = true;
		this->Reset();
	}

	void Action::Initialize()
	{
		initialized_ = true;
		started_ = Time::Now();
	}

	void Action::Update()
	{
		if (!initialized_)
		{
			Initialize();
		}
	}

	void Action::ResetTime()
	{
	}
}