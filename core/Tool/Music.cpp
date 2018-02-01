#include "..\etools.h"
#include "..\emanagers.h"

using namespace e2d;

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=nullptr; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=nullptr; } }
#endif

inline bool TraceError(LPCTSTR sPrompt)
{
	WARN_IF(true, "EMusic error: %s failed!", sPrompt);
	return false;
}

inline bool TraceError(LPCTSTR sPrompt, HRESULT hr)
{
	WARN_IF(true, "EMusic error: %s (%#X)", sPrompt, hr);
	return false;
}


EMusic::EMusic()
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

EMusic::~EMusic()
{
	_close();
}

bool EMusic::_open(LPWSTR strFileName)
{
	if (m_bOpened)
	{
		WARN_IF(true, L"EMusic can be opened only once!");
		return false;
	}

	if (strFileName == nullptr)
	{
		WARN_IF(true, L"EMusic::_open Invalid file name.");
		return false;
	}

	IXAudio2 * pXAudio2 = EMusicManager::getIXAudio2();
	if (!pXAudio2)
	{
		WARN_IF(true, L"IXAudio2 nullptr pointer error!");
		return false;
	}

	// 定位 wave 文件
	wchar_t strFilePath[MAX_PATH];
	if (!_findMediaFileCch(strFilePath, MAX_PATH, strFileName))
	{
		WARN_IF(true, L"Failed to find media file: %s", strFileName);
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
	if (FAILED(hr = pXAudio2->CreateSourceVoice(&m_pSourceVoice, m_pwfx)))
	{
		TraceError(L"Error %#X creating source voice", hr);
		SAFE_DELETE_ARRAY(m_pbWaveData);
		return false;
	}

	m_bOpened = true;
	m_bPlaying = false;
	return true;
}

bool EMusic::play(int nLoopCount)
{
	HRESULT hr;

	if (m_bPlaying)
	{
		stop();
	}

	// 提交 wave 样本数据
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.pAudioData = m_pbWaveData;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = m_dwSize;
	buffer.LoopCount = nLoopCount;

	if (FAILED(hr = m_pSourceVoice->SubmitSourceBuffer(&buffer)))
	{
		TraceError(L"Error %#X submitting source buffer", hr);
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

bool EMusic::pause()
{
	if (m_pSourceVoice)
	{
		if (SUCCEEDED(m_pSourceVoice->Stop()))
		{
			m_bPlaying = false;
			return true;
		}
	}
	return false;
}

bool EMusic::resume()
{
	if (m_pSourceVoice)
	{
		if (SUCCEEDED(m_pSourceVoice->Start()))
		{
			m_bPlaying = true;
			return true;
		}
	}
	return false;
}

bool EMusic::stop()
{
	if (m_pSourceVoice)
	{
		if (SUCCEEDED(m_pSourceVoice->Stop()))
		{
			m_pSourceVoice->ExitLoop();
			m_pSourceVoice->FlushSourceBuffers();
			m_bPlaying = false;
			return true;
		}
	}
	return false;
}

bool EMusic::isPlaying()
{
	if (m_pSourceVoice)
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

float e2d::EMusic::getVolume() const
{
	float fVolume = 0.0f;
	if (m_pSourceVoice)
	{
		m_pSourceVoice->GetVolume(&fVolume);
	}
	return fVolume;
}

bool e2d::EMusic::setVolume(float fVolume)
{
	if (m_pSourceVoice)
	{
		return SUCCEEDED(m_pSourceVoice->SetVolume(min(max(fVolume, -224), 224)));
	}
	return false;
}

float e2d::EMusic::getFrequencyRatio() const
{
	float fFrequencyRatio = 0.0f;
	if (m_pSourceVoice)
	{
		m_pSourceVoice->GetFrequencyRatio(&fFrequencyRatio);
	}
	return fFrequencyRatio;
}

bool e2d::EMusic::setFrequencyRatio(float fFrequencyRatio)
{
	if (m_pSourceVoice)
	{
		fFrequencyRatio = min(max(fFrequencyRatio, XAUDIO2_MIN_FREQ_RATIO), XAUDIO2_MAX_FREQ_RATIO);
		return SUCCEEDED(m_pSourceVoice->SetFrequencyRatio(fFrequencyRatio));
	}
	return false;
}

IXAudio2SourceVoice * e2d::EMusic::getIXAudio2SourceVoice() const
{
	return m_pSourceVoice;
}

void EMusic::_close()
{
	if (m_pSourceVoice)
	{
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

bool EMusic::_readMMIO()
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

bool EMusic::_resetFile()
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

bool EMusic::_read(BYTE* pBuffer, DWORD dwSizeToRead)
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

bool EMusic::_findMediaFileCch(WCHAR* strDestPath, int cchDest, LPCWSTR strFilename)
{
	bool bFound = false;

	if (nullptr == strFilename || strFilename[0] == 0 || nullptr == strDestPath || cchDest < 10)
		return false;

	// Get the exe name, and exe path
	WCHAR strExePath[MAX_PATH] = { 0 };
	WCHAR strExeName[MAX_PATH] = { 0 };
	WCHAR* strLastSlash = nullptr;
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
	WCHAR strLeafName[MAX_PATH] = { 0 };
	wcscpy_s(strLeafName, MAX_PATH, strFilename);

	WCHAR strFullPath[MAX_PATH] = { 0 };
	WCHAR strFullFileName[MAX_PATH] = { 0 };
	WCHAR strSearch[MAX_PATH] = { 0 };
	WCHAR* strFilePart = nullptr;

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
