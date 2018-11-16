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

#include "audio.h"
#include "base.hpp"
#include "modules.h"
#include "logs.h"
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <assert.h>
namespace easy2d
{
	//-------------------------------------------------------
	// Voice
	//-------------------------------------------------------

	Voice::Voice()
		: source_voice_(nullptr)
	{
	}

	Voice::Voice(IXAudio2SourceVoice * source_voice)
		: source_voice_(source_voice)
	{
	}

	Voice::~Voice()
	{
		Destroy();

		devices::Audio::Instance()->DeleteVoice(this);
	}

	HRESULT Voice::Play(const BYTE * wave_data, UINT32 data_size, UINT32 loop_count)
	{
		if (!source_voice_)
			return E_UNEXPECTED;

		XAUDIO2_BUFFER buffer = { 0 };
		buffer.pAudioData = wave_data;
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.AudioBytes = data_size;
		buffer.LoopCount = loop_count;

		HRESULT hr = source_voice_->SubmitSourceBuffer(&buffer);
		if (SUCCEEDED(hr))
		{
			hr = source_voice_->Start();
		}
		return hr;
	}

	HRESULT Voice::Pause()
	{
		if (!source_voice_)
			return E_UNEXPECTED;

		return source_voice_->Stop();
	}

	HRESULT Voice::Resume()
	{
		if (!source_voice_)
			return E_UNEXPECTED;

		return source_voice_->Start();
	}

	HRESULT Voice::Stop()
	{
		if (!source_voice_)
			return E_UNEXPECTED;

		HRESULT hr = source_voice_->Stop();
		if (SUCCEEDED(hr))
		{
			hr = source_voice_->ExitLoop();
		}

		if (SUCCEEDED(hr))
		{
			hr = source_voice_->FlushSourceBuffers();
		}
		return hr;
	}

	HRESULT Voice::GetVolume(float * volume) const
	{
		if (!source_voice_)
			return E_UNEXPECTED;

		if (volume == nullptr)
			return E_POINTER;

		source_voice_->GetVolume(volume);
		return S_OK;
	}

	HRESULT Voice::SetVolume(float volume)
	{
		if (!source_voice_)
			return E_UNEXPECTED;

		volume = std::min(std::max(volume, -224.f), 224.f);
		return source_voice_->SetVolume(volume);
	}

	HRESULT Voice::GetBuffersQueued(UINT32 * queued) const
	{
		if (!source_voice_)
			return E_UNEXPECTED;

		if (queued == nullptr)
			return E_POINTER;

		XAUDIO2_VOICE_STATE state;
		source_voice_->GetState(&state);
		*queued = state.BuffersQueued;
		return S_OK;
	}

	void Voice::Destroy()
	{
		if (source_voice_)
		{
			source_voice_->Stop();
			source_voice_->FlushSourceBuffers();
			source_voice_->DestroyVoice();
			source_voice_ = nullptr;
		}
	}

	void Voice::SetSourceVoice(IXAudio2SourceVoice * source_voice)
	{
		Destroy();
		source_voice_ = source_voice;
	}


	namespace devices
	{
		//-------------------------------------------------------
		// AudioDevice
		//-------------------------------------------------------

		AudioDevice::AudioDevice()
			: x_audio2_(nullptr)
			, mastering_voice_(nullptr)
			, initialized(false)
		{
			modules::Init();
		}

		AudioDevice::~AudioDevice()
		{
			E2D_LOG("Destroying audio device");

			ClearVoiceCache();

			if (mastering_voice_)
			{
				mastering_voice_->DestroyVoice();
				mastering_voice_ = nullptr;
			}

			SafeRelease(x_audio2_);

			modules::MediaFoundation.MFShutdown();

			modules::Destroy();
		}

		void AudioDevice::Init(bool debug)
		{
			if (initialized)
				return;

			E2D_LOG("Initing audio device");

			ThrowIfFailed(
				modules::MediaFoundation.MFStartup(MF_VERSION, MFSTARTUP_FULL)
			);

			ThrowIfFailed(
				modules::XAudio2.XAudio2Create(&x_audio2_, 0, XAUDIO2_DEFAULT_PROCESSOR)
			);

			ThrowIfFailed(
				x_audio2_->CreateMasteringVoice(&mastering_voice_)
			);

			initialized = true;
		}

		HRESULT AudioDevice::CreateVoice(Voice* voice, WAVEFORMATEX * wfx)
		{
			HRESULT hr;
			IXAudio2SourceVoice* source_voice;

			if (!voice)
				return E_POINTER;
			
			hr = x_audio2_->CreateSourceVoice(&source_voice, wfx, 0, XAUDIO2_DEFAULT_FREQ_RATIO);
			if (SUCCEEDED(hr))
			{
				voice->SetSourceVoice(source_voice);
				voice_cache_.push_back(voice);
			}
			return hr;
		}

		void AudioDevice::DeleteVoice(Voice * voice)
		{
			for (auto iter = voice_cache_.begin(); iter != voice_cache_.end(); ++iter)
			{
				if (*iter == voice)
				{
					voice_cache_.erase(iter);
					break;
				}
			}
		}

		void AudioDevice::ClearVoiceCache()
		{
			for (auto voice : voice_cache_)
			{
				voice->Destroy();
			}
			voice_cache_.clear();
		}

		void AudioDevice::Open()
		{
			x_audio2_->StartEngine();
		}

		void AudioDevice::Close()
		{
			x_audio2_->StopEngine();
		}
	}
}