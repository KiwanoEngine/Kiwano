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
#include <kiwano/render/Shape.h>

namespace kiwano
{

/**
 * \addtogroup Render
 * @{
 */

/// \~chinese
/// @brief ��״�ϲ���ʽ
enum class CombineMode
{
    Union,      ///< ���� (A + B)
    Intersect,  ///< ���� (A + B)
    Xor,        ///< �ԳƲ ((A - B) + (B - A))
    Exclude     ///< � (A - B)
};

/// \~chinese
/// @brief ��״������
class KGE_API ShapeMaker : public NativeObject
{
public:
    ShapeMaker();

    ~ShapeMaker();

    /// \~chinese
    /// @brief ��ȡ���ɵ���״
    RefPtr<Shape> GetShape();

    /// \~chinese
    /// @brief ���ͼ��
    void Clear();

    /// \~chinese
    /// @brief ��ʼ���·������������
    /// @param begin_pos ·����ʼ��
    void BeginPath(const Point& begin_pos = Point());

    /// \~chinese
    /// @brief ����·�����ر�������
    /// @param closed ·���Ƿ�պ�
    void EndPath(bool closed = false);

    /// \~chinese
    /// @brief ���һ���߶�
    /// @param point �˵�
    void AddLine(const Point& point);

    /// \~chinese
    /// @brief ��Ӷ����߶�
    /// @param points �˵㼯��
    void AddLines(const Vector<Point>& points);

    /// \~chinese
    /// @brief ��Ӷ����߶�
    /// @param points �˵�����
    /// @param count �˵�����
    void AddLines(const Point* points, size_t count);

    /// \~chinese
    /// @brief ���һ�����η�����������
    /// @param point1 ���������ߵĵ�һ�����Ƶ�
    /// @param point2 ���������ߵĵڶ������Ƶ�
    /// @param point3 ���������ߵ��յ�
    void AddBezier(const Point& point1, const Point& point2, const Point& point3);

    /// \~chinese
    /// @brief ��ӻ���
    /// @param point �յ�
    /// @param radius ��Բ�뾶
    /// @param rotation ��Բ��ת�Ƕ�
    /// @param clockwise ˳ʱ�� or ��ʱ��
    /// @param is_small �Ƿ�ȡС�� 180�� �Ļ�
    void AddArc(const Point& point, const Size& radius, float rotation, bool clockwise = true, bool is_small = true);

    /// \~chinese
    /// @brief �ϲ���״
    /// @param shape_a �������״A
    /// @param shape_b �������״B
    /// @param mode �ϲ���ʽ
    /// @param matrix Ӧ�õ�������״B�ϵĶ�ά�任
    /// @return ���غϲ������״
    static RefPtr<Shape> Combine(RefPtr<Shape> shape_a, RefPtr<Shape> shape_b, CombineMode mode,
                                 const Matrix3x2* matrix = nullptr);

    /// \~chinese
    /// @brief �������ɵ���״
    /// @note Ӧ��ϵͳ���øú���
    void SetShape(RefPtr<Shape> shape);

private:
    /// \~chinese
    /// @brief ��������
    /// @note Ӧ��ϵͳ���øú���
    void OpenStream();

    /// \~chinese
    /// @brief �ر�������
    /// @note Ӧ��ϵͳ���øú���
    void CloseStream();

    /// \~chinese
    /// @brief �������Ƿ��Ѿ���
    /// @note Ӧ��ϵͳ���øú���
    bool IsStreamOpened() const;

private:
    RefPtr<Shape> shape_;
};

/** @} */

}  // namespace kiwano
