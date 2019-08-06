// Copyright (c) 2016-2018 Easy2D - Nomango
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef __E2D_TOOL_H__
#define __E2D_TOOL_H__


#include "e2dutil.h"

namespace easy2d
{

	// ����
	class Music
		: public Ref
	{
	public:
		Music();

		Music(
			const std::wstring& file_path	/* �����ļ�·�� */
		);

		Music(
			Resource& res					/* ������Դ */
		);

		virtual ~Music();

		// �������ļ�
		bool Load(
			const std::wstring& file_path	/* �����ļ�·�� */
		);

		// ��������Դ
		bool Load(
			Resource& res					/* ������Դ */
		);

		// ����
		bool Play(
			int loop_count = 0				/* ����ѭ������ (-1 Ϊѭ������) */
		);

		// ��ͣ
		void Pause();

		// ����
		void Resume();

		// ֹͣ
		void Stop();

		// �رղ�������Դ
		void Close();

		// �Ƿ����ڲ���
		bool IsPlaying() const;

		// ��ȡ����
		float GetVolume() const;

		// ��������
		bool SetVolume(
			float volume	/* 1 Ϊԭʼ����, ���� 1 Ϊ�Ŵ�����, 0 Ϊ��С���� */
		);

		// ��ȡ IXAudio2SourceVoice ����
		IXAudio2SourceVoice * GetSourceVoice() const;

	protected:
		E2D_DISABLE_COPY(Music);

	protected:
		bool					opened_;
		bool					playing_;
		UINT32					size_;
		BYTE*					wave_data_;
		IXAudio2SourceVoice*	voice_;
	};


	// ���ֲ�����
	class Player
	{
	public:
		Player();

		~Player();

		// Ԥ����������Դ
		bool Load(
			const std::wstring& file_path	/* �����ļ�·�� */
		);

		// ��������
		bool Play(
			const std::wstring& file_path,	/* �����ļ�·�� */
			int loop_count = 0				/* ����ѭ������ (-1 Ϊѭ������) */
		);

		// ��ͣ����
		void Pause(
			const std::wstring& file_path	/* �����ļ�·�� */
		);

		// ������������
		void Resume(
			const std::wstring& file_path	/* �����ļ�·�� */
		);

		// ֹͣ����
		void Stop(
			const std::wstring& file_path	/* �����ļ�·�� */
		);

		// ��ȡ���ֲ���״̬
		bool IsPlaying(
			const std::wstring& file_path	/* �����ļ�·�� */
		);

		// Ԥ����������Դ
		bool Load(
			Resource& res		/* ������Դ */
		);

		// ��������
		bool Play(
			Resource& res,		/* ������Դ */
			int loop_count = 0	/* ����ѭ������ (-1 Ϊѭ������) */
		);

		// ��ͣ����
		void Pause(
			Resource& res		/* ������Դ */
		);

		// ������������
		void Resume(
			Resource& res		/* ������Դ */
		);

		// ֹͣ����
		void Stop(
			Resource& res		/* ������Դ */
		);

		// ��ȡ���ֲ���״̬
		bool IsPlaying(
			Resource& res		/* ������Դ */
		);

		// ��ȡ����
		float GetVolume() const;

		// ��������
		void SetVolume(
			float volume		/* 1.0 Ϊԭʼ���� */
		);

		// ��ͣ��������
		void PauseAll();

		// ����������������
		void ResumeAll();

		// ֹͣ��������
		void StopAll();

		// �������
		static void ClearCache();

	protected:
		E2D_DISABLE_COPY(Player);

	protected:
		float volume_;

		static std::map<size_t, Music*> musics_;
	};


	// ���ݹ�����
	class Data
	{
	public:
		Data(
			const std::wstring& key,				/* ��ֵ */
			const std::wstring& field = L"Defalut"	/* �ֶ����� */
		);

		// �������Ƿ����
		bool Exists() const;

		// ���� int ���͵�ֵ
		bool SaveInt(
			int value
		);

		// ���� float ���͵�ֵ
		bool SaveFloat(
			float value
		);

		// ���� double ���͵�ֵ
		bool SaveDouble(
			double value
		);

		// ���� bool ���͵�ֵ
		bool SaveBool(
			bool value
		);

		// ���� std::wstring ���͵�ֵ
		bool SaveString(
			const std::wstring& value
		);

		// ��ȡ int ���͵�ֵ
		int GetInt() const;

		// ��ȡ float ���͵�ֵ
		float GetFloat() const;

		// ��ȡ double ���͵�ֵ
		double GetDouble() const;

		// ��ȡ bool ���͵�ֵ
		bool GetBool() const;

		// ��ȡ �ַ��� ���͵�ֵ
		std::wstring GetString();

	protected:
		std::wstring key_;
		std::wstring field_;
		const std::wstring& data_path_;
	};


	// �ļ�
	class File
	{
	public:
		File();

		File(
			const std::wstring& file_name
		);

		virtual ~File();

		// ���ļ�
		bool Open(
			const std::wstring& file_name
		);

		// �ļ��Ƿ����
		bool Exists() const;

		// ɾ���ļ�
		bool Delete();

		// ��ȡ�ļ�·��
		const std::wstring& GetPath() const;

		// ��ȡ�ļ���չ��
		std::wstring GetExtension() const;

		// �ͷ���Դ����ʱ�ļ�Ŀ¼
		static File Extract(
			Resource& res,						/* ��Դ */
			const std::wstring& dest_file_name	/* Ŀ���ļ��� */
		);

		// ����ļ�����·��
		static void AddSearchPath(
			const std::wstring& path
		);

		// �������ļ��Ի���
		static File ShowOpenDialog(
			const std::wstring& title = L"��",		/* �Ի������ */
			const std::wstring& filter = L""		/* ɸѡ��չ�������� "*.jpg;*.jpeg" */
		);

		// ���������ļ��Ի���
		static File ShowSaveDialog(
			const std::wstring& title = L"����",		/* �Ի������ */
			const std::wstring& def_file = L"",		/* Ĭ�ϱ�����ļ��� */
			const std::wstring& def_ext = L""		/* Ĭ��׷�ӵ���չ�������� "txt" */
		);

	protected:
		std::wstring	file_path_;

		static std::list<std::wstring> search_paths_;
	};


	// ·��
	class Path
	{
	public:
		// ��ȡ���ݵ�Ĭ�ϱ���·��
		static const std::wstring& GetDataPath();

		// ��ȡ��ʱ�ļ�Ŀ¼
		static const std::wstring& GetTemporaryPath();

		// ��ȡ LocalAppData Ŀ¼
		static const std::wstring& GetLocalAppDataPath();

		// ��ȡ��ǰ���������·��
		static const std::wstring& GetExeFilePath();
	};

} // end of easy2d namespace


#endif // __E2D_TOOL_H__
