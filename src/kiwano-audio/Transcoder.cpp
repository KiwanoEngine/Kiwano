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

#ifndef INITGUID
#define INITGUID  // MFAudioFormat_PCM, MF_MT_MAJOR_TYPE, MF_MT_SUBTYPE, MFMediaType_Audio
#endif

#include <kiwano-audio/Transcoder.h>
#include <kiwano-audio/libraries.h>
#include <kiwano/core/Common.h>
#include <kiwano/core/Resource.h>
#include <kiwano/utils/Logger.h>
#include <kiwano/platform/win32/ComPtr.hpp>
#include <kiwano/platform/win32/libraries.h>
#include <kiwano/platform/FileSystem.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

namespace kiwano
{
namespace audio
{

HRESULT ReadSource(IMFSourceReader* reader, AudioMetadata& output_metadata, std::unique_ptr<uint8_t[]>& output_data,
                   uint32_t& output_size);

Transcoder::Transcoder(const String& file_path)
{
    Load(file_path);
}

Transcoder::Transcoder(const Resource& res)
{
    Load(res);
}

Transcoder::Transcoder(const BinaryData& data, const AudioMetadata& metadata)
{
    Load(data, metadata);
}

Transcoder::~Transcoder()
{
    Clear();
}

AudioMetadata Transcoder::GetMetadata() const
{
    return metadata_;
}

BinaryData Transcoder::GetData() const
{
    return data_;
}

void Transcoder::Clear()
{
    if (metadata_.extra_data != nullptr)
    {
        WAVEFORMATEX* wave_format = reinterpret_cast<WAVEFORMATEX*>(metadata_.extra_data);
        ::CoTaskMemFree(wave_format);
        metadata_.extra_data = nullptr;
    }

    data_ = {};
    raw_.reset();
}

bool Transcoder::Load(const BinaryData& data, const AudioMetadata& metadata)
{
    data_     = data;
    metadata_ = metadata;
    return true;
}

bool Transcoder::Load(const String& file_path)
{
    if (!FileSystem::GetInstance().IsFileExists(file_path))
    {
        KGE_WARNF("Media file '%s' not found", file_path.c_str());
        return false;
    }

    String     full_path = FileSystem::GetInstance().GetFullPathForFile(file_path);
    WideString path      = strings::NarrowToWide(full_path);

    ComPtr<IMFSourceReader> reader;

    HRESULT hr = dlls::MediaFoundation::Get().MFCreateSourceReaderFromURL(path.c_str(), nullptr, &reader);
    if (SUCCEEDED(hr))
    {
        hr = ReadSource(reader.Get(), metadata_, raw_, data_.size);

        if (SUCCEEDED(hr))
        {
            data_.buffer = raw_.get();
        }
    }

    if (FAILED(hr))
    {
        Fail(strings::Format("%s failed (%#x): %s", __FUNCTION__, hr, "Load audio failed"));
        return false;
    }
    return true;
}

bool Transcoder::Load(const Resource& res)
{
    HRESULT hr = S_OK;

    ComPtr<IStream>         stream;
    ComPtr<IMFByteStream>   byte_stream;
    ComPtr<IMFSourceReader> reader;

    BinaryData data = res.GetData();
    if (!data.IsValid())
    {
        Fail("invalid audio data");
        return false;
    }

    stream = win32::dlls::Shlwapi::Get().SHCreateMemStream(static_cast<const BYTE*>(data.buffer),
                                                           static_cast<uint32_t>(data.size));

    if (stream == nullptr)
    {
        Fail("SHCreateMemStream failed");
        return false;
    }

    if (SUCCEEDED(hr))
    {
        hr = dlls::MediaFoundation::Get().MFCreateMFByteStreamOnStream(stream.Get(), &byte_stream);
    }

    if (SUCCEEDED(hr))
    {
        hr = dlls::MediaFoundation::Get().MFCreateSourceReaderFromByteStream(byte_stream.Get(), nullptr, &reader);
    }

    if (SUCCEEDED(hr))
    {
        hr = ReadSource(reader.Get(), metadata_, raw_, data_.size);

        if (SUCCEEDED(hr))
        {
            data_.buffer = raw_.get();
        }
    }

    if (FAILED(hr))
    {
        Fail(strings::Format("%s failed (%#x): %s", __FUNCTION__, hr, "Load audio failed"));
        return false;
    }
    return true;
}

HRESULT ReadSource(IMFSourceReader* reader, AudioMetadata& output_metadata, std::unique_ptr<uint8_t[]>& output_data,
                   uint32_t& output_size)
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

    if (SUCCEEDED(hr))
    {
        output_metadata.format          = AudioFormat::PCM;
        output_metadata.channels        = uint16_t(wave_format->nChannels);
        output_metadata.samples_per_sec = uint32_t(wave_format->nSamplesPerSec);
        output_metadata.bits_per_sample = uint16_t(wave_format->wBitsPerSample);
        output_metadata.block_align     = uint16_t(wave_format->nBlockAlign);
        output_metadata.extra_data      = wave_format;
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

        output_data = std::make_unique<uint8_t[]>(max_stream_size);

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
                output_size = uint32_t(position);
            }
            else
            {
                output_data.reset();
                output_size = 0;
            }
        }
    }
    return hr;
}
}  // namespace audio
}  // namespace kiwano
