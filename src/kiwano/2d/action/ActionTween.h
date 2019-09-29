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
#include "Action.h"
#include "../../base/Logger.h"

namespace kiwano
{
	// ��������
	using EaseFunc = Function<float(float)>;

	// ��������ö��
	// See https://easings.net for more information
	struct Ease
	{
		static KGE_API EaseFunc Linear;			// ����
		static KGE_API EaseFunc EaseIn;			// �������
		static KGE_API EaseFunc EaseOut;		// �ɿ����
		static KGE_API EaseFunc EaseInOut;		// �������, ���ɿ����
		static KGE_API EaseFunc ExpoIn;			// �����伫��
		static KGE_API EaseFunc ExpoOut;		// �ɼ������
		static KGE_API EaseFunc ExpoInOut;		// ����������, ���ɼ������
		static KGE_API EaseFunc ElasticIn;		// ����㸳�赯��
		static KGE_API EaseFunc ElasticOut;		// ���յ㸳�赯��
		static KGE_API EaseFunc ElasticInOut;	// �������յ㸳�赯��
		static KGE_API EaseFunc BounceIn;		// ����㸳�跴����
		static KGE_API EaseFunc BounceOut;		// ���յ㸳�跴����
		static KGE_API EaseFunc BounceInOut;	// �������յ㸳�跴����
		static KGE_API EaseFunc BackIn;
		static KGE_API EaseFunc BackOut;
		static KGE_API EaseFunc BackInOut;
		static KGE_API EaseFunc QuadIn;
		static KGE_API EaseFunc QuadOut;
		static KGE_API EaseFunc QuadInOut;
		static KGE_API EaseFunc CubicIn;
		static KGE_API EaseFunc CubicOut;
		static KGE_API EaseFunc CubicInOut;
		static KGE_API EaseFunc QuartIn;
		static KGE_API EaseFunc QuartOut;
		static KGE_API EaseFunc QuartInOut;
		static KGE_API EaseFunc QuintIn;
		static KGE_API EaseFunc QuintOut;
		static KGE_API EaseFunc QuintInOut;
		static KGE_API EaseFunc SineIn;
		static KGE_API EaseFunc SineOut;
		static KGE_API EaseFunc SineInOut;
	};


	// ���䶯��
	class KGE_API ActionTween
		: public Action
	{
	public:
		ActionTween();

		ActionTween(
			Duration duration,
			EaseFunc func
		);

		// �Զ��建������
		void SetEaseFunc(
			EaseFunc const& func
		);

		EaseFunc const& GetEaseFunc() const;

		Duration GetDuration() const;

		void SetDuration(Duration duration);

	protected:
		void Update(ActorPtr target, Duration dt) override;

		virtual void UpdateTween(ActorPtr target, float percent) = 0;

	protected:
		Duration dur_;
		EaseFunc ease_func_;
	};


