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
	{
	}

	Frames::Frames(Array<ImagePtr> const& frames)
	{
		this->Add(frames);
	}

	Frames::~Frames()
	{
	}

	void Frames::Add(ImagePtr const& frame)
	{
		E2D_ASSERT(frame && "Frames::Add failed, NULL pointer exception");

		if (frame)
		{
			frames_.push_back(frame);
		}
	}

	void Frames::Add(Array<ImagePtr> const& frames)
	{
		if (frames_.empty())
			frames_ = frames;
		else
		{
			frames_.reserve(frames_.size() + frames.size());
			for (const auto& image : frames)
				Add(image);
		}
	}

	Array<ImagePtr> const& Frames::GetFrames() const
	{
		return frames_;
	}

	FramesPtr Frames::Clone() const
	{
		auto animation = new (std::nothrow) Frames;
		if (animation)
		{
			for (const auto& frame : frames_)
			{
				animation->Add(frame);
			}
		}
		return animation;
	}

	FramesPtr Frames::Reverse() const
	{
		auto animation = new (std::nothrow) Frames;
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
