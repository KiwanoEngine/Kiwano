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
#include <kiwano-audio/Sound.h>
#include <kiwano/base/ObjectBase.h>

namespace kiwano
{
namespace audio
{
KGE_DECLARE_SMART_PTR(SoundPlayer);

/**
 * \addtogroup Audio
 * @{
 */

/**
 * \~chinese
 * @brief 音频播放器
 */
class KGE_API SoundPlayer : public ObjectBase
{
public:
    SoundPlayer();

    ~SoundPlayer();

    /// \~chinese
    /// @brief 加载本地音频文件
    /// @param file_path 本地音频文件路径
    /// @return 音频标识符
    size_t Load(const String& file_path);

    /// \~chinese
    /// @brief 加载音频资源
    /// @param res 音频资源
    /// @return 音频标识符
    size_t Load(const Resource& res);

    /// \~chinese
    /// @brief 播放音频
    /// @param id 音频标识符
    /// @param loop_count 播放循环次数，设置 -1 为循环播放
    void Play(size_t id, int loop_count = 0);

    /// \~chinese
    /// @brief 暂停音频
    /// @param id 音频标识符
    void Pause(size_t id);

    /// \~chinese
    /// @brief 继续播放音频
    /// @param id 音频标识符
    void Resume(size_t id);

    /// \~chinese
    /// @brief 停止音频
    /// @param id 音频标识符
    void Stop(size_t id);

    /// \~chinese
    /// @brief 获取音频播放状态
    /// @param id 音频标识符
    bool IsPlaying(size_t id);

    /// \~chinese
    /// @brief 获取音量
    float GetVolume() const;

    /// \~chinese
    /// @brief 设置音量
    /// @param volume 音量大小，1.0 为原始音量, 大于 1 为放大音量, 0 为最小音量
    void SetVolume(float volume);

    /// \~chinese
    /// @brief 获取本地音频文件id
    /// @param file_path 本地音频文件路径
    /// @return 音频标识符
    size_t GetId(const String& file_path) const;

    /// \~chinese
    /// @brief 获取音频资源id
    /// @param res 音频资源
    /// @return 音频标识符
    size_t GetId(const Resource& res) const;

    /// \~chinese
    /// @brief 获取音乐对象
    /// @param id 音频标识符
    SoundPtr GetSound(size_t id) const;

    /// \~chinese
    /// @brief 暂停所有音频
    void PauseAll();

    /// \~chinese
    /// @brief 继续播放所有音频
    void ResumeAll();

    /// \~chinese
    /// @brief 停止所有音频
    void StopAll();

    /// \~chinese
    /// @brief 清除缓存
    void ClearCache();

private:
    float volume_;

    using SoundMap = Map<size_t, SoundPtr>;
    SoundMap sound_cache_;
};

/** @} */
}  // namespace audio
}  // namespace kiwano
