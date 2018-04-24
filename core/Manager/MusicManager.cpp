#include "..\e2dmanager.h"
#include "..\e2dtool.h"
#include <map>


typedef std::pair<UINT, e2d::Music *> MusicPair;
typedef std::map<UINT, e2d::Music *> MusicMap;

static MusicMap& GetMusicList()
{
	static MusicMap s_List;
	return s_List;
}


bool e2d::MusicManager::preload(String strFilePath)
{
	UINT nRet = strFilePath.getHashCode();

	if (GetMusicList().end() != GetMusicList().find(nRet))
	{
		return true;
	}
	else
	{
		Music * pPlayer = new Music();

		if (pPlayer->open(strFilePath))
		{
			GetMusicList().insert(MusicPair(nRet, pPlayer));
			pPlayer->retain();
			return true;
		}
		else
		{
			pPlayer->release();
			pPlayer = nullptr;
		}
	}
	return false;
}

bool e2d::MusicManager::play(String strFilePath, int nLoopCount)
{
	if (MusicManager::preload(strFilePath))
	{
		UINT nRet = strFilePath.getHashCode();
		Music * pMusic = GetMusicList()[nRet];
		if (pMusic->play(nLoopCount))
		{
			return true;
		}
	}
	return false;
}

void e2d::MusicManager::pause(String strFilePath)
{
	auto music = MusicManager::get(strFilePath);
	if (music)
	{
		music->pause();
	}
}

void e2d::MusicManager::resume(String strFilePath)
{
	auto music = MusicManager::get(strFilePath);
	if (music)
	{
		music->resume();
	}
}

void e2d::MusicManager::stop(String strFilePath)
{
	auto music = MusicManager::get(strFilePath);
	if (music)
	{
		music->stop();
	}
}

e2d::Music * e2d::MusicManager::get(String strFilePath)
{
	if (strFilePath.isEmpty())
		return nullptr;

	UINT nRet = strFilePath.getHashCode();

	if (GetMusicList().end() != GetMusicList().find(nRet))
		return GetMusicList()[nRet];

	return nullptr;
}

void e2d::MusicManager::pauseAll()
{
	for (auto iter = GetMusicList().begin(); iter != GetMusicList().end(); iter++)
	{
		(*iter).second->pause();
	}
}

void e2d::MusicManager::resumeAll()
{
	for (auto iter = GetMusicList().begin(); iter != GetMusicList().end(); iter++)
	{
		(*iter).second->resume();
	}
}

void e2d::MusicManager::stopAll()
{
	for (auto iter = GetMusicList().begin(); iter != GetMusicList().end(); iter++)
	{
		(*iter).second->stop();
	}
}

void e2d::MusicManager::__uninit()
{
	for (auto iter = GetMusicList().begin(); iter != GetMusicList().end(); iter++)
	{
		(*iter).second->close();
		(*iter).second->release();
	}
	GetMusicList().clear();
}
