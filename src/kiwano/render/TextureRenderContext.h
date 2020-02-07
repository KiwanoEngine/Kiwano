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
#include <kiwano/render/RenderContext.h>

namespace kiwano
{
class Renderer;

KGE_DECLARE_SMART_PTR(TextureRenderContext);

/**
 * \addtogroup Render
 * @{
 */

/// \~chinese
/// @brief ������Ⱦ������
/// @details ������Ⱦ�����Ľ���Ⱦ�����һ�����������
class KGE_API TextureRenderContext : public RenderContext
{
    friend class Renderer;

public:
    /// \~chinese
    /// @brief ����������Ⱦ������
    static TextureRenderContextPtr Create();

    /// \~chinese
    /// @brief ����������Ⱦ������
    /// @param size �����������С
    static TextureRenderContextPtr Create(Size const& desired_size);

    /// \~chinese
    /// @brief �Ƿ���Ч
    bool IsValid() const;

    /// \~chinese
    /// @brief ��ȡ��Ⱦ���
    /// @param[out] texture �������
    /// @return �����Ƿ�ɹ�
    bool GetOutput(Texture& texture);

private:
    TextureRenderContext();

    ComPtr<ID2D1BitmapRenderTarget> GetBitmapRenderTarget() const;

    void SetBitmapRenderTarget(ComPtr<ID2D1BitmapRenderTarget> ctx);

private:
    ComPtr<ID2D1BitmapRenderTarget> bitmap_rt_;
};

/** @} */

inline bool TextureRenderContext::IsValid() const
{
    return bitmap_rt_ != nullptr;
}

inline ComPtr<ID2D1BitmapRenderTarget> TextureRenderContext::GetBitmapRenderTarget() const
{
    return bitmap_rt_;
}

inline void TextureRenderContext::SetBitmapRenderTarget(ComPtr<ID2D1BitmapRenderTarget> ctx)
{
    bitmap_rt_ = ctx;
}
}  // namespace kiwano
