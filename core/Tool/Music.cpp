#include "..\e2dtool.h"


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


e2d::Music::Music()
	: _opened(false)
	, _playing(false)
	, _wfx(nullptr)
	, _hmmio(nullptr)
	, _resBuffer(nullptr)
	, _waveData(nullptr)
	, _dwSize(0)
	, _voice(nullptr)
{
}

e2d::Music::Music(const e2d::String & filePath)
	: _opened(false)
	, _playing(false)
	, _wfx(nullptr)
	, _hmmio(nullptr)
	, _resBuffer(nullptr)
	, _waveData(nullptr)
	, _dwSize(0)
	, _voice(nullptr)
{
	this->open(filePath);
}

e2d::Music::Music(int resNameId, const String & resType)
	: _opened(false)
	, _playing(false)
	, _wfx(nullptr)
	, _hmmio(nullptr)
	, _resBuffer(nullptr)
	, _waveData(nullptr)
	, _dwSize(0)
	, _voice(nullptr)
{
	this->open(resNameId, resType);
}

e2d::Music::~Music()
{
	close();
}

bool e2d::Music::open(const e2d::String& filePath)
{
	if (_opened)
	{
		WARN_IF(true, "MusicInfo can be opened only once!");
		return false;
	}

	if (filePath.isEmpty())
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
	wchar_t pFilePath[MAX_PATH];
	if (!_findMediaFileCch(pFilePath, MAX_PATH, filePath))
	{
		WARN_IF(true, "Failed to find media file: %s", pFilePath);
		return false;
	}

	_hmmio = mmioOpen(pFilePath, nullptr, MMIO_ALLOCBUF | MMIO_READ);

	if (nullptr == _hmmio)
	{
		return TraceError(L"mmioOpen");
	}

	if (!_readMMIO())
	{
		// 读取非 wave 文件时 ReadMMIO 调用失败
		mmioClose(_hmmio, 0);
		return TraceError(L"_readMMIO");
	}

	if (!_resetFile())
		return TraceError(L"_resetFile");

	// 重置文件后，wave 文件的大小是 _ck.cksize
	_dwSize = _ck.cksize;

	// 将样本数据读取到内存中
	_waveData = new (std::nothrow) BYTE[_dwSize];

	if (!_read(_waveData, _dwSize))
	{
		TraceError(L"Failed to read WAV data");
		SAFE_DELETE_ARRAY(_waveData);
		return false;
	}

	// 创建音源
	HRESULT hr;
	if (FAILED(hr = s_pXAudio2->CreateSourceVoice(&_voice, _wfx)))
	{
		TraceError(L"Create source voice error", hr);
		SAFE_DELETE_ARRAY(_waveData);
		return false;
	}

	_opened = true;
	_playing = false;
	return true;
}

bool e2d::Music::open(int resNameId, const e2d::String& resType)
{
	HRSRC hResInfo;
	HGLOBAL hResData;
	DWORD dwSize;
	void* pvRes;

	if (_opened)
	{
		WARN_IF(true, "MusicInfo can be opened only once!");
		return false;
	}

	if (!s_pXAudio2)
	{
		WARN_IF(true, "IXAudio2 nullptr pointer error!");
		return false;
	}

	if (nullptr == (hResInfo = FindResourceW(HINST_THISCOMPONENT, MAKEINTRESOURCE(resNameId), resType)))
		return TraceError(L"FindResource");

	if (nullptr == (hResData = LoadResource(HINST_THISCOMPONENT, hResInfo)))
		return TraceError(L"LoadResource");

	if (0 == (dwSize = SizeofResource(HINST_THISCOMPONENT, hResInfo)))
		return TraceError(L"SizeofResource");

	if (nullptr == (pvRes = LockResource(hResData)))
		return TraceError(L"LockResource");

	_resBuffer = new (std::nothrow) CHAR[dwSize];
	memcpy(_resBuffer, pvRes, dwSize);

	MMIOINFO mmioInfo;
	ZeroMemory(&mmioInfo, sizeof(mmioInfo));
	mmioInfo.fccIOProc = FOURCC_MEM;
	mmioInfo.cchBuffer = dwSize;
	mmioInfo.pchBuffer = (CHAR*)_resBuffer;

	_hmmio = mmioOpen(nullptr, &mmioInfo, MMIO_ALLOCBUF | MMIO_READ);

	if (!_readMMIO())
	{
		// 读取非 wave 文件时 ReadMMIO 调用失败
		mmioClose(_hmmio, 0);
		return TraceError(L"ReadMMIO");
	}

	if (!_resetFile())
		return TraceError(L"ResetFile");

	// 重置文件后，wave 文件的大小是 _ck.cksize
	_dwSize = _ck.cksize;

	// 将样本数据读取到内存中
	_waveData = new (std::nothrow) BYTE[_dwSize];

	if (!_read(_waveData, _dwSize))
	{
		TraceError(L"Failed to read WAV data");
		SAFE_DELETE_ARRAY(_waveData);
		return false;
	}

	// 创建音源
	HRESULT hr;
	if (FAILED(hr = s_pXAudio2->CreateSourceVoice(&_voice, _wfx)))
	{
		TraceError(L"Create source voice error", hr);
		SAFE_DELETE_ARRAY(_waveData);
		return false;
	}

	_opened = true;
	_playing = false;
	return true;
}

