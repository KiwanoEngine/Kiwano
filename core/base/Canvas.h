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

namespace easy2d
{
	E2D_DECLARE_SMART_PTR(CanvasBrush);

	// ����
	class CanvasBrush
		: public RefCounter
	{
	public:
		CanvasBrush();

		virtual ~CanvasBrush();

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

		// ����������ɫ
		void SetLineColor(
			const Color& color
		);

		// ���������ɫ
		void SetFillColor(
			const Color& color
		);

		// �����������
		void SetStrokeWidth(
			float width
		);

		// ���������ཻ��ʽ
		void SetStrokeStyle(
			StrokeStyle stroke
		);

		// ��ȡ������ɫ
		Color GetLineColor() const;

		// ��ȡ�����ɫ
		Color GetFillColor() const;

		// ��ȡ�������
		float GetStrokeWidth() const;

		// ��ȡ�����ཻ��ʽ
		StrokeStyle GetStrokeStyle() const;

	protected:
		float				stroke_width_;
		StrokeStyle			stroke_;
		cpRenderTarget		render_target_;
		cpSolidColorBrush	fill_brush_;
		cpSolidColorBrush	line_brush_;
		cpStrokeStyle		stroke_style_;
	};


	// ����
	class Canvas
		: public Node
	{
	public:
		Canvas();

		Canvas(
			float width,
			float height
		);

		virtual ~Canvas();

		virtual void OnDraw(CanvasBrush& brush) = 0;

		// ���û�ˢ
		void SetBrush(
			spCanvasBrush const& brush
		);

		virtual void OnDraw() override;

	private:
		spCanvasBrush brush_;
	};
}