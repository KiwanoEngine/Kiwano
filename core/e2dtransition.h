#pragma once
#include "e2dutil.h"

namespace e2d
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
		virtual void Init(
			Scene * prev,
			Scene * next
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
		float	delta_;
		Time	started_;
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

		virtual void Init(
			Scene * prev,
			Scene * next
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

		virtual void Init(
			Scene * prev,
			Scene * next
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

		virtual void Init(
			Scene * prev,
			Scene * next
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

		virtual void Init(
			Scene * prev,
			Scene * next
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
		explicit MoveTransition(
			float moveDuration,		/* 动画持续时长 */
			float rotation = 360	/* 旋转度数 */
		);

	protected:
		virtual void Update() override;

		virtual void Init(
			Scene * prev,
			Scene * next
		) override;

		virtual void Reset() override;

	protected:
		float	rotation_;
	}

}