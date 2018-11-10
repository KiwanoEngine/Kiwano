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

#include "Music.h"
#include "Transcoder.h"
#include "File.h"
#include "../base/modules.h"
#include "../base/audio.h"
#include "../base/logs.h"

namespace easy2d
{
	Music::Music()
		: opened_(false)
		, playing_(false)
		, wave_data_(nullptr)
		, size_(0)
		, voice_(nullptr)
	{
	}

	Music::Music(const String& file_path)
		: opened_(false)
		, playing_(false)
		, wave_data_(nullptr)
		, size_(0)
		, voice_(nullptr)
	{
		Load(file_path);
	}

	Music::Music(Resource& res)
		: opened_(false)
		, playing_(false)
		, wave_data_(nullptr)
		, size_(0)
		, voice_(nullptr)
	{
		Load(res);
	}

	Music::~Music()
	{
		Close();
	}

	bool Music::Load(const String & file_path)
	{
		if (opened_)
		{
			Close();
		}

		File music_file;
		if (!music_file.Open(file_path))
		{
			E2D_WARNING("Media file not found.");
			return false;
		}

		// 用户输入的路径不一定是完整路径，因为用户可能通过 File::AddSearchPath 添加
		// 默认搜索路径，所以需要通过 File::GetPath 获取完整路径
		String music_file_path = music_file.GetPath();

		Transcoder transcoder;
		if (!transcoder.LoadMediaFile(music_file_path.c_str(), &wave_data_, &size_))
		{
			return false;
		}

		HRESULT hr = audio::instance.CreateVoice(&voice_, transcoder.GetWaveFormatEx());
		if (FAILED(hr))
		{
			if (wave_data_)
			{
				delete[] wave_data_;
				wave_data_ = nullptr;
			}
			logs::Trace(L"Create source voice error", hr);
			return false;
		}

		opened_ = true;
		return true;
	}

	bool Music::Load(Resource& res)
	{
		if (opened_)
		{
			Close();
		}

		Transcoder transcoder;
		if (!transcoder.LoadMediaResource(res, &wave_data_, &size_))
		{
			return false;
		}

		HRESULT hr = audio::instance.CreateVoice(&voice_, transcoder.GetWaveFormatEx());
		if (FAILED(hr))
		{
			if (wave_data_)
			{
				delete[] wave_data_;
				wave_data_ = nullptr;
			}
			logs::Trace(L"Create source voice error", hr);
			return false;
		}

		opened_ = true;
		return true;
	}

	bool Music::Play(int loop_count)
	{
		if (!opened_)
		{
			E2D_WARNING("Music must be opened first!");
			return false;
		}

		if (voice_ == nullptr)
		{
			E2D_WARNING("IXAudio2SourceVoice Null pointer exception!");
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
			logs::Trace(L"Submitting source buffer error", hr);
			return false;
		}

		hr = voice_->Start(0);

		playing_ = SUCCEEDED(hr);

		return playing_;
	}

	void Music::Pause()
	{
		if (voice_)
		{
			if (SUCCEEDED(voice_->Stop()))
			{
				playing_ = false;
			}
		}
	}

	void Music::Resume()
	{
		if (voice_)
		{
			if (SUCCEEDED(voice_->Start()))
			{
				playing_ = true;
			}
		}
	}

	void Music::Stop()
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

	void Music::Close()
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

	bool Music::IsPlaying() const
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

	float Music::GetVolume() const
	{
		if (voice_)
		{
			float volume = 0.f;
			voice_->GetVolume(&volume);
			return volume;
		}
		return 0.f;
	}

	bool Music::SetVolume(float volume)
	{
		if (voice_)
		{
			volume = std::min(std::max(volume, -224.f), 224.f);
			return SUCCEEDED(voice_->SetVolume(volume));
		}
		return false;
	}

	IXAudio2SourceVoice * Music::GetSourceVoice() const
	{
		return voice_;
	}
}