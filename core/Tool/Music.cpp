#include "..\e2dtool.h"
#include "..\e2dmodule.h"


#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p); (p)=nullptr; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p); (p)=nullptr; } }
#endif

inline bool TraceError(wchar_t* sPrompt)
{
	WARN("MusicInfo error: %s failed!", sPrompt);
	return false;
}

inline bool TraceError(wchar_t* sPrompt, HRESULT hr)
{
	WARN("MusicInfo error: %s (%#X)", sPrompt, hr);
	return false;
}


e2d::Music::Music()
	: opened_(false)
	, wfx_(nullptr)
	, hmmio_(nullptr)
	, buffer_(nullptr)
	, wave_data_(nullptr)
	, size_(0)
	, voice_(nullptr)
	, callback_()
{
}

e2d::Music::Music(const e2d::String & file_path)
	: opened_(false)
	, wfx_(nullptr)
	, hmmio_(nullptr)
	, buffer_(nullptr)
	, wave_data_(nullptr)
	, size_(0)
	, voice_(nullptr)
	, callback_()
{
	this->Open(file_path);
}

e2d::Music::Music(const Resource& res)
	: opened_(false)
	, wfx_(nullptr)
	, hmmio_(nullptr)
	, buffer_(nullptr)
	, wave_data_(nullptr)
	, size_(0)
	, voice_(nullptr)
	, callback_()
{
	this->Open(res);
}

e2d::Music::~Music()
{
	Close();
}

bool e2d::Music::Open(const e2d::String & file_path)
{
	if (opened_)
	{
		Close();
	}

	if (file_path.IsEmpty())
	{
		WARN("MusicInfo::Open Invalid file name.");
		return false;
	}

	String actualFilePath = File(file_path).GetPath();
	if (actualFilePath.IsEmpty())
	{
		WARN("MusicInfo::Open File not found.");
		return false;
	}

	// 定位 wave 文件
	wchar_t pFilePath[MAX_PATH];
	if (!_findMediaFileCch(pFilePath, MAX_PATH, (const wchar_t *)actualFilePath))
	{
		WARN("Failed to Find media file: %s", pFilePath);
		return false;
	}

	hmmio_ = mmioOpen(pFilePath, nullptr, MMIO_ALLOCBUF | MMIO_READ);

	if (nullptr == hmmio_)
	{
		return TraceError(L"mmioOpen");
	}

	if (!_readMMIO())
	{
		// 读取非 wave 文件时 ReadMMIO 调用失败
		mmioClose(hmmio_, 0);
		return TraceError(L"ReadMMIO");
	}

	if (!_resetFile())
		return TraceError(L"ResetFile");

	// 重置文件后，wave 文件的大小是 ck_.cksize
	size_ = ck_.cksize;

	// 将样本数据读取到内存中
	wave_data_ = new BYTE[size_];

	if (!_read(wave_data_, size_))
	{
		TraceError(L"Failed to read WAV data");
		SAFE_DELETE_ARRAY(wave_data_);
		return false;
	}

	// 创建音源
	auto xAudio2 = Audio::GetInstance()->GetXAudio2();
	HRESULT hr = xAudio2->CreateSourceVoice(&voice_, wfx_, 0, XAUDIO2_DEFAULT_FREQ_RATIO, &callback_);

	if (FAILED(hr))
	{
		TraceError(L"Create source voice error", hr);
		SAFE_DELETE_ARRAY(wave_data_);
		return false;
	}

	opened_ = true;
	return true;
}

bool e2d::Music::Open(const Resource& res)
{
	if (opened_)
	{
		Close();
	}

	HRSRC hResInfo;
	HGLOBAL hResData;
	DWORD dwSize;
	void* pvRes;

	if (nullptr == (hResInfo = FindResourceW(HINST_THISCOMPONENT, MAKEINTRESOURCE(res.name), (LPCWSTR)res.type)))
		return TraceError(L"FindResource");

	if (nullptr == (hResData = LoadResource(HINST_THISCOMPONENT, hResInfo)))
		return TraceError(L"LoadResource");

	if (0 == (dwSize = SizeofResource(HINST_THISCOMPONENT, hResInfo)))
		return TraceError(L"SizeofResource");

	if (nullptr == (pvRes = LockResource(hResData)))
		return TraceError(L"LockResource");

	buffer_ = new CHAR[dwSize];
	memcpy(buffer_, pvRes, dwSize);

	MMIOINFO mmioInfo;
	ZeroMemory(&mmioInfo, sizeof(mmioInfo));
	mmioInfo.fccIOProc = FOURCC_MEM;
	mmioInfo.cchBuffer = dwSize;
	mmioInfo.pchBuffer = (CHAR*)buffer_;

	hmmio_ = mmioOpen(nullptr, &mmioInfo, MMIO_ALLOCBUF | MMIO_READ);

	if (nullptr == hmmio_)
	{
		return TraceError(L"mmioOpen");
	}

	if (!_readMMIO())
	{
		// 读取非 wave 文件时 ReadMMIO 调用失败
		mmioClose(hmmio_, 0);
		return TraceError(L"ReadMMIO");
	}

	if (!_resetFile())
		return TraceError(L"ResetFile");

	// 重置文件后，wave 文件的大小是 ck_.cksize
	size_ = ck_.cksize;

	// 将样本数据读取到内存中
	wave_data_ = new BYTE[size_];

	if (!_read(wave_data_, size_))
	{
		TraceError(L"Failed to read WAV data");
		SAFE_DELETE_ARRAY(wave_data_);
		return false;
	}

	// 创建音源
	auto xAudio2 = Audio::GetInstance()->GetXAudio2();
	HRESULT hr = xAudio2->CreateSourceVoice(&voice_, wfx_, 0, XAUDIO2_DEFAULT_FREQ_RATIO, &callback_);

	if (FAILED(hr))
	{
		TraceError(L"Create source voice error", hr);
		SAFE_DELETE_ARRAY(wave_data_);
		return false;
	}

	opened_ = true;
	return true;
}

