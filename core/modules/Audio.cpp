#include "..\e2dmodule.h"


e2d::Audio * e2d::Audio::instance_ = nullptr;

e2d::Audio * e2d::Audio::GetInstance()
{
	if (!instance_)
	{
		instance_ = new (std::nothrow) Audio;
	}
	return instance_;
}

void e2d::Audio::DestroyInstance()
{
	if (instance_)
	{
		delete instance_;
		instance_ = nullptr;
	}
}

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

IXAudio2 * e2d::Audio::GetXAudio2()
{
	return x_audio2_;
}

IXAudio2MasteringVoice * e2d::Audio::GetMasteringVoice()
{
	return mastering_voice_;
}