bool e2d::Music::play(int nLoopCount)
{
	if (!_opened)
	{
		WARN_IF(true, "MusicInfo::play Failed: MusicInfo must be opened first!");
		return false;
	}

	if (_voice == nullptr)
	{
		WARN_IF(true, "MusicInfo::play Failed: IXAudio2SourceVoice Null pointer exception!");
		return false;
	}

	if (_playing)
	{
		stop();
	}

	nLoopCount = min(nLoopCount, XAUDIO2_LOOP_INFINITE - 1);
	nLoopCount = (nLoopCount < 0) ? XAUDIO2_LOOP_INFINITE : nLoopCount;

	// 提交 wave 样本数据
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.pAudioData = _waveData;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = _dwSize;
	buffer.LoopCount = nLoopCount;

	HRESULT hr;
	if (FAILED(hr = _voice->SubmitSourceBuffer(&buffer)))
	{
		TraceError(L"Submitting source buffer error", hr);
		_voice->DestroyVoice();
		SAFE_DELETE_ARRAY(_waveData);
		return false;
	}

	if (SUCCEEDED(hr = _voice->Start(0)))
	{
		// 播放成功，保留这个对象
		_playing = true;
	}

	return SUCCEEDED(hr);
}

void e2d::Music::pause()
{
	if (_voice)
	{
		if (SUCCEEDED(_voice->Stop()))
		{
			_playing = false;
		}
	}
}

void e2d::Music::resume()
{
	if (_voice)
	{
		if (SUCCEEDED(_voice->Start()))
		{
			_playing = true;
		}
	}
}

void e2d::Music::stop()
{
	if (_voice)
	{
		if (SUCCEEDED(_voice->Stop()))
		{
			_voice->ExitLoop();
			_voice->FlushSourceBuffers();
			_playing = false;
		}
	}
}

void e2d::Music::close()
{
	if (_voice)
	{
		_voice->Stop();
		_voice->FlushSourceBuffers();
		_voice->DestroyVoice();
		_voice = nullptr;
	}

	if (_hmmio != nullptr)
	{
		mmioClose(_hmmio, 0);
		_hmmio = nullptr;
	}

	SAFE_DELETE_ARRAY(_resBuffer);
	SAFE_DELETE_ARRAY(_waveData);
	SAFE_DELETE_ARRAY(_wfx);

	_opened = false;
	_playing = false;
}

bool e2d::Music::isPlaying() const
{
	if (_opened && _voice)
	{
		XAUDIO2_VOICE_STATE state;
		_voice->GetState(&state);

		if (state.BuffersQueued == 0)
		{
			_playing = false;
		}
		return _playing;
	}
	else
	{
		return false;
	}
}

IXAudio2SourceVoice * e2d::Music::getIXAudio2SourceVoice() const
{
	return _voice;
}

bool e2d::Music::setVolume(double volume)
{
	if (_voice)
	{
		return SUCCEEDED(_voice->SetVolume(float(volume)));
	}
	return false;
}

