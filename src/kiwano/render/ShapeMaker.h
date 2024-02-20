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
/// @brief 形状合并方式
enum class CombineMode
{
    Union,      ///< 并集 (A + B)
    Intersect,  ///< 交集 (A + B)
    Xor,        ///< 对称差集 ((A - B) + (B - A))
    Exclude     ///< 差集 (A - B)
};

/// \~chinese
/// @brief 形状生成器
class KGE_API ShapeMaker : public NativeObject
{
public:
    ShapeMaker();

    ~ShapeMaker();

    /// \~chinese
    /// @brief 获取生成的形状
    RefPtr<Shape> GetShape();

    /// \~chinese
    /// @brief 清空图形
    void Clear();

    /// \~chinese
    /// @brief 开始添加路径并打开输入流
    /// @param begin_pos 路径起始点
    void BeginPath(const Point& begin_pos = Point());

    /// \~chinese
    /// @brief 结束路径并关闭输入流
    /// @param closed 路径是否闭合
    void EndPath(bool closed = false);

    /// \~chinese
    /// @brief 添加一条线段
    /// @param point 端点
    void AddLine(const Point& point);

    /// \~chinese
    /// @brief 添加多条线段
    /// @param points 端点集合
    void AddLines(const Vector<Point>& points);

    /// \~chinese
    /// @brief 添加多条线段
    /// @param points 端点数组
    /// @param count 端点数量
    void AddLines(const Point* points, size_t count);

    /// \~chinese
    /// @brief 添加一条三次方贝塞尔曲线
    /// @param point1 贝塞尔曲线的第一个控制点
    /// @param point2 贝塞尔曲线的第二个控制点
    /// @param point3 贝塞尔曲线的终点
    void AddBezier(const Point& point1, const Point& point2, const Point& point3);

    /// \~chinese
    /// @brief 添加弧线
    /// @param point 终点
    /// @param radius 椭圆半径
    /// @param rotation 椭圆旋转角度
    /// @param clockwise 顺时针 or 逆时针
    /// @param is_small 是否取小于 180° 的弧
    void AddArc(const Point& point, const Size& radius, float rotation, bool clockwise = true, bool is_small = true);

    /// \~chinese
    /// @brief 合并形状
    /// @param shape_a 输入的形状A
    /// @param shape_b 输入的形状B
    /// @param mode 合并方式
    /// @param matrix 应用到输入形状B上的二维变换
    /// @return 返回合并后的形状
    static RefPtr<Shape> Combine(RefPtr<Shape> shape_a, RefPtr<Shape> shape_b, CombineMode mode,
                                 const Matrix3x2* matrix = nullptr);

    /// \~chinese
    /// @brief 设置生成的形状
    /// @note 应由系统调用该函数
    void SetShape(RefPtr<Shape> shape);

private:
    /// \~chinese
    /// @brief 打开输入流
    /// @note 应由系统调用该函数
    void OpenStream();

    /// \~chinese
    /// @brief 关闭输入流
    /// @note 应由系统调用该函数
    void CloseStream();

    /// \~chinese
    /// @brief 输入流是否已经打开
    /// @note 应由系统调用该函数
    bool IsStreamOpened() const;

private:
    RefPtr<Shape> shape_;
};

/** @} */

}  // namespace kiwano
