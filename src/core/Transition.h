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
#include "include-forwards.h"
#include "time.h"

namespace easy2d
{
	class Scene;

	// 场景过渡
	class Transition
		: public Object
	{
		friend class Game;

	public:
		explicit Transition(
			Duration const& duration
		);

		virtual ~Transition();

		bool IsDone();

	protected:
		virtual void Init(
			SpScene const& prev,
			SpScene const& next
		);

		virtual void Update(Duration const& dt);

		virtual void Render();

		virtual void Stop();

		virtual void Reset() { };

	protected:
		bool			done_;
		float			process_;
		Duration		duration_;
		Duration		delta_;
		Size			window_size_;
		SpScene			out_scene_;
		SpScene			in_scene_;
		CpLayer			out_layer_;
		CpLayer			in_layer_;
		LayerProperties out_layer_prop_;
		LayerProperties in_layer_prop_;
	};


	// 淡入淡出过渡
	class FadeTransition
		: public Transition
	{
	public:
		explicit FadeTransition(
			Duration const& duration	/* 动画持续时长 */
		);

	protected:
		// 更新动画
		virtual void Update(Duration const& dt) override;

		virtual void Init(
			SpScene const& prev,
			SpScene const& next
		) override;
	};


	// 渐变过渡
	class EmergeTransition
		: public Transition
	{
	public:
		explicit EmergeTransition(
			Duration const& duration	/* 动画持续时长 */
		);

	protected:
		virtual void Update(Duration const& dt) override;

		virtual void Init(
			SpScene const& prev,
			SpScene const& next
		) override;
	};


	// 盒状过渡
	class BoxTransition
		: public Transition
	{
	public:
		explicit BoxTransition(
			Duration const& duration	/* 动画持续时长 */
		);

	protected:
		virtual void Update(Duration const& dt) override;

		virtual void Init(
			SpScene const& prev,
			SpScene const& next
		) override;
	};


	// 位移过渡
	class MoveTransition
		: public Transition
	{
	public:
		explicit MoveTransition(
			Duration const& moveDuration,	/* 动画持续时长 */
			Direction direction				/* 移动方向 */
		);

	protected:
		virtual void Update(Duration const& dt) override;

		virtual void Init(
			SpScene const& prev,
			SpScene const& next
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
			Duration const& moveDuration,	/* 动画持续时长 */
			float rotation = 360			/* 旋转度数 */
		);

	protected:
		virtual void Update(Duration const& dt) override;

		virtual void Init(
			SpScene const& prev,
			SpScene const& next
		) override;

		virtual void Reset() override;

	protected:
		float	rotation_;
	};
}
