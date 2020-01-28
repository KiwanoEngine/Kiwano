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
 * @brief ��Ƶ
 */
class KGE_API Sound : public virtual ObjectBase
{
    friend class AudioEngine;

public:
    Sound();

    virtual ~Sound();

    /// \~chinese
    /// @brief �򿪱�����Ƶ�ļ�
    /// @param res ������Ƶ�ļ�·��
    bool Load(String const& file_path);

    /// \~chinese
    /// @brief ����Ƶ��Դ
    /// @param res ��Ƶ��Դ
    bool Load(Resource const& res);

    /// \~chinese
    /// @brief �Ƿ���Ч
    bool IsValid() const;

    /// \~chinese
    /// @brief ����
    /// @param loop_count ����ѭ������������ -1 Ϊѭ������
    void Play(int loop_count = 0);

    /// \~chinese
    /// @brief ��ͣ
    void Pause();

    /// \~chinese
    /// @brief ����
    void Resume();

    /// \~chinese
    /// @brief ֹͣ
    void Stop();

    /// \~chinese
    /// @brief �رղ�������Դ
    void Close();

    /// \~chinese
    /// @brief �Ƿ����ڲ���
    bool IsPlaying() const;

    /// \~chinese
    /// @brief ��ȡ����
    float GetVolume() const;

    /// \~chinese
    /// @brief ��������
    /// @param volume ������С��1.0 Ϊԭʼ����, ���� 1 Ϊ�Ŵ�����, 0 Ϊ��С����
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
