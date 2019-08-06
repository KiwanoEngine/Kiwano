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

#pragma once
#include "../base/base.h"
#include "../base/Resource.h"

namespace easy2d
{
	class Music;

	// ���ֲ�����
	class Player
	{
		E2D_DISABLE_COPY(Player);

	public:
		Player();

		~Player();

		// Ԥ����������Դ
		bool Load(
			const String& file_path	/* �����ļ�·�� */
		);

		// ��������
		bool Play(
			const String& file_path,	/* �����ļ�·�� */
			int loop_count = 0				/* ����ѭ������ (-1 Ϊѭ������) */
		);

		// ��ͣ����
		void Pause(
			const String& file_path	/* �����ļ�·�� */
		);

		// ������������
		void Resume(
			const String& file_path	/* �����ļ�·�� */
		);

		// ֹͣ����
		void Stop(
			const String& file_path	/* �����ļ�·�� */
		);

		// ��ȡ���ֲ���״̬
		bool IsPlaying(
			const String& file_path	/* �����ļ�·�� */
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
		void ClearCache();

	protected:
		float volume_;
		std::map<size_t, Music*> musics_cache_;
	};
}
