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
#include <kiwano/core/ObjectBase.h>
#include <kiwano/render/DirectX/D2DDeviceResources.h>

namespace kiwano
{

KGE_DECLARE_SMART_PTR(StrokeStyle);

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
class StrokeStyle : public virtual ObjectBase
{
public:
    /// \~chinese
    /// @brief 创建线条样式
    /// @param cap 线条端点样式
    /// @param line_join 线条交点样式
    /// @param dash 线条虚线样式
    /// @param dash_offset 线条虚线偏移量
    static StrokeStylePtr Create(CapStyle cap, LineJoinStyle line_join = LineJoinStyle::Miter,
                                 DashStyle dash = DashStyle::Solid, float dash_offset = 0.0f);

    /// \~chinese
    /// @brief 创建线条样式
    /// @param cap 线条端点样式
    /// @param line_join 线条交点样式
    /// @param dash_array 线条虚线的长度与间隙数组
    /// @param dash_size 线条虚线数组大小
    /// @param dash_offset 线条虚线偏移量
    static StrokeStylePtr Create(CapStyle cap, LineJoinStyle line_join = LineJoinStyle::Miter,
                                 const float* dash_array = nullptr, size_t dash_size = 0, float dash_offset = 0.0f);

    /// \~chinese
    /// @brief 创建线条样式
    /// @tparam _DashSize 线条虚线数组大小
    /// @param cap 线条端点样式
    /// @param line_join 线条交点样式
    /// @param dash_array 线条虚线的长度与间隙数组
    /// @param dash_offset 线条虚线偏移量
    template <size_t _DashSize>
    static inline StrokeStylePtr Create(CapStyle cap, LineJoinStyle line_join = LineJoinStyle::Miter,
                                        float (&dash_array)[_DashSize] = nullptr, float dash_offset = 0.0f)
    {
        return StrokeStyle::Create(cap, line_join, dash_array, _DashSize, dash_offset);
    }

    StrokeStyle();

    /// \~chinese
    /// @brief 是否有效
    bool IsValid() const;

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
    float         dash_offset_;
    Vector<float> dash_array_;

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
public:
    ComPtr<ID2D1StrokeStyle> GetStrokeStyle() const;

    void SetStrokeStyle(ComPtr<ID2D1StrokeStyle> style);

private:
    ComPtr<ID2D1StrokeStyle> style_;
#endif
};

/** @} */

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

inline void StrokeStyle::SetCapStyle(CapStyle cap)
{
    cap_ = cap;
    style_.reset();
}

inline void StrokeStyle::SetLineJoinStyle(LineJoinStyle line_join)
{
    line_join_ = line_join;
    style_.reset();
}

inline void StrokeStyle::SetDashOffset(float dash_offset)
{
    dash_offset_ = dash_offset;
    style_.reset();
}

inline bool StrokeStyle::IsValid() const
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    return style_ != nullptr;
#else
    return false;  // not supported
#endif
}

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
inline void StrokeStyle::SetStrokeStyle(ComPtr<ID2D1StrokeStyle> style)
{
    style_ = style;
}
#endif

}  // namespace kiwano
