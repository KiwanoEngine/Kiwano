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
		public:
			Player();

			~Player();

			// ���ر�����Ƶ�ļ�, ���ظ���Դ��ʶ��
			UInt32 Load(
				String const& file_path
			);

			// ����������Դ, ���ظ���Դ��ʶ��
			UInt32 Load(
				Resource const& res		/* ������Դ */
			);

			// ��������
			void Play(
				UInt32 id,				/* ��ʶ�� */
				Int32 loop_count = 0		/* ����ѭ������ (-1 Ϊѭ������) */
			);

			// ��ͣ����
			void Pause(
				UInt32 id				/* ��ʶ�� */
			);

			// ������������
			void Resume(
				UInt32 id				/* ��ʶ�� */
			);

			// ֹͣ����
			void Stop(
				UInt32 id				/* ��ʶ�� */
			);

			// ��ȡ���ֲ���״̬
			bool IsPlaying(
				UInt32 id				/* ��ʶ�� */
			);

			// ��ȡ����
			Float32 GetVolume() const;

			// ��������
			void SetVolume(
				Float32 volume			/* 1.0 Ϊԭʼ���� */
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
			Float32 volume_;

			using MusicMap = Map<UInt32, SoundPtr>;
			MusicMap sound_cache_;
		};
	}
}
