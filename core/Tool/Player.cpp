#include "..\e2dtool.h"


e2d::Player::Player()
	: _volume(1.f)
{
}

e2d::Player::~Player()
{
	if (!_musicList.empty())
	{
		for (const auto& pair : _musicList)
		{
			delete pair.second;
		}
	}

	CoUninitialize();
}

bool e2d::Player::preload(const String & filePath)
{
	if (filePath.empty())
		return false;

	Music * music = new (std::nothrow) Music();

	if (music && music->open(filePath))
	{
		music->volume(_volume);
		_musicList.insert(std::make_pair(filePath.hash(), music));
		return true;
	}
	return false;
}

bool e2d::Player::play(const String & filePath, int nLoopCount)
{
	if (filePath.empty())
		return false;

	if (Player::preload(filePath))
	{
		auto music = _musicList[filePath.hash()];
		if (music->play(nLoopCount))
		{
			return true;
		}
	}
	return false;
}

void e2d::Player::pause(const String & filePath)
{
	if (filePath.empty())
		return;

	size_t hash = filePath.hash();
	if (_musicList.end() != _musicList.find(hash))
		_musicList[hash]->pause();
}

void e2d::Player::resume(const String & filePath)
{
	if (filePath.empty())
		return;

	size_t hash = filePath.hash();
	if (_musicList.end() != _musicList.find(hash))
		_musicList[hash]->resume();
}

void e2d::Player::stop(const String & filePath)
{
	if (filePath.empty())
		return;

	size_t hash = filePath.hash();
	if (_musicList.end() != _musicList.find(hash))
		_musicList[hash]->stop();
}

bool e2d::Player::playing(const String & filePath)
{
	if (filePath.empty())
		return false;

	size_t hash = filePath.hash();
	if (_musicList.end() != _musicList.find(hash))
		return _musicList[hash]->playing();
	return false;
}

bool e2d::Player::preload(const Resource& res)
{
	if (_musicList.end() != _musicList.find(res.resNameId))
		return true;

	Music * music = new (std::nothrow) Music();

	if (music && music->open(res))
	{
		music->volume(_volume);
		_musicList.insert(std::make_pair(res.resNameId, music));
		return true;
	}
	return false;
}

bool e2d::Player::play(const Resource& res, int nLoopCount)
{
	if (Player::preload(res))
	{
		auto music = _musicList[res.resNameId];
		if (music->play(nLoopCount))
		{
			return true;
		}
	}
	return false;
}

void e2d::Player::pause(const Resource& res)
{
	if (_musicList.end() != _musicList.find(res.resNameId))
		_musicList[res.resNameId]->pause();
}

void e2d::Player::resume(const Resource& res)
{
	if (_musicList.end() != _musicList.find(res.resNameId))
		_musicList[res.resNameId]->resume();
}

void e2d::Player::stop(const Resource& res)
{
	if (_musicList.end() != _musicList.find(res.resNameId))
		_musicList[res.resNameId]->stop();
}

bool e2d::Player::playing(const Resource& res) const
{
	if (_musicList.end() != _musicList.find(res.resNameId))
		return _musicList.at(res.resNameId)->playing();
	return false;
}

float e2d::Player::volume() const
{
	return _volume;
}

void e2d::Player::volume(float volume)
{
	_volume = std::min(std::max(volume, -224.f), 224.f);
	for (const auto& pair : _musicList)
	{
		pair.second->volume(_volume);
	}
}

void e2d::Player::pauseAll()
{
	for (const auto& pair : _musicList)
	{
		pair.second->pause();
	}
}

void e2d::Player::resumeAll()
{
	for (const auto& pair : _musicList)
	{
		pair.second->resume();
	}
}

void e2d::Player::stopAll()
{
	for (const auto& pair : _musicList)
	{
		pair.second->stop();
	}
}

void e2d::Player::clearCache()
{
	for (const auto& pair : _musicList)
	{
		delete pair.second;
	}
	_musicList.clear();
}
