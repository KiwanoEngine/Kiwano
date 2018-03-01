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


bool e2d::MusicManager::add(const String & strFilePath)
{
	Music * pPlayer = get(strFilePath);
	if (pPlayer)
	{
		return true;
	}
	else
	{
		UINT nRet = strFilePath.getHashCode();
		pPlayer = new Music();

		if (pPlayer->_open(strFilePath))
		{
			getMusicList().insert(MusicPair(nRet, pPlayer));
			return true;
		}
		else
		{
			delete pPlayer;
			return false;
		}
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

void e2d::MusicManager::pauseAllMusics()
{
	for (auto iter : getMusicList())
	{
		iter.second->pause();
	}
}

void e2d::MusicManager::resumeAllMusics()
{
	for (auto iter : getMusicList())
	{
		iter.second->resume();
	}
}

void e2d::MusicManager::stopAllMusics()
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
		iter.second->_close();
		delete iter.second;
	}
	getMusicList().clear();

	if (s_pMasteringVoice)
	{
		s_pMasteringVoice->DestroyVoice();
	}

	SafeReleaseInterface(&s_pXAudio2);
}