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
#include "base.h"
#include "modules.h"
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

namespace easy2d
{
	namespace audio
	{
		AudioDevice instance;

		AudioDevice::AudioDevice()
			: x_audio2_(nullptr)
			, mastering_voice_(nullptr)
		{
		}

		AudioDevice::~AudioDevice()
		{
		}

		void AudioDevice::Initialize()
		{
			ThrowIfFailed(
				modules::MediaFoundation.MFStartup(MF_VERSION, MFSTARTUP_FULL)
			);

			ThrowIfFailed(
				modules::XAudio2.XAudio2Create(&x_audio2_, 0, XAUDIO2_DEFAULT_PROCESSOR)
			);

			ThrowIfFailed(
				x_audio2_->CreateMasteringVoice(&mastering_voice_)
			);
		}

		void AudioDevice::Uninitialize()
		{
			if (mastering_voice_)
			{
				mastering_voice_->DestroyVoice();
				mastering_voice_ = nullptr;
			}

			SafeRelease(x_audio2_);

			modules::MediaFoundation.MFShutdown();
		}

		HRESULT AudioDevice::CreateVoice(IXAudio2SourceVoice ** voice, WAVEFORMATEX * wfx)
		{
			return x_audio2_->CreateSourceVoice(voice, wfx, 0, XAUDIO2_DEFAULT_FREQ_RATIO);
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