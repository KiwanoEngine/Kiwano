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
#include "../core/include-forwards.h"
#include "../core/Resource.h"
#include <unordered_map>

namespace easy2d
{
	// ���ֲ�����
	class Player
		: protected Noncopyable
	{
		using MusicMap = Map<size_t, SpMusic>;

	public:
		Player();

		~Player();

		// Ԥ����������Դ
		bool Load(
			Resource const& res			/* ������Դ */
		);

		// ��������
		bool Play(
			Resource const& res,	/* ������Դ */
			int loop_count = 0		/* ����ѭ������ (-1 Ϊѭ������) */
		);

		// ��ͣ����
		void Pause(
			Resource const& res		/* ������Դ */
		);

		// ������������
		void Resume(
			Resource const& res		/* ������Դ */
		);

		// ֹͣ����
		void Stop(
			Resource const& res		/* ������Դ */
		);

		// ��ȡ���ֲ���״̬
		bool IsPlaying(
			Resource const& res		/* ������Դ */
		);

		// ��ȡ����
		float GetVolume() const;

		// ��������
		void SetVolume(
			float volume			/* 1.0 Ϊԭʼ���� */
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
		float		volume_;
		MusicMap	musics_cache_;
	};
}
