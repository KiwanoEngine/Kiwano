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
#include <kiwano/core/BinaryData.h>
#include <kiwano/base/ObjectBase.h>
#include <kiwano/platform/NativeObject.hpp>

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
 * @brief ��Ƶ��ʽ
 */
enum class AudioFormat
{
    PCM,
};

/**
 * \~chinese
 * @brief ��ƵԪ����
 */
struct AudioMeta
{
    AudioFormat format          = AudioFormat::PCM;  ///< ��Ƶ��ʽ
    uint16_t    channels        = 2;                 ///< ������������Ϊ1��������Ϊ2
    uint32_t    samples_per_sec = 44100;             ///< �����ʣ�11025 ��ʾ 11.025kHz��PCM��ʽ�Ĳ�����ͨ��Ϊ44.1kHz
    uint16_t    bits_per_sample = 16;                ///< λ�PCM��ʽΪ 8 �� 16
    uint16_t    block_align     = 4;                 ///< ����룬PCM��ʽͨ���� (channels * bits_per_sample) / 8

    inline uint32_t avg_bytes_per_sec() const noexcept
    {
        return uint32_t(this->samples_per_sec * this->block_align);
    }
};

/**
 * \~chinese
 * @brief ��Ƶ����
 */
class AudioData : public NativeObject
{
public:
    /// \~chinese
    /// @brief ��Ƶ����
    /// @param data ��Ƶ����
    /// @param meta ��ƵԪ����
    AudioData(const BinaryData& data, const AudioMeta& meta);

    /// \~chinese
    /// @brief ��ȡ��ƵԪ����
    AudioMeta GetMeta() const;

    /// \~chinese
    /// @brief ��ȡ����
    BinaryData GetData() const;

protected:
    AudioData() = default;

protected:
    BinaryData data_;
    AudioMeta  meta_;
};

/** @} */

}  // namespace audio
}  // namespace kiwano
