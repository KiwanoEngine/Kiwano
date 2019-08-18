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
#include "ActionTween.h"
#include "../../renderer/Geometry.h"  // Geometry, GeometrySink

namespace kiwano
{
	// ·�����߶���
	class KGE_API ActionWalk
		: public ActionTween
	{
	public:
		ActionWalk(
			Duration duration,		/* ����ʱ�� */
			bool rotating = false,	/* ��·�����߷�����ת */
			Float32 start = 0.f,		/* ��� */
			Float32 end = 1.f,		/* �յ� */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);

		ActionWalk(
			Duration duration,		/* ����ʱ�� */
			Geometry const& path,	/* ·�� */
			bool rotating = false,	/* ��·�����߷�����ת */
			Float32 start = 0.f,		/* ��� */
			Float32 end = 1.f,		/* �յ� */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		);

		// ��ȡ�ö����Ŀ�������
		ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		ActionPtr Reverse() const override;

		// ��ʼ���·��
		void BeginPath();

		// ����·��
		void EndPath(
			bool closed = false		/* ·���Ƿ�պ� */
		);

		// ���һ���߶�
		void AddLine(
			Point const& point		/* �˵� */
		);

		// ��Ӷ����߶�
		void AddLines(
			Vector<Point> const& points
		);

		// ���һ�����η�����������
		void AddBezier(
			Point const& point1,	/* ���������ߵĵ�һ�����Ƶ� */
			Point const& point2,	/* ���������ߵĵڶ������Ƶ� */
			Point const& point3		/* ���������ߵ��յ� */
		);

		// ��ӻ���
		void AddArc(
			Point const& point,		/* �յ� */
			Size const& radius,		/* ��Բ�뾶 */
			Float32 rotation,			/* ��Բ��ת�Ƕ� */
			bool clockwise = true,	/* ˳ʱ�� or ��ʱ�� */
			bool is_small = true	/* �Ƿ�ȡС�� 180�� �Ļ� */
		);

		// ���·��
		void ClearPath();

		// ��ȡ·��
		inline Geometry GetPath() const		{ return path_; }

		// ����·��
		inline void SetPath(Geometry path)	{ path_ = path; }

	protected:
		void Init(ActorPtr target) override;

		void UpdateTween(ActorPtr target, Float32 percent) override;

	protected:
		bool			rotating_;
		Float32			start_;
		Float32			end_;
		Float32			length_;
		Point			start_pos_;
		Geometry		path_;
		GeometrySink	sink_;
	};
}
