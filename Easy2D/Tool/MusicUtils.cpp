#include "..\Easy2d.h"


/* 注：MusicUtils 类完全仿照 Cocos2dx 中的 SimpleAudioEngine 实现 */

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

	void Close();
	void Open(tstring pFileName, UINT uId);
	void Play(UINT uTimes = 1);
	void Pause();
	void Resume();
	void Stop();
	void Rewind();
	bool IsPlaying();
	UINT GetSoundID();

private:
	friend LRESULT WINAPI _SoundPlayProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	void _SendGenericCommand(int nCommand, DWORD_PTR param1 = 0, DWORD_PTR parma2 = 0);

	HWND        _wnd;
	MCIDEVICEID _dev;
	UINT        _soundID;
	UINT        _times;
	bool        _playing;
	tstring		strExt;
};



#define WIN_CLASS_NAME		"Easy2dCallbackWnd"

static HINSTANCE s_hInstance;
static MCIERROR  s_mciError;

LRESULT WINAPI _SoundPlayProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

MciPlayer::MciPlayer()
	: _wnd(NULL)
	, _dev(0L)
	, _soundID(0)
	, _times(0)
	, _playing(false)
	, strExt(_T(""))
{
	if (!s_hInstance)
	{
		s_hInstance = GetModuleHandle(NULL);		// Grab An Instance For Our Window

		WNDCLASS  wc;								// Windows Class Structure

													// Redraw On Size, And Own DC For Window.
		wc.style = 0;
		wc.lpfnWndProc = _SoundPlayProc;			// WndProc Handles Messages
		wc.cbClsExtra = 0;							// No Extra Window Data
		wc.cbWndExtra = 0;							// No Extra Window Data
		wc.hInstance = s_hInstance;					// Set The Instance
		wc.hIcon = 0;								// Load The Default Icon
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);	// Load The Arrow Pointer
		wc.hbrBackground = NULL;					// No Background Required For GL
		wc.lpszMenuName = NULL;						// We Don't Want A Menu
		wc.lpszClassName = _T(WIN_CLASS_NAME);		// Set The Class Name

		if (!RegisterClass(&wc)						// Register Our Class
			&& GetLastError() != 1410)				// Class is Already Existent
		{
			return;
		}
	}

	_wnd = CreateWindowEx(
		WS_EX_APPWINDOW,							// Extended Style For The Window
		_T(WIN_CLASS_NAME),							// Class Name
		NULL,										// Window Title
		WS_POPUPWINDOW,								// Defined Window Style
		0, 0,										// Window Position
		0, 0,										// Window Width And Height
		NULL,										// No Parent Window
		NULL,										// No Menu
		s_hInstance,								// Instance
		NULL);										// No Param

	if (_wnd)
	{
		SetWindowLongPtr(_wnd, GWLP_USERDATA, (LONG_PTR)this);
	}
}

MciPlayer::~MciPlayer()
{
	Close();
	DestroyWindow(_wnd);
}

void MciPlayer::Open(tstring pFileName, UINT uId)
{
	if (pFileName.empty() || !_wnd) return;
	int nLen = (int)pFileName.size();
	if (!nLen) return;

	strExt = FileUtils::getFileExtension(pFileName);

	Close();

	MCI_OPEN_PARMS mciOpen = { 0 };
	MCIERROR mciError;
	mciOpen.lpstrDeviceType = (LPCTSTR)MCI_ALL_DEVICE_ID;
	mciOpen.lpstrElementName = pFileName.c_str();

	mciError = mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT, reinterpret_cast<DWORD_PTR>(&mciOpen));
	if (mciError) return;

	_dev = mciOpen.wDeviceID;
	_soundID = uId;
	_playing = false;
}

