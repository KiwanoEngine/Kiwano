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

#include <kiwano-audio/MediaFoundation/MFTranscoder.h>
#include <kiwano-audio/MediaFoundation/mflib.h>
#include <kiwano/core/Common.h>
#include <kiwano/utils/Logger.h>
#include <kiwano/platform/win32/ComPtr.hpp>
#include <kiwano/platform/win32/libraries.h>

namespace kiwano
{
namespace audio
{

HRESULT ReadSource(RefPtr<AudioData>& output, IMFSourceReader* reader);

MFTranscoder::MFTranscoder()
{
    HRESULT hr = dlls::MediaFoundation::Get().MFStartup(MF_VERSION, MFSTARTUP_FULL);
    KGE_THROW_IF_FAILED(hr, "Initialize Media Foundation failed!");
}

MFTranscoder::~MFTranscoder()
{
    dlls::MediaFoundation::Get().MFShutdown();
}

RefPtr<AudioData> MFTranscoder::Decode(StringView file_path)
{
    ComPtr<IMFSourceReader> reader;

    WideString path = strings::NarrowToWide(file_path);
    HRESULT    hr   = dlls::MediaFoundation::Get().MFCreateSourceReaderFromURL(path.c_str(), nullptr, &reader);

    RefPtr<AudioData> output;
    if (SUCCEEDED(hr))
    {
        hr = ReadSource(output, reader.Get());
    }

    if (FAILED(hr))
    {
        KGE_ERROR(strings::Format("%s failed (%#x): %s", __FUNCTION__, hr, "Load audio failed"));
        return nullptr;
    }
    return output;
}

RefPtr<AudioData> MFTranscoder::Decode(const Resource& res)
{
    HRESULT hr = S_OK;

    ComPtr<IStream>         stream;
    ComPtr<IMFByteStream>   byte_stream;
    ComPtr<IMFSourceReader> reader;

    BinaryData data = res.GetData();
    if (!data.IsValid())
    {
        KGE_ERROR("invalid audio data");
        return nullptr;
    }

    stream = win32::dlls::Shlwapi::Get().SHCreateMemStream(static_cast<const BYTE*>(data.buffer),
                                                           static_cast<uint32_t>(data.size));

    if (stream == nullptr)
    {
        KGE_ERROR("SHCreateMemStream failed");
        return nullptr;
    }

    if (SUCCEEDED(hr))
    {
        hr = dlls::MediaFoundation::Get().MFCreateMFByteStreamOnStream(stream.Get(), &byte_stream);
    }

    if (SUCCEEDED(hr))
    {
        hr = dlls::MediaFoundation::Get().MFCreateSourceReaderFromByteStream(byte_stream.Get(), nullptr, &reader);
    }

    RefPtr<AudioData> output;
    if (SUCCEEDED(hr))
    {
        hr = ReadSource(output, reader.Get());
    }

    if (FAILED(hr))
    {
        KGE_ERROR(strings::Format("%s failed (%#x): %s", __FUNCTION__, hr, "Load audio failed"));
        return nullptr;
    }
    return output;
}

class MFAudioData : public AudioData
{
public:
    MFAudioData(WAVEFORMATEX* wave_fmt, std::unique_ptr<uint8_t[]> raw, uint32_t size)
        : wave_fmt_(wave_fmt)
        , raw_(std::move(raw))
    {
        data_                 = BinaryData{ raw_.get(), size };
        meta_.format          = AudioFormat::PCM;
        meta_.channels        = uint16_t(wave_fmt->nChannels);
        meta_.samples_per_sec = uint32_t(wave_fmt->nSamplesPerSec);
        meta_.bits_per_sample = uint16_t(wave_fmt->wBitsPerSample);
        meta_.block_align     = uint16_t(wave_fmt->nBlockAlign);

        SetNative(wave_fmt);
    }

    virtual ~MFAudioData()
    {
        if (wave_fmt_ != nullptr)
        {
            ::CoTaskMemFree(wave_fmt_);
            wave_fmt_ = nullptr;
        }
    }

