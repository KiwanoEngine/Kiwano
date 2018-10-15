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

#include "..\e2dmodule.h"


e2d::Audio::Audio()
	: x_audio2_(nullptr)
	, mastering_voice_(nullptr)
{
	ThrowIfFailed(
		MFStartup(MF_VERSION)
	);

	ThrowIfFailed(
		XAudio2Create(&x_audio2_)
	);

	ThrowIfFailed(
		x_audio2_->CreateMasteringVoice(&mastering_voice_)
	);
}

e2d::Audio::~Audio()
{
	if (mastering_voice_)
	{
		mastering_voice_->DestroyVoice();
		mastering_voice_ = nullptr;
	}

	SafeRelease(x_audio2_);

	MFShutdown();
}

HRESULT e2d::Audio::CreateVoice(IXAudio2SourceVoice ** voice, WAVEFORMATEX * wfx)
{
	return x_audio2_->CreateSourceVoice(voice, wfx, 0, XAUDIO2_DEFAULT_FREQ_RATIO);
}

void e2d::Audio::Open()
{
	x_audio2_->StartEngine();
}

void e2d::Audio::Close()
{
	x_audio2_->StopEngine();
}
