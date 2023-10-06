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
#include <kiwano/core/Resource.h>
#include <kiwano-audio/AudioData.h>

namespace kiwano
{
namespace audio
{
class AudioModule;
class SoundPlayer;

KGE_DECLARE_SMART_PTR(Sound);
KGE_DECLARE_SMART_PTR(SoundCallback);

/**
 * \addtogroup Audio
 * @{
 */

/**
 * \~chinese
 * @brief 音频回调
 */
class KGE_API SoundCallback : public NativeObject
{
public:
    /// \~chinese
    /// @brief 创建一个回调，在音频开始播放时执行
    static SoundCallbackPtr OnStart(const Function<void(Sound* sound)>& cb);

    /// \~chinese
    /// @brief 创建一个回调，在音频循环结束时执行
    static SoundCallbackPtr OnLoopEnd(const Function<void(Sound* sound)>& cb);

    /// \~chinese
    /// @brief 创建一个回调，在音频结束时执行
    static SoundCallbackPtr OnEnd(const Function<void(Sound* sound)>& cb);

    /// \~chinese
    /// @brief 创建一个回调，在音频修改音量时执行
    static SoundCallbackPtr OnVolumeChanged(const Function<float(Sound* sound, float volume)>& cb);

    /// \~chinese
    /// @brief 在音频开始播放时执行
    virtual inline void OnStart(Sound* sound) {}

    /// \~chinese
    /// @brief 在音频循环结束时执行
    virtual inline void OnLoopEnd(Sound* sound) {}

    /// \~chinese
    /// @brief 在音频结束时执行
    virtual inline void OnEnd(Sound* sound) {}

    /// \~chinese
    /// @brief 在音频修改音量时执行
    virtual inline float OnVolumeChanged(Sound* sound, float volume)
    {
        return volume;
    }
};

/**
 * \~chinese
 * @brief 音频
 */
class KGE_API Sound : public NativeObject
{
    friend class AudioModule;
    friend class SoundPlayer;

public:
    /// \~chinese
    /// @brief 创建音频对象
    /// @param file_path 本地音频文件路径
    Sound(const String& file_path);

    /// \~chinese
    /// @brief 创建音频对象
    /// @param res 音频资源
    /// @param ext 音频类型，决定了使用何种解码器
    Sound(const Resource& res, const String& ext = "");

    /// \~chinese
    /// @brief 创建音频对象
    /// @param data 音频数据
    Sound(AudioDataPtr data);

    Sound();

    virtual ~Sound();

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

    /// \~chinese
    /// @brief 添加回调
    void AddCallback(SoundCallbackPtr callback);

    /// \~chinese
    /// @brief 获取所有回调
    List<SoundCallbackPtr>& GetCallbacks();

    /// \~chinese
    /// @brief 获取所有回调
    const List<SoundCallbackPtr>& GetCallbacks() const;

protected:
    bool Load(AudioDataPtr data);

    SoundCallbackPtr GetCallbackChain();

    void ResetVolume();

private:
    bool         opened_;
    bool         playing_;
    float        volume_;
    AudioDataPtr data_;

    SoundCallbackPtr       callback_chain_;
    List<SoundCallbackPtr> callbacks_;
};

/** @} */

inline List<SoundCallbackPtr>& kiwano::audio::Sound::GetCallbacks()
{
    return callbacks_;
}

inline const List<SoundCallbackPtr>& kiwano::audio::Sound::GetCallbacks() const
{
    return callbacks_;
}

inline void kiwano::audio::Sound::AddCallback(SoundCallbackPtr callback)
{
    callbacks_.push_back(callback);
}

}  // namespace audio
}  // namespace kiwano
