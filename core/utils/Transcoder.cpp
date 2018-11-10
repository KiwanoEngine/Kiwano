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

#include "Transcoder.h"
#include "../base/base.h"
#include "../base/modules.h"
#include "../base/logs.h"
#include <shlwapi.h>

namespace easy2d
{
	Transcoder::Transcoder()
		: wave_format_(nullptr)
	{
	}
	
	Transcoder::~Transcoder()
	{
		if (wave_format_)
		{
			::CoTaskMemFree(wave_format_);
			wave_format_ = nullptr;
		}
	}

	WAVEFORMATEX* Transcoder::GetWaveFormatEx() const
	{
		return wave_format_;
	}

	bool Transcoder::LoadMediaFile(LPCWSTR file_path, BYTE** wave_data, UINT32* wave_data_size)
	{
		HRESULT hr = S_OK;

		IMFSourceReader* reader = nullptr;

		hr = modules::MediaFoundation.MFCreateSourceReaderFromURL(
			file_path,
			nullptr,
			&reader
		);

		if (SUCCEEDED(hr))
		{
			hr = ReadSource(reader, wave_data, wave_data_size);
		}

		SafeRelease(reader);

		return SUCCEEDED(hr);
	}

	bool Transcoder::LoadMediaResource(Resource& res, BYTE** wave_data, UINT32* wave_data_size)
	{
		HRESULT	hr = S_OK;

		HINSTANCE		 hinstance = GetModuleHandle(nullptr);
		IStream*		 stream = nullptr;
		IMFByteStream*	 byte_stream = nullptr;
		IMFSourceReader* reader = nullptr;

		if (!res.Load()) { return false; }

		stream = SHCreateMemStream(
			static_cast<const BYTE*>(res.GetData()),
			static_cast<UINT>(res.GetDataSize())
		);

		if (stream == nullptr)
		{
			logs::Trace(L"SHCreateMemStream");
			return false;
		}

		if (SUCCEEDED(hr))
		{
			hr = modules::MediaFoundation.MFCreateMFByteStreamOnStream(stream, &byte_stream);
		}

		if (SUCCEEDED(hr))
		{
			hr = modules::MediaFoundation.MFCreateSourceReaderFromByteStream(
				byte_stream,
				nullptr,
				&reader
			);
		}

		if (SUCCEEDED(hr))
		{
			hr = ReadSource(reader, wave_data, wave_data_size);
		}

		SafeRelease(stream);
		SafeRelease(byte_stream);
		SafeRelease(reader);

		return SUCCEEDED(hr);
	}

	HRESULT Transcoder::ReadSource(IMFSourceReader* reader, BYTE** wave_data, UINT32* wave_data_size)
	{
		HRESULT hr = S_OK;
		DWORD max_stream_size = 0;

		IMFMediaType* partial_type = nullptr;
		IMFMediaType* uncompressed_type = nullptr;

		hr = modules::MediaFoundation.MFCreateMediaType(&partial_type);

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
				MF_SOURCE_READER_FIRST_AUDIO_STREAM,
				0,
				partial_type
			);
		}

		// 从 IMFMediaType 中获取 WAVEFORMAT 结构
		if (SUCCEEDED(hr))
		{
			hr = reader->GetCurrentMediaType(
				MF_SOURCE_READER_FIRST_AUDIO_STREAM,
				&uncompressed_type
			);
		}

		// 指定音频流
		if (SUCCEEDED(hr))
		{
			hr = reader->SetStreamSelection(
				MF_SOURCE_READER_FIRST_AUDIO_STREAM,
				true
			);
		}

		// 获取 WAVEFORMAT 数据
		if (SUCCEEDED(hr))
		{
			UINT32 size = 0;
			hr = modules::MediaFoundation.MFCreateWaveFormatExFromMFMediaType(
				uncompressed_type,
				&wave_format_,
				&size,
				MFWaveFormatExConvertFlag_Normal
			);
		}

		// 估算音频流大小
		if (SUCCEEDED(hr))
		{
			PROPVARIANT prop;
			PropVariantInit(&prop);

			hr = reader->GetPresentationAttribute(
				MF_SOURCE_READER_MEDIASOURCE,
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

			IMFSample*		sample = nullptr;
			IMFMediaBuffer*	buffer = nullptr;
			BYTE*			data = new (std::nothrow) BYTE[max_stream_size];

			if (data == nullptr)
			{
				logs::Trace(L"Low memory");
				hr = E_OUTOFMEMORY;
			}
			else
			{
				while (true)
				{
					hr = reader->ReadSample(
						MF_SOURCE_READER_FIRST_AUDIO_STREAM,
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
							BYTE *audio_data = nullptr;
							DWORD sample_buffer_length = 0;

							hr = buffer->Lock(
								&audio_data,
								nullptr,
								&sample_buffer_length
							);

							if (SUCCEEDED(hr))
							{
								for (DWORD i = 0; i < sample_buffer_length; i++)
								{
									data[position++] = audio_data[i];
								}
								hr = buffer->Unlock();
							}
						}
						SafeRelease(buffer);
					}
					SafeRelease(sample);

					if (FAILED(hr)) { break; }
				}

				if (SUCCEEDED(hr))
				{
					*wave_data = data;
					*wave_data_size = position;
				}
			}
		}

		SafeRelease(partial_type);
		SafeRelease(uncompressed_type);

		return hr;
	}
}