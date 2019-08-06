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
#include "logs.h"
#include "Geometry.h"  // ActionPath
#include "../math/ease.hpp"

namespace easy2d
{
	// ��������
	using EaseFunc = std::function<float(float)>;

	// ��������ö��
	// See https://easings.net for more information
	struct Ease
	{
		static E2D_API EaseFunc Linear;			// ����
		static E2D_API EaseFunc EaseIn;			// �������
		static E2D_API EaseFunc EaseOut;		// �ɿ����
		static E2D_API EaseFunc EaseInOut;		// �������, ���ɿ����
		static E2D_API EaseFunc ExpoIn;			// �����伫��
		static E2D_API EaseFunc ExpoOut;		// �ɼ������
		static E2D_API EaseFunc ExpoInOut;		// ����������, ���ɼ������
		static E2D_API EaseFunc ElasticIn;		// ����㸳�赯��
		static E2D_API EaseFunc ElasticOut;		// ���յ㸳�赯��
		static E2D_API EaseFunc ElasticInOut;	// �������յ㸳�赯��
		static E2D_API EaseFunc BounceIn;		// ����㸳�跴����
		static E2D_API EaseFunc BounceOut;		// ���յ㸳�跴����
		static E2D_API EaseFunc BounceInOut;	// �������յ㸳�跴����
		static E2D_API EaseFunc BackIn;
		static E2D_API EaseFunc BackOut;
		static E2D_API EaseFunc BackInOut;
		static E2D_API EaseFunc QuadIn;
		static E2D_API EaseFunc QuadOut;
		static E2D_API EaseFunc QuadInOut;
		static E2D_API EaseFunc CubicIn;
		static E2D_API EaseFunc CubicOut;
		static E2D_API EaseFunc CubicInOut;
		static E2D_API EaseFunc QuartIn;
		static E2D_API EaseFunc QuartOut;
		static E2D_API EaseFunc QuartInOut;
		static E2D_API EaseFunc QuintIn;
		static E2D_API EaseFunc QuintOut;
		static E2D_API EaseFunc QuintInOut;
		static E2D_API EaseFunc SineIn;
		static E2D_API EaseFunc SineOut;
		static E2D_API EaseFunc SineInOut;
	};

	inline EaseFunc MakeEaseIn(float rate) { return std::bind(math::EaseIn, std::placeholders::_1, rate); }
	inline EaseFunc MakeEaseOut(float rate) { return std::bind(math::EaseOut, std::placeholders::_1, rate); }
	inline EaseFunc MakeEaseInOut(float rate) { return std::bind(math::EaseInOut, std::placeholders::_1, rate); }
	inline EaseFunc MakeEaseElasticIn(float period) { return std::bind(math::EaseElasticIn, std::placeholders::_1, period); }
	inline EaseFunc MakeEaseElasticOut(float period) { return std::bind(math::EaseElasticOut, std::placeholders::_1, period); }
	inline EaseFunc MakeEaseElasticInOut(float period) { return std::bind(math::EaseElasticInOut, std::placeholders::_1, period); }


	// ���䶯��
	class E2D_API ActionTween
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
		void Update(NodePtr const& target, Duration dt) override;

		virtual void UpdateTween(NodePtr const& target, float percent) = 0;

