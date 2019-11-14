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
#include <kiwano/common/intrusive_ptr.hpp>
#include <kiwano/core/ObjectBase.h>
#include <kiwano-audio/Sound.h>

namespace kiwano
{
	namespace audio
	{
		KGE_DECLARE_SMART_PTR(SoundPlayer);

		// ���ֲ�����
		class KGE_API SoundPlayer
			: protected ObjectBase
		{
		public:
			SoundPlayer();

			~SoundPlayer();

			// ���ر�����Ƶ�ļ�, ���ظ���Դ�ı�ʶ��
			size_t Load(
				String const& file_path
			);

			// ����������Դ, ���ظ���Դ�ı�ʶ��
			size_t Load(
				Resource const& res			/* ������Դ */
			);

			// ��������
			void Play(
				size_t id,				/* ��ʶ�� */
				int loop_count = 0			/* ����ѭ������ (-1 Ϊѭ������) */
			);

			// ��ͣ����
			void Pause(
				size_t id				/* ��ʶ�� */
			);

			// ������������
			void Resume(
				size_t id				/* ��ʶ�� */
			);

			// ֹͣ����
			void Stop(
				size_t id				/* ��ʶ�� */
			);

			// ��ȡ���ֲ���״̬
			bool IsPlaying(
				size_t id				/* ��ʶ�� */
			);

			// ��ȡ����
			float GetVolume() const;

			// ��������
			void SetVolume(
				float volume				/* 1.0 Ϊԭʼ���� */
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

			using SoundMap = Map<size_t, SoundPtr>;
			SoundMap sound_cache_;
		};
	}
}