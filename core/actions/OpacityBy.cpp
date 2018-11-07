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


namespace easy2d
{
	OpacityBy::OpacityBy(float duration, float opacity)
		: FiniteTimeAction(duration)
	{
		delta_val_ = opacity;
	}

	void OpacityBy::Init()
	{
		FiniteTimeAction::Init();

		if (target_)
		{
			start_val_ = target_->GetOpacity();
		}
	}

	void OpacityBy::Update()
	{
		FiniteTimeAction::Update();

		if (target_)
		{
			target_->SetOpacity(start_val_ + delta_val_ * delta_);
		}
	}

	OpacityBy * OpacityBy::Clone() const
	{
		return new OpacityBy(duration_, delta_val_);
	}

	OpacityBy * OpacityBy::Reverse() const
	{
		return new OpacityBy(duration_, -delta_val_);
	}
}