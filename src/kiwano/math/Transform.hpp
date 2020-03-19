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
#include <kiwano/math/Vec2.hpp>
#include <kiwano/math/Matrix.hpp>

namespace kiwano
{
namespace math
{

/**
 * \~chinese
 * @brief 二维放射变换
 */
template <typename _Ty>
class TransformT
{
public:
    using ValueType = _Ty;

    float            rotation;  ///< 旋转
    Vec2T<ValueType> position;  ///< 坐标
    Vec2T<ValueType> scale;     ///< 缩放
    Vec2T<ValueType> skew;      ///< 错切角度

public:
    TransformT();

    /// \~chinese
    /// @brief 将二维放射变换转换为矩阵
    Matrix3x2T<ValueType> ToMatrix() const;

    bool operator==(const TransformT& rhs) const;
};

template <typename _Ty>
inline TransformT<_Ty>::TransformT()
    : position()
    , rotation(0.f)
    , scale(1.f, 1.f)
    , skew(0.f, 0.f)
{
}

template <typename _Ty>
Matrix3x2T<_Ty> TransformT<_Ty>::ToMatrix() const
{
    if (!skew.IsOrigin())
    {
        return Matrix3x2T<_Ty>::Skewing(skew) * Matrix3x2T<_Ty>::SRT(position, scale, rotation);
    }
    return Matrix3x2T<_Ty>::SRT(position, scale, rotation);
}

template <typename _Ty>
bool TransformT<_Ty>::operator==(const TransformT& rhs) const
{
    return position == rhs.position && rotation == rhs.rotation && scale == rhs.scale && skew == rhs.skew;
}

}  // namespace math
}  // namespace kiwano
