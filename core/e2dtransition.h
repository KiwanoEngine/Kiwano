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
#include "e2dutil.h"
#include "utils\time.h"

namespace easy2d
{

	class Game;
	class Scene;

	// 场景过渡
	class Transition
		: public Ref
	{
		friend class Game;

	public:
		explicit Transition(
			float duration
		);

		virtual ~Transition();

		// 场景过渡动画是否结束
		bool IsDone();

	protected:
		// 初始化场景过渡动画
		virtual void Initialize(
			Scene * prev,
			Scene * next,
			Game * game
		);

		// 更新场景过渡动画
		virtual void Update();

		// 渲染场景过渡动画
		virtual void Draw();

		// 停止场景过渡动画
		virtual void Stop();

		// 重置场景过渡动画
		virtual void Reset() { };

	protected:
		bool	done_;
		float	duration_;
		float	process_;
		time::TimePoint	started_;
		Size	window_size_;
		Scene*	out_scene_;
		Scene*	in_scene_;
		ID2D1Layer * out_layer_;
		ID2D1Layer * in_layer_;
		D2D1_LAYER_PARAMETERS out_layer_param_;
		D2D1_LAYER_PARAMETERS in_layer_param_;
	};


	// 淡入淡出过渡
	class FadeTransition
		: public Transition
	{
	public:
		explicit FadeTransition(
			float duration		/* 动画持续时长 */
		);

	protected:
		// 更新动画
		virtual void Update() override;

		virtual void Initialize(
			Scene * prev,
			Scene * next,
			Game * game
		) override;
	};


	// 渐变过渡
	class EmergeTransition
		: public Transition
	{
	public:
		explicit EmergeTransition(
			float duration		/* 动画持续时长 */
		);

	protected:
		virtual void Update() override;

		virtual void Initialize(
			Scene * prev,
			Scene * next,
			Game * game
		) override;
	};


	// 盒状过渡
	class BoxTransition
		: public Transition
	{
	public:
		explicit BoxTransition(
			float duration		/* 动画持续时长 */
		);

	protected:
		virtual void Update() override;

		virtual void Initialize(
			Scene * prev,
			Scene * next,
			Game * game
		) override;
	};


	// 位移过渡
	class MoveTransition
		: public Transition
	{
	public:
		explicit MoveTransition(
			float moveDuration,						/* 动画持续时长 */
			Direction direction = Direction::Left	/* 移动方向 */
		);

	protected:
		virtual void Update() override;

		virtual void Initialize(
			Scene * prev,
			Scene * next,
			Game * game
		) override;

		virtual void Reset() override;

	protected:
		Direction	direction_;
		Point		pos_delta_;
		Point		start_pos_;
	};


	// 旋转过渡
	class RotationTransition
		: public Transition
	{
	public:
		explicit RotationTransition(
			float moveDuration,		/* 动画持续时长 */
			float rotation = 360	/* 旋转度数 */
		);

	protected:
		virtual void Update() override;

		virtual void Initialize(
			Scene * prev,
			Scene * next,
			Game * game
		) override;

		virtual void Reset() override;

	protected:
		float	rotation_;
	};

} // end of easy2d namespace
