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
#include <kiwano/render/Bitmap.h>
#include <kiwano/render/GifImage.h>

namespace kiwano
{
/**
 * \addtogroup Render
 * @{
 */

/**
 * \~chinese
 * @brief ŒªÕºª∫¥Ê
 */
class KGE_API BitmapCache final
{
public:
    /// \~chinese
    /// @brief ‘§º”‘ÿ±æµÿÕº∆¨
    RefPtr<Bitmap> Preload(StringView file_path);

    /// \~chinese
    /// @brief ‘§º”‘ÿÕº∆¨◊ ‘¥
    RefPtr<Bitmap> Preload(const Resource& res);

    /// \~chinese
    /// @brief ‘§º”‘ÿ±æµÿGIFÕº∆¨
    RefPtr<GifImage> PreloadGif(StringView file_path);

    /// \~chinese
    /// @brief ‘§º”‘ÿGIFÕº∆¨◊ ‘¥
    RefPtr<GifImage> PreloadGif(const Resource& res);

    /// \~chinese
    /// @brief ÃÌº”ŒªÕºª∫¥Ê
    void AddBitmap(size_t key, RefPtr<Bitmap> Bitmap);

    /// \~chinese
    /// @brief ÃÌº”GIFÕºœÒª∫¥Ê
    void AddGifImage(size_t key, RefPtr<GifImage> gif);

    /// \~chinese
    /// @brief ªÒ»°ŒªÕºª∫¥Ê
    RefPtr<Bitmap> GetBitmap(size_t key) const;

    /// \~chinese
    /// @brief ªÒ»°GIFÕºœÒª∫¥Ê
    RefPtr<GifImage> GetGifImage(size_t key) const;

    /// \~chinese
    /// @brief “∆≥˝ŒªÕºª∫¥Ê
    void RemoveBitmap(size_t key);

    /// \~chinese
    /// @brief “∆≥˝GIFÕºœÒª∫¥Ê
    void RemoveGifImage(size_t key);

    /// \~chinese
    /// @brief «Âø’ª∫¥Ê
    void Clear();

    ~BitmapCache();

private:
    BitmapCache();

private:
    using BitmapMap = UnorderedMap<size_t, RefPtr<Bitmap>>;
    BitmapMap Bitmap_cache_;

    using GifImageMap = UnorderedMap<size_t, RefPtr<GifImage>>;
    GifImageMap gif_Bitmap_cache_;
};

/** @} */
}  // namespace kiwano
