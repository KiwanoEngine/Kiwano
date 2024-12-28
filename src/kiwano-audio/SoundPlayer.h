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

namespace kiwano
{
namespace audio
{

/**
 * \addtogroup Audio
 * @{
 */

/**
 * \~chinese
 * @brief 音频播放器
 */
class KGE_API SoundPlayer : public BaseObject
{
public:
    using SoundList = List<RefPtr<Sound>>;

    SoundPlayer();

    ~SoundPlayer();

    /// \~chinese
    /// @brief 预加载音频
    /// @details
    RefPtr<AudioData> Preload(StringView file_path);

    /// \~chinese
    /// @brief 预加载音频资源
    RefPtr<AudioData> Preload(const Resource& res, StringView ext = "");

    /// \~chinese
    /// @brief 播放音频
    /// @param sound 音频
    /// @param loop_count 播放循环次数，设置 -1 为循环播放
    void Play(RefPtr<Sound> sound, int loop_count = 0);

    /// \~chinese
    /// @brief 播放音频
    /// @param file_path 本地音频文件路径
    /// @param loop_count 播放循环次数，设置 -1 为循环播放
    RefPtr<Sound> Play(StringView file_path, int loop_count = 0);

    /// \~chinese
    /// @brief 播放音频
    /// @param res 音频资源
    /// @param loop_count 播放循环次数，设置 -1 为循环播放
    RefPtr<Sound> Play(const Resource& res, int loop_count = 0);

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
    /// @brief 获取正在播放的音频列表
    const SoundList& GetPlayingList() const;

    /// \~chinese
    /// @brief 获取音量
    float GetVolume() const;

    /// \~chinese
    /// @brief 设置音量
    /// @param volume 音量大小，1.0 为原始音量, 大于 1 为放大音量, 0 为最小音量
    void SetVolume(float volume);

    /// \~chinese
    /// @brief 清空缓存
    void ClearCache();

protected:
    void OnEnd(Sound* sound);

    float OnVolumeChanged(Sound* sound, float volume);

    void SetCallback(Sound* sound);

    void RemoveCallback(Sound* sound);

    void ClearTrash();

protected:
    float                 volume_;
    SoundList             sound_list_;
    SoundList             trash_;
    RefPtr<SoundCallback> callback_;

    UnorderedMap<size_t, RefPtr<AudioData>> cache_;
};

/** @} */

inline const SoundPlayer::SoundList& SoundPlayer::GetPlayingList() const
{
    return sound_list_;
}

}  // namespace audio
}  // namespace kiwano
