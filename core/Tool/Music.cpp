#include "..\e2dtool.h"
#include "..\e2dmanager.h"

using namespace e2d;

#if HIGHER_THAN_VS2010

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=nullptr; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=nullptr; } }
#endif

inline bool TraceError(wchar_t* sPrompt)
{
	WARN_IF(true, "Music error: %s failed!", sPrompt);
	return false;
}

inline bool TraceError(wchar_t* sPrompt, HRESULT hr)
{
	WARN_IF(true, "Music error: %s (%#X)", sPrompt, hr);
	return false;
}

static IXAudio2 * s_pXAudio2 = nullptr;
static IXAudio2MasteringVoice * s_pMasteringVoice = nullptr;


bool e2d::Music::__init()
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

void e2d::Music::__uninit()
{
	if (s_pMasteringVoice)
	{
		s_pMasteringVoice->DestroyVoice();
	}

	SafeReleaseInterface(&s_pXAudio2);
}

Music::Music()
	: m_bOpened(false)
	, m_bPlaying(false)
	, m_pwfx(nullptr)
	, m_hmmio(nullptr)
	, m_pResourceBuffer(nullptr)
	, m_pbWaveData(nullptr)
	, m_dwSize(0)
	, m_pSourceVoice(nullptr)
{
}

e2d::Music::Music(String strFileName)
	: m_bOpened(false)
	, m_bPlaying(false)
	, m_pwfx(nullptr)
	, m_hmmio(nullptr)
	, m_pResourceBuffer(nullptr)
	, m_pbWaveData(nullptr)
	, m_dwSize(0)
	, m_pSourceVoice(nullptr)
{
	this->open(strFileName);
}

Music::~Music()
{
	close();
}

bool Music::open(String strFileName)
{
	if (m_bOpened)
	{
		WARN_IF(true, "Music can be opened only once!");
		return false;
	}

	if (strFileName.isEmpty())
	{
		WARN_IF(true, "Music::open Invalid file name.");
		return false;
	}

	if (!s_pXAudio2)
	{
		WARN_IF(true, "IXAudio2 nullptr pointer error!");
		return false;
	}

	// 定位 wave 文件
	wchar_t strFilePath[MAX_PATH];
	if (!_findMediaFileCch(strFilePath, MAX_PATH, strFileName))
	{
		WARN_IF(true, "Failed to find media file: %s", (const wchar_t*)strFileName);
		return false;
	}

	m_hmmio = mmioOpen(strFilePath, nullptr, MMIO_ALLOCBUF | MMIO_READ);

	if (nullptr == m_hmmio)
	{
		return TraceError(L"mmioOpen");
	}

	if (!_readMMIO())
	{
		// 读取非 wave 文件时 ReadMMIO 调用失败
		mmioClose(m_hmmio, 0);
		return TraceError(L"_readMMIO");
	}

	if (!_resetFile())
		return TraceError(L"_resetFile");

	// 重置文件后，wave 文件的大小是 m_ck.cksize
	m_dwSize = m_ck.cksize;

	// 将样本数据读取到内存中
	m_pbWaveData = new BYTE[m_dwSize];

	if (!_read(m_pbWaveData, m_dwSize))
	{
		TraceError(L"Failed to read WAV data");
		SAFE_DELETE_ARRAY(m_pbWaveData);
		return false;
	}

	// 创建音源
	HRESULT hr;
	if (FAILED(hr = s_pXAudio2->CreateSourceVoice(&m_pSourceVoice, m_pwfx)))
	{
		TraceError(L"Create source voice error", hr);
		SAFE_DELETE_ARRAY(m_pbWaveData);
		return false;
	}

	m_bOpened = true;
	m_bPlaying = false;
	return true;
}

bool Music::play(int nLoopCount)
{
	if (!m_bOpened)
	{
		WARN_IF(true, "Music::play Failed: Music must be opened first!");
		return false;
	}

	if (m_pSourceVoice == nullptr)
	{
		WARN_IF(true, "Music::play Failed: IXAudio2SourceVoice Null pointer exception!");
		return false;
	}

	if (m_bPlaying)
	{
		stop();
	}

	nLoopCount = min(nLoopCount, XAUDIO2_LOOP_INFINITE - 1);
	nLoopCount = (nLoopCount < 0) ? XAUDIO2_LOOP_INFINITE : nLoopCount;

	// 提交 wave 样本数据
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.pAudioData = m_pbWaveData;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = m_dwSize;
	buffer.LoopCount = nLoopCount;

	HRESULT hr;
	if (FAILED(hr = m_pSourceVoice->SubmitSourceBuffer(&buffer)))
	{
		TraceError(L"Submitting source buffer error", hr);
		m_pSourceVoice->DestroyVoice();
		SAFE_DELETE_ARRAY(m_pbWaveData);
		return false;
	}

	if (SUCCEEDED(hr = m_pSourceVoice->Start(0)))
	{
		m_bPlaying = true;
	}
	
	return SUCCEEDED(hr);
}

