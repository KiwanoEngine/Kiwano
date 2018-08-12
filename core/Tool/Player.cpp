#include "..\e2dtool.h"


e2d::Player::Player()
	: _volume(1.f)
	, _enabled(true)
	, _xAudio2(nullptr)
	, _masteringVoice(nullptr)
{
	CoInitialize(nullptr);

	if (FAILED(XAudio2Create(&_xAudio2, 0)))
	{
		WARN("初始化 XAudio2 组件失败");
	}

	if (FAILED(_xAudio2->CreateMasteringVoice(&_masteringVoice)))
	{
		WARN("初始化 MasteringVoice 组件失败");
	}
}

e2d::Player::~Player()
{
	if (!_musicList.empty())
	{
		for (auto pair : _musicList)
		{
			delete pair.second;
		}
	}

	if (_masteringVoice)
	{
		_masteringVoice->DestroyVoice();
		_masteringVoice = nullptr;
	}

	SafeRelease(_xAudio2);

	CoUninitialize();
}

IXAudio2 * e2d::Player::getXAudio2()
{
	return _xAudio2;
}

IXAudio2MasteringVoice * e2d::Player::getMasteringVoice()
{
	return _masteringVoice;
}

bool e2d::Player::preload(const Resource& res)
{
	if (_musicList.end() != _musicList.find(res))
		return true;

	Music * music = new (std::nothrow) Music();

	if (music && music->open(res))
	{
		music->setVolume(_volume);
		_musicList.insert(std::make_pair(res, music));
		return true;
	}
	return false;
}

bool e2d::Player::play(const Resource& res, int nLoopCount)
{
	if (Player::preload(res))
	{
		auto music = _musicList[res];
		if (music->play(nLoopCount))
		{
			return true;
		}
	}
	return false;
}

void e2d::Player::pause(const Resource& res)
{
	if (_musicList.end() != _musicList.find(res))
		_musicList[res]->pause();
}

void e2d::Player::resume(const Resource& res)
{
	if (_musicList.end() != _musicList.find(res))
		_musicList[res]->pause();
}

void e2d::Player::stop(const Resource& res)
{
	if (_musicList.end() != _musicList.find(res))
		_musicList[res]->stop();
}

bool e2d::Player::isPlaying(const Resource& res)
{
	if (_musicList.end() != _musicList.find(res))
		return _musicList[res]->isPlaying();
	return false;
}

float e2d::Player::getVolume()
{
	return _volume;
}

void e2d::Player::setVolume(float volume)
{
	_volume = std::min(std::max(volume, -224.f), 224.f);
	for (auto pair : _musicList)
	{
		pair.second->setVolume(_volume);
	}
}

void e2d::Player::pauseAll()
{
	for (auto pair : _musicList)
	{
		pair.second->pause();
	}
}

void e2d::Player::resumeAll()
{
	for (auto pair : _musicList)
	{
		pair.second->resume();
	}
}

void e2d::Player::stopAll()
{
	for (auto pair : _musicList)
	{
		pair.second->stop();
	}
}

void e2d::Player::setEnabled(bool enabled)
{
	if (_enabled == enabled)
		return;

	_enabled = enabled;
	_enabled ? _xAudio2->StartEngine() : _xAudio2->StopEngine();
}

void e2d::Player::clearCache()
{
	for (auto pair : _musicList)
	{
		delete pair.second;
	}
	_musicList.clear();
}
