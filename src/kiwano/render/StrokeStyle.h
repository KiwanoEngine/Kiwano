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
#include <kiwano/platform/NativeObject.hpp>

namespace kiwano
{

/**
 * \addtogroup Render
 * @{
 */

/// \~chinese
/// @brief 线条端点样式
/// @details 线条端点样式表示线段端点部分的形状
enum class CapStyle
{
    Flat,      ///< 扁端点
    Square,    ///< 方形端点，方形突出部分等于线段宽度的一半
    Round,     ///< 圆形端点，圆直径等于线段宽度
    Triangle,  ///< 三角样式，三角斜边长度等于线段宽度
};

/// \~chinese
/// @brief 线条交点样式
/// @details 线条交点样式表示两条线相交部分的形状
enum class LineJoinStyle
{
    Miter,  ///< 斜切样式
    Bevel,  ///< 斜角样式
    Round   ///< 圆角样式
};

/// \~chinese
/// @brief 线条虚线样式
/// @details 线条虚线样式表示线段的间隙
enum class DashStyle
{
    Solid,       ///< 无间断的实线
    Dash,        ///< 斜角样式
    Dot,         ///< 圆角样式
    DashDot,     ///< 圆角样式
    DashDotDot,  ///< 圆角样式
};

/// \~chinese
/// @brief 线条样式
class StrokeStyle : public NativeObject
{
public:
    /// \~chinese
    /// @brief 创建线条样式
    /// @param width 线条宽度
    /// @param cap 线条端点样式
    /// @param line_join 线条交点样式
    StrokeStyle(float width, CapStyle cap = CapStyle::Flat, LineJoinStyle line_join = LineJoinStyle::Miter);

    /// \~chinese
    /// @brief 创建线条样式
    /// @param width 线条宽度
    /// @param cap 线条端点样式
    /// @param line_join 线条交点样式
    /// @param dash 线条虚线样式
    /// @param dash_offset 线条虚线偏移量
    StrokeStyle(float width, CapStyle cap, LineJoinStyle line_join, DashStyle dash, float dash_offset = 0.0f);

    /// \~chinese
    /// @brief 创建线条样式
    /// @param width 线条宽度
    /// @param cap 线条端点样式
    /// @param line_join 线条交点样式
    /// @param dash_array 线条虚线的长度与间隙数组
    /// @param dash_size 线条虚线数组大小
    /// @param dash_offset 线条虚线偏移量
    StrokeStyle(float width, CapStyle cap, LineJoinStyle line_join, const float* dash_array, size_t dash_size,
                float dash_offset = 0.0f);

    /// \~chinese
    /// @brief 创建线条样式
    /// @tparam _DashSize 线条虚线数组大小
    /// @param width 线条宽度
    /// @param cap 线条端点样式
    /// @param line_join 线条交点样式
    /// @param dash_array 线条虚线的长度与间隙数组
    /// @param dash_offset 线条虚线偏移量
    template <size_t _DashSize>
    StrokeStyle(float width, CapStyle cap, LineJoinStyle line_join, float (&dash_array)[_DashSize],
                float dash_offset = 0.0f)
        : StrokeStyle(width, cap, line_join, dash_array, _DashSize, dash_offset)
    {
    }

    StrokeStyle();

    /// \~chinese
    /// @brief 获取线条宽度
    float GetWidth() const;

    /// \~chinese
    /// @brief 获取线条端点样式
    CapStyle GetCapStyle() const;

    /// \~chinese
    /// @brief 获取线条交点样式
    LineJoinStyle GetLineJoinStyle() const;

    /// \~chinese
    /// @brief 获取线条虚线的长度与间隙数组
    const Vector<float>& GetDashArray() const;

    /// \~chinese
    /// @brief 获取虚线偏移量
    float GetDashOffset() const;

    /// \~chinese
    /// @brief 设置线条宽度
    /// @param width 线条宽度
    void SetWidth(float width);

    /// \~chinese
    /// @brief 设置线条端点样式
    void SetCapStyle(CapStyle cap);

    /// \~chinese
    /// @brief 设置线条交点样式
    void SetLineJoinStyle(LineJoinStyle line_join);

    /// \~chinese
    /// @brief 设置虚线样式
    /// @param dash_style 线条虚线样式
    void SetDashStyle(DashStyle dash_style);

    /// \~chinese
    /// @brief 设置虚线样式
    /// @param dash_array 线条虚线的长度与间隙数组
    void SetDashStyle(const Vector<float>& dash_array);

    /// \~chinese
    /// @brief 设置虚线样式
    /// @param dash_array 线条虚线的长度与间隙数组
    /// @param dash_size 线条虚线数组大小
    void SetDashStyle(const float* dash_array, size_t dash_size);

    /// \~chinese
    /// @brief 设置虚线样式
    /// @tparam _DashSize 线条虚线数组大小
    /// @param dash_array 线条虚线的长度与间隙数组
    template <size_t _DashSize>
    inline void SetDashStyle(float (&dash_array)[_DashSize])
    {
        SetDashStyle(dash_array, _DashSize);
    }

    /// \~chinese
    /// @brief 设置虚线偏移量
    /// @param dash_offset 线条虚线偏移量
    void SetDashOffset(float dash_offset);

private:
    CapStyle      cap_;
    LineJoinStyle line_join_;
    float         stroke_width_;
    float         dash_offset_;
    Vector<float> dash_array_;
};

/** @} */

inline float StrokeStyle::GetWidth() const
{
    return stroke_width_;
}

inline CapStyle StrokeStyle::GetCapStyle() const
{
    return cap_;
}

inline LineJoinStyle StrokeStyle::GetLineJoinStyle() const
{
    return line_join_;
}

inline const Vector<float>& StrokeStyle::GetDashArray() const
{
    return dash_array_;
}

inline float StrokeStyle::GetDashOffset() const
{
    return dash_offset_;
}

inline void StrokeStyle::SetWidth(float width)
{
    stroke_width_ = width;
}

inline void StrokeStyle::SetCapStyle(CapStyle cap)
{
    cap_ = cap;
}

inline void StrokeStyle::SetLineJoinStyle(LineJoinStyle line_join)
{
    line_join_ = line_join;
}

inline void StrokeStyle::SetDashOffset(float dash_offset)
{
    dash_offset_ = dash_offset;
}

}  // namespace kiwano
