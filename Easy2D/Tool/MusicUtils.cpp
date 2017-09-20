#include "..\Easy2d.h"
#include <Digitalv.h>
#include <mmsystem.h>
#pragma comment(lib , "winmm.lib")
#include <map>

////////////////////////////////////////////////////////////////////
// MciPlayer
////////////////////////////////////////////////////////////////////

class MciPlayer
{
public:
	MciPlayer();
	~MciPlayer();

	void close();
	void open(tstring pFileName, UINT uId);
	void play(bool bLoop = false);
	void pause();
	void resume();
	void stop();
	void rewind();
	void setVolume(float volume);
	bool isPlaying();
	UINT getSoundID();

private:
	void _sendCommand(int nCommand, DWORD_PTR param1 = 0, DWORD_PTR parma2 = 0);

	MCIDEVICEID m_dev;
	UINT        m_nSoundID;
	bool        m_bPlaying;
	bool		m_bLoop;
	tstring		m_sExt;
};


MciPlayer::MciPlayer() :
	m_dev(0L),
	m_nSoundID(0),
	m_bPlaying(false),
	m_bLoop(false),
	m_sExt(_T(""))
{
}

MciPlayer::~MciPlayer()
{
	close();	// 关闭播放器
}

void MciPlayer::open(tstring pFileName, UINT uId)
{
	// 忽略不存在的文件
	if (pFileName.empty() || !PathFileExists(pFileName.c_str())) return;
	// 获取文件后缀名
	m_sExt = FileUtils::getFileExtension(pFileName);
	// 停止当前音乐
	close();

	// 设置 MCI_OPEN_PARMS 参数
	MCI_OPEN_PARMS mciOpen = { 0 };
	mciOpen.lpstrDeviceType = (LPCTSTR)MCI_ALL_DEVICE_ID;
	mciOpen.lpstrElementName = pFileName.c_str();

	// 打开这个文件
	MCIERROR mciError;
	mciError = mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT, reinterpret_cast<DWORD_PTR>(&mciOpen));
	// 出现错误时，忽略这次操作
	if (mciError) return;

	// 保存设备等信息
	m_dev = mciOpen.wDeviceID;
	m_nSoundID = uId;
	m_bPlaying = false;
}

void MciPlayer::play(bool bLoop)
{
	// 设备为空时，忽略这次操作
	if (!m_dev)
	{
		return;
	}
	// 设置播放参数
	MCI_PLAY_PARMS mciPlay = { 0 };
	MCIERROR s_mciError;
	// 播放声音
	s_mciError = mciSendCommand(m_dev, MCI_PLAY, MCI_FROM | (bLoop ? MCI_DGV_PLAY_REPEAT : 0), reinterpret_cast<DWORD_PTR>(&mciPlay));
	// 未出错时，置 m_bPlaying 为 true
	if (!s_mciError)
	{
		m_bPlaying = true;
		m_bLoop = bLoop;
	}
}

void MciPlayer::close()
{
	// 停止音乐
	if (m_bPlaying)
	{
		stop();
	}
	// 关闭设备
	if (m_dev)
	{
		_sendCommand(MCI_CLOSE);
	}
	// 恢复默认属性
	m_dev = 0;
	m_bPlaying = false;
}

void MciPlayer::pause()
{
	// 暂停音乐
	_sendCommand(MCI_PAUSE);
	m_bPlaying = false;
}

void MciPlayer::resume()
{
	// 继续音乐
	if (m_sExt == _T(".mid"))
	{
		// midi 不支持 MCI_RESUME 参数，应使用 MCI_FROM 设置播放起始位置
		// 获取 MCI 状态
		MCI_STATUS_PARMS mciStatusParms;
		mciStatusParms.dwItem = MCI_STATUS_POSITION;
		_sendCommand(MCI_STATUS, MCI_STATUS_ITEM, reinterpret_cast<DWORD_PTR>(&mciStatusParms));
		// 设置播放起始位置，并开始播放
		MCI_PLAY_PARMS mciPlayParms;
		mciPlayParms.dwFrom = mciStatusParms.dwReturn;
		_sendCommand(MCI_PLAY, MCI_FROM, reinterpret_cast<DWORD_PTR>(&mciPlayParms));
	}
	else
	{
		// 继续播放音乐
		_sendCommand(MCI_RESUME);
		m_bPlaying = true;
	}
}

void MciPlayer::stop()
{
	// 停止音乐
	_sendCommand(MCI_STOP);
	m_bPlaying = false;
}

void MciPlayer::rewind()
{
	// 设备为空时，忽略这次操作
	if (!m_dev)
	{
		return;
	}
	// 重置播放位置
	mciSendCommand(m_dev, MCI_SEEK, MCI_SEEK_TO_START, 0);
	// 播放音乐
	MCI_PLAY_PARMS mciPlay = { 0 };
	m_bPlaying = mciSendCommand(m_dev, MCI_PLAY, (m_bLoop ? MCI_DGV_PLAY_REPEAT : 0), reinterpret_cast<DWORD_PTR>(&mciPlay)) ? false : true;
}

void MciPlayer::setVolume(float volume)
{
	volume = min(max(volume, 0), 1);
	MCI_DGV_SETAUDIO_PARMS  mciSetAudioPara = { 0 };
	mciSetAudioPara.dwItem = MCI_DGV_SETAUDIO_VOLUME;
	mciSetAudioPara.dwValue = DWORD(1000 * volume);
	mciSendCommand(m_dev, MCI_SETAUDIO, MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD)(LPVOID)&mciSetAudioPara);
}

bool MciPlayer::isPlaying()
{
	return m_bPlaying;
}

