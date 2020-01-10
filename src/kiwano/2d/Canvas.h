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
#include <kiwano/renderer/RenderTarget.h>
#include <kiwano/renderer/GeometrySink.h>

namespace kiwano
{
	KGE_DECLARE_SMART_PTR(Canvas);

	/**
	* \addtogroup Actors
	* @{
	*/

	/**
	* \~chinese
	* @brief ���������ڻ���ͼԪ
	*/
	class KGE_API Canvas
		: public Actor
	{
	public:
		/// \~chinese
		/// @brief �����ջ���
		Canvas();

		virtual ~Canvas();

		/// \~chinese
		/// @brief ��ʼ��ͼ
		void BeginDraw();

		/// \~chinese
		/// @brief ������ͼ
		void EndDraw();

		/// \~chinese
		/// @brief ���߶�
		/// @param begin �߶����
		/// @param end �߶��յ�
		void DrawLine(Point const& begin, Point const& end);

		/// \~chinese
		/// @brief ��Բ�α߿�
		/// @param center Բ��ԭ��
		/// @param radius Բ�ΰ뾶
		void DrawCircle(Point const& center, float radius);

		/// \~chinese
		/// @brief ����Բ�α߿�
		/// @param center ��Բԭ��
		/// @param radius ��Բ�뾶
		void DrawEllipse(Point const& center, Vec2 const& radius);

		/// \~chinese
		/// @brief �����α߿�
		/// @param rect ����
		void DrawRect(Rect const& rect);

		/// \~chinese
		/// @brief ��Բ�Ǿ��α߿�
		/// @param rect ����
		/// @param radius ����Բ�ǰ뾶
		void DrawRoundedRect(Rect const& rect, Vec2 const& radius);

		/// \~chinese
		/// @brief ���Բ��
		/// @param center Բ��ԭ��
		/// @param radius Բ�ΰ뾶
		void FillCircle(Point const& center, float radius);

		/// \~chinese
		/// @brief �����Բ��
		/// @param center ��Բԭ��
		/// @param radius ��Բ�뾶
		void FillEllipse(Point const& center, Vec2 const& radius);

		/// \~chinese
		/// @brief ������
		/// @param rect ����
		void FillRect(Rect const& rect);

		/// \~chinese
		/// @brief ���Բ�Ǿ���
		/// @param rect ����
		/// @param radius ����Բ�ǰ뾶
		void FillRoundedRect(Rect const& rect, Vec2 const& radius);

		/// \~chinese
		/// @brief ��������
		/// @param texture ����
		/// @param src_rect ����ü�����
		/// @param dest_rect ����Ŀ������
		void DrawTexture(TexturePtr texture, const Rect* src_rect = nullptr, const Rect* dest_rect = nullptr);

		/// \~chinese
		/// @brief �������ֲ���
		/// @param text ����
		/// @param point �������ֵ�λ��
		void DrawTextLayout(String const& text, Point const& point);

		/// \~chinese
		/// @brief �������ֲ���
		/// @param layout ���ֲ���
		/// @param point ���Ʋ��ֵ�λ��
		void DrawTextLayout(TextLayout const& layout, Point const& point);

		/// \~chinese
		/// @brief ��ʼ����·��
		/// @param begin_pos ·����ʼ��
		void BeginPath(Point const& begin_pos);

		/// \~chinese
		/// @brief ����·��
		/// @param closed ·���Ƿ�պ�
		void EndPath(bool closed = false);

		/// \~chinese
		/// @brief ���һ���߶�
		/// @param point �˵�
		void AddLine(Point const& point);

		/// \~chinese
		/// @brief ��Ӷ����߶�
		/// @param points �˵㼯��
		void AddLines(Vector<Point> const& points);

		/// \~chinese
		/// @brief ���һ�����η�����������
		/// @param point1 ���������ߵĵ�һ�����Ƶ�
		/// @param point2 ���������ߵĵڶ������Ƶ�
		/// @param point3 ���������ߵ��յ�
		void AddBezier(Point const& point1, Point const& point2, Point const& point3);

		/// \~chinese
		/// @brief ��ӻ���
		/// @param point �յ�
		/// @param radius ��Բ�뾶
		/// @param rotation ��Բ��ת�Ƕ�
		/// @param clockwise ˳ʱ�� or ��ʱ��
		/// @param is_small �Ƿ�ȡС�� 180�� �Ļ�
		void AddArc(Point const& point, Size const& radius, float rotation, bool clockwise = true, bool is_small = true);

		/// \~chinese
		/// @brief ����ߵķ�ʽ����·��
		void StrokePath();

		/// \~chinese
		/// @brief �����ķ�ʽ����·��
		void FillPath();

		/// \~chinese
		/// @brief ��ջ���
		void Clear();

		/// \~chinese
		/// @brief ��ջ���
		/// @param clear_color �����ɫ
		void Clear(Color const& clear_color);

		/// \~chinese
		/// @brief ���������ɫ
		/// @param color �����ɫ
		void SetFillColor(Color const& color);

		/// \~chinese
		/// @brief ������仭ˢ
		/// @param[in] brush ��仭ˢ
		void SetFillBrush(BrushPtr brush);

		/// \~chinese
		/// @brief ����������ɫ
		/// @param color ������ɫ
		void SetStrokeColor(Color const& color);

		/// \~chinese
		/// @brief ����������ˢ
		/// @param[in] brush ������ˢ
		void SetStrokeBrush(BrushPtr brush);

		/// \~chinese
		/// @brief �����������
		/// @param width �������
		void SetStrokeWidth(float width);

		/// \~chinese
		/// @brief ����������ʽ
		/// @param stroke_style ������ʽ
		void SetStrokeStyle(const StrokeStyle& stroke_style);

		/// \~chinese
		/// @brief �������ֻ�ˢ��ʽ
		/// @param text_style ���ֻ�ˢ��ʽ
		void SetTextStyle(TextStyle const& text_style);

		/// \~chinese
		/// @brief ���û�ˢ
		/// @param[in] brush ��ˢ
		void SetBrush(BrushPtr brush);

		/// \~chinese
		/// @brief ���û�ˢ��ά�任
		/// @param transform ��ά�任
		void SetBrushTransform(Transform const& transform);

		/// \~chinese
		/// @brief ���û�ˢ��ά�任����
		/// @param transform ��ά�任����
		void SetBrushTransform(Matrix3x2 const& transform);

		/// \~chinese
		/// @brief ���һ��ͼ��
		/// @param area ͼ������
		void PushLayerArea(LayerArea& area);

		/// \~chinese
		/// @brief ɾ�������ӵ�ͼ��
		void PopLayerArea();

		/// \~chinese
		/// @brief ���һ���ü�����
		/// @param clip_rect �ü�����
		void PushClipRect(Rect const& clip_rect);

		/// \~chinese
		/// @brief ɾ�������ӵĲü�����
		void PopClipRect();

		/// \~chinese
		/// @brief ��ȡ�������
		float GetStrokeWidth() const;

		/// \~chinese
		/// @brief ��ȡ��仭ˢ
		BrushPtr GetFillBrush() const;

		/// \~chinese
		/// @brief ��ȡ������ˢ
		BrushPtr GetStrokeBrush() const;

		/// \~chinese
		/// @brief ��������
		TexturePtr ExportToTexture() const;

		void OnRender(RenderTarget* rt) override;

	private:
		void InitRenderTargetAndBrushs();

		void UpdateCache() const;

	private:
		float			stroke_width_;
		TextStyle		text_style_;
		StrokeStyle		stroke_style_;
		GeometrySink	geo_sink_;
		BrushPtr		fill_brush_;
		BrushPtr		stroke_brush_;

		mutable bool					cache_expired_;
		mutable TexturePtr				texture_cached_;
		mutable TextureRenderTargetPtr	rt_;
	};

	/** @} */

	inline void Canvas::SetStrokeWidth(float width)
	{
		stroke_width_ = std::max(width, 0.f);
	}

	inline void Canvas::SetStrokeStyle(const StrokeStyle& stroke_style)
	{
		stroke_style_ = stroke_style;
	}

	inline void Canvas::SetTextStyle(TextStyle const& text_style)
	{
		text_style_ = text_style;
	}

	inline void Canvas::SetStrokeColor(Color const& color)
	{
		InitRenderTargetAndBrushs();
		stroke_brush_->SetColor(color);
	}

	inline void Canvas::SetFillColor(Color const& color)
	{
		InitRenderTargetAndBrushs();
		fill_brush_->SetColor(color);
	}

	inline void Canvas::SetFillBrush(BrushPtr brush)
	{
		fill_brush_ = brush;
	}

	inline void Canvas::SetStrokeBrush(BrushPtr brush)
	{
		stroke_brush_ = brush;
	}

	inline BrushPtr Canvas::GetFillBrush() const
	{
		return fill_brush_;
	}

	inline BrushPtr Canvas::GetStrokeBrush() const
	{
		return stroke_brush_;
	}

}
