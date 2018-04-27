#include "..\e2dtool.h"
#include "..\e2dmanager.h"
#include <map>


#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=nullptr; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=nullptr; } }
#endif

inline bool TraceError(wchar_t* sPrompt)
{
	WARN_IF(true, "MusicInfo error: %s failed!", sPrompt);
	return false;
}

inline bool TraceError(wchar_t* sPrompt, HRESULT hr)
{
	WARN_IF(true, "MusicInfo error: %s (%#X)", sPrompt, hr);
	return false;
}

static IXAudio2 * s_pXAudio2 = nullptr;
static IXAudio2MasteringVoice * s_pMasteringVoice = nullptr;
static float s_fMusicVolume = 1.0;


// 音乐播放器
class MusicPlayer
{
public:
	MusicPlayer();

	virtual ~MusicPlayer();

	bool open(
		const e2d::String& strFileName
	);

	bool MusicPlayer::open(
		int resNameId, 
		const e2d::String& resType
	);

	bool play(
		int nLoopCount = 0
	);

	void pause();

	void resume();

	void stop();

	void close();

	bool setVolume(
		float fVolume
	);

	bool isPlaying() const;

protected:
	bool _readMMIO();

	bool _resetFile();

	bool _read(
		BYTE* pBuffer,
		DWORD dwSizeToRead
	);

	bool _findMediaFileCch(
		wchar_t* strDestPath,
		int cchDest,
		const wchar_t * strFilename
	);

protected:
	bool m_bOpened;
	mutable bool m_bPlaying;
	DWORD m_dwSize;
	CHAR* m_pResourceBuffer;
	BYTE* m_pbWaveData;
	HMMIO m_hmmio;
	MMCKINFO m_ck;
	MMCKINFO m_ckRiff;
	WAVEFORMATEX* m_pwfx;
	IXAudio2SourceVoice* m_pSourceVoice;
};

typedef std::map<UINT, MusicPlayer *> MusicMap;

static MusicMap& GetMusicFileList()
{
	static MusicMap s_MusicFileList;
	return s_MusicFileList;
}

static MusicMap& GetMusicResList()
{
	static MusicMap s_MusicResList;
	return s_MusicResList;
}

MusicPlayer::MusicPlayer()
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

MusicPlayer::~MusicPlayer()
{
	close();
}

