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
/// @brief �����˵���ʽ
/// @details �����˵���ʽ��ʾ�߶ζ˵㲿�ֵ���״
enum class CapStyle
{
    Flat,      ///< ��˵�
    Square,    ///< ���ζ˵㣬����ͻ�����ֵ����߶ο�ȵ�һ��
    Round,     ///< Բ�ζ˵㣬Բֱ�������߶ο��
    Triangle,  ///< ������ʽ������б�߳��ȵ����߶ο��
};

/// \~chinese
/// @brief ����������ʽ
/// @details ����������ʽ��ʾ�������ཻ���ֵ���״
enum class LineJoinStyle
{
    Miter,  ///< б����ʽ
    Bevel,  ///< б����ʽ
    Round   ///< Բ����ʽ
};

/// \~chinese
/// @brief ����������ʽ
/// @details ����������ʽ��ʾ�߶εļ�϶
enum class DashStyle
{
    Solid,       ///< �޼�ϵ�ʵ��
    Dash,        ///< б����ʽ
    Dot,         ///< Բ����ʽ
    DashDot,     ///< Բ����ʽ
    DashDotDot,  ///< Բ����ʽ
};

/// \~chinese
/// @brief ������ʽ
class StrokeStyle : public NativeObject
{
public:
    /// \~chinese
    /// @brief ����������ʽ
    /// @param width �������
    /// @param cap �����˵���ʽ
    /// @param line_join ����������ʽ
    StrokeStyle(float width, CapStyle cap = CapStyle::Flat, LineJoinStyle line_join = LineJoinStyle::Miter);

    /// \~chinese
    /// @brief ����������ʽ
    /// @param width �������
    /// @param cap �����˵���ʽ
    /// @param line_join ����������ʽ
    /// @param dash ����������ʽ
    /// @param dash_offset ��������ƫ����
    StrokeStyle(float width, CapStyle cap, LineJoinStyle line_join, DashStyle dash, float dash_offset = 0.0f);

    /// \~chinese
    /// @brief ����������ʽ
    /// @param width �������
    /// @param cap �����˵���ʽ
    /// @param line_join ����������ʽ
    /// @param dash_array �������ߵĳ������϶����
    /// @param dash_size �������������С
    /// @param dash_offset ��������ƫ����
    StrokeStyle(float width, CapStyle cap, LineJoinStyle line_join, const float* dash_array, size_t dash_size,
                float dash_offset = 0.0f);

    /// \~chinese
    /// @brief ����������ʽ
    /// @tparam _DashSize �������������С
    /// @param width �������
    /// @param cap �����˵���ʽ
    /// @param line_join ����������ʽ
    /// @param dash_array �������ߵĳ������϶����
    /// @param dash_offset ��������ƫ����
    template <size_t _DashSize>
    StrokeStyle(float width, CapStyle cap, LineJoinStyle line_join, float (&dash_array)[_DashSize],
                float dash_offset = 0.0f)
        : StrokeStyle(width, cap, line_join, dash_array, _DashSize, dash_offset)
    {
    }

    StrokeStyle();

    /// \~chinese
    /// @brief ��ȡ�������
    float GetWidth() const;

    /// \~chinese
    /// @brief ��ȡ�����˵���ʽ
    CapStyle GetCapStyle() const;

    /// \~chinese
    /// @brief ��ȡ����������ʽ
    LineJoinStyle GetLineJoinStyle() const;

    /// \~chinese
    /// @brief ��ȡ�������ߵĳ������϶����
    const Vector<float>& GetDashArray() const;

    /// \~chinese
    /// @brief ��ȡ����ƫ����
    float GetDashOffset() const;

    /// \~chinese
    /// @brief �����������
    /// @param width �������
    void SetWidth(float width);

    /// \~chinese
    /// @brief ���������˵���ʽ
    void SetCapStyle(CapStyle cap);

    /// \~chinese
    /// @brief ��������������ʽ
    void SetLineJoinStyle(LineJoinStyle line_join);

    /// \~chinese
    /// @brief ����������ʽ
    /// @param dash_style ����������ʽ
    void SetDashStyle(DashStyle dash_style);

    /// \~chinese
    /// @brief ����������ʽ
    /// @param dash_array �������ߵĳ������϶����
    void SetDashStyle(const Vector<float>& dash_array);

    /// \~chinese
    /// @brief ����������ʽ
    /// @param dash_array �������ߵĳ������϶����
    /// @param dash_size �������������С
    void SetDashStyle(const float* dash_array, size_t dash_size);

    /// \~chinese
    /// @brief ����������ʽ
    /// @tparam _DashSize �������������С
    /// @param dash_array �������ߵĳ������϶����
    template <size_t _DashSize>
    inline void SetDashStyle(float (&dash_array)[_DashSize])
    {
        SetDashStyle(dash_array, _DashSize);
    }

    /// \~chinese
    /// @brief ��������ƫ����
    /// @param dash_offset ��������ƫ����
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
