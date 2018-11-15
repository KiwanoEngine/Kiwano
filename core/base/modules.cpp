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

#include "modules.h"
#include "logs.h"

namespace easy2d
{
	namespace modules
	{
		namespace
		{
			int initialize_count = 0;

			inline void SafeFreeLibrary(HMODULE instance)
			{
				if (instance)
					FreeLibrary(instance);
			}
		}

		Module_XAudio2 XAudio2;
		Module_MediaFoundation MediaFoundation;

		void Init()
		{
			initialize_count++;
			if (initialize_count > 1)
				return;

			E2D_LOG("Initing modules");

			const auto xaudio2_dll_names =
			{
				L"xaudio2_9.dll",
				L"xaudio2_8.dll",
				L"xaudio2_7.dll"
			};

			for (const auto& name : xaudio2_dll_names)
			{
				XAudio2.instance = LoadLibraryW(name);
				if (XAudio2.instance)
				{
					XAudio2.XAudio2Create = (PFN_XAudio2Create)
						GetProcAddress(XAudio2.instance, "XAudio2Create");
					break;
				}
			}

			MediaFoundation.mfplat = LoadLibraryW(L"Mfplat.dll");
			if (MediaFoundation.mfplat)
			{
				MediaFoundation.MFStartup = (PFN_MFStartup)
					GetProcAddress(MediaFoundation.mfplat, "MFStartup");

				MediaFoundation.MFShutdown = (PFN_MFShutdown)
					GetProcAddress(MediaFoundation.mfplat, "MFShutdown");

				MediaFoundation.MFCreateMediaType = (PFN_MFCreateMediaType)
					GetProcAddress(MediaFoundation.mfplat, "MFCreateMediaType");

				MediaFoundation.MFCreateWaveFormatExFromMFMediaType = (PFN_MFCreateWaveFormatExFromMFMediaType)
					GetProcAddress(MediaFoundation.mfplat, "MFCreateWaveFormatExFromMFMediaType");

				MediaFoundation.MFCreateMFByteStreamOnStream = (PFN_MFCreateMFByteStreamOnStream)
					GetProcAddress(MediaFoundation.mfplat, "MFCreateMFByteStreamOnStream");
			}

			MediaFoundation.mfreadwrite = LoadLibraryW(L"Mfreadwrite.dll");
			if (MediaFoundation.mfreadwrite)
			{
				MediaFoundation.MFCreateSourceReaderFromURL = (PFN_MFCreateSourceReaderFromURL)
					GetProcAddress(MediaFoundation.mfreadwrite, "MFCreateSourceReaderFromURL");

				MediaFoundation.MFCreateSourceReaderFromByteStream = (PFN_MFCreateSourceReaderFromByteStream)
					GetProcAddress(MediaFoundation.mfreadwrite, "MFCreateSourceReaderFromByteStream");
			}
		}

		void Destroy()
		{
			initialize_count--;
			if (initialize_count > 0)
				return;

			E2D_LOG("Destroying modules");

			SafeFreeLibrary(XAudio2.instance);
			SafeFreeLibrary(MediaFoundation.mfplat);
			SafeFreeLibrary(MediaFoundation.mfreadwrite);
		}
	}
}