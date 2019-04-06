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

#include "../easy2d-audio.h"
#include "Music.h"
#include "Transcoder.h"

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

	Music::Music(Resource const& res)
		: Music()
	{
		Load(res);
	}

	Music::~Music()
	{
		Close();
	}

	bool Music::Load(Resource const& res)
	{
		if (opened_)
		{
			Close();
		}

		HRESULT hr = S_OK;
		Transcoder transcoder;

		if (res.IsFileType())
		{
			if (!modules::Shlwapi::Get().PathFileExistsW(res.GetFileName().c_str()))
			{
				E2D_WARNING_LOG(L"Media file '%s' not found", res.GetFileName().c_str());
				return false;
			}
			hr = transcoder.LoadMediaFile(res.GetFileName(), &wave_data_, &size_);
		}
		else
		{
			hr = transcoder.LoadMediaResource(res, &wave_data_, &size_);
		}

		if (FAILED(hr))
		{
			E2D_ERROR_LOG(L"Load media file failed with HRESULT of %08X", hr);
			return false;
		}

		hr = Audio::Instance().CreateVoice(voice_, transcoder.GetWaveFormatEx());
		if (FAILED(hr))
		{
			if (wave_data_)
			{
				delete[] wave_data_;
				wave_data_ = nullptr;
			}
			E2D_ERROR_LOG(L"Create source voice failed with HRESULT of %08X", hr);
			return false;
		}

		opened_ = true;
		return true;
	}

	bool Music::Play(int loop_count)
	{
		if (!opened_)
		{
			E2D_ERROR_LOG(L"Music must be opened first!");
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
			E2D_ERROR_LOG(L"Submitting source buffer failed with HRESULT of %08X", hr);
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