void Music::pause()
{
	if (m_pSourceVoice)
	{
		if (SUCCEEDED(m_pSourceVoice->Stop()))
		{
			m_bPlaying = false;
		}
	}
}

void Music::resume()
{
	if (m_pSourceVoice)
	{
		if (SUCCEEDED(m_pSourceVoice->Start()))
		{
			m_bPlaying = true;
		}
	}
}

void Music::stop()
{
	if (m_pSourceVoice)
	{
		if (SUCCEEDED(m_pSourceVoice->Stop()))
		{
			m_pSourceVoice->ExitLoop();
			m_pSourceVoice->FlushSourceBuffers();
			m_bPlaying = false;
		}
	}
}

void Music::close()
{
	if (m_pSourceVoice)
	{
		m_pSourceVoice->Stop();
		m_pSourceVoice->FlushSourceBuffers();
		m_pSourceVoice->DestroyVoice();
		m_pSourceVoice = nullptr;
	}

	if (m_hmmio != nullptr)
	{
		mmioClose(m_hmmio, 0);
		m_hmmio = nullptr;
	}

	SAFE_DELETE_ARRAY(m_pResourceBuffer);
	SAFE_DELETE_ARRAY(m_pbWaveData);
	SAFE_DELETE_ARRAY(m_pwfx);

	m_bOpened = false;
	m_bPlaying = false;
}

bool Music::isPlaying() const
{
	if (m_bOpened && m_pSourceVoice)
	{
		XAUDIO2_VOICE_STATE state;
		m_pSourceVoice->GetState(&state);

		if (state.BuffersQueued == 0)
		{
			m_bPlaying = false;
		}
		return m_bPlaying;
	}
	else
	{
		return false;
	}
}

double Music::getVolume() const
{
	float fVolume = 0.0f;
	if (m_pSourceVoice)
	{
		m_pSourceVoice->GetVolume(&fVolume);
	}
	return static_cast<double>(fVolume);
}

bool Music::setVolume(double fVolume)
{
	if (m_pSourceVoice)
	{
		return SUCCEEDED(m_pSourceVoice->SetVolume(min(max(static_cast<float>(fVolume), -224), 224)));
	}
	return false;
}

double Music::getFrequencyRatio() const
{
	float fFrequencyRatio = 0.0f;
	if (m_pSourceVoice)
	{
		m_pSourceVoice->GetFrequencyRatio(&fFrequencyRatio);
	}
	return static_cast<double>(fFrequencyRatio);
}

bool Music::setFrequencyRatio(double fFrequencyRatio)
{
	if (m_pSourceVoice)
	{
		fFrequencyRatio = min(max(fFrequencyRatio, XAUDIO2_MIN_FREQ_RATIO), XAUDIO2_MAX_FREQ_RATIO);
		return SUCCEEDED(m_pSourceVoice->SetFrequencyRatio(static_cast<float>(fFrequencyRatio)));
	}
	return false;
}

IXAudio2 * e2d::Music::getIXAudio2()
{
	return s_pXAudio2;
}

IXAudio2MasteringVoice * e2d::Music::getIXAudio2MasteringVoice()
{
	return s_pMasteringVoice;
}

IXAudio2SourceVoice * Music::getIXAudio2SourceVoice() const
{
	return m_pSourceVoice;
}

