// Copyright (c) 2016-2018 Kiwano - Nomango
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
#include <kiwano/core/intrusive_ptr.hpp>
#include <kiwano/base/ObjectBase.h>
#include "Sound.h"

namespace kiwano
{
	namespace audio
	{
		KGE_DECLARE_SMART_PTR(Player);

		// ���ֲ�����
		class KGE_API Player
			: protected ObjectBase
		{
			using MusicMap = Map<size_t, SoundPtr>;

		public:
			Player();

			~Player();

			// ����������Դ
			bool Load(
				Resource const& res		/* ������Դ */
			);

			// ��������
			void Play(
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
			MusicMap	sound_cache_;
		};
	}
}
