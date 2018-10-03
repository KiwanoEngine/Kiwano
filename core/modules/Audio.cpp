#include "..\e2dmodule.h"


e2d::Audio::Audio()
	: x_audio2_(nullptr)
	, mastering_voice_(nullptr)
{
	::CoInitialize(nullptr);

	ThrowIfFailed(
		XAudio2Create(&x_audio2_, 0)
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

	::CoUninitialize();
}

HRESULT e2d::Audio::CreateVoice(IXAudio2SourceVoice ** voice, WAVEFORMATEX * wfx, VoiceCallback * callback)
{
	return x_audio2_->CreateSourceVoice(voice, wfx, 0, XAUDIO2_DEFAULT_FREQ_RATIO, callback);;
}

void e2d::Audio::Open()
{
	x_audio2_->StartEngine();
}

void e2d::Audio::Close()
{
	x_audio2_->StopEngine();
}
