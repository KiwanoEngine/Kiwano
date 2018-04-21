#include "..\e2dmanager.h"
#include "..\e2dtool.h"
#include <map>

#if HIGHER_THAN_VS2010

static IXAudio2 * s_pXAudio2 = nullptr;
static IXAudio2MasteringVoice * s_pMasteringVoice = nullptr;

#else

static HINSTANCE s_hInstance = nullptr;

#endif


typedef std::pair<UINT, e2d::Music *> MusicPair;
typedef std::map<UINT, e2d::Music *> MusicList;

static MusicList& getMusicList()
{
	static MusicList s_List;
	return s_List;
}


bool e2d::MusicManager::preload(String strFilePath)
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

bool e2d::MusicManager::play(String strFilePath, int nLoopCount)
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

	if (getMusicList().end() != getMusicList().find(nRet))
		return getMusicList()[nRet];

	return nullptr;
}

void e2d::MusicManager::pauseAll()
{
	for (auto iter = getMusicList().begin(); iter != getMusicList().end(); iter++)
	{
		(*iter).second->pause();
	}
}

void e2d::MusicManager::resumeAll()
{
	for (auto iter = getMusicList().begin(); iter != getMusicList().end(); iter++)
	{
		(*iter).second->resume();
	}
}

void e2d::MusicManager::stopAll()
{
	for (auto iter = getMusicList().begin(); iter != getMusicList().end(); iter++)
	{
		(*iter).second->stop();
	}
}


#if HIGHER_THAN_VS2010

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

#else

HINSTANCE e2d::MusicManager::getHInstance()
{
	return s_hInstance;
}

bool e2d::MusicManager::__init()
{
	s_hInstance = HINST_THISCOMPONENT;

	WNDCLASS  wc;
	wc.style = 0;
	wc.lpfnWndProc = Music::MusicProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = s_hInstance;
	wc.hIcon = 0;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = MUSIC_CLASS_NAME;

	if (!RegisterClass(&wc) && 1410 != GetLastError())
	{
		return false;
	}
	return true;
}

void e2d::MusicManager::__uninit()
{
	for (auto iter = getMusicList().begin(); iter != getMusicList().end(); iter++)
	{
		(*iter).second->close();
		(*iter).second->release();
	}
	getMusicList().clear();
}

#endif