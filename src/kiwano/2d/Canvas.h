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
#include <kiwano/2d/Actor.h>
#include <kiwano/render/Frame.h>
#include <kiwano/render/ShapeMaker.h>
#include <kiwano/render/RenderContext.h>

namespace kiwano
{

KGE_DECLARE_SMART_PTR(Canvas);

/**
 * \addtogroup Actors
 * @{
 */

/**
 * \~chinese
 * @brief 画布
 * @details 用于绘制图形、图像、文字等各种类型的图元，同时可以将绘制内容导出至图像
 */
class KGE_API Canvas : public Actor
{
public:
    /// \~chinese
    /// @brief 创建画布
    /// @param size 画布大小
    Canvas(const Size& size);

    /// \~chinese
    /// @brief 获取2D绘图上下文
    RenderContextPtr GetContext2D() const;

    /// \~chinese
    /// @brief 清空画布大小并重设画布大小
    /// @warning 该函数会导致原绘图上下文失效
    void ResizeAndClear(Size size);

    /// \~chinese
    /// @brief 导出纹理
    TexturePtr ExportToTexture() const;

    void OnRender(RenderContext& ctx) override;

private:
    TexturePtr       texture_cached_;
    RenderContextPtr render_ctx_;
};

/** @} */

}  // namespace kiwano
