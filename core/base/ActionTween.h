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
	// ��������ö��
	// See https://easings.net for more information
	enum class EaseFunc
	{
		Linear,			// ����
		EaseIn,			// �������
		EaseOut,		// �ɿ����
		EaseInOut,		// �������, ���ɿ����
		ExpoIn,			// �����伫��
		ExpoOut,		// �ɼ������
		ExpoInOut,		// ����������, ���ɼ������
		ElasticIn,		// ����㸳�赯��
		ElasticOut,		// ���յ㸳�赯��
		ElasticInOut,	// �������յ㸳�赯��
		BounceIn,		// ����㸳�跴����
		BounceOut,		// ���յ㸳�跴����
		BounceInOut,	// �������յ㸳�跴����
		BackIn,
		BackOut,
		BackInOut,
		QuadIn,
		QuadOut,
		QuadInOut,
		CubicIn,
		CubicOut,
		CubicInOut,
		QuartIn,
		QuartOut,
		QuartInOut,
		QuintIn,
		QuintOut,
		QuintInOut,
		SineIn,
		SineOut,
		SineInOut,
	};

	// ��������
	using EaseFunction = std::function<float(float)>;

	inline EaseFunction MakeEaseIn(float rate) { return std::bind(math::EaseIn, std::placeholders::_1, rate); }
	inline EaseFunction MakeEaseOut(float rate) { return std::bind(math::EaseOut, std::placeholders::_1, rate); }
	inline EaseFunction MakeEaseInOut(float rate) { return std::bind(math::EaseInOut, std::placeholders::_1, rate); }
	inline EaseFunction MakeEaseElasticIn(float period) { return std::bind(math::EaseElasticIn, std::placeholders::_1, period); }
	inline EaseFunction MakeEaseElasticOut(float period) { return std::bind(math::EaseElasticOut, std::placeholders::_1, period); }
	inline EaseFunction MakeEaseElasticInOut(float period) { return std::bind(math::EaseElasticInOut, std::placeholders::_1, period); }


	// ���䶯��
	class Tween
		: public Action
	{
	public:
		Tween();

		explicit Tween(
			Duration const& duration,
			EaseFunc func
		);

		// ���û�������
		void SetEaseFunction(
			EaseFunc func
		);

		// �Զ��建������
		void SetEaseFunction(
			EaseFunction func
		);

		virtual void Reset() override;

		Duration const& GetDuration() const;

		void SetDuration(Duration const& duration);

	protected:
		virtual void Init(Node* target) override;

		virtual void Update(Node* target, Duration const& dt) override;

		virtual void UpdateStep(Node* target, float step) = 0;

	protected:
		Duration     duration_;
		Duration     elapsed_;
		EaseFunc     ease_type_;
		EaseFunction ease_func_;
	};


	// ���λ�ƶ���
	class MoveBy
		: public Tween
	{
	public:
		explicit MoveBy(
			Duration const& duration,			/* ����ʱ�� */
			Point const& vector,				/* �ƶ����� */
			EaseFunc func = EaseFunc::Linear	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		virtual SpAction Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual SpAction Reverse() const override;

	protected:
		virtual void Init(Node* target) override;

		virtual void UpdateStep(Node* target, float step) override;

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
		explicit MoveTo(
			Duration const& duration,			/* ����ʱ�� */
			Point const& pos,					/* Ŀ������ */
			EaseFunc func = EaseFunc::Linear	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		virtual SpAction Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual SpAction Reverse() const override
		{
			logs::Errorln("Reverse() not supported in MoveTo");
			return nullptr;
		}

	protected:
		virtual void Init(Node* target) override;

	protected:
		Point end_pos_;
	};


	// �����Ծ����
	class JumpBy
		: public Tween
	{
	public:
		explicit JumpBy(
			Duration const& duration,			/* ����ʱ�� */
			Point const& vec,					/* ��Ծ���� */
			float height,						/* ��Ծ�߶� */
			int jumps = 1,						/* ��Ծ���� */
			EaseFunc func = EaseFunc::Linear	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		virtual SpAction Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual SpAction Reverse() const override;

	protected:
		virtual void Init(Node* target) override;

		virtual void UpdateStep(Node* target, float step) override;

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
		explicit JumpTo(
			Duration const& duration,			/* ����ʱ�� */
			Point const& pos,					/* Ŀ������ */
			float height,						/* ��Ծ�߶� */
			int jumps = 1,						/* ��Ծ���� */
			EaseFunc func = EaseFunc::Linear	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		virtual SpAction Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual SpAction Reverse() const override
		{
			logs::Errorln("Reverse() not supported in JumpTo");
			return nullptr;
		}

	protected:
		virtual void Init(Node* target) override;

	protected:
		Point end_pos_;
	};


	// ������Ŷ���
	class ScaleBy
		: public Tween
	{
	public:
		explicit ScaleBy(
			Duration const& duration,			/* ����ʱ�� */
			float scale,						/* ��Ա仯ֵ */
			EaseFunc func = EaseFunc::Linear	/* �ٶȱ仯 */
		);

		explicit ScaleBy(
			Duration const& duration,			/* ����ʱ�� */
			float scale_x,						/* ����������Ա仯ֵ */
			float scale_y,						/* ����������Ա仯ֵ */
			EaseFunc func = EaseFunc::Linear	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		virtual SpAction Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual SpAction Reverse() const override;

	protected:
		virtual void Init(Node* target) override;

		virtual void UpdateStep(Node* target, float step) override;

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
		explicit ScaleTo(
			Duration const& duration,			/* ����ʱ�� */
			float scale,						/* Ŀ��ֵ */
			EaseFunc func = EaseFunc::Linear	/* �ٶȱ仯 */
		);

		explicit ScaleTo(
			Duration const& duration,			/* ����ʱ�� */
			float scale_x,						/* ��������Ŀ��ֵ */
			float scale_y,						/* ��������Ŀ��ֵ */
			EaseFunc func = EaseFunc::Linear	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		virtual SpAction Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual SpAction Reverse() const override
		{
			logs::Errorln("Reverse() not supported in ScaleTo");
			return nullptr;
		}

	protected:
		virtual void Init(Node* target) override;

	protected:
		float	end_scale_x_;
		float	end_scale_y_;
	};


	// ͸������Խ��䶯��
	class OpacityBy
		: public Tween
	{
	public:
		explicit OpacityBy(
			Duration const& duration,			/* ����ʱ�� */
			float opacity,						/* ��Ա仯ֵ */
			EaseFunc func = EaseFunc::Linear	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		virtual SpAction Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual SpAction Reverse() const override;

	protected:
		virtual void Init(Node* target) override;

		virtual void UpdateStep(Node* target, float step) override;

	protected:
		float start_val_;
		float delta_val_;
	};


	// ͸���Ƚ��䶯��
	class OpacityTo
		: public OpacityBy
	{
	public:
		explicit OpacityTo(
			Duration const& duration,			/* ����ʱ�� */
			float opacity,						/* Ŀ��ֵ */
			EaseFunc func = EaseFunc::Linear	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		virtual SpAction Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual SpAction Reverse() const override
		{
			logs::Errorln("Reverse() not supported in OpacityTo");
			return nullptr;
		}

	protected:
		virtual void Init(Node* target) override;

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
			Duration const& duration,			/* ����ʱ�� */
			EaseFunc func = EaseFunc::Linear	/* �ٶȱ仯 */
		);
	};


	// ��������
	class FadeOut
		: public OpacityTo
	{
	public:
		// ������������
		explicit FadeOut(
			Duration const& duration,			/* ����ʱ�� */
			EaseFunc func = EaseFunc::Linear	/* �ٶȱ仯 */
		);
	};


	// �����ת����
	class RotateBy
		: public Tween
	{
	public:
		explicit RotateBy(
			Duration const& duration,			/* ����ʱ�� */
			float rotation,						/* ��Ա仯ֵ */
			EaseFunc func = EaseFunc::Linear	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		virtual SpAction Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual SpAction Reverse() const override;

	protected:
		virtual void Init(Node* target) override;

		virtual void UpdateStep(Node* target, float step) override;

	protected:
		float start_val_;
		float delta_val_;
	};


	// ��ת����
	class RotateTo
		: public RotateBy
	{
	public:
		explicit RotateTo(
			Duration const& duration,			/* ����ʱ�� */
			float rotation,						/* Ŀ��ֵ */
			EaseFunc func = EaseFunc::Linear	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		virtual SpAction Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual SpAction Reverse() const override
		{
			logs::Errorln("Reverse() not supported in RotateTo");
			return nullptr;
		}

	protected:
		virtual void Init(Node* target) override;

	protected:
		float end_val_;
	};


	// ·������
	class PathAction
		: public Tween
	{
	public:
		explicit PathAction(
			Duration const& duration,			/* ����ʱ�� */
			SpGeometry const& geo,				/* ����ͼ�� */
			bool rotating = false,				/* ��·�����߷�����ת */
			float start = 0.f,					/* ��� */
			float end = 1.f,					/* �յ� */
			EaseFunc func = EaseFunc::Linear	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		virtual SpAction Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual SpAction Reverse() const override;

	protected:
		virtual void Init(Node* target) override;

		virtual void UpdateStep(Node* target, float step) override;

	protected:
		bool		rotating_;
		float		start_;
		float		end_;
		Point		start_pos_;
		SpGeometry	geo_;
	};
}
