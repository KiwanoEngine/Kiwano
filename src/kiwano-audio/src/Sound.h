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
#include <kiwano/base/Resource.h>
#include <xaudio2.h>
#include "Transcoder.h"

namespace kiwano
{
	namespace audio
	{
		KGE_DECLARE_SMART_PTR(Sound);

		class KGE_API Sound
			: public ObjectBase
		{
		public:
			Sound();

			Sound(
				String const& file_path	/* 本地音频文件 */
			);

			Sound(
				Resource const& res		/* 音乐资源 */
			);

			virtual ~Sound();

			// 打开本地音频文件
			bool Load(
				String const& file_path
			);

			// 打开音乐资源
			bool Load(
				Resource const& res		/* 音乐资源 */
			);

			// 播放
			void Play(
				int loop_count = 0		/* 播放循环次数 (-1 为循环播放) */
			);

			// 暂停
			void Pause();

			// 继续
			void Resume();

			// 停止
			void Stop();

			// 关闭并回收资源
			void Close();

			// 是否正在播放
			bool IsPlaying() const;

			// 获取音量
			float GetVolume() const;

			// 设置音量
			void SetVolume(
				float volume	/* 1 为原始音量, 大于 1 为放大音量, 0 为最小音量 */
			);

		protected:
			bool		opened_;
			bool		playing_;
			Transcoder	transcoder_;

			IXAudio2SourceVoice*	voice_;
		};
	}
}