void MciPlayer::Play(UINT uTimes /* = 1 */)
{
	if (!_dev)
	{
		return;
	}
	MCI_PLAY_PARMS mciPlay = { 0 };
	mciPlay.dwCallback = reinterpret_cast<DWORD_PTR>(_wnd);
	s_mciError = mciSendCommand(_dev, MCI_PLAY, MCI_FROM | MCI_NOTIFY, reinterpret_cast<DWORD_PTR>(&mciPlay));
	if (!s_mciError)
	{
		_playing = true;
		_times = uTimes;
	}
}

void MciPlayer::Close()
{
	if (_playing)
	{
		Stop();
	}
	if (_dev)
	{
		_SendGenericCommand(MCI_CLOSE);
	}
	_dev = 0;
	_playing = false;
}

void MciPlayer::Pause()
{
	_SendGenericCommand(MCI_PAUSE);
	_playing = false;
}

void MciPlayer::Resume()
{
	if (strExt == _T(".mid"))
	{
		// midi not support MCI_RESUME, should get the position and use MCI_FROM
		MCI_STATUS_PARMS mciStatusParms;
		MCI_PLAY_PARMS   mciPlayParms;
		mciStatusParms.dwItem = MCI_STATUS_POSITION;
		_SendGenericCommand(MCI_STATUS, MCI_STATUS_ITEM, reinterpret_cast<DWORD_PTR>(&mciStatusParms)); // MCI_STATUS
		mciPlayParms.dwFrom = mciStatusParms.dwReturn;  // get position
		_SendGenericCommand(MCI_PLAY, MCI_FROM, reinterpret_cast<DWORD_PTR>(&mciPlayParms)); // MCI_FROM
	}
	else
	{
		_SendGenericCommand(MCI_RESUME);
		_playing = true;
	}
}

void MciPlayer::Stop()
{
	_SendGenericCommand(MCI_STOP);
	_playing = false;
	_times = 0;
}

void MciPlayer::Rewind()
{
	if (!_dev)
	{
		return;
	}
	mciSendCommand(_dev, MCI_SEEK, MCI_SEEK_TO_START, 0);

	MCI_PLAY_PARMS mciPlay = { 0 };
	mciPlay.dwCallback = reinterpret_cast<DWORD_PTR>(_wnd);
	_playing = mciSendCommand(_dev, MCI_PLAY, MCI_NOTIFY, reinterpret_cast<DWORD_PTR>(&mciPlay)) ? false : true;
}

bool MciPlayer::IsPlaying()
{
	return _playing;
}

UINT MciPlayer::GetSoundID()
{
	return _soundID;
}

void MciPlayer::_SendGenericCommand(int nCommand, DWORD_PTR param1 /*= 0*/, DWORD_PTR parma2 /*= 0*/)
{
	if (!_dev)
	{
		return;
	}
	mciSendCommand(_dev, nCommand, param1, parma2);
}




////////////////////////////////////////////////////////////////////
// MusicUtils
////////////////////////////////////////////////////////////////////


typedef std::map<unsigned int, MciPlayer *> MusicList;
typedef std::pair<unsigned int, MciPlayer *> Music;


static unsigned int _Hash(tstring key);


static MusicList& sharedList()
{
	static MusicList s_List;
	return s_List;
}

static MciPlayer& sharedMusic()
{
	static MciPlayer s_Music;
	return s_Music;
}

void MusicUtils::end()
{
	sharedMusic().Close();

	for (auto& iter : sharedList())
	{
		SAFE_DELETE(iter.second);
	}
	sharedList().clear();
	return;
}

//////////////////////////////////////////////////////////////////////////
// BackgroundMusic
//////////////////////////////////////////////////////////////////////////

void MusicUtils::playBackgroundMusic(tstring pszFilePath, bool bLoop)
{
	if (pszFilePath.empty())
	{
		return;
	}

	sharedMusic().Open(pszFilePath, ::_Hash(pszFilePath));
	sharedMusic().Play((bLoop) ? -1 : 1);
}

void MusicUtils::stopBackgroundMusic(bool bReleaseData)
{
	if (bReleaseData)
	{
		sharedMusic().Close();
	}
	else
	{
		sharedMusic().Stop();
	}
}

