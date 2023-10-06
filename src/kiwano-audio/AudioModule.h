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
#include <kiwano/base/Module.h>
#include <xaudio2.h>

namespace kiwano
{
namespace audio
{

/**
 * \~chinese
 * \defgroup Audio ��Ƶģ��
 */

/**
 * \addtogroup Audio
 * @{
 */

/**
 * \~chinese
 * @brief ��Ƶģ��
 */
class KGE_API AudioModule
    : public Singleton<AudioModule>
    , public Module
{
    friend Singleton<AudioModule>;

public:
    /// \~chinese
    /// @brief ������Ƶ�豸
    void Open();

    /// \~chinese
    /// @brief �ر���Ƶ�豸
    void Close();

    /// \~chinese
    /// @brief ע�������
    /// @param ext �ļ����ͣ��磺ogg����* ΪĬ�Ͻ�����
    /// @param transcoder ������
    void RegisterTranscoder(const String& ext, TranscoderPtr transcoder);

    /// \~chinese
    /// @brief ��ȡ������
    TranscoderPtr GetTranscoder(const String& ext);

    /// \~chinese
    /// @brief ������Ƶ
    /// @param file_path ������Ƶ�ļ�·��
    AudioDataPtr Decode(const String& file_path);

    /// \~chinese
    /// @brief ������Ƶ
    /// @param res ��Ƶ��Դ
    /// @param ext ��Ƶ���ͣ�������ʹ�ú��ֽ�����
    AudioDataPtr Decode(const Resource& res, const String& ext = "");

    /// \~chinese
    /// @brief ������Ƶ
    bool CreateSound(Sound& sound, AudioDataPtr data);

public:
    void SetupModule() override;

    void DestroyModule() override;

    ~AudioModule();

private:
    AudioModule();

private:
    IXAudio2*               x_audio2_;
    IXAudio2MasteringVoice* mastering_voice_;

    UnorderedMap<String, TranscoderPtr> registered_transcoders_;
};

/** @} */

}  // namespace audio
}  // namespace kiwano
