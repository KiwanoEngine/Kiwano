#include "..\e2dbase.h"


e2d::Audio * e2d::Audio::_instance = nullptr;

e2d::Audio * e2d::Audio::getInstance()
{
	if (!_instance)
	{
		_instance = new (std::nothrow) Audio;
	}
	return _instance;
}

void e2d::Audio::destroyInstance()
{
	if (_instance)
	{
		delete _instance;
		_instance = nullptr;
	}
}

IXAudio2 * e2d::Audio::getXAudio2()
{
	return _xAudio2;
}

IXAudio2MasteringVoice * e2d::Audio::getMasteringVoice()
{
	return _masteringVoice;
}

e2d::Audio::Audio()
	: _xAudio2(nullptr)
	, _masteringVoice(nullptr)
{
	::CoInitialize(nullptr);

	ThrowIfFailed(
		XAudio2Create(&_xAudio2, 0)
	);

	ThrowIfFailed(
		_xAudio2->CreateMasteringVoice(&_masteringVoice)
	);
}

e2d::Audio::~Audio()
{
	if (_masteringVoice)
	{
		_masteringVoice->DestroyVoice();
		_masteringVoice = nullptr;
	}

	SafeRelease(_xAudio2);

	::CoUninitialize();
}
