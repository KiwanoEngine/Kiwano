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

namespace easy2d
{
    // ��������
	class FiniteTimeAction
		: public Action
	{
		E2D_DISABLE_COPY(FiniteTimeAction);

	public:
		// �����ض�ʱ���ĳ�������
		explicit FiniteTimeAction(
			Duration const& duration
		);

		// ���ö���
		virtual void Reset() override;

	protected:
		// ��ʼ������
		virtual void Init(Node* target) override;

		// ���¶���
		virtual void Update(Node* target, Duration const& dt) override;

	protected:
		Duration duration_;
		float process_;
	};


	// ���λ�ƶ���
	class MoveBy
		: public FiniteTimeAction
	{
		E2D_DISABLE_COPY(MoveBy);

	public:
		explicit MoveBy(
			Duration const& duration,	/* ����ʱ�� */
			Point const& vector			/* �ƶ����� */
		);

		// ��ȡ�ö����Ŀ�������
		virtual spAction Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual spAction Reverse() const override;

	protected:
		// ��ʼ������
		virtual void Init(Node* target) override;

		// ���¶���
		virtual void Update(Node* target, Duration const& dt) override;

	protected:
		Point	start_pos_;
		Point	prev_pos_;
		Point	delta_pos_;
	};


	// λ�ƶ���
	class MoveTo
		: public MoveBy
	{
		E2D_DISABLE_COPY(MoveTo);

	public:
		explicit MoveTo(
			Duration const& duration,	/* ����ʱ�� */
			Point const& pos			/* Ŀ������ */
		);

		// ��ȡ�ö����Ŀ�������
		virtual spAction Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual spAction Reverse() const override
		{
			logs::Errorln("Reverse() not supported in MoveTo");
			return nullptr;
		}

	protected:
		// ��ʼ������
		virtual void Init(Node* target) override;

	protected:
		Point end_pos_;
	};


	// �����Ծ����
	class JumpBy
		: public FiniteTimeAction
	{
		E2D_DISABLE_COPY(JumpBy);

	public:
		explicit JumpBy(
			Duration const& duration,	/* ����ʱ�� */
			Point const& vec,			/* ��Ծ���� */
			float height,				/* ��Ծ�߶� */
			int jumps = 1				/* ��Ծ���� */
		);

		// ��ȡ�ö����Ŀ�������
		virtual spAction Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual spAction Reverse() const override;

	protected:
		// ��ʼ������
		virtual void Init(Node* target) override;

		// ���¶���
		virtual void Update(Node* target, Duration const& dt) override;

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
		E2D_DISABLE_COPY(JumpTo);

	public:
		explicit JumpTo(
			Duration const& duration,	/* ����ʱ�� */
			Point const& pos,			/* Ŀ������ */
			float height,				/* ��Ծ�߶� */
			int jumps = 1				/* ��Ծ���� */
		);

		// ��ȡ�ö����Ŀ�������
		virtual spAction Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual spAction Reverse() const override
		{
			logs::Errorln("Reverse() not supported in JumpTo");
			return nullptr;
		}

	protected:
		// ��ʼ������
		virtual void Init(Node* target) override;

	protected:
		Point end_pos_;
	};


	// ������Ŷ���
	class ScaleBy
		: public FiniteTimeAction
	{
		E2D_DISABLE_COPY(ScaleBy);

	public:
		explicit ScaleBy(
			Duration const& duration,	/* ����ʱ�� */
			float scale					/* ��Ա仯ֵ */
		);

		explicit ScaleBy(
			Duration const& duration,	/* ����ʱ�� */
			float scale_x,				/* ����������Ա仯ֵ */
			float scale_y				/* ����������Ա仯ֵ */
		);

		// ��ȡ�ö����Ŀ�������
		virtual spAction Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual spAction Reverse() const override;

	protected:
		// ��ʼ������
		virtual void Init(Node* target) override;

		// ���¶���
		virtual void Update(Node* target, Duration const& dt) override;

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
		E2D_DISABLE_COPY(ScaleTo);

	public:
		explicit ScaleTo(
			Duration const& duration,	/* ����ʱ�� */
			float scale					/* Ŀ��ֵ */
		);

		explicit ScaleTo(
			Duration const& duration,	/* ����ʱ�� */
			float scale_x,				/* ��������Ŀ��ֵ */
			float scale_y				/* ��������Ŀ��ֵ */
		);

		// ��ȡ�ö����Ŀ�������
		virtual spAction Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual spAction Reverse() const override
		{
			logs::Errorln("Reverse() not supported in ScaleTo");
			return nullptr;
		}

	protected:
		// ��ʼ������
		virtual void Init(Node* target) override;

	protected:
		float	end_scale_x_;
		float	end_scale_y_;
	};


