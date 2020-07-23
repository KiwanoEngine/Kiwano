// Copyright (c) 2016-2019 Kiwano - Nomango
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
#include <kiwano/render/GifImage.h>
#include <kiwano/render/Texture.h>

namespace kiwano
{
/**
 * \addtogroup Render
 * @{
 */

/**
 * \~chinese
 * @brief 纹理缓存
 */
class KGE_API TextureCache final : public Singleton<TextureCache>
{
    friend Singleton<TextureCache>;

public:
    /// \~chinese
    /// @brief 添加或获取纹理
    TexturePtr AddOrGetTexture(const String& file_path);

    /// \~chinese
    /// @brief 添加或获取纹理
    TexturePtr AddOrGetTexture(const Resource& res);

    /// \~chinese
    /// @brief 添加或获取GIF图像
    GifImagePtr AddOrGetGifImage(const String& file_path);

    /// \~chinese
    /// @brief 添加或获取GIF图像
    GifImagePtr AddOrGetGifImage(const Resource& res);

    /// \~chinese
    /// @brief 移除纹理缓存
    void RemoveTexture(const String& file_path);

    /// \~chinese
    /// @brief 移除纹理缓存
    void RemoveTexture(const Resource& res);

    /// \~chinese
    /// @brief 移除GIF图像缓存
    void RemoveGifImage(const String& file_path);

    /// \~chinese
    /// @brief 移除GIF图像缓存
    void RemoveGifImage(const Resource& res);

    /// \~chinese
    /// @brief 清空缓存
    void Clear();

    virtual ~TextureCache();

private:
    TextureCache();

private:
    using TextureMap = UnorderedMap<size_t, TexturePtr>;
    TextureMap texture_cache_;

    using GifImageMap = UnorderedMap<size_t, GifImagePtr>;
    GifImageMap gif_texture_cache_;
};

/** @} */
}  // namespace kiwano
