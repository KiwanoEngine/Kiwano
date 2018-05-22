#pragma once
#include "e2dmacros.h"
#include "e2dcommon.h"

namespace e2d
{
	template<class Interface>
	inline void SafeRelease(Interface*& p)
	{
		if (p != nullptr)
		{
			p->Release();
			p = nullptr;
		}
	}

	class Music;

	// 音源回调
	class VoiceCallback 
		: public IXAudio2VoiceCallback
	{
	public:
		VoiceCallback(Music * music);
		~VoiceCallback();

		void __stdcall OnStreamEnd();

		void __stdcall OnBufferEnd(
			void * pBufferContext
		);

		void __stdcall OnBufferStart(
			void * pBufferContext
		);

		void __stdcall OnLoopEnd(
			void * pBufferContext
		);

		void __stdcall OnVoiceProcessingPassEnd();

		void __stdcall OnVoiceProcessingPassStart(
			UINT32 SamplesRequired
		);

		void __stdcall OnVoiceError(
			void * pBufferContext,
			HRESULT Error
		);

		void SetFuncOnStreamEnd(
			const Function& func
		);

		void SetFuncOnLoopEnd(
			const Function& func
		);

	protected:
		Music * _music;
		Function _loopEndFunc;
		Function _streamEndFunc;
	};


	// 文字渲染器
	class TextRenderer 
		: public IDWriteTextRenderer
	{
	public:
		TextRenderer(
			ID2D1Factory* pD2DFactory,
			ID2D1HwndRenderTarget* pRT,
			ID2D1SolidColorBrush* pBrush
		);

		~TextRenderer();

		STDMETHOD_(void, SetTextStyle)(
			CONST D2D1_COLOR_F &fillColor,
			BOOL hasOutline,
			CONST D2D1_COLOR_F &outlineColor,
			FLOAT outlineWidth,
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
		FLOAT fOutlineWidth;
		BOOL bShowOutline_;
		D2D1_LINE_JOIN nOutlineJoin_;
		ID2D1Factory* pD2DFactory_;
		ID2D1HwndRenderTarget* pRT_;
		ID2D1SolidColorBrush* pBrush_;
	};

}