#include "..\e2dtool.h"
#include <map>

typedef std::map<UINT, e2d::Music *> MusicMap;

static MusicMap& GetMusicFileList()
{
	static MusicMap s_MusicFileList;
	return s_MusicFileList;
}

static MusicMap& GetMusicResList()
{
	static MusicMap s_MusicResList;
	return s_MusicResList;
}

static float s_fMusicVolume = 1.0;


bool e2d::Player::preload(const String& filePath)
{
	UINT hash = filePath.getHashCode();

	if (GetMusicFileList().end() != GetMusicFileList().find(hash))
	{
		return true;
	}
	else
	{
		Music * music = new (std::nothrow) Music();

		if (music->open(filePath))
		{
			music->setVolume(s_fMusicVolume);
			GetMusicFileList().insert(std::pair<UINT, Music *>(hash, music));
			return true;
		}
		else
		{
			delete music;
			music = nullptr;
		}
	}
	return false;
}

bool e2d::Player::preload(int resNameId, const String& resType)
{
	if (GetMusicResList().end() != GetMusicResList().find(resNameId))
	{
		return true;
	}
	else
	{
		Music * music = new (std::nothrow) Music();

		if (music->open(resNameId, resType))
		{
			music->setVolume(s_fMusicVolume);
			GetMusicResList().insert(std::pair<UINT, Music *>(resNameId, music));
			return true;
		}
		else
		{
			delete music;
			music = nullptr;
		}
	}
	return false;
}

bool e2d::Player::play(const String& filePath, int nLoopCount)
{
	if (Player::preload(filePath))
	{
		UINT hash = filePath.getHashCode();
		auto music = GetMusicFileList()[hash];
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
		auto music = GetMusicResList()[resNameId];
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

	if (GetMusicFileList().end() != GetMusicFileList().find(hash))
		GetMusicFileList()[hash]->pause();
}

void e2d::Player::pause(int resNameId, const String& resType)
{
	if (GetMusicResList().end() != GetMusicResList().find(resNameId))
		GetMusicResList()[resNameId]->pause();
}

void e2d::Player::resume(const String& filePath)
{
	if (filePath.isEmpty())
		return;

	UINT hash = filePath.getHashCode();

	if (GetMusicFileList().end() != GetMusicFileList().find(hash))
		GetMusicFileList()[hash]->resume();
}

void e2d::Player::resume(int resNameId, const String& resType)
{
	if (GetMusicResList().end() != GetMusicResList().find(resNameId))
		GetMusicResList()[resNameId]->pause();
}

void e2d::Player::stop(const String& filePath)
{
	if (filePath.isEmpty())
		return;

	UINT hash = filePath.getHashCode();

	if (GetMusicFileList().end() != GetMusicFileList().find(hash))
		GetMusicFileList()[hash]->stop();
}

void e2d::Player::stop(int resNameId, const String& resType)
{
	if (GetMusicResList().end() != GetMusicResList().find(resNameId))
		GetMusicResList()[resNameId]->stop();
}

bool e2d::Player::isPlaying(const String& filePath)
{
	if (filePath.isEmpty())
		return false;

	UINT hash = filePath.getHashCode();

	if (GetMusicFileList().end() != GetMusicFileList().find(hash))
		return GetMusicFileList()[hash]->isPlaying();

	return false;
}

bool e2d::Player::isPlaying(int resNameId, const String& resType)
{
	if (GetMusicResList().end() != GetMusicResList().find(resNameId))
		return GetMusicResList()[resNameId]->isPlaying();
	return false;
}

double e2d::Player::getVolume()
{
	return s_fMusicVolume;
}

void e2d::Player::setVolume(double volume)
{
	s_fMusicVolume = min(max(float(volume), -224), 224);
	for (auto pair : GetMusicFileList())
	{
		pair.second->setVolume(s_fMusicVolume);
	}
}

void e2d::Player::pauseAll()
{
	for (auto pair : GetMusicFileList())
	{
		pair.second->pause();
	}
}

void e2d::Player::resumeAll()
{
	for (auto pair : GetMusicFileList())
	{
		pair.second->resume();
	}
}

void e2d::Player::stopAll()
{
	for (auto pair : GetMusicFileList())
	{
		pair.second->stop();
	}
}

void e2d::Player::__uninit()
{
	for (auto pair : GetMusicFileList())
	{
		pair.second->close();
		delete pair.second;
	}

	GetMusicFileList().clear();
}