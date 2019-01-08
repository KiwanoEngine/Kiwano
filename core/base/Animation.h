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
	// 帧动画
	class Animation
		: public Action
	{
	public:
		Animation();

		explicit Animation(
			SpFrames const& animation
		);

		virtual ~Animation();

		// 获取动画
		SpFrames GetAnimation() const;

		// 设置动画
		void SetAnimation(
			SpFrames const& animation
		);

		// 获取该动作的拷贝对象
		virtual SpAction Clone() const override;

		// 获取该动作的倒转
		virtual SpAction Reverse() const override;

		// 重置动作
		virtual void Reset() override;

	protected:
		// 初始化动作
		virtual void Init(Node* target) override;

		// 更新动作
		virtual void Update(Node* target, Duration const& dt) override;

	protected:
		size_t frame_index_;
		Duration delta_;
		SpFrames frames_;
	};
}
