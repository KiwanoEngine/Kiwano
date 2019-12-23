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
#include <kiwano/2d/Actor.h>
#include <kiwano/renderer/Geometry.h>
#include <kiwano/renderer/StrokeStyle.h>

namespace kiwano
{
	KGE_DECLARE_SMART_PTR(ShapeActor);
	KGE_DECLARE_SMART_PTR(LineActor);
	KGE_DECLARE_SMART_PTR(RectActor);
	KGE_DECLARE_SMART_PTR(RoundRectActor);
	KGE_DECLARE_SMART_PTR(CircleActor);
	KGE_DECLARE_SMART_PTR(EllipseActor);
	KGE_DECLARE_SMART_PTR(PolygonActor);
	KGE_DECLARE_SMART_PTR(PathActor);

	/**
	* \addtogroup Actors
	* @{
	*/

	/**
	* \~chinese
	* @brief ��άͼ�ν�ɫ
	*/
	class KGE_API ShapeActor
		: public Actor
	{
	public:
		ShapeActor();

		ShapeActor(Geometry const& geometry);

		virtual ~ShapeActor();

		/// \~chinese
		/// @brief ��ȡ�����ɫ
		Color GetFillColor() const;

		/// \~chinese
		/// @brief ��ȡ������ɫ
		Color GetStrokeColor() const;

		/// \~chinese
		/// @brief ��ȡ�������
		float GetStrokeWidth() const;

		/// \~chinese
		/// @brief ��ȡ������ʽ
		StrokeStyle SetStrokeStyle() const;

		/// \~chinese
		/// @brief ��ȡ��״
		Geometry GetGeometry() const;

		/// \~chinese
		/// @brief ��ȡ�߽�
		Rect GetBounds() const override;

		/// \~chinese
		/// @brief ��ȡ���а�Χ��
		Rect GetBoundingBox() const override;

		/// \~chinese
		/// @brief �жϵ��Ƿ�����״��
		bool ContainsPoint(const Point& point) const override;

		/// \~chinese
		/// @brief ���������ɫ
		void SetFillColor(const Color& color);

		/// \~chinese
		/// @brief ����������ɫ
		void SetStrokeColor(const Color& color);

		/// \~chinese
		/// @brief ����������ȣ�Ĭ��Ϊ 1.0
		void SetStrokeWidth(float width);

		/// \~chinese
		/// @brief ����������ʽ
		void SetStrokeStyle(StrokeStyle stroke_style);

		/// \~chinese
		/// @brief ���ü�����״
		void SetGeometry(Geometry const& geometry);

		void OnRender(RenderTarget* rt) override;

	private:
		Color		fill_color_;
		Color		stroke_color_;
		float		stroke_width_;
		StrokeStyle	stroke_style_;
		Rect		bounds_;
		Geometry	geo_;
	};


	/// \~chinese
	/// @brief �߶�ͼ�ν�ɫ
	class KGE_API LineActor
		: public ShapeActor
	{
	public:
		LineActor();

		/// \~chinese
		/// @brief �߶�ͼ�ν�ɫ
		/// @param begin �߶����
		/// @param end �߶��յ�
		LineActor(Point const& begin, Point const& end);

		virtual ~LineActor();

		/// \~chinese
		/// @brief ��ȡ�߶����
		Point const& GetBeginPoint() const;

		/// \~chinese
		/// @brief ��ȡ�߶��յ�
		Point const& GetEndPoint() const;

		/// \~chinese
		/// @brief �����߶����
		/// @param begin �߶����
		void SetBeginPoint(Point const& begin);

		/// \~chinese
		/// @brief �����߶��յ�
		/// @param end �߶��յ�
		void SetEndPoint(Point const& end);

		/// \~chinese
		/// @brief ���þ��δ�С
		/// @param begin �߶����
		/// @param end �߶��յ�
		void SetLine(Point const& begin, Point const& end);

	private:
		Point begin_;
		Point end_;
	};


	/// \~chinese
	/// @brief ���ν�ɫ
	class KGE_API RectActor
		: public ShapeActor
	{
	public:
		RectActor();

		/// \~chinese
		/// @brief ������ν�ɫ
		/// @param size ���δ�С
		RectActor(Size const& size);

		virtual ~RectActor();

		/// \~chinese
		/// @brief ��ȡ���δ�С
		Size const& GetRectSize() const;

		/// \~chinese
		/// @brief ���þ��δ�С
		/// @param size ���δ�С
		void SetRectSize(Size const& size);

	private:
		Size rect_size_;
	};



	/// \~chinese
	/// @brief Բ�Ǿ��ν�ɫ
	class KGE_API RoundRectActor
		: public ShapeActor
	{
	public:
		RoundRectActor();

		/// \~chinese
		/// @brief ����Բ�Ǿ��ν�ɫ
		/// @param size Բ�Ǿ��δ�С
		/// @param radius Բ�ǰ뾶
		RoundRectActor(Size const& size, Vec2 const& radius);

		virtual ~RoundRectActor();

		/// \~chinese
		/// @brief ��ȡԲ�ǰ뾶
		Vec2 GetRadius() const;

		/// \~chinese
		/// @brief ��ȡԲ�Ǿ��δ�С
		Size GetRectSize() const;

		/// \~chinese
		/// @brief ����Բ�ǰ뾶
		/// @param radius Բ�ǰ뾶
		void SetRadius(Vec2 const& radius);

		/// \~chinese
		/// @brief ����Բ�Ǿ��δ�С
		/// @param size Բ�Ǿ��δ�С
		void SetRectSize(Size const& size);

		/// \~chinese
		/// @brief ����Բ�Ǿ���
		/// @param size Բ�Ǿ��δ�С
		/// @param radius Բ�ǰ뾶
		void SetRoundedRect(Size const& size, Vec2 const& radius);

	private:
		Size rect_size_;
		Vec2 radius_;
	};


