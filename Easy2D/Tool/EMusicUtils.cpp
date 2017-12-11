#include "..\etools.h"
#include <mmsystem.h>
#include "..\Win\MciPlayer.h"
#include <map>

typedef std::pair<UINT, MciPlayer *> Music;
typedef std::map<UINT, MciPlayer *> MusicList;

static MusicList& getMciPlayerList()
{
	static MusicList s_List;
	return s_List;
}


UINT e2d::EMusicUtils::playMusic(const EString & musicFilePath, int repeatTimes)
{
	UINT nRet = preloadMusic(musicFilePath);

	if (nRet)
	{
		getMciPlayerList()[nRet]->play(repeatTimes);
	}
	return nRet;
}

UINT e2d::EMusicUtils::playMusic(const EString & musicResourceName, const EString & musicResourceType, const EString & musicExtension, int repeatTimes)
{
	UINT nRet = preloadMusic(musicResourceName, musicResourceType, musicExtension);

	if (nRet)
	{
		getMciPlayerList()[nRet]->play(repeatTimes);
	}
	return nRet;
}

UINT e2d::EMusicUtils::preloadMusic(const EString & musicFilePath)
{
	if (musicFilePath.isEmpty()) 
		return 0;

	UINT nRet = musicFilePath.hash();

	if (getMciPlayerList().end() != getMciPlayerList().find(nRet)) 
		return nRet;

	getMciPlayerList().insert(Music(nRet, new MciPlayer()));
	MciPlayer * pPlayer = getMciPlayerList()[nRet];
	pPlayer->open(musicFilePath, nRet);

	if (nRet == pPlayer->getMusicID()) return nRet;

	delete pPlayer;
	getMciPlayerList().erase(nRet);
	return 0;
}

UINT e2d::EMusicUtils::preloadMusic(const EString & musicResourceName, const EString & musicResourceType, const EString & musicExtension)
{
	if (musicResourceName.isEmpty() || musicResourceType.isEmpty())
		return 0;

	UINT nRet = musicResourceName.hash();

	if (getMciPlayerList().end() != getMciPlayerList().find(nRet)) 
		return nRet;

	getMciPlayerList().insert(Music(nRet, new MciPlayer()));
	MciPlayer * pPlayer = getMciPlayerList()[nRet];
	pPlayer->open(musicResourceName, musicResourceType, musicExtension, nRet);

	if (nRet == pPlayer->getMusicID()) return nRet;

	delete pPlayer;
	getMciPlayerList().erase(nRet);
	return 0;
}

bool e2d::EMusicUtils::resumeMusic(UINT musicId)
{
	MusicList::iterator p = getMciPlayerList().find(musicId);
	if (p != getMciPlayerList().end())
	{
		p->second->resume();
		return true;
	}
	return false;
}

bool e2d::EMusicUtils::resumeMusic(const EString & musicName)
{
	return resumeMusic(musicName.hash());;
}

bool e2d::EMusicUtils::pauseMusic(UINT musicId)
{
	MusicList::iterator p = getMciPlayerList().find(musicId);
	if (p != getMciPlayerList().end())
	{
		p->second->pause();
		return true;
	}
	return false;
}

bool e2d::EMusicUtils::pauseMusic(const EString & musicName)
{
	return pauseMusic(musicName.hash());
}

bool e2d::EMusicUtils::stopMusic(UINT musicId)
{
	MusicList::iterator p = getMciPlayerList().find(musicId);
	if (p != getMciPlayerList().end())
	{
		p->second->stop();
		return true;
	}
	return false;
}

bool e2d::EMusicUtils::stopMusic(const EString & musicName)
{
	return stopMusic(musicName.hash());;
}

void e2d::EMusicUtils::pauseAllMusics()
{
	for (auto& iter : getMciPlayerList())
	{
		iter.second->pause();
	}
}

void e2d::EMusicUtils::resumeAllMusics()
{
	for (auto& iter : getMciPlayerList())
	{
		iter.second->resume();
	}
}

void e2d::EMusicUtils::stopAllMusics()
{
	for (auto& iter : getMciPlayerList())
	{
		iter.second->stop();
	}
}