    WAVEFORMATEX*              wave_fmt_;
    std::unique_ptr<uint8_t[]> raw_;
};

HRESULT ReadSource(RefPtr<AudioData>& output, IMFSourceReader* reader)
{
    HRESULT hr = S_OK;

    ComPtr<IMFMediaType> partial_type;
    ComPtr<IMFMediaType> uncompressed_type;

    hr = dlls::MediaFoundation::Get().MFCreateMediaType(&partial_type);

    if (SUCCEEDED(hr))
    {
        hr = partial_type->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
    }

    if (SUCCEEDED(hr))
    {
        hr = partial_type->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
    }

    // 设置 source reader 的媒体类型，它将使用合适的解码器去解码这个音频
    if (SUCCEEDED(hr))
    {
        hr = reader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, partial_type.Get());
    }

    // 从 IMFMediaType 中获取 WAVEFORMAT 结构
    if (SUCCEEDED(hr))
    {
        hr = reader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, &uncompressed_type);
    }

    // 指定音频流
    if (SUCCEEDED(hr))
    {
        hr = reader->SetStreamSelection((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, true);
    }

    // 获取 WAVEFORMAT 数据
    WAVEFORMATEX* wave_format = nullptr;
    if (SUCCEEDED(hr))
    {
        uint32_t size = 0;

        hr = dlls::MediaFoundation::Get().MFCreateWaveFormatExFromMFMediaType(
            uncompressed_type.Get(), &wave_format, &size, (DWORD)MFWaveFormatExConvertFlag_Normal);
    }

    // 估算音频流大小
    DWORD max_stream_size = 0;
    if (SUCCEEDED(hr))
    {
        PROPVARIANT prop;
        PropVariantInit(&prop);

        hr = reader->GetPresentationAttribute((DWORD)MF_SOURCE_READER_MEDIASOURCE, MF_PD_DURATION, &prop);

        if (SUCCEEDED(hr))
        {
            LONGLONG duration = prop.uhVal.QuadPart;
            max_stream_size   = static_cast<DWORD>((duration * wave_format->nAvgBytesPerSec) / 10000000 + 1);
        }
        PropVariantClear(&prop);
    }

    // 读取音频数据
    if (SUCCEEDED(hr))
    {
        DWORD flags    = 0;
        DWORD position = 0;

        auto output_data = std::make_unique<uint8_t[]>(max_stream_size);

        ComPtr<IMFSample>      sample;
        ComPtr<IMFMediaBuffer> buffer;

        if (output_data == nullptr)
        {
            KGE_ERRORF("Low memory");
            hr = E_OUTOFMEMORY;
        }
        else
        {
            while (true)
            {
                hr = reader->ReadSample((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &flags, nullptr,
                                        &sample);

                if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
                {
                    break;
                }

                if (sample == nullptr)
                {
                    continue;
                }

                if (SUCCEEDED(hr))
                {
                    hr = sample->ConvertToContiguousBuffer(&buffer);

                    if (SUCCEEDED(hr))
                    {
                        BYTE* audio_data           = nullptr;
                        DWORD sample_buffer_length = 0;

                        hr = buffer->Lock(&audio_data, nullptr, &sample_buffer_length);

                        if (position + sample_buffer_length >= max_stream_size)
                        {
                            hr = E_FAIL;
                        }

                        if (SUCCEEDED(hr))
                        {
                            ::memcpy(output_data.get() + position, audio_data, sample_buffer_length);
                            position += sample_buffer_length;
                            hr = buffer->Unlock();
                        }
                    }
                    buffer = nullptr;
                }
                sample = nullptr;

                if (FAILED(hr))
                {
                    break;
                }
            }

            if (SUCCEEDED(hr))
            {
                output = new MFAudioData(wave_format, std::move(output_data), uint32_t(position));
                return hr;
            }
        }
    }
    return hr;
}
}  // namespace audio
}  // namespace kiwano
