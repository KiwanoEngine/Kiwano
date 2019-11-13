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
#include <kiwano/core/Library.h>
#include <xaudio2.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

namespace kiwano
{
	namespace audio
	{
		namespace modules
		{
			class KGE_API XAudio2
			{
			public:
				static inline XAudio2& Get()
				{
					static XAudio2 instance;
					return instance;
				}

				// XAudio2 functions
				typedef HRESULT(WINAPI* PFN_XAudio2Create)(IXAudio2**, UINT32, XAUDIO2_PROCESSOR);

				PFN_XAudio2Create XAudio2Create;

			private:
				XAudio2();

				XAudio2(const XAudio2&) = delete;
				XAudio2& operator=(const XAudio2&) = delete;

				Library xaudio2;
			};


			class KGE_API MediaFoundation
			{
			public:
				static inline MediaFoundation& Get()
				{
					static MediaFoundation instance;
					return instance;
				}

				// MediaFoundation functions
				typedef HRESULT(WINAPI* PFN_MFStartup)(ULONG, DWORD);
				typedef HRESULT(WINAPI* PFN_MFShutdown)();
				typedef HRESULT(WINAPI* PFN_MFCreateMediaType)(IMFMediaType**);
				typedef HRESULT(WINAPI* PFN_MFCreateWaveFormatExFromMFMediaType)(IMFMediaType*, WAVEFORMATEX**, UINT32*, UINT32);
				typedef HRESULT(WINAPI* PFN_MFCreateSourceReaderFromURL)(LPCWSTR, IMFAttributes*, IMFSourceReader**);
				typedef HRESULT(WINAPI* PFN_MFCreateSourceReaderFromByteStream)(IMFByteStream*, IMFAttributes*, IMFSourceReader**);
				typedef HRESULT(WINAPI* PFN_MFCreateMFByteStreamOnStream)(IStream*, IMFByteStream**);

				PFN_MFStartup MFStartup;
				PFN_MFShutdown MFShutdown;
				PFN_MFCreateMediaType MFCreateMediaType;
				PFN_MFCreateWaveFormatExFromMFMediaType MFCreateWaveFormatExFromMFMediaType;
				PFN_MFCreateSourceReaderFromURL MFCreateSourceReaderFromURL;
				PFN_MFCreateSourceReaderFromByteStream MFCreateSourceReaderFromByteStream;
				PFN_MFCreateMFByteStreamOnStream MFCreateMFByteStreamOnStream;

			private:
				MediaFoundation();

				MediaFoundation(const MediaFoundation&) = delete;
				MediaFoundation& operator=(const MediaFoundation&) = delete;

				Library mfplat;
				Library mfreadwrite;
			};
		}
	}
}
