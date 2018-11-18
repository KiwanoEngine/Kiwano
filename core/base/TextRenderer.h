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
#include "d2dres.hpp"

namespace easy2d
{
	class ITextRenderer
		: public IDWriteTextRenderer
	{
	public:
		static HRESULT Create(
			ITextRenderer** ppTextRenderer,
			ID2D1Factory* pD2DFactory,
			ID2D1RenderTarget* pRT,
			ID2D1SolidColorBrush* pBrush
		);

		STDMETHOD_(void, SetTextStyle)(
			CONST D2D1_COLOR_F &fillColor,
			BOOL outline,
			CONST D2D1_COLOR_F &outlineColor,
			FLOAT outlineWidth,
			ID2D1StrokeStyle* outlineJoin
		);

		STDMETHOD(DrawGlyphRun)(
			__maybenull void* clientDrawingContext,
			FLOAT baselineOriginX,
			FLOAT baselineOriginY,
			DWRITE_MEASURING_MODE measuringMode,
			__in DWRITE_GLYPH_RUN const* glyphRun,
			__in DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
			IUnknown* clientDrawingEffect
		);

		STDMETHOD(DrawUnderline)(
			__maybenull void* clientDrawingContext,
			FLOAT baselineOriginX,
			FLOAT baselineOriginY,
			__in DWRITE_UNDERLINE const* underline,
			IUnknown* clientDrawingEffect
		);

		STDMETHOD(DrawStrikethrough)(
			__maybenull void* clientDrawingContext,
			FLOAT baselineOriginX,
			FLOAT baselineOriginY,
			__in DWRITE_STRIKETHROUGH const* strikethrough,
			IUnknown* clientDrawingEffect
		);

		STDMETHOD(DrawInlineObject)(
			__maybenull void* clientDrawingContext,
			FLOAT originX,
			FLOAT originY,
			IDWriteInlineObject* inlineObject,
			BOOL IsSideways,
			BOOL IsRightToLeft,
			IUnknown* clientDrawingEffect
		);

		STDMETHOD(IsPixelSnappingDisabled)(
			__maybenull void* clientDrawingContext,
			__out BOOL* isDisabled
		);

		STDMETHOD(GetCurrentTransform)(
			__maybenull void* clientDrawingContext,
			__out DWRITE_MATRIX* transform
		);

		STDMETHOD(GetPixelsPerDip)(
			__maybenull void* clientDrawingContext,
			__out FLOAT* pixelsPerDip
		);

	public:
		unsigned long STDMETHODCALLTYPE AddRef();
		unsigned long STDMETHODCALLTYPE Release();
		HRESULT STDMETHODCALLTYPE QueryInterface(
			IID const& riid,
			void** ppvObject
		);

	private:
		ITextRenderer(
			ID2D1Factory* pD2DFactory,
			ID2D1RenderTarget* pRT,
			ID2D1SolidColorBrush* pBrush
		);

		~ITextRenderer();

	private:
		unsigned long			cRefCount_;
		D2D1_COLOR_F			sFillColor_;
		D2D1_COLOR_F			sOutlineColor_;
		FLOAT					fOutlineWidth;
		BOOL					bShowOutline_;
		ID2D1Factory*			pD2DFactory_;
		ID2D1RenderTarget*		pRT_;
		ID2D1SolidColorBrush*	pBrush_;
		ID2D1StrokeStyle*		pCurrStrokeStyle_;
	};
}

E2D_DECLARE_D2D_SMART_PTR(easy2d::ITextRenderer, cpTextRenderer);
