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
#include "../base/SmartPtr.hpp"
#include "D2DDeviceResources.h"

namespace kiwano
{
	// ������
	class KGE_API Geometry
	{
	public:
		// ��ȡ���а�Χ��
		Rect GetBoundingBox(
			Matrix const& transform = Matrix()
		) const;

		// �ж�ͼ���Ƿ������
		bool ContainsPoint(
			Point const& point
		);

		// ��ȡͼ��չ����һ��ֱ�ߵĳ���
		float GetLength();

		// �������
		float ComputeArea();

		// ����ͼ��·���ϵ��λ�ú���������
		bool ComputePointAtLength(
			float length,
			Point& point,
			Vec2& tangent
		);

	public:
		Geometry();

		Geometry(ComPtr<ID2D1Geometry> geo);

	public:
		inline ComPtr<ID2D1Geometry> GetGeometry() const		{ return geo_; }

		inline void SetGeometry(ComPtr<ID2D1Geometry> geometry)	{ geo_ = geometry; }

		inline operator bool() const							{ return static_cast<bool>(geo_); }

	protected:
		ComPtr<ID2D1Geometry> geo_;
	};


	// ������������
	class KGE_API GeometrySink
		: protected noncopyable
	{
	public:
		GeometrySink();

		// ��ʼ���·��
		GeometrySink& BeginPath(
			Point const& begin_pos = Point{}	/* ��ʼ�� */
		);

		// ����·��
		GeometrySink& EndPath(
			bool closed = false		/* ·���Ƿ�պ� */
		);

		// ���һ���߶�
		GeometrySink& AddLine(
			Point const& point		/* �˵� */
		);

		// ��Ӷ����߶�
		GeometrySink& AddLines(
			Vector<Point> const& points
		);

		// ���һ�����η�����������
		GeometrySink& AddBezier(
			Point const& point1,	/* ���������ߵĵ�һ�����Ƶ� */
			Point const& point2,	/* ���������ߵĵڶ������Ƶ� */
			Point const& point3		/* ���������ߵ��յ� */
		);

		// ��ӻ���
		GeometrySink& AddArc(
			Point const& point,		/* �յ� */
			Size const& radius,		/* ��Բ�뾶 */
			float rotation,			/* ��Բ��ת�Ƕ� */
			bool clockwise = true,	/* ˳ʱ�� or ��ʱ�� */
			bool is_small = true	/* �Ƿ�ȡС�� 180�� �Ļ� */
		);

		// ��ȡ����·��������
		Geometry GetGeometry();

	public:
		inline ComPtr<ID2D1PathGeometry> GetPathGeometry() const	{ return path_geo_; }

		inline void SetPathGeometry(ComPtr<ID2D1PathGeometry> path)	{ path_geo_ = path; }

	protected:
		ComPtr<ID2D1PathGeometry> path_geo_;
		ComPtr<ID2D1GeometrySink> sink_;
	};
}
