// Copyright (c) 2016-2018 Kiwano - Nomango
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
#include "include-forwards.h"

namespace kiwano
{
	// 序列帧
	class KGE_API Frames
		: public virtual Object
	{
	public:
		Frames();

		explicit Frames(
			Array<ImagePtr> const& frames	/* 序列帧 */
		);

		virtual ~Frames();

		// 添加关键帧
		void Add(
			ImagePtr frame
		);

		// 添加多个关键帧
		void Add(
			Array<ImagePtr> const& frames
		);

		// 获取关键帧
		Array<ImagePtr> const& GetFrames() const;

		// 获取帧动画的拷贝对象
		FramesPtr Clone() const;

		// 获取帧动画的倒转
		FramesPtr Reverse() const;

	protected:
		Array<ImagePtr>	frames_;
	};
}
