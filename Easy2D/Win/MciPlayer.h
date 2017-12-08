#pragma once

class MciPlayer
{
public:
	MciPlayer();
	~MciPlayer();

	void close();
	bool open(const e2d::EString & pFileName, UINT uId);
	bool open(const e2d::EString & pResouceName, const e2d::EString & pResouceType, const e2d::EString & musicExtension, UINT uId);
	void play(int repeatTimes);
	void pause();
	void resume();
	void stop();
	void rewind();
	bool isPlaying();
	UINT getMusicID();

private:
	friend LRESULT WINAPI _MciPlayerProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	void _sendCommand(int nCommand, DWORD_PTR param1 = 0, DWORD_PTR parma2 = 0);

	MCIDEVICEID m_dev;
	HWND        m_wnd;
	UINT		m_nMusicID;
	bool        m_bPlaying;
	int			m_nRepeatTimes;
};
