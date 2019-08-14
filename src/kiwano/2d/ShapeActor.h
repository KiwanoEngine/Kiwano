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
#include "Actor.h"
#include "../renderer/Geometry.h"

namespace kiwano
{
	// ��άͼ�ν�ɫ
	class KGE_API ShapeActor
		: public Actor
	{
	public:
		ShapeActor();

		ShapeActor(
			Geometry geometry
		);

		virtual ~ShapeActor();

		// ��ȡ�����ɫ
		Color GetFillColor() const				{ return fill_color_; }

		// ��ȡ������ɫ
		Color GetStrokeColor() const			{ return stroke_color_; }

		// ��ȡ��������
		float GetStrokeWidth() const			{ return stroke_width_; }

		// ��ȡ�����ཻ��ʽ
		StrokeStyle SetOutlineJoinStyle() const	{ return outline_join_; }

		// ��ȡ�߽�
		Rect GetBounds() const override;

		// ��ȡ���а�Χ��
		Rect GetBoundingBox() const override;

		// ���������ɫ
		void SetFillColor(
			const Color& color
		);

		// ����������ɫ
		void SetStrokeColor(
			const Color& color
		);

		// ������������
		void SetStrokeWidth(
			float width
		);

		// ���������ཻ��ʽ
		void SetOutlineJoinStyle(
			StrokeStyle outline_join
		);

		// ������״
		inline void SetGeometry(Geometry geometry)	{ geo_ = geometry; }

		// ��ȡ��״
		inline Geometry GetGeometry() const			{ return geo_; }

		void OnRender(Renderer* renderer) override;

	protected:
		Color		fill_color_;
		Color		stroke_color_;
		float		stroke_width_;
		StrokeStyle	outline_join_;
		Geometry	geo_;
	};


	// ֱ��
	class KGE_API LineActor
		: public ShapeActor
	{
	public:
		LineActor();

		LineActor(
			Point const& end_pos
		);

		virtual ~LineActor();

		Point const& GetEndPoint() const { return end_; }

		void SetEndPoint(
			Point const& end
		);

	protected:
		Point end_;
	};


	// ���ν�ɫ
	class KGE_API RectActor
		: public ShapeActor
	{
	public:
		RectActor();

		RectActor(
			Size const& size
		);

		virtual ~RectActor();

		void SetRectSize(Size const& size);

		inline Size const& GetRectSize() const { return size_; }

	protected:
		Size size_;
	};


	// Բ�Ǿ��ν�ɫ
	class KGE_API RoundRectActor
		: public ShapeActor
	{
	public:
		RoundRectActor();

		RoundRectActor(
			Size const& size,
			Vec2 const& radius
		);

		virtual ~RoundRectActor();

		void SetRadius(
			Vec2 const& radius
		);

		void SetRectSize(
			Size const& size
		);

		void SetRoundedRect(
			Size const& size,
			Vec2 const& radius
		);

		inline Vec2 GetRadius() const	{ return radius_; }

		inline Size GetRectSize() const	{ return size_; }

	protected:
		Size size_;
		Vec2 radius_;
	};


	// Բ�ν�ɫ
	class KGE_API CircleActor
		: public ShapeActor
	{
	public:
		CircleActor();

		CircleActor(
			float radius
		);

		virtual ~CircleActor();

		inline float GetRadius() const { return radius_; }

		void SetRadius(float radius);

	protected:
		float radius_;
	};


	// ��Բ��ɫ
	class KGE_API EllipseActor
		: public ShapeActor
	{
	public:
		EllipseActor();

		EllipseActor(
			Vec2 const& radius
		);

		virtual ~EllipseActor();

		Vec2 GetRadius() const { return radius_; }

		void SetRadius(
			Vec2 const& radius
		);

	protected:
		Vec2 radius_;
	};


	// ·����ɫ
	class KGE_API PathActor
		: public ShapeActor
	{
	public:
		PathActor();

		virtual ~PathActor();

		// ��ʼ����·��
		void BeginPath(
			Point const& begin_pos = Point{}	/* ��ʼ�� */
		);

		// ����·��
		void EndPath(
			bool closed = true		/* ·���Ƿ�պ� */
		);

		// ����һ���߶�
		void AddLine(
			Point const& point		/* �˵� */
		);

		// ���Ӷ����߶�
		void AddLines(
			Vector<Point> const& points
		);

		// ����һ�����η�����������
		void AddBezier(
			Point const& point1,	/* ���������ߵĵ�һ�����Ƶ� */
			Point const& point2,	/* ���������ߵĵڶ������Ƶ� */
			Point const& point3		/* ���������ߵ��յ� */
		);

		// ���ӻ���
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
		GeometrySink sink_;
	};

}