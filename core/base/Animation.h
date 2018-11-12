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
#include "Action.h"
#include "Image.h"

namespace easy2d
{
	// 帧动画
	class Animation
		: public RefCounter
	{
		E2D_DISABLE_COPY(Animation);

	public:
		typedef std::vector<Image*> Images;

		Animation();

		explicit Animation(
			const Images& frames	/* 关键帧数组 */
		);

		explicit Animation(
			float interval			/* 帧间隔（秒） */
		);

		explicit Animation(
			float interval,			/* 帧间隔（秒） */
			const Images& frames	/* 关键帧数组 */
		);

		virtual ~Animation();

		// 添加关键帧
		void Add(
			Image * frame			/* 关键帧 */
		);

		// 添加多个关键帧
		void Add(
			const Images& frames	/* 关键帧数组 */
		);

		// 获取帧间隔
		float GetInterval() const;

		// 获取关键帧
		const Images& GetFrames() const;

		// 设置每一帧的时间间隔
		void SetInterval(
			float interval		/* 帧间隔（秒） */
		);

		// 获取帧动画的拷贝对象
		Animation * Clone() const;

		// 获取帧动画的倒转
		Animation * Reverse() const;

	protected:
		float	interval_;
		Images	frames_;
	};


	// 精灵动作
	class Animate
		: public Action
	{
		E2D_DISABLE_COPY(Animate);

	public:
		Animate();

		explicit Animate(
			Animation * animation
		);

		virtual ~Animate();

		// 获取动画
		virtual Animation * GetAnimation() const;

		// 设置动画
		virtual void SetAnimation(
			Animation * animation
		);

		// 获取该动作的拷贝对象
		virtual Animate * Clone() const override;

		// 获取该动作的倒转
		virtual Animate * Reverse() const override;

		// 重置动作
		virtual void Reset() override;

	protected:
		// 初始化动作
		virtual void Initialize() override;

		// 更新动作
		virtual void Update() override;

		// 重置动作时间
		virtual void ResetTime() override;

	protected:
		UINT frame_index_;
		Animation * animation_;
	};
}
