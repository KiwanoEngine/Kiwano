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
#include <kiwano/render/Shape.h>

namespace kiwano
{

KGE_DECLARE_SMART_PTR(Layer);

/**
 * \addtogroup Render
 * @{
 */

/**
 * \~chinese
 * @brief 图层
 */
class KGE_API Layer : public NativeObject
{
public:
    Layer();

    /// \~chinese
    /// @brief 获取图层裁剪区域
    Rect const& GetClipRect() const;

    /// \~chinese
    /// @brief 获取图层透明度
    float GetOpacity() const;

    /// \~chinese
    /// @brief 获取几何蒙层
    ShapePtr GetMaskShape() const;

    /// \~chinese
    /// @brief 获取几何蒙层变换
    Matrix3x2 const& GetMaskTransform() const;

    /// \~chinese
    /// @brief 设置图层裁剪区域
    void SetClipRect(Rect const& rect);

    /// \~chinese
    /// @brief 设置图层透明度
    void SetOpacity(float opacity);

    /// \~chinese
    /// @brief 设置几何蒙层
    void SetMaskShape(ShapePtr mask);

    /// \~chinese
    /// @brief 设置几何蒙层变换
    void SetMaskTransform(Matrix3x2 const& matrix);

private:
    Rect      clip_rect_;
    float     opacity_;
    ShapePtr  mask_;
    Matrix3x2 mask_transform_;
};

/** @} */

inline Rect const& Layer::GetClipRect() const
{
    return clip_rect_;
}

inline float Layer::GetOpacity() const
{
    return opacity_;
}

inline ShapePtr Layer::GetMaskShape() const
{
    return mask_;
}

inline Matrix3x2 const& Layer::GetMaskTransform() const
{
    return mask_transform_;
}

inline void Layer::SetClipRect(Rect const& rect)
{
    clip_rect_ = rect;
}

inline void Layer::SetOpacity(float opacity)
{
    opacity_ = opacity;
}

inline void Layer::SetMaskShape(ShapePtr mask)
{
    mask_ = mask;
}

inline void Layer::SetMaskTransform(Matrix3x2 const& matrix)
{
    mask_transform_ = matrix;
}

}  // namespace kiwano
