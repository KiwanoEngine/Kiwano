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
 * @brief ������
 */
class KGE_API TextureCache final
{
public:
    /// \~chinese
    /// @brief Ԥ���ر���ͼƬ
    RefPtr<Texture> Preload(StringView file_path);

    /// \~chinese
    /// @brief Ԥ����ͼƬ��Դ
    RefPtr<Texture> Preload(const Resource& res);

    /// \~chinese
    /// @brief Ԥ���ر���GIFͼƬ
    RefPtr<GifImage> PreloadGif(StringView file_path);

    /// \~chinese
    /// @brief Ԥ����GIFͼƬ��Դ
    RefPtr<GifImage> PreloadGif(const Resource& res);

    /// \~chinese
    /// @brief ���������
    void AddTexture(size_t key, RefPtr<Texture> texture);

    /// \~chinese
    /// @brief ���GIFͼ�񻺴�
    void AddGifImage(size_t key, RefPtr<GifImage> gif);

    /// \~chinese
    /// @brief ��ȡ������
    RefPtr<Texture> GetTexture(size_t key) const;

    /// \~chinese
    /// @brief ��ȡGIFͼ�񻺴�
    RefPtr<GifImage> GetGifImage(size_t key) const;

    /// \~chinese
    /// @brief �Ƴ�������
    void RemoveTexture(size_t key);

    /// \~chinese
    /// @brief �Ƴ�GIFͼ�񻺴�
    void RemoveGifImage(size_t key);

    /// \~chinese
    /// @brief ��ջ���
    void Clear();

    ~TextureCache();

private:
    TextureCache();

private:
    using TextureMap = UnorderedMap<size_t, RefPtr<Texture>>;
    TextureMap texture_cache_;

    using GifImageMap = UnorderedMap<size_t, RefPtr<GifImage>>;
    GifImageMap gif_texture_cache_;
};

/** @} */
}  // namespace kiwano
