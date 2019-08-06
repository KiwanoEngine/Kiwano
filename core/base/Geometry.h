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
	public:
		Geometry();

		virtual ~Geometry();

		cpGeometry const& GetD2DGeometry() const { return geo_; }

		float GetLength();

		bool ComputePointAt(
			float length,
			Point* point,
			Point* tangent
		);

		// �ж�ͼ���Ƿ������
		bool ContainsPoint(
			Point const& point
		);

		// �ж���ͼ���ཻ״̬
		GeometryRelation GetRelationWith(
			spGeometry const& other
		);

	protected:
		cpGeometry geo_;
	};


	// ���ξ���
	class RectangleGeometry
		: public Geometry
	{
	public:
		RectangleGeometry();

		RectangleGeometry(
			Size const& rect_size
		);

		virtual ~RectangleGeometry();

		Size const& GetSize() const { return size_; }

		void SetSize(Size const& rect_size);

	protected:
		Size size_;
	};


	// ����Բ��
	class CircleGeometry
		: public Geometry
	{
	public:
		CircleGeometry();

		CircleGeometry(
			float radius
		);

		virtual ~CircleGeometry();

		float GetRadius() const { return radius_; }

		void SetRadius(
			float radius
		);

	protected:
		float radius_;
	};


	// ������Բ
	class EllipseGeometry
		: public Geometry
	{
	public:
		EllipseGeometry();

		EllipseGeometry(
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

	protected:
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
		void BeginPath();

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
}
