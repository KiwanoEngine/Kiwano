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
		: public RefCounter
	{
		using Images = std::vector< spImage >;

	public:
		Animation();

		explicit Animation(
			const Images& frames		/* 关键帧数组 */
		);

		explicit Animation(
			Duration const& interval	/* 帧间隔（秒） */
		);

		explicit Animation(
			Duration const& interval,	/* 帧间隔（秒） */
			const Images& frames		/* 关键帧数组 */
		);

		virtual ~Animation();

		// 添加关键帧
		void Add(
			spImage const& frame	/* 关键帧 */
		);

		// 添加多个关键帧
		void Add(
			const Images& frames	/* 关键帧数组 */
		);

		// 获取帧间隔
		Duration const& GetInterval() const;

		// 获取关键帧
		Images const& GetFrames() const;

		// 设置每一帧的时间间隔
		void SetInterval(
			Duration const& interval	/* 帧间隔（秒） */
		);

		// 获取帧动画的拷贝对象
		spAnimation Clone() const;

		// 获取帧动画的倒转
		spAnimation Reverse() const;

	protected:
		Duration	interval_;
		Images		frames_;
	};


	// 精灵动作
	class Animate
		: public Action
	{
	public:
		Animate();

		explicit Animate(
			spAnimation const& animation
		);

		virtual ~Animate();

		// 获取动画
		spAnimation GetAnimation() const;

		// 设置动画
		void SetAnimation(
			spAnimation const& animation
		);

		// 获取该动作的拷贝对象
		virtual spAction Clone() const override;

		// 获取该动作的倒转
		virtual spAction Reverse() const override;

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
		spAnimation animation_;
	};
}
