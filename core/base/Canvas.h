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
#include "Node.h"
#include "Font.hpp"
#include "TextStyle.hpp"
#include "TextRenderer.h"

#undef DrawText

namespace easy2d
{
	// ����
	class Canvas
		: public Node
	{
	public:
		Canvas();

		Canvas(
			Size const& size
		);

		Canvas(
			float width,
			float height
		);

		virtual ~Canvas();

		// ��ʼ��ͼ
		void BeginDraw();

		// ������ͼ
		void EndDraw();

		// ��ֱ��
		void DrawLine(
			const Point& begin,
			const Point& end
		);

		// ��Բ�α߿�
		void DrawCircle(
			const Point& center,
			float radius
		);

		// ����Բ�α߿�
		void DrawEllipse(
			const Point& center,
			float radius_x,
			float radius_y
		);

		// �����α߿�
		void DrawRect(
			const Rect& rect
		);

		// ��Բ�Ǿ��α߿�
		void DrawRoundedRect(
			const Rect& rect,
			float radius_x,
			float radius_y
		);

		// ��ͼƬ
		void DrawImage(
			spImage const& image,
			float opacity = 1.f
		);

		// ������
		void DrawText(
			String const& text,		/* ���� */
			Point const& point		/* ����λ�� */
		);

		// ������ͼ�α߿�
		void DrawGeometry(
			spGeometry const& geo
		);

		// ���Բ��
		void FillCircle(
			const Point& center,
			float radius
		);

		// �����Բ��
		void FillEllipse(
			const Point& center,
			float radius_x,
			float radius_y
		);

		// ������
		void FillRect(
			const Rect& rect
		);

		// ���Բ�Ǿ���
		void FillRoundedRect(
			const Rect& rect,
			float radius_x,
			float radius_y
		);

		// ��伸��ͼ��
		void FillGeometry(
			spGeometry const& geo
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
			std::vector<Point> const& points
		);

		// ���һ�����η�����������
		void AddBezier(
			Point const& point1,	/* ���������ߵĵ�һ�����Ƶ� */
			Point const& point2,	/* ���������ߵĵڶ������Ƶ� */
			Point const& point3		/* ���������ߵ��յ� */
		);

		// ·�����
		void StrokePath();

		// ·�����
		void FillPath();

		// ��ջ���
		void Clear();

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

		// �������ֻ�ˢ��ʽ
		void SetTextStyle(
			Font const& font,
			TextStyle const& text_style
		);

		// ��ȡ�����ɫ
		Color GetFillColor() const;

		// ��ȡ������ɫ
		Color GetStrokeColor() const;

		// ��ȡ�������
		float GetStrokeWidth() const;

		// �任����
		void SetBrushTransform(
			math::Matrix const& transform
		);

		// ����ΪͼƬ
		spImage ExportToImage() const;

		virtual void OnDraw() override;

	protected:
		cpBitmap const& GetBitmap() const;

	protected:
		mutable bool			cache_expired_;
		mutable cpBitmap		bitmap_cached_;
		float					stroke_width_;
		Font					text_font_;
		TextStyle				text_style_;
		cpPathGeometry			current_geometry_;
		cpGeometrySink			current_sink_;
		cpStrokeStyle			outline_join_style_;
		cpSolidColorBrush		fill_brush_;
		cpSolidColorBrush		stroke_brush_;
		cpSolidColorBrush		text_brush_;
		cpTextRenderer			text_renderer_;
		cpBitmapRenderTarget	render_target_;
	};
}