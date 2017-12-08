#include "..\etools.h"
#include <mmsystem.h>
#include "winbase.h"
#include "MciPlayer.h"
#pragma comment(lib , "winmm.lib")

#define WIN_CLASS_NAME L"MciPlayerCallbackWnd"

static HINSTANCE s_hInstance = nullptr;

LRESULT WINAPI _MciPlayerProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static bool ExtractResource(LPCTSTR strDstFile, LPCTSTR strResType, LPCTSTR strResName);


MciPlayer::MciPlayer() 
	: m_wnd(NULL)
	, m_dev(0L)
	, m_nMusicID(0)
	, m_bPlaying(false)
	, m_nRepeatTimes(0)
{
	if (!s_hInstance)
	{
		s_hInstance = HINST_THISCOMPONENT;

		WNDCLASS  wc;
		wc.style = 0;
		wc.lpfnWndProc = _MciPlayerProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = s_hInstance;
		wc.hIcon = 0;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = WIN_CLASS_NAME;

		if (!RegisterClass(&wc) && 1410 != GetLastError())
		{
			return;
		}
	}

	m_wnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		WIN_CLASS_NAME,
		NULL,
		WS_POPUPWINDOW,
		0, 0, 0, 0,
		NULL,
		NULL,
		s_hInstance,
		NULL);

	if (m_wnd)
	{
		SetWindowLongPtr(m_wnd, GWLP_USERDATA, (LONG_PTR)this);
	}
}

MciPlayer::~MciPlayer()
{
	close();
	DestroyWindow(m_wnd);
}

bool MciPlayer::open(const e2d::EString & pFileName, UINT uId)
{
	if (pFileName.empty())
		return false;

	close();

	MCI_OPEN_PARMS mciOpen = { 0 };
	mciOpen.lpstrDeviceType = (LPCTSTR)MCI_ALL_DEVICE_ID;
	mciOpen.lpstrElementName = pFileName.c_str();

	MCIERROR mciError;
	mciError = mciSendCommand(
		0,
		MCI_OPEN,
		MCI_OPEN_ELEMENT,
		reinterpret_cast<DWORD_PTR>(&mciOpen)
	);

	if (mciError == 0)
	{
		m_dev = mciOpen.wDeviceID;
		m_nMusicID = uId;
		m_bPlaying = false;
		return true;
	}
	return false;
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
		return open(tempFileName, uId);
	}
	return false;
}

void MciPlayer::play(int repeatTimes)
{
	if (!m_dev)
	{
		return;
	}

	MCI_PLAY_PARMS mciPlay = { 0 };
	mciPlay.dwCallback = reinterpret_cast<DWORD_PTR>(m_wnd);

	// 播放声音
	MCIERROR mciError = mciSendCommand(
		m_dev,
		MCI_PLAY,
		MCI_FROM | MCI_NOTIFY,
		reinterpret_cast<DWORD_PTR>(&mciPlay)
	);

	if (!mciError)
	{
		m_bPlaying = true;
		m_nRepeatTimes = repeatTimes;
	}
}

void MciPlayer::close()
{
	if (m_bPlaying)
	{
		stop();
	}

	if (m_dev)
	{
		_sendCommand(MCI_CLOSE);
	}

	m_dev = 0;
	m_bPlaying = false;
}

void MciPlayer::pause()
{
	_sendCommand(MCI_PAUSE);
	m_bPlaying = false;
}

void MciPlayer::resume()
{
	_sendCommand(MCI_RESUME);
	m_bPlaying = true;
}

void MciPlayer::stop()
{
	_sendCommand(MCI_STOP);
	m_bPlaying = false;
}

void MciPlayer::rewind()
{
	if (!m_dev)
	{
		return;
	}

	mciSendCommand(m_dev, MCI_SEEK, MCI_SEEK_TO_START, 0);

	MCI_PLAY_PARMS mciPlay = { 0 };
	mciPlay.dwCallback = reinterpret_cast<DWORD_PTR>(m_wnd);

	MCIERROR mciError = mciSendCommand(
		m_dev,
		MCI_PLAY,
		MCI_NOTIFY,
		reinterpret_cast<DWORD_PTR>(&mciPlay)
	);
	m_bPlaying = mciError ? false : true;
}

bool MciPlayer::isPlaying()
{
	return m_bPlaying;
}

UINT MciPlayer::getMusicID()
{
	return m_nMusicID;
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

LRESULT WINAPI _MciPlayerProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	MciPlayer * pPlayer = NULL;

	if (Msg == MM_MCINOTIFY
		&& wParam == MCI_NOTIFY_SUCCESSFUL
		&& (pPlayer = (MciPlayer *)GetWindowLongPtr(hWnd, GWLP_USERDATA)))
	{
		if (pPlayer->m_nRepeatTimes > 0)
		{
			pPlayer->m_nRepeatTimes--;
		}

		if (pPlayer->m_nRepeatTimes)
		{
			mciSendCommand(lParam, MCI_SEEK, MCI_SEEK_TO_START, 0);

			MCI_PLAY_PARMS mciPlay = { 0 };
			mciPlay.dwCallback = reinterpret_cast<DWORD_PTR>(hWnd);
			mciSendCommand(lParam, MCI_PLAY, MCI_NOTIFY, reinterpret_cast<DWORD_PTR>(&mciPlay));
		}
		else
		{
			pPlayer->m_bPlaying = false;
			return 0;
		}
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}