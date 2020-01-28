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
#include <kiwano-audio/Transcoder.h>
#include <kiwano/core/Common.h>
#include <kiwano/core/Component.h>
#include <xaudio2.h>

namespace kiwano
{
namespace audio
{

/**
 * \~chinese
 * \defgroup Audio ��Ƶ����
 */

/**
 * \addtogroup Audio
 * @{
 */

/**
 * \~chinese
 * @brief ��Ƶ����
 */
class KGE_API AudioEngine
    : public Singleton<AudioEngine>
    , public ComponentBase
{
    friend Singleton<AudioEngine>;

public:
    /// \~chinese
    /// @brief ������Ƶ�豸
    void Open();

    /// \~chinese
    /// @brief �ر���Ƶ�豸
    void Close();

    /// \~chinese
    /// @brief �ӽ��������ݻ����д�����Ƶ����
    bool CreateSound(Sound& sound, const Transcoder::Buffer& buffer);

public:
    void SetupComponent() override;

    void DestroyComponent() override;

private:
    AudioEngine();

    ~AudioEngine();

private:
    IXAudio2*               x_audio2_;
    IXAudio2MasteringVoice* mastering_voice_;
};

/** @} */

}  // namespace audio
}  // namespace kiwano
