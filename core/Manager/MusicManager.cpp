#include "..\emanagers.h"
#include "..\etools.h"
#include <map>

static IXAudio2 * s_pXAudio2 = nullptr;
static IXAudio2MasteringVoice * s_pMasteringVoice = nullptr;

typedef std::pair<UINT, e2d::EMusic *> MusicPair;
typedef std::map<UINT, e2d::EMusic *> MusicList;

static MusicList& getMusicList()
{
	static MusicList s_List;
	return s_List;
}


bool e2d::EMusicManager::add(const EString & strFilePath)
{
	EMusic * pPlayer = get(strFilePath);
	if (pPlayer)
	{
		return true;
	}
	else
	{
		UINT nRet = strFilePath.hash();
		pPlayer = new EMusic();

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

e2d::EMusic * e2d::EMusicManager::get(const EString & strFilePath)
{
	if (strFilePath.isEmpty())
		return nullptr;

	UINT nRet = strFilePath.hash();

	if (getMusicList().end() != getMusicList().find(nRet))
		return getMusicList()[nRet];

	return nullptr;
}

void e2d::EMusicManager::pauseAllMusics()
{
	for (auto iter : getMusicList())
	{
		iter.second->pause();
	}
}

void e2d::EMusicManager::resumeAllMusics()
{
	for (auto iter : getMusicList())
	{
		iter.second->resume();
	}
}

void e2d::EMusicManager::stopAllMusics()
{
	for (auto iter : getMusicList())
	{
		iter.second->stop();
	}
}

IXAudio2 * e2d::EMusicManager::getIXAudio2()
{
	return s_pXAudio2;
}

IXAudio2MasteringVoice * e2d::EMusicManager::getIXAudio2MasteringVoice()
{
	return s_pMasteringVoice;
}

bool e2d::EMusicManager::__init()
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

void e2d::EMusicManager::__uninit()
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