bool e2d::Music::_readMMIO()
{
	MMCKINFO ckIn;
	PCMWAVEFORMAT pcmWaveFormat;

	memset(&ckIn, 0, sizeof(ckIn));

	_wfx = nullptr;

	if ((0 != mmioDescend(_hmmio, &_ckRiff, nullptr, 0)))
		return TraceError(L"mmioDescend");

	// 确认文件是一个合法的 wave 文件
	if ((_ckRiff.ckid != FOURCC_RIFF) ||
		(_ckRiff.fccType != mmioFOURCC('W', 'A', 'V', 'E')))
		return TraceError(L"mmioFOURCC");

	// 在输入文件中查找 'fmt' 块
	ckIn.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (0 != mmioDescend(_hmmio, &ckIn, &_ckRiff, MMIO_FINDCHUNK))
		return TraceError(L"mmioDescend");

	// 'fmt' 块至少应和 PCMWAVEFORMAT 一样大
	if (ckIn.cksize < (LONG)sizeof(PCMWAVEFORMAT))
		return TraceError(L"sizeof(PCMWAVEFORMAT)");

	// 将 'fmt' 块读取到 pcmWaveFormat 中
	if (mmioRead(_hmmio, (HPSTR)&pcmWaveFormat,
		sizeof(pcmWaveFormat)) != sizeof(pcmWaveFormat))
		return TraceError(L"mmioRead");

	// 分配 WAVEFORMATEX，但如果它不是 PCM 格式，再读取一个 WORD 大小
	// 的数据，这个数据就是额外分配的大小
	if (pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM)
	{
		_wfx = (WAVEFORMATEX*) new (std::nothrow) CHAR[sizeof(WAVEFORMATEX)];

		// 拷贝数据
		memcpy(_wfx, &pcmWaveFormat, sizeof(pcmWaveFormat));
		_wfx->cbSize = 0;
	}
	else
	{
		// 读取额外数据的大小
		WORD cbExtraBytes = 0L;
		if (mmioRead(_hmmio, (CHAR*)&cbExtraBytes, sizeof(WORD)) != sizeof(WORD))
			return TraceError(L"mmioRead");

		_wfx = (WAVEFORMATEX*) new (std::nothrow) CHAR[sizeof(WAVEFORMATEX) + cbExtraBytes];

		// 拷贝数据
		memcpy(_wfx, &pcmWaveFormat, sizeof(pcmWaveFormat));
		_wfx->cbSize = cbExtraBytes;

		// 读取额外数据
		if (mmioRead(_hmmio, (CHAR*)(((BYTE*)&(_wfx->cbSize)) + sizeof(WORD)),
			cbExtraBytes) != cbExtraBytes)
		{
			SAFE_DELETE(_wfx);
			return TraceError(L"mmioRead");
		}
	}

	if (0 != mmioAscend(_hmmio, &ckIn, 0))
	{
		SAFE_DELETE(_wfx);
		return TraceError(L"mmioAscend");
	}

	return true;
}

bool e2d::Music::_resetFile()
{
	// Seek to the data
	if (-1 == mmioSeek(_hmmio, _ckRiff.dwDataOffset + sizeof(FOURCC),
		SEEK_SET))
		return TraceError(L"mmioSeek");

	// Search the input file for the 'data' chunk.
	_ck.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (0 != mmioDescend(_hmmio, &_ck, &_ckRiff, MMIO_FINDCHUNK))
		return TraceError(L"mmioDescend");

	return true;
}

bool e2d::Music::_read(BYTE* pBuffer, DWORD dwSizeToRead)
{
	MMIOINFO mmioinfoIn; // current status of _hmmio

	if (0 != mmioGetInfo(_hmmio, &mmioinfoIn, 0))
		return TraceError(L"mmioGetInfo");

	UINT cbDataIn = dwSizeToRead;
	if (cbDataIn > _ck.cksize)
		cbDataIn = _ck.cksize;

	_ck.cksize -= cbDataIn;

	for (DWORD cT = 0; cT < cbDataIn; ++cT)
	{
		// Copy the bytes from the io to the buffer.
		if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead)
		{
			if (0 != mmioAdvance(_hmmio, &mmioinfoIn, MMIO_READ))
				return TraceError(L"mmioAdvance");

			if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead)
				return TraceError(L"mmioinfoIn.pchNext");
		}

		// Actual copy.
		*((BYTE*)pBuffer + cT) = *((BYTE*)mmioinfoIn.pchNext);
		++mmioinfoIn.pchNext;
	}

	if (0 != mmioSetInfo(_hmmio, &mmioinfoIn, 0))
		return TraceError(L"mmioSetInfo");

	return true;
}

bool e2d::Music::_findMediaFileCch(wchar_t* strDestPath, int cchDest, const wchar_t * strFilename)
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
		e2d::SafeReleaseInterface(s_pXAudio2);
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

	e2d::SafeReleaseInterface(s_pXAudio2);
}