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
/// @brief ��״��Ϸ�ʽ
enum class CombineMode
{
    Union,      ///< ���� (A + B)
    Intersect,  ///< ���� (A + B)
    Xor,        ///< �ԳƲ ((A - B) + (B - A))
    Exclude     ///< � (A - B)
};

/// \~chinese
/// @brief ��״������
class KGE_API ShapeSink : protected Noncopyable
{
public:
    ShapeSink();

    ~ShapeSink();

    /// \~chinese
    /// @brief ��������
    void Open();

    /// \~chinese
    /// @brief �ر�������
    void Close();

    /// \~chinese
    /// @brief �������Ƿ��Ѿ���
    bool IsOpened() const;

    /// \~chinese
    /// @brief ��ȡ���ɵ���״
    /// @note ����δ�ر������������Զ��ر�
    ShapePtr GetShape();

    /// \~chinese
    /// @brief �����״������
    /// @param input �������״
    /// @param input_matrix Ӧ�õ�������״�ϵĶ�ά�任
    /// @note ����δ�������������Զ���
    ShapeSink& AddShape(ShapePtr input, const Matrix3x2* input_matrix = nullptr);

    /// \~chinese
    /// @brief ��ʼ���·��
    /// @param begin_pos ·����ʼ��
    /// @note ����δ�������������Զ���
    ShapeSink& BeginPath(Point const& begin_pos = Point());

    /// \~chinese
    /// @brief ����·��
    /// @param closed ·���Ƿ�պ�
    ShapeSink& EndPath(bool closed = false);

    /// \~chinese
    /// @brief ���һ���߶�
    /// @param point �˵�
    ShapeSink& AddLine(Point const& point);

    /// \~chinese
    /// @brief ��Ӷ����߶�
    /// @param points �˵㼯��
    ShapeSink& AddLines(Vector<Point> const& points);

    /// \~chinese
    /// @brief ��Ӷ����߶�
    /// @param points �˵�����
    /// @param count �˵�����
    ShapeSink& AddLines(const Point* points, size_t count);

    /// \~chinese
    /// @brief ���һ�����η�����������
    /// @param point1 ���������ߵĵ�һ�����Ƶ�
    /// @param point2 ���������ߵĵڶ������Ƶ�
    /// @param point3 ���������ߵ��յ�
    ShapeSink& AddBezier(Point const& point1, Point const& point2, Point const& point3);

    /// \~chinese
    /// @brief ��ӻ���
    /// @param point �յ�
    /// @param radius ��Բ�뾶
    /// @param rotation ��Բ��ת�Ƕ�
    /// @param clockwise ˳ʱ�� or ��ʱ��
    /// @param is_small �Ƿ�ȡС�� 180�� �Ļ�
    ShapeSink& AddArc(Point const& point, Size const& radius, float rotation, bool clockwise = true,
                         bool is_small = true);

    /// \~chinese
    /// @brief �����״������������������
    /// @param shape_a �������״A
    /// @param shape_b �������״B
    /// @param mode ��Ϸ�ʽ
    /// @param matrix Ӧ�õ�������״B�ϵĶ�ά�任
    /// @note ����δ�������������Զ���
    ShapeSink& Combine(ShapePtr shape_a, ShapePtr shape_b, CombineMode mode, const Matrix3x2* matrix = nullptr);

    /// \~chinese
    /// @brief ���ͼ��
    void Clear();

private:
    ShapePtr                  shape_;

#if defined(KGE_WIN32)
public:
    ComPtr<ID2D1PathGeometry> GetPathGeometry() const;

    void SetPathGeometry(ComPtr<ID2D1PathGeometry> path);

    ComPtr<ID2D1GeometrySink> GetGeometrySink() const;

    void SetGeometrySink(ComPtr<ID2D1GeometrySink> sink);

private:
    ComPtr<ID2D1GeometrySink> sink_;
    ComPtr<ID2D1PathGeometry> path_geo_;
#endif
};

/** @} */

#if defined(KGE_WIN32)
inline ComPtr<ID2D1PathGeometry> ShapeSink::GetPathGeometry() const
{
    return path_geo_;
}

inline void ShapeSink::SetPathGeometry(ComPtr<ID2D1PathGeometry> path)
{
    path_geo_ = path;
}

inline ComPtr<ID2D1GeometrySink> ShapeSink::GetGeometrySink() const
{
    return sink_;
}

inline void ShapeSink::SetGeometrySink(ComPtr<ID2D1GeometrySink> sink)
{
    sink_ = sink;
}
#endif

}  // namespace kiwano
