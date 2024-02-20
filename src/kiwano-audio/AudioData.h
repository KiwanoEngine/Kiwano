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
 * @brief 音频格式
 */
enum class AudioFormat
{
    PCM,
};

/**
 * \~chinese
 * @brief 音频元数据
 */
struct AudioMeta
{
    AudioFormat format          = AudioFormat::PCM;  ///< 音频格式
    uint16_t    channels        = 2;                 ///< 声道，单声道为1，立体声为2
    uint32_t    samples_per_sec = 44100;             ///< 采样率，11025 表示 11.025kHz。PCM格式的采样率通常为44.1kHz
    uint16_t    bits_per_sample = 16;                ///< 位深，PCM格式为 8 或 16
    uint16_t    block_align     = 4;                 ///< 块对齐，PCM格式通常是 (channels * bits_per_sample) / 8

    inline uint32_t avg_bytes_per_sec() const noexcept
    {
        return uint32_t(this->samples_per_sec * this->block_align);
    }
};

/**
 * \~chinese
 * @brief 音频数据
 */
class AudioData : public NativeObject
{
public:
    /// \~chinese
    /// @brief 音频数据
    /// @param data 音频数据
    /// @param meta 音频元数据
    AudioData(const BinaryData& data, const AudioMeta& meta);

    /// \~chinese
    /// @brief 获取音频元数据
    AudioMeta GetMeta() const;

    /// \~chinese
    /// @brief 获取数据
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
