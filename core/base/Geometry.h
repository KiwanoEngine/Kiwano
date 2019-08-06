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
#include "base.hpp"
#include "Unit.h"

namespace easy2d
{
	// ����ͼ�μ��ཻ��ϵ
	enum class GeometryRelation : int
	{
		Unknown,
		Disjoin,		// �޽���
		IsContained,	// ������
		Contains,		// ����
		Overlap			// �ص�
	};


	// ���γ���
	class Geometry
		: public Unit
	{
		friend class Canvas;
		friend class GeometryNode;

	public:
		Geometry();

		virtual ~Geometry();

		// ��ȡ���а�Χ��
		Rect GetBoundingBox();

		// �ж�ͼ���Ƿ������
		bool ContainsPoint(
			Point const& point
		);

		// �ж���ͼ���ཻ״̬
		GeometryRelation GetRelationWith(
			spGeometry const& other
		);

		// ��ȡͼ��չ����һ��ֱ�ߵĳ���
		float GetLength();

		// ����ͼ��·���ϵ��λ�ú���������
		bool ComputePointAt(
			float length,
			Point* point,
			Point* tangent
		);

		// �������
		float ComputeArea();

	protected:
		cpGeometry geo_;
	};


	// ֱ��
	class LineGeometry
		: public Geometry
	{
	public:
		LineGeometry();

		LineGeometry(
			Point const& begin,
			Point const& end
		);

		virtual ~LineGeometry();

		Point const& GetBegin() const { return begin_; }

		Point const& GetEnd() const { return end_; }

		void SetLine(
			Point const& begin,
			Point const& end
		);

		void SetBegin(
			Point const& begin
		);

		void SetEnd(
			Point const& end
		);

	protected:
		Point begin_;
		Point end_;
	};


	// ���ξ���
	class RectangleGeometry
		: public Geometry
	{
	public:
		RectangleGeometry();

		RectangleGeometry(
			Rect const& rect
		);

		RectangleGeometry(
			Point const& left_top,
			Size const& size
		);

		virtual ~RectangleGeometry();

		Rect const& GetRect() const { return rect_; }

		void SetRect(Rect const& rect);

	protected:
		Rect rect_;
	};


	// ����Բ��
	class CircleGeometry
		: public Geometry
	{
	public:
		CircleGeometry();

		CircleGeometry(
			Point const& center,
			float radius
		);

		virtual ~CircleGeometry();

		float GetRadius() const { return radius_; }

		void SetRadius(
			float radius
		);

		Point const& GetCenter() const { return center_; }

		void SetCenter(
			Point const& center
		);

		void SetCircle(
			Point const& center,
			float radius
		);

	protected:
		Point center_;
		float radius_;
	};


	// ������Բ
	class EllipseGeometry
		: public Geometry
	{
	public:
		EllipseGeometry();

		EllipseGeometry(
			Point const& center,
			float radius_x,
			float radius_y
		);

		virtual ~EllipseGeometry();

		float GetRadiusX() const { return radius_x_; }

		float GetRadiusY() const { return radius_y_; }

		void SetRadius(
			float radius_x,
			float radius_y
		);

		Point const& GetCenter() const { return center_; }

		void SetCenter(
			Point const& center
		);

		void SetEllipse(
			Point const& center,
			float radius_x,
			float radius_y
		);

	protected:
		Point center_;
		float radius_x_;
		float radius_y_;
	};


	// ����·��
	class PathGeometry
		: public Geometry
	{
	public:
		PathGeometry();

		virtual ~PathGeometry();

		// ��ʼ���·��
		void BeginPath(
			Point const& begin_pos = Point{}	/* ��ʼ�� */
		);

		// ����·��
		void EndPath(
			bool closed = true		/* ·���Ƿ�պ� */
		);

		// ���һ���߶�
		void AddLine(
			Point const& point		/* �˵� */
		);

		// ��Ӷ����߶�
		void AddLines(
			std::vector<Point> const& points
		);

		// ���һ�����η�����������
		void AddBezier(
			Point const& point1,	/* ���������ߵĵ�һ�����Ƶ� */
			Point const& point2,	/* ���������ߵĵڶ������Ƶ� */
			Point const& point3		/* ���������ߵ��յ� */
		);

		// ���·��
		void ClearPath();

	protected:
		cpPathGeometry	current_geometry_;
		cpGeometrySink	current_sink_;
	};


	// ����Բ�Ǿ���
	class RoundedRectGeometry
		: public Geometry
	{
	public:
		RoundedRectGeometry();

		RoundedRectGeometry(
			Rect const& rect,
			float radius_x,
			float radius_y
		);

		virtual ~RoundedRectGeometry();

		float GetRadiusX() const { return radius_x_; }

		float GetRadiusY() const { return radius_y_; }

		void SetRadius(
			float radius_x,
			float radius_y
		);

		Rect const& GetRect() const { return rect_; }

		void SetRect(
			Rect const& rect
		);

		void SetRoundedRect(
			Rect const& rect,
			float radius_x,
			float radius_y
		);

	protected:
		Rect	rect_;
		float	radius_x_;
		float	radius_y_;
	};
}