bool e2d::Music::Play(int loop_count)
{
	if (!opened_)
	{
		WARN("Music::Play Failed: Music must be opened first!");
		return false;
	}

	if (voice_ == nullptr)
	{
		WARN("Music::Play Failed: IXAudio2SourceVoice Null pointer exception!");
		return false;
	}

	XAUDIO2_VOICE_STATE state;
	voice_->GetState(&state);
	if (state.BuffersQueued)
	{
		Stop();
	}

	loop_count = std::min(loop_count, XAUDIO2_LOOP_INFINITE - 1);
	loop_count = (loop_count < 0) ? XAUDIO2_LOOP_INFINITE : loop_count;

	// 提交 wave 样本数据
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.pAudioData = wave_data_;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = size_;
	buffer.LoopCount = loop_count;

	HRESULT hr;
	if (FAILED(hr = voice_->SubmitSourceBuffer(&buffer)))
	{
		TraceError(L"Submitting source buffer error", hr);
		voice_->DestroyVoice();
		SAFE_DELETE_ARRAY(wave_data_);
		return false;
	}

	hr = voice_->Start(0);

	return SUCCEEDED(hr);
}

void e2d::Music::Pause()
{
	if (voice_)
	{
		voice_->Stop();
	}
}

void e2d::Music::Resume()
{
	if (voice_)
	{
		voice_->Start();
	}
}

void e2d::Music::Stop()
{
	if (voice_)
	{
		if (SUCCEEDED(voice_->Stop()))
		{
			voice_->ExitLoop();
			voice_->FlushSourceBuffers();
		}
	}
}

void e2d::Music::Close()
{
	if (voice_)
	{
		voice_->Stop();
		voice_->FlushSourceBuffers();
		voice_->DestroyVoice();
		voice_ = nullptr;
	}

	if (hmmio_ != nullptr)
	{
		mmioClose(hmmio_, 0);
		hmmio_ = nullptr;
	}

	SAFE_DELETE_ARRAY(buffer_);
	SAFE_DELETE_ARRAY(wave_data_);
	SAFE_DELETE_ARRAY(wfx_);

	opened_ = false;
}

bool e2d::Music::IsPlaying() const
{
	if (opened_ && voice_)
	{
		XAUDIO2_VOICE_STATE state;
		voice_->GetState(&state);
		if (state.BuffersQueued)
			return true;
	}
	return false;
}

IXAudio2SourceVoice * e2d::Music::GetSourceVoice() const
{
	return voice_;
}

bool e2d::Music::SetVolume(float volume)
{
	if (voice_)
	{
		return SUCCEEDED(voice_->SetVolume(volume));
	}
	return false;
}

void e2d::Music::SetCallbackOnEnd(const Function & func)
{
	callback_.SetCallbackOnStreamEnd(func);
}

void e2d::Music::SetCallbackOnLoopEnd(const Function & func)
{
	callback_.SetCallbackOnLoopEnd(func);
}