bool Music::_readMMIO()
{
	MMCKINFO ckIn;
	PCMWAVEFORMAT pcmWaveFormat;

	memset(&ckIn, 0, sizeof(ckIn));

	m_pwfx = nullptr;

	if ((0 != mmioDescend(m_hmmio, &m_ckRiff, nullptr, 0)))
		return TraceError(L"mmioDescend");

	// 确认文件是一个合法的 wave 文件
	if ((m_ckRiff.ckid != FOURCC_RIFF) ||
		(m_ckRiff.fccType != mmioFOURCC('W', 'A', 'V', 'E')))
		return TraceError(L"mmioFOURCC");

	// 在输入文件中查找 'fmt' 块
	ckIn.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (0 != mmioDescend(m_hmmio, &ckIn, &m_ckRiff, MMIO_FINDCHUNK))
		return TraceError(L"mmioDescend");

	// 'fmt' 块至少应和 PCMWAVEFORMAT 一样大
	if (ckIn.cksize < (LONG)sizeof(PCMWAVEFORMAT))
		return TraceError(L"sizeof(PCMWAVEFORMAT)");

	// 将 'fmt' 块读取到 pcmWaveFormat 中
	if (mmioRead(m_hmmio, (HPSTR)&pcmWaveFormat,
		sizeof(pcmWaveFormat)) != sizeof(pcmWaveFormat))
		return TraceError(L"mmioRead");

	// 分配 WAVEFORMATEX，但如果它不是 PCM 格式，再读取一个 WORD 大小
	// 的数据，这个数据就是额外分配的大小
	if (pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM)
	{
		m_pwfx = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)];

		// 拷贝数据
		memcpy(m_pwfx, &pcmWaveFormat, sizeof(pcmWaveFormat));
		m_pwfx->cbSize = 0;
	}
	else
	{
		// 读取额外数据的大小
		WORD cbExtraBytes = 0L;
		if (mmioRead(m_hmmio, (CHAR*)&cbExtraBytes, sizeof(WORD)) != sizeof(WORD))
			return TraceError(L"mmioRead");

		m_pwfx = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX) + cbExtraBytes];

		// 拷贝数据
		memcpy(m_pwfx, &pcmWaveFormat, sizeof(pcmWaveFormat));
		m_pwfx->cbSize = cbExtraBytes;

		// 读取额外数据
		if (mmioRead(m_hmmio, (CHAR*)(((BYTE*)&(m_pwfx->cbSize)) + sizeof(WORD)),
			cbExtraBytes) != cbExtraBytes)
		{
			SAFE_DELETE(m_pwfx);
			return TraceError(L"mmioRead");
		}
	}

	if (0 != mmioAscend(m_hmmio, &ckIn, 0))
	{
		SAFE_DELETE(m_pwfx);
		return TraceError(L"mmioAscend");
	}

	return true;
}

bool Music::_resetFile()
{
	// Seek to the data
	if (-1 == mmioSeek(m_hmmio, m_ckRiff.dwDataOffset + sizeof(FOURCC),
		SEEK_SET))
		return TraceError(L"mmioSeek");

	// Search the input file for the 'data' chunk.
	m_ck.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (0 != mmioDescend(m_hmmio, &m_ck, &m_ckRiff, MMIO_FINDCHUNK))
		return TraceError(L"mmioDescend");

	return true;
}

bool Music::_read(BYTE* pBuffer, DWORD dwSizeToRead)
{
	MMIOINFO mmioinfoIn; // current status of m_hmmio

	if (0 != mmioGetInfo(m_hmmio, &mmioinfoIn, 0))
		return TraceError(L"mmioGetInfo");

	UINT cbDataIn = dwSizeToRead;
	if (cbDataIn > m_ck.cksize)
		cbDataIn = m_ck.cksize;

	m_ck.cksize -= cbDataIn;

	for (DWORD cT = 0; cT < cbDataIn; cT++)
	{
		// Copy the bytes from the io to the buffer.
		if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead)
		{
			if (0 != mmioAdvance(m_hmmio, &mmioinfoIn, MMIO_READ))
				return TraceError(L"mmioAdvance");

			if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead)
				return TraceError(L"mmioinfoIn.pchNext");
		}

		// Actual copy.
		*((BYTE*)pBuffer + cT) = *((BYTE*)mmioinfoIn.pchNext);
		mmioinfoIn.pchNext++;
	}

	if (0 != mmioSetInfo(m_hmmio, &mmioinfoIn, 0))
		return TraceError(L"mmioSetInfo");

	return true;
}

bool Music::_findMediaFileCch(wchar_t* strDestPath, int cchDest, const wchar_t * strFilename)
{
	bool bFound = false;

	if (nullptr == strFilename || nullptr == strDestPath || cchDest < 10)
		return false;

	// Get the exe name, and exe path
	wchar_t strExePath[MAX_PATH] = { 0 };
	wchar_t strExeName[MAX_PATH] = { 0 };
	wchar_t* strLastSlash = nullptr;
	GetModuleFileName(HINST_THISCOMPONENT, strExePath, MAX_PATH);
	strExePath[MAX_PATH - 1] = 0;
	strLastSlash = wcsrchr(strExePath, TEXT('\\'));
	if (strLastSlash)
	{
		wcscpy_s(strExeName, MAX_PATH, &strLastSlash[1]);

		// Chop the exe name from the exe path
		*strLastSlash = 0;

		// Chop the .exe from the exe name
		strLastSlash = wcsrchr(strExeName, TEXT('.'));
		if (strLastSlash)
			*strLastSlash = 0;
	}

	wcscpy_s(strDestPath, cchDest, strFilename);
	if (GetFileAttributes(strDestPath) != 0xFFFFFFFF)
		return true;

	// Search all parent directories starting at .\ and using strFilename as the leaf name
	wchar_t strLeafName[MAX_PATH] = { 0 };
	wcscpy_s(strLeafName, MAX_PATH, strFilename);

	wchar_t strFullPath[MAX_PATH] = { 0 };
	wchar_t strFullFileName[MAX_PATH] = { 0 };
	wchar_t strSearch[MAX_PATH] = { 0 };
	wchar_t* strFilePart = nullptr;

	GetFullPathName(L".", MAX_PATH, strFullPath, &strFilePart);
	if (strFilePart == nullptr)
		return false;

	while (strFilePart != nullptr && *strFilePart != '\0')
	{
		swprintf_s(strFullFileName, MAX_PATH, L"%s\\%s", strFullPath, strLeafName);
		if (GetFileAttributes(strFullFileName) != 0xFFFFFFFF)
		{
			wcscpy_s(strDestPath, cchDest, strFullFileName);
			bFound = true;
			break;
		}

		swprintf_s(strFullFileName, MAX_PATH, L"%s\\%s\\%s", strFullPath, strExeName, strLeafName);
		if (GetFileAttributes(strFullFileName) != 0xFFFFFFFF)
		{
			wcscpy_s(strDestPath, cchDest, strFullFileName);
			bFound = true;
			break;
		}

		swprintf_s(strSearch, MAX_PATH, L"%s\\..", strFullPath);
		GetFullPathName(strSearch, MAX_PATH, strFullPath, &strFilePart);
	}
	if (bFound)
		return true;

	// 失败时，将文件作为路径返回，同时也返回错误代码
	wcscpy_s(strDestPath, cchDest, strFilename);

	return false;
}


