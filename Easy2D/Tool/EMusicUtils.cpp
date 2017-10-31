#include "..\etools.h"
#include "..\Win\winbase.h"
#include <map>
#include <mmsystem.h>
#include <Digitalv.h>
#pragma comment(lib , "winmm.lib")

static size_t Hash(const e2d::EString & key);
static bool ExtractResource(LPCTSTR strDstFile, LPCTSTR strResType, LPCTSTR strResName);


////////////////////////////////////////////////////////////////////
// MciPlayer
////////////////////////////////////////////////////////////////////

class MciPlayer
{
public:
	MciPlayer();
	~MciPlayer();

	void close();
	bool open(const e2d::EString & pFileName, UINT uId);
	bool open(const e2d::EString & pResouceName, const e2d::EString & pResouceType, const e2d::EString & musicExtension, UINT uId);
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
	e2d::EString m_sTempFileName;
};


MciPlayer::MciPlayer() :
	m_dev(0L),
	m_nSoundID(0),
	m_bPlaying(false),
	m_bLoop(false)
{
}

MciPlayer::~MciPlayer()
{
	close();	// 关闭播放器
}

bool MciPlayer::open(const e2d::EString & pFileName, UINT uId)
{
	// 忽略不存在的文件
	if (pFileName.empty()) 
		return false;

	// 停止当前音乐
	close();

	// 设置 MCI_OPEN_PARMS 参数
	MCI_OPEN_PARMS mciOpen = { 0 };
	mciOpen.lpstrElementName = pFileName.c_str();

	// 打开这个文件
	MCIERROR mciError;
	mciError = mciSendCommand(
		0, 
		MCI_OPEN, 
		MCI_OPEN_ELEMENT | MCI_NOTIFY,
		reinterpret_cast<DWORD_PTR>(&mciOpen)
	);

	if (mciError)
	{
		return false;
	}
	else
	{
		// 保存设备等信息
		m_dev = mciOpen.wDeviceID;
		m_nSoundID = uId;
		m_bPlaying = false;
		return true;
	}
}

