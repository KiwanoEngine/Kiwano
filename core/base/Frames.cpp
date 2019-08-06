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

#include "Frames.h"
#include "Image.h"
#include "logs.h"

namespace easy2d
{
	Frames::Frames()
		: interval_(1)
	{
	}

	Frames::Frames(Images const& frames)
		: interval_(1)
	{
		this->Add(frames);
	}

	Frames::Frames(Duration const& interval)
		: interval_(interval)
	{
	}

	Frames::Frames(Duration const& interval, Images const& frames)
		: interval_(interval)
	{
		this->Add(frames);
	}

	Frames::~Frames()
	{
	}

	void Frames::SetInterval(Duration const& interval)
	{
		interval_ = interval;
	}

	void Frames::Add(spImage const& frame)
	{
		E2D_ASSERT(frame && "Frames::Add failed, NULL pointer exception");

		if (frame)
		{
			frames_.push_back(frame);
		}
	}

	void Frames::Add(Images const& frames)
	{
		for (const auto &image : frames)
		{
			this->Add(image);
		}
	}

	Duration const& Frames::GetInterval() const
	{
		return interval_;
	}

	Frames::Images const& Frames::GetFrames() const
	{
		return frames_;
	}

	spFrames Frames::Clone() const
	{
		auto animation = new (std::nothrow) Frames(interval_);
		if (animation)
		{
			for (const auto& frame : frames_)
			{
				animation->Add(frame);
			}
		}
		return animation;
	}

	spFrames Frames::Reverse() const
	{
		auto animation = new (std::nothrow) Frames(interval_);
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