	// ͸������Խ��䶯��
	class OpacityBy
		: public FiniteTimeAction
	{
		E2D_DISABLE_COPY(OpacityBy);

	public:
		explicit OpacityBy(
			Duration const& duration,	/* ����ʱ�� */
			float opacity				/* ��Ա仯ֵ */
		);

		// ��ȡ�ö����Ŀ�������
		virtual spAction Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual spAction Reverse() const override;

	protected:
		// ��ʼ������
		virtual void Init(Node* target) override;

		// ���¶���
		virtual void Update(Node* target, Duration const& dt) override;

	protected:
		float start_val_;
		float delta_val_;
	};


	// ͸���Ƚ��䶯��
	class OpacityTo
		: public OpacityBy
	{
		E2D_DISABLE_COPY(OpacityTo);

	public:
		explicit OpacityTo(
			Duration const& duration,	/* ����ʱ�� */
			float opacity				/* Ŀ��ֵ */
		);

		// ��ȡ�ö����Ŀ�������
		virtual spAction Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual spAction Reverse() const override
		{
			logs::Errorln("Reverse() not supported in OpacityTo");
			return nullptr;
		}

	protected:
		// ��ʼ������
		virtual void Init(Node* target) override;

	protected:
		float end_val_;
	};


	// ���붯��
	class FadeIn
		: public OpacityTo
	{
		E2D_DISABLE_COPY(FadeIn);

	public:
		// �������붯��
		explicit FadeIn(
			Duration const& duration	/* ����ʱ�� */
		);
	};


	// ��������
	class FadeOut
		: public OpacityTo
	{
		E2D_DISABLE_COPY(FadeOut);

	public:
		// ������������
		explicit FadeOut(
			Duration const& duration	/* ����ʱ�� */
		);
	};


	// �����ת����
	class RotateBy
		: public FiniteTimeAction
	{
		E2D_DISABLE_COPY(RotateBy);

	public:
		explicit RotateBy(
			Duration const& duration,	/* ����ʱ�� */
			float rotation				/* ��Ա仯ֵ */
		);

		// ��ȡ�ö����Ŀ�������
		virtual spAction Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual spAction Reverse() const override;

	protected:
		// ��ʼ������
		virtual void Init(Node* target) override;

		// ���¶���
		virtual void Update(Node* target, Duration const& dt) override;

	protected:
		float start_val_;
		float delta_val_;
	};


	// ��ת����
	class RotateTo
		: public RotateBy
	{
		E2D_DISABLE_COPY(RotateTo);

	public:
		explicit RotateTo(
			Duration const& duration,	/* ����ʱ�� */
			float rotation				/* Ŀ��ֵ */
		);

		// ��ȡ�ö����Ŀ�������
		virtual spAction Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual spAction Reverse() const override
		{
			logs::Errorln("Reverse() not supported in RotateTo");
			return nullptr;
		}

	protected:
		// ��ʼ������
		virtual void Init(Node* target) override;

	protected:
		float end_val_;
	};


	// ��ʱ����
	class Delay
		: public Action
	{
		E2D_DISABLE_COPY(Delay);

	public:
		explicit Delay(
			Duration const& duration	/* �ӳ�ʱ�����룩 */
		);

		// ��ȡ�ö����Ŀ�������
		virtual spAction Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual spAction Reverse() const override;

		// ���ö���
		virtual void Reset() override;

	protected:
		// ��ʼ������
		virtual void Init(Node* target) override;

		// ���¶���
		virtual void Update(Node* target, Duration const& dt) override;

	protected:
		Duration delay_;
		Duration delta_;
	};
}
