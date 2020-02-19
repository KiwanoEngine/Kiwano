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
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

namespace kiwano
{
namespace audio
{
class Sound;

/**
 * \addtogroup Audio
 * @{
 */

/**
 * \~chinese
 * @brief 音频解码器
 */
class KGE_API Transcoder
{
    friend class Sound;

public:
    /**
     * \~chinese
     * @brief 音频数据缓冲
     */
    struct Buffer
    {
        BYTE*               data;    ///< 音频数据
        uint32_t            size;    ///< 音频数据大小
        const WAVEFORMATEX* format;  ///< 音频数据格式
    };

    Transcoder();

    ~Transcoder();

    /// \~chinese
    /// @brief 获取数据缓冲
    Buffer GetBuffer() const;

    /// \~chinese
    /// @brief 清空数据缓冲
    void ClearBuffer();

private:
    /// \~chinese
    /// @brief 解码本地音频文件
    HRESULT LoadMediaFile(const String& file_path);

    /// \~chinese
    /// @brief 解码音频资源
    HRESULT LoadMediaResource(const Resource& res);

    /// \~chinese
    /// @brief 读取音频源数据
    HRESULT ReadSource(IMFSourceReader* reader);

private:
    BYTE*         wave_data_;
    uint32_t      wave_size_;
    WAVEFORMATEX* wave_format_;
};

/** @} */
}  // namespace audio
}  // namespace kiwano
