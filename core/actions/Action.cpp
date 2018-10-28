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

easy2d::Action::Action() 
	: running_(false)
	, done_(false)
	, initialized_(false)
	, target_(nullptr)
{
}

easy2d::Action::~Action()
{
}

bool easy2d::Action::IsRunning()
{
	return running_;
}

void easy2d::Action::Resume()
{
	running_ = true;
}

void easy2d::Action::Pause()
{
	running_ = false;
}

void easy2d::Action::Stop()
{
	done_ = true;
}

const std::wstring& easy2d::Action::GetName() const
{
	return name_;
}

void easy2d::Action::SetName(const std::wstring& name)
{
	name_ = name;
}

easy2d::Node * easy2d::Action::GetTarget()
{
	return target_;
}

void easy2d::Action::Reset()
{
	initialized_ = false;
	done_ = false;
	started_ = Time::Now();
}

bool easy2d::Action::IsDone() const
{
	return done_;
}

void easy2d::Action::StartWithTarget(Node* target)
{
	target_ = target;
	running_ = true;
	this->Reset();
}

void easy2d::Action::Init()
{
	initialized_ = true;
	started_ = Time::Now();
}

void easy2d::Action::Update()
{
	if (!initialized_)
	{
		Init();
	}
}

void easy2d::Action::ResetTime()
{
}