	protected:
		Duration dur_;
		EaseFunc ease_func_;
	};


	// ���λ�ƶ���
	class E2D_API ActionMoveBy
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
		void Init(NodePtr const& target) override;

		void UpdateTween(NodePtr const& target, float percent) override;

	protected:
		Point	start_pos_;
		Point	prev_pos_;
		Point	delta_pos_;
	};


	// λ�ƶ���
	class E2D_API ActionMoveTo
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
			E2D_ERROR_LOG(L"Reverse() not supported in ActionMoveTo");
			return nullptr;
		}

	protected:
		void Init(NodePtr const& target) override;

	protected:
		Point end_pos_;
	};


	// �����Ծ����
	class E2D_API ActionJumpBy
		: public ActionTween
	{
	public:
		ActionJumpBy(
			Duration duration,		/* ����ʱ�� */
			Point const& vec,		/* ��Ծ���� */
			float height,			/* ��Ծ�߶� */
			int jumps = 1,			/* ��Ծ���� */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		ActionPtr Reverse() const override;

	protected:
		void Init(NodePtr const& target) override;

		void UpdateTween(NodePtr const& target, float percent) override;

	protected:
		Point	start_pos_;
		Point	delta_pos_;
		float	height_;
		int		jumps_;
		Point	prev_pos_;
	};


	// ��Ծ����
	class E2D_API ActionJumpTo
		: public ActionJumpBy
	{
	public:
		ActionJumpTo(
			Duration duration,		/* ����ʱ�� */
			Point const& pos,		/* Ŀ������ */
			float height,			/* ��Ծ�߶� */
			int jumps = 1,			/* ��Ծ���� */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual ActionPtr Reverse() const override
		{
			E2D_ERROR_LOG(L"Reverse() not supported in ActionJumpTo");
			return nullptr;
		}

	protected:
		void Init(NodePtr const& target) override;

	protected:
		Point end_pos_;
	};


	// ������Ŷ���
	class E2D_API ActionScaleBy
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
		void Init(NodePtr const& target) override;

		void UpdateTween(NodePtr const& target, float percent) override;

	protected:
		float	start_scale_x_;
		float	start_scale_y_;
		float	delta_x_;
		float	delta_y_;
	};


	// ���Ŷ���
	class E2D_API ActionScaleTo
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
			E2D_ERROR_LOG(L"Reverse() not supported in ActionScaleTo");
			return nullptr;
		}

	protected:
		void Init(NodePtr const& target) override;

	protected:
		float	end_scale_x_;
		float	end_scale_y_;
	};


	// ͸������Խ��䶯��
	class E2D_API ActionOpacityBy
		: public ActionTween
	{
	public:
		ActionOpacityBy(
			Duration duration,		/* ����ʱ�� */
			float opacity,			/* ��Ա仯ֵ */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		ActionPtr Reverse() const override;

	protected:
		void Init(NodePtr const& target) override;

		void UpdateTween(NodePtr const& target, float percent) override;

	protected:
		float start_val_;
		float delta_val_;
	};


	// ͸���Ƚ��䶯��
	class E2D_API ActionOpacityTo
		: public ActionOpacityBy
	{
	public:
		ActionOpacityTo(
			Duration duration,		/* ����ʱ�� */
			float opacity,			/* Ŀ��ֵ */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual ActionPtr Reverse() const override
		{
			E2D_ERROR_LOG(L"Reverse() not supported in ActionOpacityTo");
			return nullptr;
		}

	protected:
		void Init(NodePtr const& target) override;

	protected:
		float end_val_;
	};


	// ���붯��
	class E2D_API ActionFadeIn
		: public ActionOpacityTo
	{
	public:
		// �������붯��
		explicit ActionFadeIn(
			Duration duration,		/* ����ʱ�� */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);
	};


	// ��������
	class E2D_API ActionFadeOut
		: public ActionOpacityTo
	{
	public:
		// ������������
		explicit ActionFadeOut(
			Duration duration,				/* ����ʱ�� */
			EaseFunc func = Ease::Linear	/* �ٶȱ仯 */
		);
	};


	// �����ת����
	class E2D_API ActionRotateBy
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
		void Init(NodePtr const& target) override;

		void UpdateTween(NodePtr const& target, float percent) override;

	protected:
		float start_val_;
		float delta_val_;
	};


	// ��ת����
	class E2D_API ActionRotateTo
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
			E2D_ERROR_LOG(L"Reverse() not supported in ActionRotateTo");
			return nullptr;
		}

	protected:
		void Init(NodePtr const& target) override;

	protected:
		float end_val_;
	};


	// ·������
	class E2D_API ActionPath
		: public ActionTween
	{
	public:
		ActionPath(
			Duration duration,		/* ����ʱ�� */
			GeometryPtr const& geo,	/* ����ͼ�� */
			bool rotating = false,	/* ��·�����߷�����ת */
			float start = 0.f,		/* ��� */
			float end = 1.f,		/* �յ� */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		ActionPtr Reverse() const override;

	protected:
		void Init(NodePtr const& target) override;

		void UpdateTween(NodePtr const& target, float percent) override;

	protected:
		bool		rotating_;
		float		start_;
		float		end_;
		Point		start_pos_;
		GeometryPtr	geo_;
	};
}
