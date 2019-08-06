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
#include "../core/d2dhelper.hpp"
#include "../core/modules.h"
#include "../core/logs.h"
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

	const WAVEFORMATEX* Transcoder::GetWaveFormatEx() const
	{
		return wave_format_;
	}

	HRESULT Transcoder::LoadMediaFile(LPCWSTR file_path, BYTE** wave_data, UINT32* wave_data_size)
	{
		HRESULT hr = S_OK;

		IntrusivePtr<IMFSourceReader> reader;

		hr = modules::MediaFoundation::Get().MFCreateSourceReaderFromURL(
			file_path,
			nullptr,
			&reader
		);

		if (SUCCEEDED(hr))
		{
			hr = ReadSource(reader.Get(), wave_data, wave_data_size);
		}

		return hr;
	}

	HRESULT Transcoder::LoadMediaResource(Resource const& res, BYTE** wave_data, UINT32* wave_data_size)
	{
		HRESULT	hr = S_OK;

		IntrusivePtr<IStream> stream;
		IntrusivePtr<IMFByteStream> byte_stream;
		IntrusivePtr<IMFSourceReader> reader;

		LPVOID buffer;
		DWORD buffer_size;
		if (!res.Load(buffer, buffer_size)) { return false; }

		stream = modules::Shlwapi::Get().SHCreateMemStream(
			static_cast<const BYTE*>(buffer),
			static_cast<UINT>(buffer_size)
		);

		if (stream == nullptr)
		{
			logs::Errorln(L"SHCreateMemStream");
			return E_OUTOFMEMORY;
		}

		if (SUCCEEDED(hr))
		{
			hr = modules::MediaFoundation::Get().MFCreateMFByteStreamOnStream(stream.Get(), &byte_stream);
		}

		if (SUCCEEDED(hr))
		{
			hr = modules::MediaFoundation::Get().MFCreateSourceReaderFromByteStream(
				byte_stream.Get(),
				nullptr,
				&reader
			);
		}

		if (SUCCEEDED(hr))
		{
			hr = ReadSource(reader.Get(), wave_data, wave_data_size);
		}

		return hr;
	}

	HRESULT Transcoder::ReadSource(IMFSourceReader* reader, BYTE** wave_data, UINT32* wave_data_size)
	{
		HRESULT hr = S_OK;
		DWORD max_stream_size = 0;

		IntrusivePtr<IMFMediaType> partial_type;
		IntrusivePtr<IMFMediaType> uncompressed_type;

		hr = modules::MediaFoundation::Get().MFCreateMediaType(&partial_type);

		if (SUCCEEDED(hr))
		{
			hr = partial_type->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
		}

		if (SUCCEEDED(hr))
		{
			hr = partial_type->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
		}

		// ���� source reader ��ý�����ͣ�����ʹ�ú��ʵĽ�����ȥ���������Ƶ
		if (SUCCEEDED(hr))
		{
			hr = reader->SetCurrentMediaType(
				(DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,
				0,
				partial_type.Get()
			);
		}

		// �� IMFMediaType �л�ȡ WAVEFORMAT �ṹ
		if (SUCCEEDED(hr))
		{
			hr = reader->GetCurrentMediaType(
				(DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,
				&uncompressed_type
			);
		}

		// ָ����Ƶ��
		if (SUCCEEDED(hr))
		{
			hr = reader->SetStreamSelection(
				(DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,
				true
			);
		}

		// ��ȡ WAVEFORMAT ����
		if (SUCCEEDED(hr))
		{
			UINT32 size = 0;
			hr = modules::MediaFoundation::Get().MFCreateWaveFormatExFromMFMediaType(
				uncompressed_type.Get(),
				&wave_format_,
				&size,
				(DWORD)MFWaveFormatExConvertFlag_Normal
			);
		}

		// ������Ƶ����С
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

		// ��ȡ��Ƶ����
		if (SUCCEEDED(hr))
		{
			DWORD flags = 0;
			DWORD position = 0;
			BYTE* data = new (std::nothrow) BYTE[max_stream_size];

			IntrusivePtr<IMFSample> sample;
			IntrusivePtr<IMFMediaBuffer> buffer;

			if (data == nullptr)
			{
				logs::Errorln(L"Low memory");
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
						buffer = nullptr;
					}
					sample = nullptr;

					if (FAILED(hr)) { break; }
				}

				if (SUCCEEDED(hr))
				{
					*wave_data = data;
					*wave_data_size = position;
				}
			}
		}

		return hr;
	}
}