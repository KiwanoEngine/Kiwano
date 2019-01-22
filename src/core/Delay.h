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

#pragma once
#include "Action.hpp"

namespace easy2d
{
	// 延时
	class Delay
		: public Action
	{
	public:
		explicit Delay(
			Duration const& duration	/* 延迟时长（秒） */
		);

		// 获取该动作的拷贝对象
		virtual ActionPtr Clone() const override;

		// 获取该动作的倒转
		virtual ActionPtr Reverse() const override;

		// 重置动作
		virtual void Reset() override;

	protected:
		virtual void Init(Node* target) override;

		virtual void Update(Node* target, Duration const& dt) override;

	protected:
		Duration delay_;
		Duration delta_;
	};
}
