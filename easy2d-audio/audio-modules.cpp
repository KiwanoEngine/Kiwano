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

#include "easy2d-audio.h"
#include "audio-modules.h"

namespace easy2d
{
	namespace modules
	{
		XAudio2::XAudio2()
		{
			const auto xaudio2_dll_names =
			{
				L"xaudio2_9.dll",
				L"xaudio2_8.dll",
				L"xaudio2_7.dll"
			};

			for (const auto& name : xaudio2_dll_names)
			{
				xaudio2 = LoadLibraryW(name);
				if (xaudio2)
				{
					XAudio2Create = (PFN_XAudio2Create)
						GetProcAddress(xaudio2, "XAudio2Create");
					break;
				}
			}

			if (!xaudio2)
			{
				E2D_LOG(L"load xaudio2.dll failed");
			}
		}

		MediaFoundation::MediaFoundation()
		{
			mfplat = LoadLibraryW(L"Mfplat.dll");
			if (mfplat)
			{
				MFStartup = (PFN_MFStartup)
					GetProcAddress(mfplat, "MFStartup");

				MFShutdown = (PFN_MFShutdown)
					GetProcAddress(mfplat, "MFShutdown");

				MFCreateMediaType = (PFN_MFCreateMediaType)
					GetProcAddress(mfplat, "MFCreateMediaType");

				MFCreateWaveFormatExFromMFMediaType = (PFN_MFCreateWaveFormatExFromMFMediaType)
					GetProcAddress(mfplat, "MFCreateWaveFormatExFromMFMediaType");

				MFCreateMFByteStreamOnStream = (PFN_MFCreateMFByteStreamOnStream)
					GetProcAddress(mfplat, "MFCreateMFByteStreamOnStream");
			}
			else
			{
				E2D_LOG(L"load Mfplat.dll failed");
			}

			mfreadwrite = LoadLibraryW(L"Mfreadwrite.dll");
			if (mfreadwrite)
			{
				MFCreateSourceReaderFromURL = (PFN_MFCreateSourceReaderFromURL)
					GetProcAddress(mfreadwrite, "MFCreateSourceReaderFromURL");

				MFCreateSourceReaderFromByteStream = (PFN_MFCreateSourceReaderFromByteStream)
					GetProcAddress(mfreadwrite, "MFCreateSourceReaderFromByteStream");
			}
			else
			{
				E2D_LOG(L"load Mfreadwrite.dll failed");
			}
		}
	}
}