bool e2d::Music::_readMMIO()
{
	MMCKINFO ckIn;
	PCMWAVEFORMAT pcmWaveFormat;

	memset(&ckIn, 0, sizeof(ckIn));

	wfx_ = nullptr;

	if ((0 != mmioDescend(hmmio_, &ck_riff_, nullptr, 0)))
		return TraceError(L"mmioDescend");

	// 确认文件是一个合法的 wave 文件
	if ((ck_riff_.ckid != FOURCC_RIFF) ||
		(ck_riff_.fccType != mmioFOURCC('W', 'A', 'V', 'E')))
		return TraceError(L"mmioFOURCC");

	// 在输入文件中查找 'fmt' 块
	ckIn.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (0 != mmioDescend(hmmio_, &ckIn, &ck_riff_, MMIO_FINDCHUNK))
		return TraceError(L"mmioDescend");

	// 'fmt' 块至少应和 PCMWAVEFORMAT 一样大
	if (ckIn.cksize < (LONG)sizeof(PCMWAVEFORMAT))
		return TraceError(L"sizeof(PCMWAVEFORMAT)");

	// 将 'fmt' 块读取到 pcmWaveFormat 中
	if (mmioRead(hmmio_, (HPSTR)&pcmWaveFormat,
		sizeof(pcmWaveFormat)) != sizeof(pcmWaveFormat))
		return TraceError(L"mmioRead");

	// 分配 WAVEFORMATEX，但如果它不是 PCM 格式，再读取一个 WORD 大小
	// 的数据，这个数据就是额外分配的大小
	if (pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM)
	{
		wfx_ = (WAVEFORMATEX*) new CHAR[sizeof(WAVEFORMATEX)];

		// 拷贝数据
		memcpy(wfx_, &pcmWaveFormat, sizeof(pcmWaveFormat));
		wfx_->cbSize = 0;
	}
	else
	{
		// 读取额外数据的大小
		WORD cbExtraBytes = 0L;
		if (mmioRead(hmmio_, (CHAR*)&cbExtraBytes, sizeof(WORD)) != sizeof(WORD))
			return TraceError(L"mmioRead");

		wfx_ = (WAVEFORMATEX*) new CHAR[sizeof(WAVEFORMATEX) + cbExtraBytes];

		// 拷贝数据
		memcpy(wfx_, &pcmWaveFormat, sizeof(pcmWaveFormat));
		wfx_->cbSize = cbExtraBytes;

		// 读取额外数据
		if (mmioRead(hmmio_, (CHAR*)(((BYTE*)&(wfx_->cbSize)) + sizeof(WORD)),
			cbExtraBytes) != cbExtraBytes)
		{
			SAFE_DELETE(wfx_);
			return TraceError(L"mmioRead");
		}
	}

	if (0 != mmioAscend(hmmio_, &ckIn, 0))
	{
		SAFE_DELETE(wfx_);
		return TraceError(L"mmioAscend");
	}

	return true;
}

bool e2d::Music::_resetFile()
{
	// Seek to the data
	if (-1 == mmioSeek(hmmio_, ck_riff_.dwDataOffset + sizeof(FOURCC),
		SEEK_SET))
		return TraceError(L"mmioSeek");

	// Search the input file for the 'data' chunk.
	ck_.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (0 != mmioDescend(hmmio_, &ck_, &ck_riff_, MMIO_FINDCHUNK))
		return TraceError(L"mmioDescend");

	return true;
}

bool e2d::Music::_read(BYTE* buffer, DWORD size_to_read)
{
	MMIOINFO mmioinfoIn; // current status of hmmio_

	if (0 != mmioGetInfo(hmmio_, &mmioinfoIn, 0))
		return TraceError(L"mmioGetInfo");

	UINT cbDataIn = size_to_read;
	if (cbDataIn > ck_.cksize)
		cbDataIn = ck_.cksize;

	ck_.cksize -= cbDataIn;

	for (DWORD cT = 0; cT < cbDataIn; ++cT)
	{
		// Copy the bytes from the io to the buffer.
		if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead)
		{
			if (0 != mmioAdvance(hmmio_, &mmioinfoIn, MMIO_READ))
				return TraceError(L"mmioAdvance");

			if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead)
				return TraceError(L"mmioinfoIn.pchNext");
		}

		// Actual copy.
		*((BYTE*)buffer + cT) = *((BYTE*)mmioinfoIn.pchNext);
		++mmioinfoIn.pchNext;
	}

	if (0 != mmioSetInfo(hmmio_, &mmioinfoIn, 0))
		return TraceError(L"mmioSetInfo");

	return true;
}

bool e2d::Music::_findMediaFileCch(wchar_t* dest_path, int cch_dest, const wchar_t * file_name)
{
	bool bFound = false;

	if (nullptr == file_name || nullptr == dest_path || cch_dest < 10)
		return false;

	// Get the exe name, and exe path
	wchar_t strExePath[MAX_PATH] = { 0 };
	wchar_t strExeName[MAX_PATH] = { 0 };
	GetModuleFileName(HINST_THISCOMPONENT, strExePath, MAX_PATH);
	strExePath[MAX_PATH - 1] = 0;

	wchar_t* strLastSlash = wcsrchr(strExePath, TEXT('\\'));
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

	wcscpy_s(dest_path, cch_dest, file_name);
	if (GetFileAttributes(dest_path) != 0xFFFFFFFF)
		return true;

	// Search all parent directories starting At .\ and using file_name as the leaf name
	wchar_t strLeafName[MAX_PATH] = { 0 };
	wcscpy_s(strLeafName, MAX_PATH, file_name);

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
			wcscpy_s(dest_path, cch_dest, strFullFileName);
			bFound = true;
			break;
		}

		swprintf_s(strFullFileName, MAX_PATH, L"%s\\%s\\%s", strFullPath, strExeName, strLeafName);
		if (GetFileAttributes(strFullFileName) != 0xFFFFFFFF)
		{
			wcscpy_s(dest_path, cch_dest, strFullFileName);
			bFound = true;
			break;
		}

		swprintf_s(strSearch, MAX_PATH, L"%s\\..", strFullPath);
		GetFullPathName(strSearch, MAX_PATH, strFullPath, &strFilePart);
	}
	if (bFound)
		return true;

	// 失败时，将文件作为路径返回，同时也返回错误代码
	wcscpy_s(dest_path, cch_dest, file_name);

	return false;
}
