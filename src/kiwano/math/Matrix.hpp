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
#include <algorithm>
#include <kiwano/math/Rect.hpp>
#include <kiwano/math/Vec2.hpp>

namespace kiwano
{
namespace math
{
template <typename _Ty, typename _Lty, typename _Rty>
struct MatrixMultiply;

template <typename _Ty>
struct Matrix3x2T
{
    using ValueType = _Ty;
    using Vec2Type  = Vec2T<ValueType>;
    using RectType  = RectT<ValueType>;

    union {
        struct
        {
            _Ty m[6];  // m[3][2]
        };

        struct
        {
            _Ty _11, _12, _21, _22, _31, _32;
        };
    };

    Matrix3x2T()
        : _11(1.f)
        , _12(0.f)
        , _21(0.f)
        , _22(1.f)
        , _31(0.f)
        , _32(0.f)
    {
    }

    Matrix3x2T(ValueType _11, ValueType _12, ValueType _21, ValueType _22, ValueType _31, ValueType _32)
        : _11(_11)
        , _12(_12)
        , _21(_21)
        , _22(_22)
        , _31(_31)
        , _32(_32)
    {
    }

    explicit Matrix3x2T(const ValueType* p)
    {
        for (int i = 0; i < 6; i++)
            m[i] = p[i];
    }

    Matrix3x2T(const Matrix3x2T& other)
        : _11(other._11)
        , _12(other._12)
        , _21(other._21)
        , _22(other._22)
        , _31(other._31)
        , _32(other._32)
    {
    }

KGE_SUPPRESS_WARNING_PUSH
KGE_SUPPRESS_WARNING(26495)  // ignore warning "always initialize member variables"

    template <typename _MTy>
    Matrix3x2T(const _MTy& other)
    {
        for (int i = 0; i < 6; i++)
            m[i] = other[i];
    }

KGE_SUPPRESS_WARNING_POP

    inline ValueType operator[](uint32_t index) const
    {
        return m[index];
    }

    inline ValueType& operator[](uint32_t index)
    {
        return m[index];
    }

    inline Matrix3x2T& operator=(const Matrix3x2T& other)
    {
        for (int i = 0; i < 6; i++)
            m[i] = other[i];
        return (*this);
    }

    template <typename _Lty, typename _Rty>
    inline Matrix3x2T& operator=(MatrixMultiply<ValueType, _Lty, const _Rty>& other)
    {
        Matrix3x2T result(other);
        (*this) = result;
        return (*this);
    }

    inline Matrix3x2T& operator*=(const Matrix3x2T& other)
    {
        return operator=((*this) * other);
    }

    inline void Identity()
    {
        _11 = 1.f;
        _12 = 0.f;
        _21 = 0.f;
        _22 = 1.f;
        _31 = 0.f;
        _32 = 0.f;
    }

    inline bool IsIdentity() const
    {
        return _11 == 1.f && _12 == 0.f && _21 == 0.f && _22 == 1.f && _31 == 0.f && _32 == 0.f;
    }

    inline Matrix3x2T Invert() const
    {
        ValueType det = 1.f / Determinant();
        return Matrix3x2T(det * _22, -det * _12, -det * _21, det * _11, det * (_21 * _32 - _22 * _31),
                          det * (_12 * _31 - _11 * _32));
    }

    inline bool IsInvertible() const
    {
        return 0 != Determinant();
    }

    inline ValueType Determinant() const
    {
        return (_11 * _22) - (_12 * _21);
    }

    inline Vec2Type Transform(const Vec2Type& v) const
    {
        return Vec2Type(v.x * _11 + v.y * _21 + _31, v.x * _12 + v.y * _22 + _32);
    }

    RectType Transform(const RectType& rect) const
    {
        Vec2Type top_left     = Transform(rect.GetLeftTop());
        Vec2Type top_right    = Transform(rect.GetRightTop());
        Vec2Type bottom_left  = Transform(rect.GetLeftBottom());
        Vec2Type bottom_right = Transform(rect.GetRightBottom());

        ValueType left   = std::min(std::min(top_left.x, top_right.x), std::min(bottom_left.x, bottom_right.x));
        ValueType right  = std::max(std::max(top_left.x, top_right.x), std::max(bottom_left.x, bottom_right.x));
        ValueType top    = std::min(std::min(top_left.y, top_right.y), std::min(bottom_left.y, bottom_right.y));
        ValueType bottom = std::max(std::max(top_left.y, top_right.y), std::max(bottom_left.y, bottom_right.y));

        return RectType{ left, top, right, bottom };
    }

