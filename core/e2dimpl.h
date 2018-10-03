#pragma once
#include "e2dutil.h"

namespace e2d
{


	class Music;

	// 音源回调
	class VoiceCallback
		: public IXAudio2VoiceCallback
	{
	public:
		VoiceCallback();

		~VoiceCallback();

		STDMETHOD_(void, OnVoiceProcessingPassStart) (THIS_ UINT32 BytesRequired) {}

		STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS) {}

		STDMETHOD_(void, OnStreamEnd) (THIS);

		STDMETHOD_(void, OnBufferStart) (THIS_ void* pBufferContext) {}

		STDMETHOD_(void, OnBufferEnd) (THIS_ void* pBufferContext);

		STDMETHOD_(void, OnLoopEnd) (THIS_ void* pBufferContext);

		STDMETHOD_(void, OnVoiceError) (THIS_ void* pBufferContext, HRESULT Error) {}

		STDMETHOD_(void, SetCallbackOnStreamEnd) (THIS_ const Function& func);

		STDMETHOD_(void, SetCallbackOnLoopEnd) (THIS_ const Function& func);

	protected:
		Function loop_end_callback_;
		Function stream_end_callback_;
	};


	// 文字渲染器
	class E2DTextRender
		: public IDWriteTextRenderer
	{
	private:
		E2DTextRender();

		~E2DTextRender();

	public:
		static HRESULT Create(
			E2DTextRender** ppTextRenderer,
			ID2D1Factory* pD2DFactory,
			ID2D1HwndRenderTarget* pRT,
			ID2D1SolidColorBrush* pBrush
		);

		STDMETHOD_(void, SetTextStyle)(
			CONST D2D1_COLOR_F &fillColor,
			BOOL outline,
			CONST D2D1_COLOR_F &outline_color,
			FLOAT outline_width,
			D2D1_LINE_JOIN outlineJoin
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
		unsigned long cRefCount_;
		D2D1_COLOR_F sFillColor_;
		D2D1_COLOR_F sOutlineColor_;
		FLOAT fOutlineWidth;
		BOOL bShowOutline_;
		ID2D1Factory* pD2DFactory_;
		ID2D1HwndRenderTarget* pRT_;
		ID2D1SolidColorBrush* pBrush_;
		ID2D1StrokeStyle * pCurrStrokeStyle_;
	};


	// 运行时异常
	class RuntimeException
		: public std::exception
	{
	public:
		RuntimeException() E2D_NOEXCEPT
			: exception("unknown runtime exception", 1)
		{
		}

		explicit RuntimeException(
			char const* const message
		) E2D_NOEXCEPT
			: exception(message, 1)
		{
		}
	};


	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			// 在此处设置断点以捕获 D2D API 异常.
			static char s_str[64] = {};
			sprintf_s(s_str, "Failure with HRESULT of %08X", static_cast<unsigned int>(hr));
			throw RuntimeException(s_str);
		}
	}

}