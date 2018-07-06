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
	for (auto pair : _fileList)
		GC::release(pair.second);
	_fileList.clear();

	for (auto pair : _resList)
		GC::release(pair.second);
	_resList.clear();

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
	UINT hash = filePath.getHashCode();

	if (_fileList.end() != _fileList.find(hash))
	{
		return true;
	}
	else
	{
		Music * music = new (e2d::autorelease) Music();

		if (music->open(filePath))
		{
			GC::retain(music);
			music->setVolume(_volume);
			_fileList.insert(std::pair<UINT, Music *>(hash, music));
			return true;
		}
	}
	return false;
}

bool e2d::Player::preload(int resNameId, const String& resType)
{
	if (_resList.end() != _resList.find(resNameId))
	{
		return true;
	}
	else
	{
		Music * music = new (e2d::autorelease) Music();

		if (music->open(resNameId, resType))
		{
			GC::retain(music);
			music->setVolume(_volume);
			_resList.insert(std::pair<UINT, Music *>(resNameId, music));
			return true;
		}
	}
	return false;
}

bool e2d::Player::play(const String& filePath, int nLoopCount)
{
	if (Player::preload(filePath))
	{
		UINT hash = filePath.getHashCode();
		auto music = _fileList[hash];
		if (music->play(nLoopCount))
		{
			return true;
		}
	}
	return false;
}

bool e2d::Player::play(int resNameId, const String& resType, int nLoopCount)
{
	if (Player::preload(resNameId, resType))
	{
		auto music = _resList[resNameId];
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

	UINT hash = filePath.getHashCode();

	if (_fileList.end() != _fileList.find(hash))
		_fileList[hash]->pause();
}

void e2d::Player::pause(int resNameId, const String& resType)
{
	if (_resList.end() != _resList.find(resNameId))
		_resList[resNameId]->pause();
}

void e2d::Player::resume(const String& filePath)
{
	if (filePath.isEmpty())
		return;

	UINT hash = filePath.getHashCode();

	if (_fileList.end() != _fileList.find(hash))
		_fileList[hash]->resume();
}

void e2d::Player::resume(int resNameId, const String& resType)
{
	if (_resList.end() != _resList.find(resNameId))
		_resList[resNameId]->pause();
}

void e2d::Player::stop(const String& filePath)
{
	if (filePath.isEmpty())
		return;

	UINT hash = filePath.getHashCode();

	if (_fileList.end() != _fileList.find(hash))
		_fileList[hash]->stop();
}

void e2d::Player::stop(int resNameId, const String& resType)
{
	if (_resList.end() != _resList.find(resNameId))
		_resList[resNameId]->stop();
}

bool e2d::Player::isPlaying(const String& filePath)
{
	if (filePath.isEmpty())
		return false;

	UINT hash = filePath.getHashCode();

	if (_fileList.end() != _fileList.find(hash))
		return _fileList[hash]->isPlaying();

	return false;
}

bool e2d::Player::isPlaying(int resNameId, const String& resType)
{
	if (_resList.end() != _resList.find(resNameId))
		return _resList[resNameId]->isPlaying();
	return false;
}

double e2d::Player::getVolume()
{
	return _volume;
}

void e2d::Player::setVolume(double volume)
{
	_volume = std::min(std::max(float(volume), -224.f), 224.f);
	for (auto pair : _fileList)
	{
		pair.second->setVolume(_volume);
	}

	for (auto pair : _resList)
	{
		pair.second->setVolume(_volume);
	}
}

void e2d::Player::pauseAll()
{
	for (auto pair : _fileList)
	{
		pair.second->pause();
	}

	for (auto pair : _resList)
	{
		pair.second->pause();
	}
}

void e2d::Player::resumeAll()
{
	for (auto pair : _fileList)
	{
		pair.second->resume();
	}

	for (auto pair : _resList)
	{
		pair.second->resume();
	}
}

void e2d::Player::stopAll()
{
	for (auto pair : _fileList)
	{
		pair.second->stop();
	}

	for (auto pair : _resList)
	{
		pair.second->stop();
	}
}

void e2d::Player::clearCache()
{
	for (auto pair : _fileList)
	{
		GC::release(pair.second);
	}
	_fileList.clear();

	for (auto pair : _resList)
	{
		GC::release(pair.second);
	}
	_resList.clear();
}