bool MusicPlayer::open(const e2d::String& strFileName)
{
	if (m_bOpened)
	{
		WARN_IF(true, "MusicInfo can be opened only once!");
		return false;
	}

	if (strFileName.isEmpty())
	{
		WARN_IF(true, "MusicInfo::open Invalid file name.");
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

bool MusicPlayer::open(int resNameId, const e2d::String& resType)
{
	HRSRC hResInfo;
	HGLOBAL hResData;
	DWORD dwSize;
	void* pvRes;

	if (m_bOpened)
	{
		WARN_IF(true, "MusicInfo can be opened only once!");
		return false;
	}

	if (!s_pXAudio2)
	{
		WARN_IF(true, "IXAudio2 nullptr pointer error!");
		return false;
	}

	// Loading it as a file failed, so try it as a resource
	if (nullptr == (hResInfo = FindResourceW(HINST_THISCOMPONENT, MAKEINTRESOURCE(resNameId), resType)))
		return TraceError(L"FindResource");

	if (nullptr == (hResData = LoadResource(HINST_THISCOMPONENT, hResInfo)))
		return TraceError(L"LoadResource");

	if (0 == (dwSize = SizeofResource(HINST_THISCOMPONENT, hResInfo)))
		return TraceError(L"SizeofResource");

	if (nullptr == (pvRes = LockResource(hResData)))
		return TraceError(L"LockResource");

	m_pResourceBuffer = new CHAR[dwSize];
	memcpy(m_pResourceBuffer, pvRes, dwSize);

	MMIOINFO mmioInfo;
	ZeroMemory(&mmioInfo, sizeof(mmioInfo));
	mmioInfo.fccIOProc = FOURCC_MEM;
	mmioInfo.cchBuffer = dwSize;
	mmioInfo.pchBuffer = (CHAR*)m_pResourceBuffer;

	m_hmmio = mmioOpen(nullptr, &mmioInfo, MMIO_ALLOCBUF | MMIO_READ);

	if (!_readMMIO())
	{
		// ReadMMIO will fail if its an not a wave file
		mmioClose(m_hmmio, 0);
		return TraceError(L"ReadMMIO");
	}

	if (!_resetFile())
		return TraceError(L"ResetFile");

	// After the reset, the size of the wav file is m_ck.cksize so store it now
	m_dwSize = m_ck.cksize;

	// Read the sample data into memory
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

bool MusicPlayer::play(int nLoopCount)
{
	if (!m_bOpened)
	{
		WARN_IF(true, "MusicInfo::play Failed: MusicInfo must be opened first!");
		return false;
	}

	if (m_pSourceVoice == nullptr)
	{
		WARN_IF(true, "MusicInfo::play Failed: IXAudio2SourceVoice Null pointer exception!");
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

void MusicPlayer::pause()
{
	if (m_pSourceVoice)
	{
		if (SUCCEEDED(m_pSourceVoice->Stop()))
		{
			m_bPlaying = false;
		}
	}
}

void MusicPlayer::resume()
{
	if (m_pSourceVoice)
	{
		if (SUCCEEDED(m_pSourceVoice->Start()))
		{
			m_bPlaying = true;
		}
	}
}

void MusicPlayer::stop()
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

void MusicPlayer::close()
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

bool MusicPlayer::isPlaying() const
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

bool MusicPlayer::setVolume(float fVolume)
{
	if (m_pSourceVoice)
	{
		return SUCCEEDED(m_pSourceVoice->SetVolume(fVolume));
	}
	return false;
}

bool MusicPlayer::_readMMIO()
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

bool MusicPlayer::_resetFile()
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

bool MusicPlayer::_read(BYTE* pBuffer, DWORD dwSizeToRead)
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

bool MusicPlayer::_findMediaFileCch(wchar_t* strDestPath, int cchDest, const wchar_t * strFilename)
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


bool e2d::Music::preload(String strFilePath)
{
	UINT nRet = strFilePath.getHashCode();

	if (GetMusicFileList().end() != GetMusicFileList().find(nRet))
	{
		return true;
	}
	else
	{
		MusicPlayer * pPlayer = new (std::nothrow) MusicPlayer();

		if (pPlayer->open(strFilePath))
		{
			pPlayer->setVolume(s_fMusicVolume);
			GetMusicFileList().insert(std::pair<UINT, MusicPlayer *>(nRet, pPlayer));
			return true;
		}
		else
		{
			delete pPlayer;
			pPlayer = nullptr;
		}
	}
	return false;
}

bool e2d::Music::preload(int resNameId, String resType)
{
	if (GetMusicResList().end() != GetMusicResList().find(resNameId))
	{
		return true;
	}
	else
	{
		MusicPlayer * pPlayer = new (std::nothrow) MusicPlayer();

		if (pPlayer->open(resNameId, resType))
		{
			pPlayer->setVolume(s_fMusicVolume);
			GetMusicResList().insert(std::pair<UINT, MusicPlayer *>(resNameId, pPlayer));
			return true;
		}
		else
		{
			delete pPlayer;
			pPlayer = nullptr;
		}
	}
	return false;
}

bool e2d::Music::play(String strFilePath, int nLoopCount)
{
	if (Music::preload(strFilePath))
	{
		UINT nRet = strFilePath.getHashCode();
		auto pMusic = GetMusicFileList()[nRet];
		if (pMusic->play(nLoopCount))
		{
			return true;
		}
	}
	return false;
}

bool e2d::Music::play(int resNameId, String resType, int nLoopCount)
{
	if (Music::preload(resNameId, resType))
	{
		auto pMusic = GetMusicResList()[resNameId];
		if (pMusic->play(nLoopCount))
		{
			return true;
		}
	}
	return false;
}

void e2d::Music::pause(String strFilePath)
{
	if (strFilePath.isEmpty())
		return;

	UINT nRet = strFilePath.getHashCode();

	if (GetMusicFileList().end() != GetMusicFileList().find(nRet))
		GetMusicFileList()[nRet]->pause();
}

void e2d::Music::pause(int resNameId, String resType)
{
	if (GetMusicResList().end() != GetMusicResList().find(resNameId))
		GetMusicResList()[resNameId]->pause();
}

void e2d::Music::resume(String strFilePath)
{
	if (strFilePath.isEmpty())
		return;

	UINT nRet = strFilePath.getHashCode();

	if (GetMusicFileList().end() != GetMusicFileList().find(nRet))
		GetMusicFileList()[nRet]->resume();
}

void e2d::Music::resume(int resNameId, String resType)
{
	if (GetMusicResList().end() != GetMusicResList().find(resNameId))
		GetMusicResList()[resNameId]->pause();
}

void e2d::Music::stop(String strFilePath)
{
	if (strFilePath.isEmpty())
		return;

	UINT nRet = strFilePath.getHashCode();

	if (GetMusicFileList().end() != GetMusicFileList().find(nRet))
		GetMusicFileList()[nRet]->stop();
}

void e2d::Music::stop(int resNameId, String resType)
{
	if (GetMusicResList().end() != GetMusicResList().find(resNameId))
		GetMusicResList()[resNameId]->stop();
}

bool e2d::Music::isPlaying(String strFilePath)
{
	if (strFilePath.isEmpty())
		return false;

	UINT nRet = strFilePath.getHashCode();

	if (GetMusicFileList().end() != GetMusicFileList().find(nRet))
		return GetMusicFileList()[nRet]->isPlaying();

	return false;
}

bool e2d::Music::isPlaying(int resNameId, String resType)
{
	if (GetMusicResList().end() != GetMusicResList().find(resNameId))
		return GetMusicResList()[resNameId]->isPlaying();
	return false;
}

double e2d::Music::getVolume()
{
	return s_fMusicVolume;
}

void e2d::Music::setVolume(double fVolume)
{
	s_fMusicVolume = min(max(static_cast<float>(fVolume), -224), 224);
	for (auto pair : GetMusicFileList())
	{
		pair.second->setVolume(s_fMusicVolume);
	}
}

void e2d::Music::pauseAll()
{
	for (auto pair : GetMusicFileList())
	{
		pair.second->pause();
	}
}

void e2d::Music::resumeAll()
{
	for (auto pair : GetMusicFileList())
	{
		pair.second->resume();
	}
}

void e2d::Music::stopAll()
{
	for (auto pair : GetMusicFileList())
	{
		pair.second->stop();
	}
}

IXAudio2 * e2d::Music::getIXAudio2()
{
	return s_pXAudio2;
}

IXAudio2MasteringVoice * e2d::Music::getIXAudio2MasteringVoice()
{
	return s_pMasteringVoice;
}

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
		e2d::SafeReleaseInterface(&s_pXAudio2);
		return false;
	}

	return true;
}

void e2d::Music::__uninit()
{
	for (auto pair : GetMusicFileList())
	{
		pair.second->close();
		delete pair.second;
	}

	GetMusicFileList().clear();

	if (s_pMasteringVoice)
	{
		s_pMasteringVoice->DestroyVoice();
	}

	e2d::SafeReleaseInterface(&s_pXAudio2);
}
