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
#include "logs.h"
#include "../math/ease.hpp"

namespace easy2d
{
	// ��������
	using EaseFunc = std::function<float(float)>;

	// ��������ö��
	// See https://easings.net for more information
	struct Ease
	{
		static EaseFunc Linear;			// ����
		static EaseFunc EaseIn;			// �������
		static EaseFunc EaseOut;		// �ɿ����
		static EaseFunc EaseInOut;		// �������, ���ɿ����
		static EaseFunc ExpoIn;			// �����伫��
		static EaseFunc ExpoOut;		// �ɼ������
		static EaseFunc ExpoInOut;		// ����������, ���ɼ������
		static EaseFunc ElasticIn;		// ����㸳�赯��
		static EaseFunc ElasticOut;		// ���յ㸳�赯��
		static EaseFunc ElasticInOut;	// �������յ㸳�赯��
		static EaseFunc BounceIn;		// ����㸳�跴����
		static EaseFunc BounceOut;		// ���յ㸳�跴����
		static EaseFunc BounceInOut;	// �������յ㸳�跴����
		static EaseFunc BackIn;
		static EaseFunc BackOut;
		static EaseFunc BackInOut;
		static EaseFunc QuadIn;
		static EaseFunc QuadOut;
		static EaseFunc QuadInOut;
		static EaseFunc CubicIn;
		static EaseFunc CubicOut;
		static EaseFunc CubicInOut;
		static EaseFunc QuartIn;
		static EaseFunc QuartOut;
		static EaseFunc QuartInOut;
		static EaseFunc QuintIn;
		static EaseFunc QuintOut;
		static EaseFunc QuintInOut;
		static EaseFunc SineIn;
		static EaseFunc SineOut;
		static EaseFunc SineInOut;
	};

	inline EaseFunc MakeEaseIn(float rate) { return std::bind(math::EaseIn, std::placeholders::_1, rate); }
	inline EaseFunc MakeEaseOut(float rate) { return std::bind(math::EaseOut, std::placeholders::_1, rate); }
	inline EaseFunc MakeEaseInOut(float rate) { return std::bind(math::EaseInOut, std::placeholders::_1, rate); }
	inline EaseFunc MakeEaseElasticIn(float period) { return std::bind(math::EaseElasticIn, std::placeholders::_1, period); }
	inline EaseFunc MakeEaseElasticOut(float period) { return std::bind(math::EaseElasticOut, std::placeholders::_1, period); }
	inline EaseFunc MakeEaseElasticInOut(float period) { return std::bind(math::EaseElasticInOut, std::placeholders::_1, period); }


	// ���䶯��
	class ActionTween
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

		void Reset() override;

		Duration GetDuration() const;

		void SetDuration(Duration duration);

	protected:
		void Init(Node* target) override;

		void Update(Node* target, Duration dt) override;

		virtual void UpdateStep(Node* target, float step) = 0;

