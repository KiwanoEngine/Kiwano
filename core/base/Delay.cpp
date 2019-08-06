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

#include "Delay.h"

namespace easy2d
{
	Delay::Delay(Duration const& duration)
		: delta_()
		, delay_(duration)
	{
	}

	void Delay::Reset()
	{
		Action::Reset();
		delta_ = Duration{};
	}

	void Delay::Init(Node* target)
	{
		Action::Init(target);
	}

	void Delay::Update(Node* target, Duration const& dt)
	{
		Action::Update(target, dt);

		delta_ += dt;

		if (delta_ >= delay_)
		{
			this->Stop();
		}
	}

	spAction Delay::Clone() const
	{
		return new (std::nothrow) Delay(delay_);
	}

	spAction Delay::Reverse() const
	{
		return new (std::nothrow) Delay(delay_);
	}

}