UINT MciPlayer::getSoundID()
{
	return m_nSoundID;
}

void MciPlayer::_sendCommand(int nCommand, DWORD_PTR param1, DWORD_PTR parma2)
{
	// 空设备时忽略这次操作
	if (!m_dev)
	{
		return;
	}
	// 向当前设备发送操作
	mciSendCommand(m_dev, nCommand, param1, parma2);
}




////////////////////////////////////////////////////////////////////
// MusicUtils
////////////////////////////////////////////////////////////////////


typedef std::map<unsigned int, MciPlayer *> MusicList;
typedef std::pair<unsigned int, MciPlayer *> Music;

static unsigned int _Hash(tstring key);


static MusicList& getMciPlayerList()
{
	static MusicList s_List;
	return s_List;
}

static MciPlayer& getBgMciPlayer()
{
	static MciPlayer s_Music;
	return s_Music;
}

void MusicUtils::end()
{
	// 停止背景音乐
	getBgMciPlayer().close();
	// 停止其他所有音乐
	for (auto& iter : getMciPlayerList())
	{
		SAFE_DELETE(iter.second);
	}
	// 清空音乐列表
	getMciPlayerList().clear();
	return;
}

void MusicUtils::setVolume(float volume)
{
	// 设置背景音乐音量
	getBgMciPlayer().setVolume(volume);
	// 设置其他音乐音量
	for (auto& iter : getMciPlayerList())
	{
		iter.second->setVolume(volume);
	}
}

void MusicUtils::setVolume(tstring pszFilePath, float volume)
{
	unsigned int nRet = ::_Hash(pszFilePath);

	MusicList::iterator p = getMciPlayerList().find(nRet);
	if (p != getMciPlayerList().end())
	{
		p->second->setVolume(volume);
	}
}

void MusicUtils::playBackgroundMusic(tstring pszFilePath, bool bLoop)
{
	if (pszFilePath.empty())
	{
		return;
	}

	getBgMciPlayer().open(pszFilePath, ::_Hash(pszFilePath));
	getBgMciPlayer().play(bLoop);
}

void MusicUtils::stopBackgroundMusic(bool bReleaseData)
{
	if (bReleaseData)
	{
		getBgMciPlayer().close();
	}
	else
	{
		getBgMciPlayer().stop();
	}
}

void MusicUtils::pauseBackgroundMusic()
{
	getBgMciPlayer().pause();
}

void MusicUtils::resumeBackgroundMusic()
{
	getBgMciPlayer().resume();
}

void MusicUtils::rewindBackgroundMusic()
{
	getBgMciPlayer().rewind();
}

bool MusicUtils::isBackgroundMusicPlaying()
{
	return getBgMciPlayer().isPlaying();
}

void MusicUtils::setBackgroundMusicVolume(float volume)
{
	getBgMciPlayer().setVolume(volume);
}

unsigned int MusicUtils::playMusic(tstring pszFilePath, bool bLoop)
{
	unsigned int nRet = ::_Hash(pszFilePath);

	preloadMusic(pszFilePath);

	MusicList::iterator p = getMciPlayerList().find(nRet);
	if (p != getMciPlayerList().end())
	{
		p->second->play(bLoop);
	}
	return nRet;
}

void MusicUtils::stopMusic(unsigned int nSoundId)
{
	MusicList::iterator p = getMciPlayerList().find(nSoundId);
	if (p != getMciPlayerList().end())
	{
		p->second->stop();
	}
}

void MusicUtils::preloadMusic(tstring pszFilePath)
{
	if (pszFilePath.empty()) return;

	int nRet = ::_Hash(pszFilePath);

	if (getMciPlayerList().end() != getMciPlayerList().find(nRet)) return;

	getMciPlayerList().insert(Music(nRet, new MciPlayer()));
	MciPlayer * pPlayer = getMciPlayerList()[nRet];
	pPlayer->open(pszFilePath, nRet);

	if (nRet == pPlayer->getSoundID()) return;

	delete pPlayer;
	getMciPlayerList().erase(nRet);
	nRet = 0;
}

void MusicUtils::pauseMusic(unsigned int nSoundId)
{
	MusicList::iterator p = getMciPlayerList().find(nSoundId);
	if (p != getMciPlayerList().end())
	{
		p->second->pause();
	}
}

void MusicUtils::pauseAllMusics()
{
	for (auto& iter : getMciPlayerList())
	{
		iter.second->pause();
	}
}

void MusicUtils::resumeMusic(unsigned int nSoundId)
{
	MusicList::iterator p = getMciPlayerList().find(nSoundId);
	if (p != getMciPlayerList().end())
	{
		p->second->resume();
	}
}

void MusicUtils::resumeAllMusics()
{
	for (auto& iter : getMciPlayerList())
	{
		iter.second->resume();
	}
}

void MusicUtils::stopAllMusics()
{
	for (auto& iter : getMciPlayerList())
	{
		iter.second->stop();
	}
}

void MusicUtils::unloadMusic(LPCTSTR pszFilePath)
{
	unsigned int nID = ::_Hash(pszFilePath);
	
	MusicList::iterator p = getMciPlayerList().find(nID);
	if (p != getMciPlayerList().end())
	{
		SAFE_DELETE(p->second);
		getMciPlayerList().erase(nID);
	}
}



unsigned int _Hash(tstring key)
{
	unsigned int len = unsigned(key.size());
	unsigned int hash = 0;

	for (unsigned i = 0; i < len; i++)
	{
		hash *= 16777619;
		hash ^= (unsigned int)(unsigned char)toupper(key[i]);
	}
	return (hash);
}