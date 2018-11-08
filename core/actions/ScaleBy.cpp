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
	ScaleBy::ScaleBy(float duration, float scale)
		: FiniteTimeAction(duration)
	{
		delta_x_ = scale;
		delta_y_ = scale;
	}

	ScaleBy::ScaleBy(float duration, float scale_x, float scale_y)
		: FiniteTimeAction(duration)
	{
		delta_x_ = scale_x;
		delta_y_ = scale_y;
	}

	void ScaleBy::Initialize()
	{
		FiniteTimeAction::Initialize();

		if (target_)
		{
			start_scale_x_ = target_->GetScaleX();
			start_scale_y_ = target_->GetScaleY();
		}
	}

	void ScaleBy::Update()
	{
		FiniteTimeAction::Update();

		if (target_)
		{
			target_->SetScale(start_scale_x_ + delta_x_ * delta_, start_scale_y_ + delta_y_ * delta_);
		}
	}

	ScaleBy * ScaleBy::Clone() const
	{
		return new ScaleBy(duration_, delta_x_, delta_y_);
	}

	ScaleBy * ScaleBy::Reverse() const
	{
		return new ScaleBy(duration_, -delta_x_, -delta_y_);
	}
}