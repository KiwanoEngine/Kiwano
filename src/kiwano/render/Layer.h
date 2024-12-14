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

/**
 * \addtogroup Render
 * @{
 */

/**
 * \~chinese
 * @brief ͼ��
 */
class KGE_API Layer
{
public:
    Rect          bounds;
    float         opacity;
    RefPtr<Shape> mask;
    Matrix3x2     mask_transform;

public:
    /// \~chinese
    /// @brief ��������ͼ��
    /// @param bounds ͼ��߽�
    /// @param opacity ͼ��͸����
    Layer(const Rect& bounds, float opacity = 1.f);

    /// \~chinese
    /// @brief ����ͼ��
    /// @param mask �����ɲ�
    /// @param mask_transform �����ɲ�任
    /// @param opacity ͼ��͸����
    /// @param bounds ͼ��߽�
    Layer(RefPtr<Shape> mask, const Matrix3x2& mask_transform = Matrix3x2(), float opacity = 1.f,
          const Rect& bounds = Rect::Infinite());
};

/** @} */

}  // namespace kiwano
