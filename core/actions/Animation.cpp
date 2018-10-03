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

e2d::Animation::Animation()
	: interval_(1)
{
}

e2d::Animation::Animation(const Images& frames)
	: interval_(1)
{
	this->Add(frames);
}

e2d::Animation::Animation(float interval)
	: interval_(interval)
{
}

e2d::Animation::Animation(float interval, const Images& frames)
	: interval_(interval)
{
	this->Add(frames);
}

e2d::Animation::~Animation()
{
	for (auto frame : frames_)
	{
		SafeRelease(frame);
	}
}

void e2d::Animation::SetInterval(float interval)
{
	interval_ = std::max(interval, 0.f);
}

void e2d::Animation::Add(Image * frame)
{
	WARN_IF(frame == nullptr, "Animation::Add failed, frame Is nullptr.");
	if (frame)
	{
		frames_.push_back(frame);
		frame->Retain();
	}
}

void e2d::Animation::Add(const Images& frames)
{
	for (const auto &image : frames)
	{
		this->Add(image);
	}
}

float e2d::Animation::GetInterval() const
{
	return interval_;
}

const e2d::Animation::Images& e2d::Animation::GetFrames() const
{
	return frames_;
}

e2d::Animation * e2d::Animation::Clone() const
{
	auto animation = new Animation(interval_);
	if (animation)
	{
		for (const auto& frame : frames_)
		{
			animation->Add(frame);
		}
	}
	return animation;
}

e2d::Animation * e2d::Animation::Reverse() const
{
	auto& oldFrames = this->GetFrames();
	Images frames(oldFrames.size());

	if (!oldFrames.empty())
	{
		for (auto iter = oldFrames.crbegin(),
			iterCrend = oldFrames.crend();
			iter != iterCrend;
			++iter)
		{
			Image* frame = *iter;
			if (frame)
			{
				frames.push_back(frame);
			}
		}
	}

	return new Animation(this->GetInterval(), frames);
}
