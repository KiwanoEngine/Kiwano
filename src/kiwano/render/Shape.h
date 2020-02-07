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
#include <kiwano/core/ObjectBase.h>
#include <kiwano/render/DirectX/D2DDeviceResources.h>

namespace kiwano
{
class RenderContext;
class Renderer;
class ShapeSink;

KGE_DECLARE_SMART_PTR(Shape);

/**
 * \addtogroup Render
 * @{
 */

/**
 * \~chinese
 * @brief ��״
 */
class KGE_API Shape : public virtual ObjectBase
{
    friend class RenderContext;
    friend class Renderer;
    friend class ShapeSink;

public:
    /// \~chinese
    /// @brief �����߶���״
    /// @param begin �߶����
    /// @param end �߶��յ�
    static ShapePtr CreateLine(Point const& begin, Point const& end);

    /// \~chinese
    /// @brief ��������
    /// @param rect ����
    static ShapePtr CreateRect(Rect const& rect);

    /// \~chinese
    /// @brief ����Բ�Ǿ���
    /// @param rect ����
    /// @param radius ����Բ�ǰ뾶
    static ShapePtr CreateRoundedRect(Rect const& rect, Vec2 const& radius);

    /// \~chinese
    /// @brief ����Բ��
    /// @param center Բ��ԭ��
    /// @param radius Բ�ΰ뾶
    static ShapePtr CreateCircle(Point const& center, float radius);

    /// \~chinese
    /// @brief ������Բ��
    /// @param center ��Բԭ��
    /// @param radius ��Բ�뾶
    static ShapePtr CreateEllipse(Point const& center, Vec2 const& radius);

    Shape();

    /// \~chinese
    /// @brief �Ƿ���Ч
    bool IsValid() const;

    /// \~chinese
    /// @brief ��ȡ���а�Χ��
    Rect GetBoundingBox() const;

    /// \~chinese
    /// @brief ��ȡ���а�Χ��
    /// @param transform ��ά�任
    Rect GetBoundingBox(Matrix3x2 const& transform) const;

    /// \~chinese
    /// @brief �ж�ͼ���Ƿ������
    /// @param point ��
    /// @param transform Ӧ�õ����ϵĶ�ά�任
    bool ContainsPoint(Point const& point, const Matrix3x2* transform = nullptr) const;

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

#if defined(KGE_WIN32)
private:
    ComPtr<ID2D1Geometry> GetGeometry() const;

    void SetGeometry(ComPtr<ID2D1Geometry> shape);

private:
    ComPtr<ID2D1Geometry> geo_;
#endif
};

/** @} */

#if defined(KGE_WIN32)
inline ComPtr<ID2D1Geometry> Shape::GetGeometry() const
{
    return geo_;
}

inline void Shape::SetGeometry(ComPtr<ID2D1Geometry> shape)
{
    geo_ = shape;
}
#endif

}  // namespace kiwano
