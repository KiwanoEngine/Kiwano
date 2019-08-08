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
#include "Node.h"
#include "../renderer/render.h"  // ID2D1Geometry

namespace kiwano
{
	// ��άͼ�νڵ�
	class KGE_API ShapeNode
		: public VisualNode
	{
	public:
		ShapeNode();

		ShapeNode(
			ComPtr<ID2D1Geometry> geometry
		);

		virtual ~ShapeNode();

		// ��ȡ�����ɫ
		Color GetFillColor() const				{ return fill_color_; }

		// ��ȡ������ɫ
		Color GetStrokeColor() const			{ return stroke_color_; }

		// ��ȡ�������
		float GetStrokeWidth() const			{ return stroke_width_; }

		// ��ȡ�����ཻ��ʽ
		StrokeStyle SetOutlineJoinStyle() const	{ return outline_join_; }

		// ��ȡ���а�Χ��
		Rect GetBoundingBox();

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

		// ���������ɫ
		void SetFillColor(
			const Color& color
		);

		// ����������ɫ
		void SetStrokeColor(
			const Color& color
		);

		// �����������
		void SetStrokeWidth(
			float width
		);

		// ���������ཻ��ʽ
		void SetOutlineJoinStyle(
			StrokeStyle outline_join
		);

		// ������״
		inline void SetGeometry(ComPtr<ID2D1Geometry> geometry)	{ geo_ = geometry; }

		// ��ȡ��״
		inline ComPtr<ID2D1Geometry> GetGeometry() const		{ return geo_; }

		void OnRender() override;

	protected:
		Color					fill_color_;
		Color					stroke_color_;
		float					stroke_width_;
		StrokeStyle				outline_join_;
		ComPtr<ID2D1Geometry>	geo_;
	};


	// ֱ��
	class KGE_API LineNode
		: public ShapeNode
	{
	public:
		LineNode();

		LineNode(
			Point const& begin,
			Point const& end
		);

		virtual ~LineNode();

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


	// ���νڵ�
	class KGE_API RectNode
		: public ShapeNode
	{
	public:
		RectNode();

		RectNode(
			Rect const& rect
		);

		RectNode(
			Point const& left_top,
			Size const& size
		);

		virtual ~RectNode();

		Rect const& GetRect() const { return rect_; }

		void SetRect(Rect const& rect);

	protected:
		Rect rect_;
	};


	// Բ�Ǿ��νڵ�
	class KGE_API RoundedRectNode
		: public ShapeNode
	{
	public:
		RoundedRectNode();

		RoundedRectNode(
			Rect const& rect,
			float radius_x,
			float radius_y
		);

		virtual ~RoundedRectNode();

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


	// Բ�νڵ�
	class KGE_API CircleNode
		: public ShapeNode
	{
	public:
		CircleNode();

		CircleNode(
			Point const& center,
			float radius
		);

		virtual ~CircleNode();

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


	// ��Բ�ڵ�
	class KGE_API EllipseNode
		: public ShapeNode
	{
	public:
		EllipseNode();

		EllipseNode(
			Point const& center,
			float radius_x,
			float radius_y
		);

		virtual ~EllipseNode();

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


	// ·���ڵ�
	class KGE_API PathNode
		: public ShapeNode
	{
	public:
		PathNode();

		virtual ~PathNode();

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
			Array<Point> const& points
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
			float rotation,			/* ��Բ��ת�Ƕ� */
			bool clockwise = true,	/* ˳ʱ�� or ��ʱ�� */
			bool is_small = true	/* �Ƿ�ȡС�� 180�� �Ļ� */
		);

		// ���·��
		void ClearPath();

	protected:
		ComPtr<ID2D1PathGeometry>	current_geometry_;
		ComPtr<ID2D1GeometrySink>	current_sink_;
	};

}