    inline void Translate(const Vec2Type& v)
    {
        _31 += _11 * v.x + _21 * v.y;
        _32 += _12 * v.x + _22 * v.y;
    }

    static inline Matrix3x2T Translation(const Vec2Type& v)
    {
        return Matrix3x2T(1.f, 0.f, 0.f, 1.f, v.x, v.y);
    }

    static inline Matrix3x2T Scaling(const Vec2Type& v)
    {
        return Matrix3x2T(v.x, 0.f, 0.f, v.y, 0.f, 0.f);
    }

    static inline Matrix3x2T Scaling(const Vec2Type& v, const Vec2Type& center)
    {
        return Matrix3x2T(v.x, 0.f, 0.f, v.y, center.x - v.x * center.x, center.y - v.y * center.y);
    }

    static inline Matrix3x2T Rotation(ValueType angle)
    {
        ValueType s = math::Sin(angle);
        ValueType c = math::Cos(angle);
        return Matrix3x2T(c, s, -s, c, 0.f, 0.f);
    }

    static inline Matrix3x2T Rotation(ValueType angle, const Vec2Type& center)
    {
        ValueType s = math::Sin(angle);
        ValueType c = math::Cos(angle);
        return Matrix3x2T(c, s, -s, c, center.x * (1 - c) + center.y * s, center.y * (1 - c) - center.x * s);
    }

    static inline Matrix3x2T SRT(const Vec2Type& trans, const Vec2Type& scale, ValueType angle)
    {
        ValueType s = math::Sin(angle);
        ValueType c = math::Cos(angle);
        return Matrix3x2T(c * scale.x, s * scale.x, -s * scale.y, c * scale.y, trans.x, trans.y);
    }

    static inline Matrix3x2T Skewing(const Vec2Type& angle)
    {
        ValueType tx = math::Tan(angle.x);
        ValueType ty = math::Tan(angle.y);
        return Matrix3x2T(1.f, -ty, -tx, 1.f, 0.f, 0.f);
    }

    static inline Matrix3x2T Skewing(const Vec2Type& angle, const Vec2Type& center)
    {
        ValueType tx = math::Tan(angle.x);
        ValueType ty = math::Tan(angle.y);
        return Matrix3x2T(1.f, -ty, -tx, 1.f, center.y * tx, center.x * ty);
    }
};

// Use template expression to optimize matrix multiply
template <typename _Ty, typename _Lty, typename _Rty>
struct MatrixMultiply
{
    const _Lty& lhs;
    const _Rty& rhs;

    MatrixMultiply(const _Lty& lhs, const _Rty& rhs)
        : lhs(lhs)
        , rhs(rhs)
    {
    }

    inline _Ty operator[](uint32_t index) const
    {
        switch (index)
        {
        case 0:
            return lhs[0] * rhs[0] + lhs[1] * rhs[2];
        case 1:
            return lhs[0] * rhs[1] + lhs[1] * rhs[3];
        case 2:
            return lhs[2] * rhs[0] + lhs[3] * rhs[2];
        case 3:
            return lhs[2] * rhs[1] + lhs[3] * rhs[3];
        case 4:
            return lhs[4] * rhs[0] + lhs[5] * rhs[2] + rhs[4];
        case 5:
            return lhs[4] * rhs[1] + lhs[5] * rhs[3] + rhs[5];
        default:
            return 0.f;
        }
    }
};

template <typename _Ty>
inline MatrixMultiply<_Ty, Matrix3x2T<_Ty>, Matrix3x2T<_Ty>> operator*(const Matrix3x2T<_Ty>& lhs,
                                                                       const Matrix3x2T<_Ty>& rhs)
{
    return MatrixMultiply<_Ty, Matrix3x2T<_Ty>, Matrix3x2T<_Ty>>(lhs, rhs);
}

template <typename _Ty, typename _Lty, typename _Rty>
inline MatrixMultiply<_Ty, MatrixMultiply<_Ty, _Lty, _Rty>, Matrix3x2T<_Ty>>
operator*(MatrixMultiply<_Ty, _Lty, const _Rty>& lhs, const Matrix3x2T<_Ty>& rhs)
{
    return MatrixMultiply<_Ty, MatrixMultiply<_Ty, _Lty, _Rty>, Matrix3x2T<_Ty>>(lhs, rhs);
}
}  // namespace math
}  // namespace kiwano
