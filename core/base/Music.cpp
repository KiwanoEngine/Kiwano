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
#include "../utils/Transcoder.h"
#include "../utils/File.h"
#include "modules.h"
#include "audio.h"
#include "logs.h"

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

	Music::Music(String const& file_path)
		: opened_(false)
		, playing_(false)
		, wave_data_(nullptr)
		, size_(0)
		, voice_(nullptr)
	{
		Load(file_path);
	}

	Music::Music(Resource const& res)
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

	bool Music::Load(String const& file_path)
	{
		if (opened_)
		{
			Close();
		}

		File music_file;
		if (!music_file.Open(file_path))
		{
			logs::Warningln("Media file '%s' not found", file_path.c_str());
			return false;
		}

		// 用户输入的路径不一定是完整路径，因为用户可能通过 File::AddSearchPath 添加
		// 默认搜索路径，所以需要通过 File::GetPath 获取完整路径
		String music_file_path = music_file.GetPath();

		Transcoder transcoder;
		HRESULT hr = transcoder.LoadMediaFile(music_file_path.c_str(), &wave_data_, &size_);
		if (FAILED(hr))
		{
			logs::Errorln(hr, "Load media from file failed");
			return false;
		}

		hr = Audio::Instance()->CreateVoice(voice_, transcoder.GetWaveFormatEx());
		if (FAILED(hr))
		{
			if (wave_data_)
			{
				delete[] wave_data_;
				wave_data_ = nullptr;
			}
			logs::Errorln(hr, "Create source voice failed");
			return false;
		}

		opened_ = true;
		return true;
	}

	bool Music::Load(Resource const& res)
	{
		if (opened_)
		{
			Close();
		}

		Transcoder transcoder;
		HRESULT hr = transcoder.LoadMediaResource(res, &wave_data_, &size_);

		if (FAILED(hr))
		{
			logs::Errorln(hr, "Load media from resource failed");
			return false;
		}

		hr = Audio::Instance()->CreateVoice(voice_, transcoder.GetWaveFormatEx());
		if (FAILED(hr))
		{
			if (wave_data_)
			{
				delete[] wave_data_;
				wave_data_ = nullptr;
			}
			logs::Errorln(hr, "Create source voice error");
			return false;
		}

		opened_ = true;
		return true;
	}

	bool Music::Play(int loop_count)
	{
		if (!opened_)
		{
			logs::Errorln("Music must be opened first!");
			return false;
		}

		UINT32 buffers_queued = 0;
		voice_.GetBuffersQueued(&buffers_queued);
		if (buffers_queued)
			Stop();

		if (loop_count < 0)
			loop_count = XAUDIO2_LOOP_INFINITE;
		else
			loop_count = std::min(loop_count, XAUDIO2_LOOP_INFINITE - 1);

		HRESULT hr = voice_.Play(wave_data_, size_, static_cast<UINT32>(loop_count));
		if (FAILED(hr))
		{
			logs::Errorln(hr, "Submitting source buffer error");
		}

		playing_ = SUCCEEDED(hr);

		return playing_;
	}

	void Music::Pause()
	{
		if (SUCCEEDED(voice_.Pause()))
			playing_ = false;
	}

	void Music::Resume()
	{
		if (SUCCEEDED(voice_.Resume()))
			playing_ = true;
	}

	void Music::Stop()
	{
		if (SUCCEEDED(voice_.Stop()))
			playing_ = false;
	}

	void Music::Close()
	{
		voice_.Destroy();

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
		if (opened_)
		{
			UINT32 buffers_queued = 0;
			voice_.GetBuffersQueued(&buffers_queued);
			if (buffers_queued && playing_)
				return true;
		}
		return false;
	}

	float Music::GetVolume() const
	{
		float volume = 0.f;
		voice_.GetVolume(&volume);
		return volume;
	}

	bool Music::SetVolume(float volume)
	{
		return SUCCEEDED(voice_.SetVolume(volume));
	}
}