#else

#define MUSIC_CLASS_NAME L"Easy2DMusicCallbackWnd"

static HINSTANCE s_hInstance = nullptr;


bool e2d::Music::__init()
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

e2d::Music::Music()
	: m_wnd(NULL)
	, m_dev(0L)
	, m_nMusicID(0)
	, m_bPlaying(false)
	, m_nRepeatTimes(0)
{
	m_wnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		MUSIC_CLASS_NAME,
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

e2d::Music::~Music()
{
	close();
	DestroyWindow(m_wnd);
}

bool e2d::Music::open(String pFileName)
{
	if (pFileName.isEmpty())
		return false;

	close();

	MCI_OPEN_PARMS mciOpen = { 0 };
	mciOpen.lpstrDeviceType = 0;
	mciOpen.lpstrElementName = pFileName;

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
		m_nMusicID = pFileName.getHashCode();
		m_bPlaying = false;
		return true;
	}
	return false;
}

bool e2d::Music::play(int nLoopCount)
{
	if (!m_dev)
	{
		return false;
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
		m_nRepeatTimes = nLoopCount;
		return true;
	}
	return false;
}

void e2d::Music::close()
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

void e2d::Music::pause()
{
	_sendCommand(MCI_PAUSE);
	m_bPlaying = false;
}

void e2d::Music::resume()
{
	_sendCommand(MCI_RESUME);
	m_bPlaying = true;
}

void e2d::Music::stop()
{
	_sendCommand(MCI_STOP);
	m_bPlaying = false;
}

bool e2d::Music::isPlaying() const
{
	return m_bPlaying;
}

double Music::getVolume() const
{
	return 1.0f;
}

bool Music::setVolume(double fVolume)
{
	return false;
}

double Music::getFrequencyRatio() const
{
	return 1.0f;
}

bool Music::setFrequencyRatio(double fFrequencyRatio)
{
	return false;
}

void e2d::Music::_sendCommand(int nCommand, DWORD_PTR param1, DWORD_PTR parma2)
{
	// 空设备时忽略这次操作
	if (!m_dev)
	{
		return;
	}
	// 向当前设备发送操作
	mciSendCommand(m_dev, nCommand, param1, parma2);
}


LRESULT WINAPI e2d::Music::MusicProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	e2d::Music * pPlayer = NULL;

	if (Msg == MM_MCINOTIFY
		&& wParam == MCI_NOTIFY_SUCCESSFUL
		&& (pPlayer = (Music *)GetWindowLongPtr(hWnd, GWLP_USERDATA)))
	{
		if (pPlayer->m_nRepeatTimes > 0)
		{
			pPlayer->m_nRepeatTimes--;
		}

		if (pPlayer->m_nRepeatTimes)
		{
			mciSendCommand(static_cast<MCIDEVICEID>(lParam), MCI_SEEK, MCI_SEEK_TO_START, 0);

			MCI_PLAY_PARMS mciPlay = { 0 };
			mciPlay.dwCallback = reinterpret_cast<DWORD_PTR>(hWnd);
			mciSendCommand(static_cast<MCIDEVICEID>(lParam), MCI_PLAY, MCI_NOTIFY, reinterpret_cast<DWORD_PTR>(&mciPlay));
		}
		else
		{
			pPlayer->m_bPlaying = false;
			return 0;
		}
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

#endif