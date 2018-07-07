#include "..\e2dtool.h"


e2d::Player * e2d::Player::_instance = nullptr;

e2d::Player::Player()
	: _volume(1.f)
	, _xAudio2(nullptr)
	, _masteringVoice(nullptr)
{
	CoInitialize(nullptr);
}

e2d::Player::~Player()
{
	for (auto pair : _musicList)
		GC::release(pair.second);
	_musicList.clear();

	if (_masteringVoice)
		_masteringVoice->DestroyVoice();

	SafeRelease(_xAudio2);

	CoUninitialize();
}

e2d::Player * e2d::Player::getInstance()
{
	if (!_instance)
	{
		_instance = new (std::nothrow) Player;

		HRESULT hr;
		if (FAILED(hr = XAudio2Create(&_instance->_xAudio2, 0)) ||
			FAILED(hr = _instance->_xAudio2->CreateMasteringVoice(&_instance->_masteringVoice)))
		{
			throw SystemException(L"初始化 XAudio2 组件失败");
		}
	}
	return _instance;
}

void e2d::Player::destroyInstance()
{
	if (_instance)
	{
		delete _instance;
		_instance = nullptr;
	}
}

IXAudio2 * e2d::Player::getXAudio2()
{
	return _xAudio2;
}

bool e2d::Player::preload(const String& filePath)
{
	if (filePath.isEmpty())
		return false;

	return preload(Resource(filePath));
}

bool e2d::Player::preload(const Resource& res)
{
	if (_musicList.end() != _musicList.find(res))
		return true;

	Music * music = new (e2d::autorelease) Music();

	if (music->open(res))
	{
		GC::retain(music);
		music->setVolume(_volume);
		_musicList.insert(std::make_pair(res, music));
		return true;
	}
	return false;
}

bool e2d::Player::play(const String& filePath, int nLoopCount)
{
	if (filePath.isEmpty())
		return false;

	return play(Resource(filePath), nLoopCount);
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

void e2d::Player::pause(const String& filePath)
{
	if (filePath.isEmpty())
		return;

	pause(Resource(filePath));
}

void e2d::Player::pause(const Resource& res)
{
	if (_musicList.end() != _musicList.find(res))
		_musicList[res]->pause();
}

void e2d::Player::resume(const String& filePath)
{
	if (filePath.isEmpty())
		return;

	resume(Resource(filePath));
}

void e2d::Player::resume(const Resource& res)
{
	if (_musicList.end() != _musicList.find(res))
		_musicList[res]->pause();
}

void e2d::Player::stop(const String& filePath)
{
	if (filePath.isEmpty())
		return;

	stop(Resource(filePath));
}

void e2d::Player::stop(const Resource& res)
{
	if (res.isResource())
	{

	}
	if (_musicList.end() != _musicList.find(res))
		_musicList[res]->stop();
}

bool e2d::Player::isPlaying(const String& filePath)
{
	if (filePath.isEmpty())
		return false;

	return isPlaying(Resource(filePath));
}

bool e2d::Player::isPlaying(const Resource& res)
{
	if (_musicList.end() != _musicList.find(res))
		return _musicList[res]->isPlaying();
	return false;
}

double e2d::Player::getVolume()
{
	return _volume;
}

void e2d::Player::setVolume(double volume)
{
	_volume = std::min(std::max(float(volume), -224.f), 224.f);
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

void e2d::Player::clearCache()
{
	for (auto pair : _musicList)
	{
		GC::release(pair.second);
	}
	_musicList.clear();
}
