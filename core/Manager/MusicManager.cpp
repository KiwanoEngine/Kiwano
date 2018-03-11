#include "..\emanagers.h"
#include "..\etools.h"
#include <map>

static IXAudio2 * s_pXAudio2 = nullptr;
static IXAudio2MasteringVoice * s_pMasteringVoice = nullptr;

typedef std::pair<UINT, e2d::Music *> MusicPair;
typedef std::map<UINT, e2d::Music *> MusicList;

static MusicList& getMusicList()
{
	static MusicList s_List;
	return s_List;
}


bool e2d::MusicManager::preload(const String & strFilePath)
{
	UINT nRet = strFilePath.getHashCode();

	if (getMusicList().end() != getMusicList().find(nRet))
	{
		return true;
	}
	else
	{
		Music * pPlayer = new Music();

		if (pPlayer->open(strFilePath))
		{
			getMusicList().insert(MusicPair(nRet, pPlayer));
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

bool e2d::MusicManager::play(const String & strFilePath, int nLoopCount)
{
	if (MusicManager::preload(strFilePath))
	{
		UINT nRet = strFilePath.getHashCode();
		Music * pMusic = getMusicList()[nRet];
		if (pMusic->play(nLoopCount))
		{
			return true;
		}
	}
	return false;
}

void e2d::MusicManager::pause(const String & strFilePath)
{
	auto music = MusicManager::get(strFilePath);
	if (music)
	{
		music->pause();
	}
}

void e2d::MusicManager::resume(const String & strFilePath)
{
	auto music = MusicManager::get(strFilePath);
	if (music)
	{
		music->resume();
	}
}

void e2d::MusicManager::stop(const String & strFilePath)
{
	auto music = MusicManager::get(strFilePath);
	if (music)
	{
		music->stop();
	}
}

e2d::Music * e2d::MusicManager::get(const String & strFilePath)
{
	if (strFilePath.isEmpty())
		return nullptr;

	UINT nRet = strFilePath.getHashCode();

	if (getMusicList().end() != getMusicList().find(nRet))
		return getMusicList()[nRet];

	return nullptr;
}

void e2d::MusicManager::pauseAll()
{
	for (auto iter : getMusicList())
	{
		iter.second->pause();
	}
}

void e2d::MusicManager::resumeAll()
{
	for (auto iter : getMusicList())
	{
		iter.second->resume();
	}
}

void e2d::MusicManager::stopAll()
{
	for (auto iter : getMusicList())
	{
		iter.second->stop();
	}
}

IXAudio2 * e2d::MusicManager::getIXAudio2()
{
	return s_pXAudio2;
}

IXAudio2MasteringVoice * e2d::MusicManager::getIXAudio2MasteringVoice()
{
	return s_pMasteringVoice;
}

bool e2d::MusicManager::__init()
{
	HRESULT hr;

	if (FAILED(hr = XAudio2Create(&s_pXAudio2, 0)))
	{
		WARN_IF(true, "Failed to init XAudio2 engine");
		return false;
	}

	if (FAILED(hr = s_pXAudio2->CreateMasteringVoice(&s_pMasteringVoice)))
	{
		WARN_IF(true, "Failed creating mastering voice");
		SafeReleaseInterface(&s_pXAudio2);
		return false;
	}

	return true;
}

void e2d::MusicManager::__uninit()
{
	for (auto iter : getMusicList())
	{
		iter.second->close();
		iter.second->release();
	}
	getMusicList().clear();

	if (s_pMasteringVoice)
	{
		s_pMasteringVoice->DestroyVoice();
	}

	SafeReleaseInterface(&s_pXAudio2);
}