bool MciPlayer::open(const e2d::EString & pResouceName, const e2d::EString & pResouceType, const e2d::EString & musicExtension, UINT uId)
{
	// 忽略不存在的文件
	if (pResouceName.empty() || pResouceType.empty() || musicExtension.empty()) return false;

	// 获取临时文件目录
	e2d::EString tempFileName = e2d::EFileUtils::getTempPath();

	// 产生临时文件的文件名
	tempFileName.append(L"\\");
	tempFileName.append(std::to_wstring(uId));
	tempFileName.append(L"." + musicExtension);

	// 导出资源为临时文件
	if (ExtractResource(tempFileName.c_str(), pResouceType.c_str(), pResouceName.c_str()))
	{
		if (open(tempFileName, uId))
		{
			m_sTempFileName = tempFileName;
			return true;
		}
	}
	return false;
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
	s_mciError = mciSendCommand(
		m_dev, 
		MCI_PLAY, 
		MCI_FROM | MCI_NOTIFY | (bLoop ? MCI_DGV_PLAY_REPEAT : 0),
		reinterpret_cast<DWORD_PTR>(&mciPlay)
	);
	
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
	// 删除临时文件
	if (!m_sTempFileName.empty())
	{
		DeleteFile(m_sTempFileName.c_str());
		m_sTempFileName.clear();
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
	// 继续播放音乐
	_sendCommand(MCI_RESUME);
	m_bPlaying = true;
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
	mciSendCommand(
		m_dev,
		MCI_SEEK, 
		MCI_SEEK_TO_START | MCI_NOTIFY, 
		0
	);
	// 播放音乐
	MCI_PLAY_PARMS mciPlay = { 0 };
	MCIERROR s_mciError;

	// 播放声音
	s_mciError = mciSendCommand(
		m_dev, 
		MCI_PLAY,
		MCI_NOTIFY | (m_bLoop ? MCI_DGV_PLAY_REPEAT : 0),
		reinterpret_cast<DWORD_PTR>(&mciPlay)
	);
	m_bPlaying = s_mciError ? false : true;
}

void MciPlayer::setVolume(float volume)
{
	MCI_DGV_SETAUDIO_PARMS  mciSetAudioPara = { 0 };
	mciSetAudioPara.dwItem = MCI_DGV_SETAUDIO_VOLUME;
	mciSetAudioPara.dwValue = DWORD(1000 * min(max(volume, 0), 1));
	mciSendCommand(
		m_dev, 
		MCI_SETAUDIO, 
		MCI_NOTIFY | MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM,
		(DWORD_PTR)&mciSetAudioPara
	);
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
// e2d::EMusicUtils
////////////////////////////////////////////////////////////////////


typedef std::map<unsigned int, MciPlayer *> MusicList;
typedef std::pair<unsigned int, MciPlayer *> Music;


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

void e2d::EMusicUtils::end()
{
	// 停止背景音乐
	getBgMciPlayer().close();
	// 停止其他所有音乐
	for (auto& iter : getMciPlayerList())
	{
		SafeDelete(&iter.second);
	}
	// 清空音乐列表
	getMciPlayerList().clear();
	return;
}

void e2d::EMusicUtils::setVolume(float volume)
{
	// 设置背景音乐音量
	getBgMciPlayer().setVolume(volume);
	// 设置其他音乐音量
	for (auto& iter : getMciPlayerList())
	{
		iter.second->setVolume(volume);
	}
}

void e2d::EMusicUtils::setVolume(const EString & musicFilePath, float volume)
{
	unsigned int nRet = ::Hash(musicFilePath);

	MusicList::iterator p = getMciPlayerList().find(nRet);
	if (p != getMciPlayerList().end())
	{
		p->second->setVolume(volume);
	}
}

void e2d::EMusicUtils::playBackgroundMusic(const EString & musicFilePath, bool bLoop)
{
	if (musicFilePath.empty())
	{
		return;
	}

	getBgMciPlayer().open(musicFilePath, ::Hash(musicFilePath));
	getBgMciPlayer().play(bLoop);
}

void e2d::EMusicUtils::playBackgroundMusic(const EString & musicResourceName, const EString & musicResourceType, const EString & musicExtension, bool loop)
{
	if (musicResourceName.empty() || musicResourceType.empty())
	{
		return;
	}

	getBgMciPlayer().open(musicResourceName, musicResourceType, musicExtension, ::Hash(musicResourceName));
	getBgMciPlayer().play(loop);
}

void e2d::EMusicUtils::stopBackgroundMusic(bool bReleaseData)
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

void e2d::EMusicUtils::pauseBackgroundMusic()
{
	getBgMciPlayer().pause();
}

void e2d::EMusicUtils::resumeBackgroundMusic()
{
	getBgMciPlayer().resume();
}

void e2d::EMusicUtils::rewindBackgroundMusic()
{
	getBgMciPlayer().rewind();
}

bool e2d::EMusicUtils::isBackgroundMusicPlaying()
{
	return getBgMciPlayer().isPlaying();
}

void e2d::EMusicUtils::setBackgroundMusicVolume(float volume)
{
	getBgMciPlayer().setVolume(volume);
}

void e2d::EMusicUtils::playMusic(const EString & musicFilePath, bool bLoop)
{
	unsigned int nRet = ::Hash(musicFilePath);

	preloadMusic(musicFilePath);

	MusicList::iterator p = getMciPlayerList().find(nRet);
	if (p != getMciPlayerList().end())
	{
		p->second->play(bLoop);
	}
}

void e2d::EMusicUtils::playMusic(const EString & musicResourceName, const EString & musicResourceType, const EString & musicExtension, bool loop)
{
	unsigned int nRet = ::Hash(musicResourceName);

	preloadMusic(musicResourceName, musicResourceType, musicExtension);

	MusicList::iterator p = getMciPlayerList().find(nRet);
	if (p != getMciPlayerList().end())
	{
		p->second->play(loop);
	}
}

void e2d::EMusicUtils::_stopMusic(size_t nSoundId)
{
	MusicList::iterator p = getMciPlayerList().find(nSoundId);
	if (p != getMciPlayerList().end())
	{
		p->second->stop();
	}
}

void e2d::EMusicUtils::preloadMusic(const EString & musicFilePath)
{
	if (musicFilePath.empty()) return;

	int nRet = ::Hash(musicFilePath);

	if (getMciPlayerList().end() != getMciPlayerList().find(nRet)) return;

	getMciPlayerList().insert(Music(nRet, new MciPlayer()));
	MciPlayer * pPlayer = getMciPlayerList()[nRet];
	pPlayer->open(musicFilePath, nRet);

	if (nRet == pPlayer->getSoundID()) return;

	delete pPlayer;
	getMciPlayerList().erase(nRet);
	nRet = 0;
}

void e2d::EMusicUtils::preloadMusic(const EString & musicResourceName, const EString & musicResourceType, const EString & musicExtension)
{
	if (musicResourceName.empty() || musicResourceType.empty()) return;

	int nRet = ::Hash(musicResourceName);

	if (getMciPlayerList().end() != getMciPlayerList().find(nRet)) return;

	getMciPlayerList().insert(Music(nRet, new MciPlayer()));
	MciPlayer * pPlayer = getMciPlayerList()[nRet];
	pPlayer->open(musicResourceName, musicResourceType, musicExtension, nRet);

	if (nRet == pPlayer->getSoundID()) return;

	delete pPlayer;
	getMciPlayerList().erase(nRet);
	nRet = 0;
}

void e2d::EMusicUtils::_pauseMusic(size_t nSoundId)
{
	MusicList::iterator p = getMciPlayerList().find(nSoundId);
	if (p != getMciPlayerList().end())
	{
		p->second->pause();
	}
}

void e2d::EMusicUtils::pauseAllMusics()
{
	for (auto& iter : getMciPlayerList())
	{
		iter.second->pause();
	}
}

void e2d::EMusicUtils::_resumeMusic(size_t nSoundId)
{
	MusicList::iterator p = getMciPlayerList().find(nSoundId);
	if (p != getMciPlayerList().end())
	{
		p->second->resume();
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

void e2d::EMusicUtils::unloadMusic(const EString & musicFilePath)
{
	unsigned int nID = ::Hash(musicFilePath);

	MusicList::iterator p = getMciPlayerList().find(nID);
	if (p != getMciPlayerList().end())
	{
		SafeDelete(&p->second);
		getMciPlayerList().erase(nID);
	}
}




size_t Hash(const e2d::EString & key)
{
	static std::hash<e2d::EString> h;
	return h(key);
}

bool ExtractResource(LPCTSTR strDstFile, LPCTSTR strResType, LPCTSTR strResName)
{
	// 创建文件
	HANDLE hFile = ::CreateFile(strDstFile, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	// 查找资源文件中、加载资源到内存、得到资源大小
	HRSRC	hRes = ::FindResource(NULL, strResName, strResType);
	HGLOBAL	hMem = ::LoadResource(NULL, hRes);
	DWORD	dwSize = ::SizeofResource(NULL, hRes);

	// 写入文件
	DWORD dwWrite = 0;  	// 返回写入字节
	::WriteFile(hFile, hMem, dwSize, &dwWrite, NULL);
	::CloseHandle(hFile);

	return true;
}