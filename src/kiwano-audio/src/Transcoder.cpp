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
#	define INITGUID		// MFAudioFormat_PCM, MF_MT_MAJOR_TYPE, MF_MT_SUBTYPE, MFMediaType_Audio
#endif

#include <kiwano/macros.h>
#include <kiwano/core/string.hpp>
#include <kiwano/base/Resource.h>
#include <kiwano/base/Logger.h>
#include <kiwano/base/win32/ComPtr.hpp>
#include <kiwano/platform/modules.h>
#include "audio-modules.h"
#include "Transcoder.h"

namespace kiwano
{
	namespace audio
	{

		Transcoder::Transcoder()
			: wave_format_(nullptr)
			, wave_data_(nullptr)
			, wave_size_(0)
		{
		}

		Transcoder::~Transcoder()
		{
			ClearBuffer();
		}

		Transcoder::Buffer Transcoder::GetBuffer() const
		{
			return Buffer{ wave_data_, wave_size_, wave_format_ };
		}

		void Transcoder::ClearBuffer()
		{
			if (wave_format_)
			{
				::CoTaskMemFree(wave_format_);
				wave_format_ = nullptr;
			}

			if (wave_data_)
			{
				delete[] wave_data_;
				wave_data_ = nullptr;
			}

			wave_size_ = 0;
		}

		HRESULT Transcoder::LoadMediaFile(String const& file_path)
		{
			HRESULT hr = S_OK;

			ComPtr<IMFSourceReader> reader;

			hr = modules::MediaFoundation::Get().MFCreateSourceReaderFromURL(
				file_path.c_str(),
				nullptr,
				&reader
			);

			if (SUCCEEDED(hr))
			{
				hr = ReadSource(reader.get());
			}

			return hr;
		}

		HRESULT Transcoder::LoadMediaResource(Resource const& res)
		{
			HRESULT	hr = S_OK;

			ComPtr<IStream> stream;
			ComPtr<IMFByteStream> byte_stream;
			ComPtr<IMFSourceReader> reader;

			Resource::Data data = res.GetData();
			if (!data) { return E_FAIL; }

			stream = kiwano::modules::Shlwapi::Get().SHCreateMemStream(
				static_cast<const Byte*>(data.buffer),
				static_cast<UInt32>(data.size)
			);

			if (stream == nullptr)
			{
				KGE_ERROR_LOG(L"SHCreateMemStream failed");
				return E_OUTOFMEMORY;
			}

			if (SUCCEEDED(hr))
			{
				hr = modules::MediaFoundation::Get().MFCreateMFByteStreamOnStream(stream.get(), &byte_stream);
			}

			if (SUCCEEDED(hr))
			{
				hr = modules::MediaFoundation::Get().MFCreateSourceReaderFromByteStream(
					byte_stream.get(),
					nullptr,
					&reader
				);
			}

			if (SUCCEEDED(hr))
			{
				hr = ReadSource(reader.get());
			}

			return hr;
		}

		HRESULT Transcoder::ReadSource(IMFSourceReader* reader)
		{
			HRESULT hr = S_OK;
			DWORD max_stream_size = 0;

			ComPtr<IMFMediaType> partial_type;
			ComPtr<IMFMediaType> uncompressed_type;

			hr = modules::MediaFoundation::Get().MFCreateMediaType(&partial_type);

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
				hr = reader->SetCurrentMediaType(
					(DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,
					0,
					partial_type.get()
				);
			}

			// 从 IMFMediaType 中获取 WAVEFORMAT 结构
			if (SUCCEEDED(hr))
			{
				hr = reader->GetCurrentMediaType(
					(DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,
					&uncompressed_type
				);
			}

			// 指定音频流
			if (SUCCEEDED(hr))
			{
				hr = reader->SetStreamSelection(
					(DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,
					true
				);
			}

			// 获取 WAVEFORMAT 数据
			if (SUCCEEDED(hr))
			{
				UInt32 size = 0;
				hr = modules::MediaFoundation::Get().MFCreateWaveFormatExFromMFMediaType(
					uncompressed_type.get(),
					&wave_format_,
					&size,
					(DWORD)MFWaveFormatExConvertFlag_Normal
				);
			}

			// 估算音频流大小
			if (SUCCEEDED(hr))
			{
				PROPVARIANT prop;
				PropVariantInit(&prop);

				hr = reader->GetPresentationAttribute(
					(DWORD)MF_SOURCE_READER_MEDIASOURCE,
					MF_PD_DURATION,
					&prop
				);

				LONGLONG duration = prop.uhVal.QuadPart;
				max_stream_size = static_cast<DWORD>(
					(duration * wave_format_->nAvgBytesPerSec) / 10000000 + 1
				);
				PropVariantClear(&prop);
			}

			// 读取音频数据
			if (SUCCEEDED(hr))
			{
				DWORD flags = 0;
				DWORD position = 0;
				Byte* data = new (std::nothrow) Byte[max_stream_size];

				ComPtr<IMFSample> sample;
				ComPtr<IMFMediaBuffer> buffer;

				if (data == nullptr)
				{
					KGE_ERROR_LOG(L"Low memory");
					hr = E_OUTOFMEMORY;
				}
				else
				{
					while (true)
					{
						hr = reader->ReadSample(
							(DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,
							0,
							nullptr,
							&flags,
							nullptr,
							&sample
						);

						if (flags & MF_SOURCE_READERF_ENDOFSTREAM) { break; }

						if (sample == nullptr) { continue; }

						if (SUCCEEDED(hr))
						{
							hr = sample->ConvertToContiguousBuffer(&buffer);

							if (SUCCEEDED(hr))
							{
								Byte* audio_data = nullptr;
								DWORD sample_buffer_length = 0;

								hr = buffer->Lock(
									&audio_data,
									nullptr,
									&sample_buffer_length
								);

								if (SUCCEEDED(hr) && sample_buffer_length <= max_stream_size)
								{
									for (DWORD i = 0; i < sample_buffer_length; i++)
									{
										data[position++] = audio_data[i];
									}
									hr = buffer->Unlock();
								}
							}
							buffer = nullptr;
						}
						sample = nullptr;

						if (FAILED(hr)) { break; }
					}

					if (SUCCEEDED(hr))
					{
						wave_data_ = data;
						wave_size_ = position;
					}
				}
			}

			return hr;
		}
	}
}
