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

#include "..\e2dtool.h"
#include "..\e2dmodule.h"


inline bool TraceError(wchar_t* prompt)
{
	WARN("Music error: %s failed!", prompt);
	return false;
}

inline bool TraceError(wchar_t* prompt, HRESULT hr)
{
	WARN("Music error: %s (%#X)", prompt, hr);
	return false;
}


namespace easy2d
{

	class Transcoder
	{
		WAVEFORMATEX* wave_format_;

	public:
		Transcoder()
			: wave_format_(nullptr)
		{
		}

		~Transcoder()
		{
			if (wave_format_)
			{
				::CoTaskMemFree(wave_format_);
				wave_format_ = nullptr;
			}
		}

		WAVEFORMATEX* GetWaveFormatEx()
		{
			return wave_format_;
		}

		bool LoadMediaFile(LPCWSTR file_path, BYTE** wave_data, UINT32* wave_data_size)
		{
			HRESULT hr = S_OK;

			IMFSourceReader* reader = nullptr;

			hr = MFCreateSourceReaderFromURL(
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

		bool LoadMediaResource(LPCWSTR res_name, LPCWSTR res_type, BYTE** wave_data, UINT32* wave_data_size)
		{
			HRESULT	hr = S_OK;
			HRSRC	res_info;
			HGLOBAL	res_data;
			DWORD	res_size;
			void*	res;

			IStream*			stream = nullptr;
			IMFByteStream*		byte_stream = nullptr;
			IMFSourceReader*	reader = nullptr;

			res_info = FindResourceW(HINST_THISCOMPONENT, res_name, res_type);
			if (res_info == nullptr)
			{
				return TraceError(L"FindResource");
			}

			res_data = LoadResource(HINST_THISCOMPONENT, res_info);
			if (res_data == nullptr)
			{
				return TraceError(L"LoadResource");
			}

			res_size = SizeofResource(HINST_THISCOMPONENT, res_info);
			if (res_size == 0)
			{
				return TraceError(L"SizeofResource");
			}

			res = LockResource(res_data);
			if (res == nullptr)
			{
				return TraceError(L"LockResource");
			}

			stream = ::SHCreateMemStream(
				static_cast<const BYTE*>(res),
				static_cast<UINT>(res_size)
			);

			if (stream == nullptr)
			{
				return TraceError(L"SHCreateMemStream");
			}

			if (SUCCEEDED(hr))
			{
				hr = MFCreateMFByteStreamOnStream(stream, &byte_stream);
			}

			if (SUCCEEDED(hr))
			{
				hr = MFCreateSourceReaderFromByteStream(
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

		HRESULT ReadSource(IMFSourceReader* reader, BYTE** wave_data, UINT32* wave_data_size)
		{
			HRESULT hr = S_OK;
			DWORD max_stream_size = 0;

			IMFMediaType* partial_type = nullptr;
			IMFMediaType* uncompressed_type = nullptr;

			hr = MFCreateMediaType(&partial_type);

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
				hr = MFCreateWaveFormatExFromMFMediaType(
					uncompressed_type,
					&wave_format_,
					&size
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
					TraceError(L"Low memory");
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
	};

}


easy2d::Music::Music()
	: opened_(false)
	, playing_(false)
	, wave_data_(nullptr)
	, size_(0)
	, voice_(nullptr)
{
}

easy2d::Music::~Music()
{
	Close();
}

bool easy2d::Music::Load(const easy2d::String & file_path)
{
	if (opened_)
	{
		Close();
	}

	File music_file;
	if (!music_file.Open(file_path))
	{
		WARN("Music::Load error: File not found.");
		return false;
	}

	// 用户输入的路径不一定是完整路径，因为用户可能通过 File::AddSearchPath 添加
	// 默认搜索路径，所以需要通过 File::GetPath 获取完整路径
	String music_file_path = music_file.GetPath();

	Transcoder transcoder;
	if (!transcoder.LoadMediaFile((LPCWSTR)music_file_path, &wave_data_, &size_))
	{
		return false;
	}

	HRESULT hr = Device::GetAudio()->CreateVoice(&voice_, transcoder.GetWaveFormatEx());
	if (FAILED(hr))
	{
		if (wave_data_)
		{
			delete[] wave_data_;
			wave_data_ = nullptr;
		}
		return TraceError(L"Create source voice error", hr);
	}

	opened_ = true;
	return true;
}

bool easy2d::Music::Load(const Resource& res)
{
	if (opened_)
	{
		Close();
	}

	Transcoder transcoder;
	if (!transcoder.LoadMediaResource(MAKEINTRESOURCE(res.id), (LPCWSTR)res.type, &wave_data_, &size_))
	{
		return false;
	}

	HRESULT hr = Device::GetAudio()->CreateVoice(&voice_, transcoder.GetWaveFormatEx());
	if (FAILED(hr))
	{
		if (wave_data_)
		{
			delete[] wave_data_;
			wave_data_ = nullptr;
		}
		return TraceError(L"Create source voice error", hr);
	}

	opened_ = true;
	return true;
}

bool easy2d::Music::Play(int loop_count)
{
	if (!opened_)
	{
		WARN("Music::Play Failed: Music must be opened first!");
		return false;
	}

	if (voice_ == nullptr)
	{
		WARN("Music::Play Failed: IXAudio2SourceVoice Null pointer exception!");
		return false;
	}

	XAUDIO2_VOICE_STATE state;
	voice_->GetState(&state);
	if (state.BuffersQueued)
	{
		Stop();
	}

	if (loop_count < 0)
	{
		loop_count = XAUDIO2_LOOP_INFINITE;
	}
	else
	{
		loop_count = std::min(loop_count, XAUDIO2_LOOP_INFINITE - 1);
	}

	// 提交 wave 样本数据
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.pAudioData = wave_data_;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = size_;
	buffer.LoopCount = loop_count;

	HRESULT hr;
	if (FAILED(hr = voice_->SubmitSourceBuffer(&buffer)))
	{
		return TraceError(L"Submitting source buffer error", hr);
	}

	hr = voice_->Start(0);

	playing_ = SUCCEEDED(hr);

	return playing_;
}

void easy2d::Music::Pause()
{
	if (voice_)
	{
		if (SUCCEEDED(voice_->Stop()))
		{
			playing_ = false;
		}
	}
}

void easy2d::Music::Resume()
{
	if (voice_)
	{
		if (SUCCEEDED(voice_->Start()))
		{
			playing_ = true;
		}
	}
}

void easy2d::Music::Stop()
{
	if (voice_)
	{
		if (SUCCEEDED(voice_->Stop()))
		{
			voice_->ExitLoop();
			voice_->FlushSourceBuffers();
			playing_ = false;
		}
	}
}

void easy2d::Music::Close()
{
	if (voice_)
	{
		voice_->Stop();
		voice_->FlushSourceBuffers();
		voice_->DestroyVoice();
		voice_ = nullptr;
	}

	if (wave_data_)
	{
		delete[] wave_data_;
		wave_data_ = nullptr;
	}

	opened_ = false;
	playing_ = false;
}

bool easy2d::Music::IsPlaying() const
{
	if (opened_ && voice_)
	{
		XAUDIO2_VOICE_STATE state;
		voice_->GetState(&state);
		if (state.BuffersQueued && playing_)
			return true;
	}
	return false;
}

float easy2d::Music::GetVolume() const
{
	if (voice_)
	{
		float volume = 0.f;
		voice_->GetVolume(&volume);
		return volume;
	}
	return 0.f;
}

bool easy2d::Music::SetVolume(float volume)
{
	if (voice_)
	{
		volume = std::min(std::max(volume, -224.f), 224.f);
		return SUCCEEDED(voice_->SetVolume(volume));
	}
	return false;
}

IXAudio2SourceVoice * easy2d::Music::GetSourceVoice() const
{
	return voice_;
}