	/// \~chinese
	/// @brief Բ�ν�ɫ
	class KGE_API CircleActor
		: public ShapeActor
	{
	public:
		CircleActor();

		/// \~chinese
		/// @brief ����Բ�ν�ɫ
		/// @param radius Բ�ΰ뾶
		CircleActor(float radius);

		virtual ~CircleActor();

		/// \~chinese
		/// @brief ��ȡԲ�ΰ뾶
		float GetRadius() const;

		/// \~chinese
		/// @brief ����Բ�ΰ뾶
		/// @param radius Բ�ΰ뾶
		void SetRadius(float radius);

	private:
		float radius_;
	};


	/// \~chinese
	/// @brief ��Բ��ɫ
	class KGE_API EllipseActor
		: public ShapeActor
	{
	public:
		EllipseActor();

		/// \~chinese
		/// @brief ������Բ��ɫ
		/// @param radius ��Բ�뾶
		EllipseActor(Vec2 const& radius);

		virtual ~EllipseActor();

		/// \~chinese
		/// @brief ��ȡ��Բ�뾶
		Vec2 GetRadius() const;

		/// \~chinese
		/// @brief ������Բ�뾶
		/// @param radius ��Բ�뾶
		void SetRadius(Vec2 const& radius);

	private:
		Vec2 radius_;
	};


	/// \~chinese
	/// @brief ����ν�ɫ
	class KGE_API PolygonActor
		: public ShapeActor
	{
	public:
		PolygonActor();

		/// \~chinese
		/// @brief �������ν�ɫ
		/// @param points ����ζ˵㼯��
		PolygonActor(Vector<Point> const& points);

		virtual ~PolygonActor();

		/// \~chinese
		/// @brief ���ö���ζ˵�
		/// @param points ����ζ˵㼯��
		void SetVertices(Vector<Point> const& points);
	};


	/// \~chinese
	/// @brief ·��ͼ�ν�ɫ
	class KGE_API PathActor
		: public ShapeActor
	{
	public:
		PathActor();

		virtual ~PathActor();

		/// \~chinese
		/// @brief ��ʼ���·��
		/// @param begin_pos ��ʼ��
		void BeginPath(Point const& begin_pos = Point());

		/// \~chinese
		/// @brief �������·��
		/// @param closed ·���Ƿ�պ�
		void EndPath(bool closed = true);

		/// \~chinese
		/// @brief ���һ���߶�
		/// @param point �߶ζ˵�
		void AddLine(Point const& point);

		/// \~chinese
		/// @brief ��Ӷ����߶�
		/// @param points �߶ζ˵㼯��
		void AddLines(Vector<Point> const& points);

		/// \~chinese
		/// @brief ���һ�����η�����������
		/// @param point1 ���������ߵĵ�һ�����Ƶ�
		/// @param point2 ���������ߵĵڶ������Ƶ�
		/// @param point3 ���������ߵ��յ�
		void AddBezier(Point const& point1, Point const& point2, Point const& point3);

		/// \~chinese
		/// @brief ��ӻ���
		/// @param point ��ԲԲ��
		/// @param radius ��Բ�뾶
		/// @param rotation ��Բ��ת�Ƕ�
		/// @param clockwise ˳ʱ�� or ��ʱ��
		/// @param is_small �Ƿ�ȡС�� 180�� �Ļ�
		void AddArc(Point const& point, Size const& radius, float rotation, bool clockwise = true, bool is_small = true);

		/// \~chinese
		/// @brief ���·��
		void ClearPath();

	private:
		GeometrySink sink_;
	};

	/** @} */
	

	inline Color		ShapeActor::GetFillColor() const				{ return fill_color_; }
	inline Color		ShapeActor::GetStrokeColor() const				{ return stroke_color_; }
	inline float		ShapeActor::GetStrokeWidth() const				{ return stroke_width_; }
	inline StrokeStyle	ShapeActor::SetStrokeStyle() const				{ return stroke_style_; }
	inline Geometry		ShapeActor::GetGeometry() const					{ return geo_; }

	inline Point const& LineActor::GetBeginPoint() const				{ return begin_; }
	inline Point const& LineActor::GetEndPoint() const					{ return end_; }
	inline void			LineActor::SetBeginPoint(Point const& begin)	{ SetLine(begin, end_); }
	inline void			LineActor::SetEndPoint(Point const& end)		{ SetLine(begin_, end); }

	inline Size const&	RectActor::GetRectSize() const					{ return rect_size_; }

	inline Vec2			RoundRectActor::GetRadius() const				{ return radius_; }
	inline Size			RoundRectActor::GetRectSize() const				{ return GetSize(); }

	inline float		CircleActor::GetRadius() const					{ return radius_; }

	inline Vec2			EllipseActor::GetRadius() const					{ return radius_; }
}