void MusicUtils::pauseBackgroundMusic()
{
	sharedMusic().Pause();
}

void MusicUtils::resumeBackgroundMusic()
{
	sharedMusic().Resume();
}

void MusicUtils::rewindBackgroundMusic()
{
	sharedMusic().Rewind();
}

bool MusicUtils::isBackgroundMusicPlaying()
{
	return sharedMusic().IsPlaying();
}

//////////////////////////////////////////////////////////////////////////
// effect function
//////////////////////////////////////////////////////////////////////////

unsigned int MusicUtils::playMusic(tstring pszFilePath, bool bLoop)
{
	unsigned int nRet = ::_Hash(pszFilePath);

	preloadMusic(pszFilePath);

	MusicList::iterator p = sharedList().find(nRet);
	if (p != sharedList().end())
	{
		p->second->Play((bLoop) ? -1 : 1);
	}
	return nRet;
}

void MusicUtils::stopMusic(unsigned int nSoundId)
{
	MusicList::iterator p = sharedList().find(nSoundId);
	if (p != sharedList().end())
	{
		p->second->Stop();
	}
}

void MusicUtils::preloadMusic(tstring pszFilePath)
{
	if (pszFilePath.empty()) return;

	int nRet = ::_Hash(pszFilePath);

	if (sharedList().end() != sharedList().find(nRet)) return;

	sharedList().insert(Music(nRet, new MciPlayer()));
	MciPlayer * pPlayer = sharedList()[nRet];
	pPlayer->Open(pszFilePath, nRet);

	if (nRet == pPlayer->GetSoundID()) return;

	delete pPlayer;
	sharedList().erase(nRet);
	nRet = 0;
}

void MusicUtils::pauseMusic(unsigned int nSoundId)
{
	MusicList::iterator p = sharedList().find(nSoundId);
	if (p != sharedList().end())
	{
		p->second->Pause();
	}
}

void MusicUtils::pauseAllMusics()
{
	for (auto& iter : sharedList())
	{
		iter.second->Pause();
	}
}

void MusicUtils::resumeMusic(unsigned int nSoundId)
{
	MusicList::iterator p = sharedList().find(nSoundId);
	if (p != sharedList().end())
	{
		p->second->Resume();
	}
}

void MusicUtils::resumeAllMusics()
{
	for (auto& iter : sharedList())
	{
		iter.second->Resume();
	}
}

void MusicUtils::stopAllMusics()
{
	for (auto& iter : sharedList())
	{
		iter.second->Stop();
	}
}

void MusicUtils::unloadMusic(LPCTSTR pszFilePath)
{
	unsigned int nID = ::_Hash(pszFilePath);
	
	MusicList::iterator p = sharedList().find(nID);
	if (p != sharedList().end())
	{
		SAFE_DELETE(p->second);
		sharedList().erase(nID);
	}
}



//////////////////////////////////////////////////////////////////////////
// static function
//////////////////////////////////////////////////////////////////////////

LRESULT WINAPI _SoundPlayProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	MciPlayer * pPlayer = NULL;
	if (MM_MCINOTIFY == Msg
		&& MCI_NOTIFY_SUCCESSFUL == wParam
		&& (pPlayer = (MciPlayer *)GetWindowLongPtr(hWnd, GWLP_USERDATA)))
	{
		if (pPlayer->_times)
		{
			--pPlayer->_times;
		}

		if (pPlayer->_times)
		{
			mciSendCommand(lParam, MCI_SEEK, MCI_SEEK_TO_START, 0);

			MCI_PLAY_PARMS mciPlay = { 0 };
			mciPlay.dwCallback = reinterpret_cast<DWORD_PTR>(hWnd);
			mciSendCommand(lParam, MCI_PLAY, MCI_NOTIFY, reinterpret_cast<DWORD_PTR>(&mciPlay));
		}
		else
		{
			pPlayer->_playing = false;
		}
		return 0;
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
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