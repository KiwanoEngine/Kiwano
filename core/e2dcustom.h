#pragma once
#include "e2dmacros.h"

namespace e2d
{
	template<class Interface>
	inline void SafeReleaseInterface(Interface **pp) 
	{ 
		if (*pp != nullptr) 
		{ 
			(*pp)->Release(); 
			(*pp) = nullptr; 
		}
	}

	// 自定义的文字渲染器
	class CustomTextRenderer 
		: public IDWriteTextRenderer
	{
	public:
		CustomTextRenderer(
			ID2D1Factory* pD2DFactory,
			ID2D1HwndRenderTarget* pRT,
			ID2D1SolidColorBrush* pBrush
		);

		~CustomTextRenderer();

		STDMETHOD_(void, SetTextStyle)(
			CONST D2D1_COLOR_F &fillColor,
			CONST D2D1_COLOR_F &outlineColor,
			FLOAT strokeWidth,
			FLOAT opacity
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
			BOOL isSideways,
			BOOL isRightToLeft,
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
		unsigned long cRefCount_;
		D2D1_COLOR_F sFillColor_;
		D2D1_COLOR_F sOutlineColor_;
		FLOAT fStrokeWidth_;
		FLOAT fOpacity_;
		ID2D1Factory* pD2DFactory_;
		ID2D1HwndRenderTarget* pRT_;
		ID2D1SolidColorBrush* pBrush_;
	};

}