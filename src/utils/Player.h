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
	// 音乐播放器
	class Player
		: protected Noncopyable
	{
		using MusicMap = Map<size_t, SpMusic>;

	public:
		Player();

		~Player();

		// 预加载音乐资源
		bool Load(
			Resource const& res			/* 音乐资源 */
		);

		// 播放音乐
		bool Play(
			Resource const& res,	/* 音乐资源 */
			int loop_count = 0		/* 播放循环次数 (-1 为循环播放) */
		);

		// 暂停音乐
		void Pause(
			Resource const& res		/* 音乐资源 */
		);

		// 继续播放音乐
		void Resume(
			Resource const& res		/* 音乐资源 */
		);

		// 停止音乐
		void Stop(
			Resource const& res		/* 音乐资源 */
		);

		// 获取音乐播放状态
		bool IsPlaying(
			Resource const& res		/* 音乐资源 */
		);

		// 获取音量
		float GetVolume() const;

		// 设置音量
		void SetVolume(
			float volume			/* 1.0 为原始音量 */
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
		float		volume_;
		MusicMap	musics_cache_;
	};
}
