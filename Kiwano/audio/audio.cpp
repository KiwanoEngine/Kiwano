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

#include "../kiwano-audio.h"
#include "audio.h"
#include "audio-modules.h"

namespace kiwano
{
	Audio::Audio()
		: x_audio2_(nullptr)
		, mastering_voice_(nullptr)
	{
	}

	Audio::~Audio()
	{
	}

	void Audio::SetupComponent(Application*)
	{
		KGE_LOG(L"Creating audio resources");

		HRESULT hr = modules::MediaFoundation::Get().MFStartup(MF_VERSION, MFSTARTUP_FULL);

		if (SUCCEEDED(hr))
		{
			hr = modules::XAudio2::Get().XAudio2Create(&x_audio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
		}

		if (SUCCEEDED(hr))
		{
			hr = x_audio2_->CreateMasteringVoice(&mastering_voice_);
		}

		ThrowIfFailed(hr);
	}

	void Audio::DestroyComponent()
	{
		KGE_LOG(L"Destroying audio resources");

		ClearVoiceCache();

		if (mastering_voice_)
		{
			mastering_voice_->DestroyVoice();
			mastering_voice_ = nullptr;
		}

		if (x_audio2_)
		{
			x_audio2_->Release();
			x_audio2_ = nullptr;
		}

		modules::MediaFoundation::Get().MFShutdown();
	}

	HRESULT Audio::CreateVoice(Voice& voice, const WAVEFORMATEX* wfx)
	{
		HRESULT hr;
		IXAudio2SourceVoice* source_voice;

		hr = x_audio2_->CreateSourceVoice(&source_voice, wfx, 0, XAUDIO2_DEFAULT_FREQ_RATIO);
		if (SUCCEEDED(hr))
		{
			voice.SetSourceVoice(source_voice);
			voice_cache_.insert(&voice);
		}
		return hr;
	}

	void Audio::DeleteVoice(Voice* voice)
	{
		voice_cache_.erase(voice);
	}

	void Audio::ClearVoiceCache()
	{
		for (auto voice : voice_cache_)
		{
			voice->Destroy();
		}
		voice_cache_.clear();
	}

	void Audio::Open()
	{
		x_audio2_->StartEngine();
	}

	void Audio::Close()
	{
		x_audio2_->StopEngine();
	}

}