	// ���λ�ƶ���
	class KGE_API ActionMoveBy
		: public ActionTween
	{
	public:
		ActionMoveBy(
			Duration duration,		/* ����ʱ�� */
			Point const& vector,	/* �ƶ����� */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		ActionPtr Reverse() const override;

	protected:
		void Init(ActorPtr target) override;

		void UpdateTween(ActorPtr target, float percent) override;

	protected:
		Point	start_pos_;
		Point	prev_pos_;
		Point	delta_pos_;
	};


	// λ�ƶ���
	class KGE_API ActionMoveTo
		: public ActionMoveBy
	{
	public:
		ActionMoveTo(
			Duration duration,		/* ����ʱ�� */
			Point const& pos,		/* Ŀ������ */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual ActionPtr Reverse() const override
		{
			KGE_ERROR_LOG(L"Reverse() not supported in ActionMoveTo");
			return nullptr;
		}

	protected:
		void Init(ActorPtr target) override;

	protected:
		Point end_pos_;
	};


	// �����Ծ����
	class KGE_API ActionJumpBy
		: public ActionTween
	{
	public:
		ActionJumpBy(
			Duration duration,		/* ����ʱ�� */
			Point const& vec,		/* ��Ծ���� */
			float height,			/* ��Ծ�߶� */
			int jumps = 1,		/* ��Ծ���� */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		ActionPtr Reverse() const override;

	protected:
		void Init(ActorPtr target) override;

		void UpdateTween(ActorPtr target, float percent) override;

	protected:
		Point	start_pos_;
		Point	delta_pos_;
		float	height_;
		int		jumps_;
		Point	prev_pos_;
	};


	// ��Ծ����
	class KGE_API ActionJumpTo
		: public ActionJumpBy
	{
	public:
		ActionJumpTo(
			Duration duration,		/* ����ʱ�� */
			Point const& pos,		/* Ŀ������ */
			float height,			/* ��Ծ�߶� */
			int jumps = 1,		/* ��Ծ���� */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual ActionPtr Reverse() const override
		{
			KGE_ERROR_LOG(L"Reverse() not supported in ActionJumpTo");
			return nullptr;
		}

	protected:
		void Init(ActorPtr target) override;

	protected:
		Point end_pos_;
	};


	// ������Ŷ���
	class KGE_API ActionScaleBy
		: public ActionTween
	{
	public:
		ActionScaleBy(
			Duration duration,		/* ����ʱ�� */
			float scale,			/* ��Ա仯ֵ */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);

		ActionScaleBy(
			Duration duration,		/* ����ʱ�� */
			float scale_x,			/* ����������Ա仯ֵ */
			float scale_y,			/* ����������Ա仯ֵ */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		ActionPtr Reverse() const override;

	protected:
		void Init(ActorPtr target) override;

		void UpdateTween(ActorPtr target, float percent) override;

	protected:
		float	start_scale_x_;
		float	start_scale_y_;
		float	delta_x_;
		float	delta_y_;
	};


	// ���Ŷ���
	class KGE_API ActionScaleTo
		: public ActionScaleBy
	{
	public:
		ActionScaleTo(
			Duration duration,		/* ����ʱ�� */
			float scale,			/* Ŀ��ֵ */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);

		ActionScaleTo(
			Duration duration,		/* ����ʱ�� */
			float scale_x,			/* ��������Ŀ��ֵ */
			float scale_y,			/* ��������Ŀ��ֵ */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual ActionPtr Reverse() const override
		{
			KGE_ERROR_LOG(L"Reverse() not supported in ActionScaleTo");
			return nullptr;
		}

	protected:
		void Init(ActorPtr target) override;

	protected:
		float	end_scale_x_;
		float	end_scale_y_;
	};


	// ͸���Ƚ��䶯��
	class KGE_API ActionFadeTo
		: public ActionTween
	{
	public:
		ActionFadeTo(
			Duration duration,		/* ����ʱ�� */
			float opacity,			/* Ŀ��ֵ */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual ActionPtr Reverse() const override
		{
			KGE_ERROR_LOG(L"Reverse() not supported in ActionFadeTo");
			return nullptr;
		}

	protected:
		void Init(ActorPtr target) override;

		void UpdateTween(ActorPtr target, float percent) override;

	protected:
		float start_val_;
		float delta_val_;
		float end_val_;
	};


	// ���붯��
	class KGE_API ActionFadeIn
		: public ActionFadeTo
	{
	public:
		// �������붯��
		explicit ActionFadeIn(
			Duration duration,		/* ����ʱ�� */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);
	};


	// ��������
	class KGE_API ActionFadeOut
		: public ActionFadeTo
	{
	public:
		// ������������
		explicit ActionFadeOut(
			Duration duration,		/* ����ʱ�� */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);
	};


	// �����ת����
	class KGE_API ActionRotateBy
		: public ActionTween
	{
	public:
		ActionRotateBy(
			Duration duration,		/* ����ʱ�� */
			float rotation,			/* ��Ա仯ֵ */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		ActionPtr Reverse() const override;

	protected:
		void Init(ActorPtr target) override;

		void UpdateTween(ActorPtr target, float percent) override;

	protected:
		float start_val_;
		float delta_val_;
	};


	// ��ת����
	class KGE_API ActionRotateTo
		: public ActionRotateBy
	{
	public:
		ActionRotateTo(
			Duration duration,		/* ����ʱ�� */
			float rotation,			/* Ŀ��ֵ */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual ActionPtr Reverse() const override
		{
			KGE_ERROR_LOG(L"Reverse() not supported in ActionRotateTo");
			return nullptr;
		}

	protected:
		void Init(ActorPtr target) override;

	protected:
		float end_val_;
	};


	// �Զ��嶯��
	class KGE_API ActionCustom
		: public ActionTween
	{
	public:
		using TweenFunc = Function<void(ActorPtr, float)>;

		ActionCustom(
			Duration duration,		/* ����ʱ�� */
			TweenFunc tween_func,	/* ���̺��� */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		ActionPtr Reverse() const override
		{
			KGE_ERROR_LOG(L"Reverse() not supported in ActionCustom");
			return nullptr;
		}

	protected:
		void Init(ActorPtr target) override;

		void UpdateTween(ActorPtr target, float percent) override;

	protected:
		TweenFunc tween_func_;
	};

}
