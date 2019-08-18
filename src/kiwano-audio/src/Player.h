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

		// 音乐播放器
		class KGE_API Player
			: protected ObjectBase
		{
		public:
			Player();

			~Player();

			// 加载本地音频文件, 返回该资源标识符
			UInt32 Load(
				String const& file_path
			);

			// 加载音乐资源, 返回该资源标识符
			UInt32 Load(
				Resource const& res		/* 音乐资源 */
			);

			// 播放音乐
			void Play(
				UInt32 id,				/* 标识符 */
				Int32 loop_count = 0		/* 播放循环次数 (-1 为循环播放) */
			);

			// 暂停音乐
			void Pause(
				UInt32 id				/* 标识符 */
			);

			// 继续播放音乐
			void Resume(
				UInt32 id				/* 标识符 */
			);

			// 停止音乐
			void Stop(
				UInt32 id				/* 标识符 */
			);

			// 获取音乐播放状态
			bool IsPlaying(
				UInt32 id				/* 标识符 */
			);

			// 获取音量
			Float32 GetVolume() const;

			// 设置音量
			void SetVolume(
				Float32 volume			/* 1.0 为原始音量 */
			);

			// 暂停所有音乐
			void PauseAll();

			// 继续播放所有音乐
			void ResumeAll();

			// 停止所有音乐
			void StopAll();

			// 清除缓存
			void ClearCache();

		protected:
			Float32 volume_;

			using MusicMap = Map<UInt32, SoundPtr>;
			MusicMap sound_cache_;
		};
	}
}
