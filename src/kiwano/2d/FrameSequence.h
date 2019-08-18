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
	class KGE_API FrameSequence
		: public ObjectBase
	{
	public:
		FrameSequence();

		explicit FrameSequence(
			Vector<FramePtr> const& frames	/* 帧序列 */
		);

		virtual ~FrameSequence();

		// 添加关键帧
		void AddFrame(
			FramePtr frame
		);

		// 添加多个关键帧
		void AddFrames(
			Vector<FramePtr> const& frames
		);

		// 获取关键帧
		FramePtr GetFrame(UInt32 index) const;

		// 获取关键帧
		Vector<FramePtr> const& GetFrames() const;

		// 获取帧动画的拷贝对象
		FrameSequencePtr Clone() const;

		// 获取帧动画的倒转
		FrameSequencePtr Reverse() const;

	protected:
		Vector<FramePtr>	frames_;
	};
}
