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

#include <kiwano-audio/libraries.h>
#include <kiwano/core/Logger.h>

namespace kiwano
{
namespace audio
{
namespace dlls
{
XAudio2::XAudio2()
    : xaudio2()
    , XAudio2Create(nullptr)
{
    const auto xaudio2_dll_names = {
        "xaudio2_9.dll",  // for Windows 10
        "xaudio2_8.dll",  // for Windows 8
        "xaudio2_7.dll"   // for DirectX SDK
    };

    for (const auto& name : xaudio2_dll_names)
    {
        if (xaudio2.Load(name))
        {
            break;
        }
    }

    if (xaudio2.IsValid())
    {
        XAudio2Create = xaudio2.GetProcess<PFN_XAudio2Create>("XAudio2Create");
    }
    else
    {
        KGE_ERROR(L"Load xaudio2.dll failed");
        throw std::runtime_error("Load xaudio2.dll failed");
    }
}

MediaFoundation::MediaFoundation()
    : mfplat()
    , mfreadwrite()
    , MFStartup(nullptr)
    , MFShutdown(nullptr)
    , MFCreateMediaType(nullptr)
    , MFCreateWaveFormatExFromMFMediaType(nullptr)
    , MFCreateSourceReaderFromURL(nullptr)
    , MFCreateSourceReaderFromByteStream(nullptr)
    , MFCreateMFByteStreamOnStream(nullptr)
{
    if (mfplat.Load("Mfplat.dll"))
    {
        MFStartup         = mfplat.GetProcess<PFN_MFStartup>("MFStartup");
        MFShutdown        = mfplat.GetProcess<PFN_MFShutdown>("MFShutdown");
        MFCreateMediaType = mfplat.GetProcess<PFN_MFCreateMediaType>("MFCreateMediaType");
        MFCreateWaveFormatExFromMFMediaType =
            mfplat.GetProcess<PFN_MFCreateWaveFormatExFromMFMediaType>("MFCreateWaveFormatExFromMFMediaType");
        MFCreateMFByteStreamOnStream =
            mfplat.GetProcess<PFN_MFCreateMFByteStreamOnStream>("MFCreateMFByteStreamOnStream");
    }
    else
    {
        KGE_ERROR(L"Load Mfplat.dll failed");
        throw std::runtime_error("Load Mfplat.dll failed");
    }

    if (mfreadwrite.Load("Mfreadwrite.dll"))
    {
        MFCreateSourceReaderFromURL =
            mfreadwrite.GetProcess<PFN_MFCreateSourceReaderFromURL>("MFCreateSourceReaderFromURL");
        MFCreateSourceReaderFromByteStream =
            mfreadwrite.GetProcess<PFN_MFCreateSourceReaderFromByteStream>("MFCreateSourceReaderFromByteStream");
    }
    else
    {
        KGE_ERROR(L"Load Mfreadwrite.dll failed");
        throw std::runtime_error("Load Mfreadwrite.dll failed");
    }
}
}  // namespace dlls
}  // namespace audio
}  // namespace kiwano
