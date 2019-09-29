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
#include "win32/D2DDeviceResources.h"

namespace kiwano
{
	// ����ת����
	struct GradientStop
	{
		float offset;
		Color color;

		GradientStop() : offset(0.f), color() {}

		GradientStop(float offset, Color color) : offset(offset), color(color) {}
	};


	// ��������ģʽ
	// ��ģʽ����ָ��������λ�������������Ĳ���
	// Clamp  (��ģʽ): �ظ����Ʊ߽���ɫ
	// Wrap   (��ģʽ): �ظ���������
	// Mirror (����ģʽ): ��ת��������
	enum class GradientExtendMode
	{
		Clamp,
		Wrap,
		Mirror
	};


	// ��ɫ��ʽ
	struct SolidColorStyle
	{
		Color color;

		SolidColorStyle(Color const& color);
	};

	// ���Խ�����ʽ
	struct LinearGradientStyle
	{
		Point begin;
		Point end;
		Vector<GradientStop> stops;
		GradientExtendMode extend_mode;

		LinearGradientStyle(
			Point const& begin,
			Point const& end,
			Vector<GradientStop> const& stops,
			GradientExtendMode extend_mode = GradientExtendMode::Clamp
		);
	};

	// ���򽥱���ʽ
	struct RadialGradientStyle
	{
		Point center;
		Vec2 offset;
		Vec2 radius;
		Vector<GradientStop> stops;
		GradientExtendMode extend_mode;

		RadialGradientStyle(
			Point const& center,
			Vec2 const& offset,
			Vec2 const& radius,
			Vector<GradientStop> const& stops,
			GradientExtendMode extend_mode = GradientExtendMode::Clamp
		);
	};

	// ����
	class KGE_API Brush
	{
	public:
		Brush();

		Brush(
			Color const& color
		);

		Brush(
			SolidColorStyle const& style
		);

		Brush(
			LinearGradientStyle const& style
		);

		Brush(
			RadialGradientStyle const& style
		);

		bool IsValid() const;

		inline void SetColor(
			Color const& color
		)
		{
			SetStyle(SolidColorStyle{ color });
		}

		void SetStyle(
			SolidColorStyle const& style
		);

		void SetStyle(
			LinearGradientStyle const& style
		);

		void SetStyle(
			RadialGradientStyle const& style
		);

		float GetOpacity() const;

		void SetOpacity(float opacity);

	public:
		enum class Type
		{
			Unknown,
			SolidColor,			// ��ɫ���
			LinearGradient,		// ���Խ���
			RadialGradient		// ���򽥱�
		};

		Type GetType() const { return type_; }

	public:
		Brush(
			ComPtr<ID2D1Brush> brush
		);

		void SetBrush(ComPtr<ID2D1Brush> const& brush);

		inline ComPtr<ID2D1Brush> const& GetBrush() const { return raw_; }

	protected:
		Type type_;
		float opacity_;
		ComPtr<ID2D1Brush> raw_;
	};

}
