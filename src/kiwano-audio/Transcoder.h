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
#include <kiwano/base/ObjectBase.h>

namespace kiwano
{
namespace audio
{
class AudioModule;

KGE_DECLARE_SMART_PTR(Transcoder);

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
struct AudioMetadata
{
    AudioFormat format       = AudioFormat::PCM;  ///< 音频格式
    uint16_t    channels     = 2;                 ///< 声道，单声道为1，立体声为2
    uint32_t samples_per_sec = 44100;    ///< 采样率，11025 表示 11.025kHz。PCM格式的采样率通常为44.1kHz
    uint16_t bits_per_sample = 16;       ///< 位深，PCM格式为 8 或 16
    uint16_t block_align     = 4;        ///< 块对齐，PCM格式通常是 (channels * bits_per_sample) / 8
    void*    extra_data      = nullptr;  ///< 额外数据，不要设置这个字段
};

/**
 * \~chinese
 * @brief 音频解码器
 */
class KGE_API Transcoder : public ObjectBase
{
    friend class AudioModule;

public:
    Transcoder(const String& file_path);

    Transcoder(const Resource& res);

    Transcoder(const BinaryData& data, const AudioMetadata& metadata);

    virtual ~Transcoder();

    /// \~chinese
    /// @brief 获取音频元数据
    AudioMetadata GetMetadata() const;

    /// \~chinese
    /// @brief 获取数据
    BinaryData GetData() const;

    /// \~chinese
    /// @brief 清空数据
    void Clear();

private:
    /// \~chinese
    /// @brief 解码本地音频文件
    bool Load(const String& file_path);

    /// \~chinese
    /// @brief 解码音频资源
    bool Load(const Resource& res);

    bool Load(const BinaryData& data, const AudioMetadata& metadata);

private:
    std::unique_ptr<uint8_t[]> raw_;
    BinaryData                 data_;
    AudioMetadata              metadata_;
};

/** @} */

}  // namespace audio
}  // namespace kiwano