	protected:
		Duration duration_;
		Duration elapsed_;
		EaseFunc ease_func_;
	};


	// ���λ�ƶ���
	class MoveBy
		: public ActionTween
	{
	public:
		MoveBy(
			Duration duration,		/* ����ʱ�� */
			Point const& vector,	/* �ƶ����� */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		ActionPtr Reverse() const override;

	protected:
		void Init(Node* target) override;

		void UpdateStep(Node* target, float step) override;

	protected:
		Point	start_pos_;
		Point	prev_pos_;
		Point	delta_pos_;
	};


	// λ�ƶ���
	class MoveTo
		: public MoveBy
	{
	public:
		MoveTo(
			Duration duration,		/* ����ʱ�� */
			Point const& pos,		/* Ŀ������ */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual ActionPtr Reverse() const override
		{
			logs::Errorln(L"Reverse() not supported in MoveTo");
			return nullptr;
		}

	protected:
		void Init(Node* target) override;

	protected:
		Point end_pos_;
	};


	// �����Ծ����
	class JumpBy
		: public ActionTween
	{
	public:
		JumpBy(
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
		void Init(Node* target) override;

		void UpdateStep(Node* target, float step) override;

	protected:
		Point	start_pos_;
		Point	delta_pos_;
		float	height_;
		int		jumps_;
		Point	prev_pos_;
	};


	// ��Ծ����
	class JumpTo
		: public JumpBy
	{
	public:
		JumpTo(
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
			logs::Errorln(L"Reverse() not supported in JumpTo");
			return nullptr;
		}

	protected:
		void Init(Node* target) override;

	protected:
		Point end_pos_;
	};


	// ������Ŷ���
	class ScaleBy
		: public ActionTween
	{
	public:
		ScaleBy(
			Duration duration,		/* ����ʱ�� */
			float scale,			/* ��Ա仯ֵ */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);

		ScaleBy(
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
		void Init(Node* target) override;

		void UpdateStep(Node* target, float step) override;

	protected:
		float	start_scale_x_;
		float	start_scale_y_;
		float	delta_x_;
		float	delta_y_;
	};


	// ���Ŷ���
	class ScaleTo
		: public ScaleBy
	{
	public:
		ScaleTo(
			Duration duration,		/* ����ʱ�� */
			float scale,			/* Ŀ��ֵ */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);

		ScaleTo(
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
			logs::Errorln(L"Reverse() not supported in ScaleTo");
			return nullptr;
		}

	protected:
		void Init(Node* target) override;

	protected:
		float	end_scale_x_;
		float	end_scale_y_;
	};


	// ͸������Խ��䶯��
	class OpacityBy
		: public ActionTween
	{
	public:
		OpacityBy(
			Duration duration,		/* ����ʱ�� */
			float opacity,			/* ��Ա仯ֵ */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		ActionPtr Reverse() const override;

	protected:
		void Init(Node* target) override;

		void UpdateStep(Node* target, float step) override;

	protected:
		float start_val_;
		float delta_val_;
	};


	// ͸���Ƚ��䶯��
	class OpacityTo
		: public OpacityBy
	{
	public:
		OpacityTo(
			Duration duration,		/* ����ʱ�� */
			float opacity,			/* Ŀ��ֵ */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual ActionPtr Reverse() const override
		{
			logs::Errorln(L"Reverse() not supported in OpacityTo");
			return nullptr;
		}

	protected:
		void Init(Node* target) override;

	protected:
		float end_val_;
	};


	// ���붯��
	class FadeIn
		: public OpacityTo
	{
	public:
		// �������붯��
		explicit FadeIn(
			Duration duration,		/* ����ʱ�� */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);
	};


	// ��������
	class FadeOut
		: public OpacityTo
	{
	public:
		// ������������
		explicit FadeOut(
			Duration duration,				/* ����ʱ�� */
			EaseFunc func = Ease::Linear	/* �ٶȱ仯 */
		);
	};


	// �����ת����
	class RotateBy
		: public ActionTween
	{
	public:
		RotateBy(
			Duration duration,		/* ����ʱ�� */
			float rotation,			/* ��Ա仯ֵ */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		ActionPtr Reverse() const override;

	protected:
		void Init(Node* target) override;

		void UpdateStep(Node* target, float step) override;

	protected:
		float start_val_;
		float delta_val_;
	};


	// ��ת����
	class RotateTo
		: public RotateBy
	{
	public:
		RotateTo(
			Duration duration,		/* ����ʱ�� */
			float rotation,			/* Ŀ��ֵ */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual ActionPtr Reverse() const override
		{
			logs::Errorln(L"Reverse() not supported in RotateTo");
			return nullptr;
		}

	protected:
		void Init(Node* target) override;

	protected:
		float end_val_;
	};


	// ·������
	class PathAction
		: public ActionTween
	{
	public:
		PathAction(
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
		void Init(Node* target) override;

		void UpdateStep(Node* target, float step) override;

	protected:
		bool		rotating_;
		float		start_;
		float		end_;
		Point		start_pos_;
		GeometryPtr	geo_;
	};
}
