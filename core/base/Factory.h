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
#include "include-forwards.h"
#include "Singleton.hpp"
#include "Font.hpp"
#include "Resource.h"
#include "TextRenderer.h"
#include "TextStyle.hpp"
#include "../math/Matrix.hpp"

namespace easy2d
{
	class FactoryImpl
		: protected Noncopyable
	{
		E2D_DECLARE_SINGLETON(FactoryImpl);

	public:
		HRESULT Init(bool debug);

		HRESULT CreateHwndRenderTarget(
			CpHwndRenderTarget& hwnd_render_target,
			D2D1_RENDER_TARGET_PROPERTIES const& properties,
			D2D1_HWND_RENDER_TARGET_PROPERTIES const& hwnd_rt_properties
		) const;

		HRESULT CreateTextRenderer(
			CpTextRenderer& text_renderer,
			CpRenderTarget const& render_target,
			CpSolidColorBrush const& brush
		);

		HRESULT CreateBitmapFromFile(
			CpBitmap& bitmap,
			CpRenderTarget const& rt,
			std::wstring const& file_path
		);

		HRESULT CreateBitmapFromResource(
			CpBitmap& bitmap,
			CpRenderTarget const& rt,
			Resource const& res
		);

		HRESULT CreateRectangleGeometry(
			CpRectangleGeometry& geo,
			Rect const& rect
		) const;

		HRESULT CreateRoundedRectangleGeometry(
			CpRoundedRectangleGeometry& geo,
			Rect const& rect,
			float radius_x,
			float radius_y
		) const;

		HRESULT CreateEllipseGeometry(
			CpEllipseGeometry& geo,
			Point const& center,
			float radius_x,
			float radius_y
		) const;

		HRESULT CreateTransformedGeometry(
			CpTransformedGeometry& transformed,
			Matrix const& matrix,
			CpGeometry const& geo
		) const;

		HRESULT CreatePathGeometry(
			CpPathGeometry& geometry
		) const;

		HRESULT CreateTextFormat(
			CpTextFormat& text_format,
			Font const& font,
			TextStyle const& text_style
		) const;

		HRESULT CreateTextLayout(
			CpTextLayout& text_layout,
			Size& layout_size,
			std::wstring const& text,
			CpTextFormat const& text_format,
			TextStyle const& text_style
		) const;

		CpStrokeStyle const& GetStrokeStyle(
			StrokeStyle stroke
		) const;

	protected:
		FactoryImpl();

		~FactoryImpl();

	protected:
		CpFactory			factory_;
		CpImagingFactory	imaging_factory_;
		CpWriteFactory		write_factory_;
		CpStrokeStyle		miter_stroke_style_;
		CpStrokeStyle		bevel_stroke_style_;
		CpStrokeStyle		round_stroke_style_;
	};

	E2D_DECLARE_SINGLETON_TYPE(FactoryImpl, Factory);
}
