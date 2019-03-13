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
#include <xaudio2.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

namespace easy2d
{
	namespace modules
	{
		class E2D_API XAudio2
		{
			XAudio2();

			HMODULE xaudio2;

			// XAudio2 functions
			typedef HRESULT(WINAPI *PFN_XAudio2Create)(IXAudio2**, UINT32, XAUDIO2_PROCESSOR);

		public:
			static XAudio2& Get()
			{
				static XAudio2 instance;
				return instance;
			}

			PFN_XAudio2Create XAudio2Create;
		};


		class E2D_API MediaFoundation
		{
			MediaFoundation();

			HMODULE mfplat;
			HMODULE mfreadwrite;

			// MediaFoundation functions
			typedef HRESULT(WINAPI *PFN_MFStartup)(ULONG, DWORD);
			typedef HRESULT(WINAPI *PFN_MFShutdown)();
			typedef HRESULT(WINAPI *PFN_MFCreateMediaType)(IMFMediaType**);
			typedef HRESULT(WINAPI *PFN_MFCreateWaveFormatExFromMFMediaType)(IMFMediaType*, WAVEFORMATEX**, UINT32*, UINT32);
			typedef HRESULT(WINAPI *PFN_MFCreateSourceReaderFromURL)(LPCWSTR, IMFAttributes*, IMFSourceReader**);
			typedef HRESULT(WINAPI *PFN_MFCreateSourceReaderFromByteStream)(IMFByteStream*, IMFAttributes*, IMFSourceReader**);
			typedef HRESULT(WINAPI *PFN_MFCreateMFByteStreamOnStream)(IStream*, IMFByteStream**);

		public:
			static MediaFoundation& Get()
			{
				static MediaFoundation instance;
				return instance;
			}

			PFN_MFStartup MFStartup;
			PFN_MFShutdown MFShutdown;
			PFN_MFCreateMediaType MFCreateMediaType;
			PFN_MFCreateWaveFormatExFromMFMediaType MFCreateWaveFormatExFromMFMediaType;
			PFN_MFCreateSourceReaderFromURL MFCreateSourceReaderFromURL;
			PFN_MFCreateSourceReaderFromByteStream MFCreateSourceReaderFromByteStream;
			PFN_MFCreateMFByteStreamOnStream MFCreateMFByteStreamOnStream;
		};
	}
}
