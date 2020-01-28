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
#include <kiwano-audio/Transcoder.h>
#include <kiwano/core/ObjectBase.h>
#include <kiwano/core/Resource.h>
#include <kiwano/platform/win32/ComPtr.hpp>
#include <xaudio2.h>

namespace kiwano
{
namespace audio
{
class AudioEngine;

KGE_DECLARE_SMART_PTR(Sound);

/**
 * \addtogroup Audio
 * @{
 */

/**
 * \~chinese
 * @brief 音频
 */
class KGE_API Sound : public virtual ObjectBase
{
    friend class AudioEngine;

public:
    Sound();

    virtual ~Sound();

    /// \~chinese
    /// @brief 打开本地音频文件
    /// @param res 本地音频文件路径
    bool Load(String const& file_path);

    /// \~chinese
    /// @brief 打开音频资源
    /// @param res 音频资源
    bool Load(Resource const& res);

    /// \~chinese
    /// @brief 是否有效
    bool IsValid() const;

    /// \~chinese
    /// @brief 播放
    /// @param loop_count 播放循环次数，设置 -1 为循环播放
    void Play(int loop_count = 0);

    /// \~chinese
    /// @brief 暂停
    void Pause();

    /// \~chinese
    /// @brief 继续
    void Resume();

    /// \~chinese
    /// @brief 停止
    void Stop();

    /// \~chinese
    /// @brief 关闭并销毁资源
    void Close();

    /// \~chinese
    /// @brief 是否正在播放
    bool IsPlaying() const;

    /// \~chinese
    /// @brief 获取音量
    float GetVolume() const;

    /// \~chinese
    /// @brief 设置音量
    /// @param volume 音量大小，1.0 为原始音量, 大于 1 为放大音量, 0 为最小音量
    void SetVolume(float volume);

private:
    IXAudio2SourceVoice* GetXAudio2Voice() const;

    void SetXAudio2Voice(IXAudio2SourceVoice* voice);

private:
    bool                 opened_;
    bool                 playing_;
    Transcoder           transcoder_;
    IXAudio2SourceVoice* voice_;
};

/** @} */

inline IXAudio2SourceVoice* Sound::GetXAudio2Voice() const
{
    return voice_;
}

inline void Sound::SetXAudio2Voice(IXAudio2SourceVoice* voice)
{
    voice_ = voice;
}
}  // namespace audio
}  // namespace kiwano
