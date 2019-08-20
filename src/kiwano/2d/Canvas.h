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
#include "../renderer/RenderTarget.h"

#ifdef DrawText
#	undef DrawText
#endif

namespace kiwano
{
	// ����
	class KGE_API Canvas
		: public Actor
	{
	public:
		Canvas();

		virtual ~Canvas();

		// ��ʼ��ͼ
		void BeginDraw();

		// ������ͼ
		void EndDraw();

		// ��ֱ��
		void DrawLine(
			Point const& begin,
			Point const& end
		);

		// ��Բ�α߿�
		void DrawCircle(
			Point const& center,
			Float32 radius
		);

		// ����Բ�α߿�
		void DrawEllipse(
			Point const& center,
			Vec2 const& radius
		);

		// �����α߿�
		void DrawRect(
			Rect const& rect
		);

		// ��Բ�Ǿ��α߿�
		void DrawRoundedRect(
			Rect const& rect,
			Vec2 const& radius
		);

		// ���Բ��
		void FillCircle(
			Point const& center,
			Float32 radius
		);

		// �����Բ��
		void FillEllipse(
			Point const& center,
			Vec2 const& radius
		);

		// ������
		void FillRect(
			Rect const& rect
		);

		// ���Բ�Ǿ���
		void FillRoundedRect(
			Rect const& rect,
			Vec2 const& radius
		);

		// ��ͼƬ
		void DrawImage(
			Image const& image,
			const Rect* src_rect = nullptr,
			const Rect* dest_rect = nullptr
		);

		// ������
		void DrawText(
			String const& text,		/* ���� */
			Point const& point		/* ����λ�� */
		);

		// ��ʼ����·��
		void BeginPath(
			Point const& begin_pos	/* ·����ʼ�� */
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
			Point const& radius,	/* ��Բ�뾶 */
			Float32 rotation,		/* ��Բ��ת�Ƕ� */
			bool clockwise = true,	/* ˳ʱ�� or ��ʱ�� */
			bool is_small = true	/* �Ƿ�ȡС�� 180�� �Ļ� */
		);

		// ·�����
		void StrokePath();

		// ·�����
		void FillPath();

		// ��ջ���
		void Clear();

		// ��ջ���
		void Clear(
			Color const& clear_color
		);

		// ���������ɫ
		void SetFillColor(
			Color const& color
		);

		// ����������ɫ
		void SetStrokeColor(
			Color const& color
		);

		// �����������
		void SetStrokeWidth(
			Float32 width
		);

		// ����������ʽ
		void SetStrokeStyle(
			StrokeStyle stroke_style
		);

		// ������������
		void SetTextFont(
			Font const& font
		);

		// �������ֻ�ˢ��ʽ
		void SetTextStyle(
			TextStyle const& text_style
		);

		// ���û���͸����
		void SetBrushOpacity(
			Float32 opacity
		);

		// ���ʶ�ά�任
		void SetBrushTransform(
			Transform const& transform
		);

		// ���ʶ�ά�任
		void SetBrushTransform(
			Matrix3x2 const& transform
		);

		// ����ͼ��
		void PushLayerArea(
			LayerArea& area
		);

		// ����ͼ��
		void PopLayerArea();

		// ���òü�����
		void PushClipRect(
			Rect const& clip_rect
		);

		// �����ü�����
		void PopClipRect();

		// ��ȡ�����ɫ
		Color GetFillColor() const;

		// ��ȡ������ɫ
		Color GetStrokeColor() const;

		// ��ȡ�������
		Float32 GetStrokeWidth() const;

		// ��ȡ����͸����
		Float32 GetBrushOpacity() const;

		// ����ΪͼƬ
		Image ExportToImage() const;

		void OnRender(RenderTarget* rt) override;

	protected:
		void UpdateCache() const;

	protected:
		Float32				stroke_width_;
		Color				fill_color_;
		Color				stroke_color_;
		Font				text_font_;
		TextStyle			text_style_;
		StrokeStyle			stroke_style_;
		GeometrySink		geo_sink_;
		ImageRenderTarget	rt_;

		mutable bool		cache_expired_;
		mutable Image		image_cached_;
	};
}
