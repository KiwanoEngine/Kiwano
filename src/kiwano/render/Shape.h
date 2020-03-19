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
#include <kiwano/render/NativeObject.h>

namespace kiwano
{
class ShapeMaker;

KGE_DECLARE_SMART_PTR(Shape);

/**
 * \addtogroup Render
 * @{
 */

/**
 * \~chinese
 * @brief ��״
 */
class KGE_API Shape : public NativeObject
{
    friend class ShapeMaker;

public:
    /// \~chinese
    /// @brief �����߶���״
    /// @param begin �߶����
    /// @param end �߶��յ�
    static ShapePtr CreateLine(const Point& begin, const Point& end);

    /// \~chinese
    /// @brief ��������
    /// @param rect ����
    static ShapePtr CreateRect(const Rect& rect);

    /// \~chinese
    /// @brief ����Բ�Ǿ���
    /// @param rect ����
    /// @param radius ����Բ�ǰ뾶
    static ShapePtr CreateRoundedRect(const Rect& rect, const Vec2& radius);

    /// \~chinese
    /// @brief ����Բ��
    /// @param center Բ��ԭ��
    /// @param radius Բ�ΰ뾶
    static ShapePtr CreateCircle(const Point& center, float radius);

    /// \~chinese
    /// @brief ������Բ��
    /// @param center ��Բԭ��
    /// @param radius ��Բ�뾶
    static ShapePtr CreateEllipse(const Point& center, const Vec2& radius);

    Shape();

    /// \~chinese
    /// @brief ��ȡ���а�Χ��
    Rect GetBoundingBox() const;

    /// \~chinese
    /// @brief ��ȡ���а�Χ��
    /// @param transform ��ά�任
    Rect GetBoundingBox(const Matrix3x2& transform) const;

    /// \~chinese
    /// @brief �ж�ͼ���Ƿ������
    /// @param point ��
    /// @param transform Ӧ�õ����ϵĶ�ά�任
    bool ContainsPoint(const Point& point, const Matrix3x2* transform = nullptr) const;

    /// \~chinese
    /// @brief ��ȡͼ��չ����һ��ֱ�ߵĳ���
    float GetLength() const;

    /// \~chinese
    /// @brief ����ͼ�����
    float ComputeArea() const;

    /// \~chinese
    /// @brief ����ͼ���ϵ��λ�ú���������
    /// @param[in] length ����ͼ���ϵ�λ�ã���Χ [0.0 - 1.0]
    /// @param[out] point ���λ��
    /// @param[out] tangent �����������
    bool ComputePointAtLength(float length, Point& point, Vec2& tangent) const;

    /// \~chinese
    /// @brief �����״
    void Clear();
};

/** @} */

}  // namespace kiwano
