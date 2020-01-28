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
#include <kiwano/math/math.h>
#include <kiwano/render/Color.h>
#include <kiwano/platform/win32/ComPtr.hpp>
#include <kiwano/platform/win32/helper.h>
#include <d2d1.h>

namespace kiwano
{
namespace DX
{
template <typename T>
inline void SafeRelease(T*& ptr)
{
    if (ptr != nullptr)
    {
        ptr->Release();
        ptr = nullptr;
    }
}

template <typename T>
inline T* SafeAcquire(T* ptr)
{
    if (ptr != nullptr)
    {
        ptr->AddRef();
    }
    return ptr;
}

//
// Point2F
//

inline D2D1_POINT_2F const& ConvertToPoint2F(Vec2 const& vec2)
{
    return reinterpret_cast<D2D1_POINT_2F const&>(vec2);
}

inline D2D1_POINT_2F& ConvertToPoint2F(Vec2& vec2)
{
    return reinterpret_cast<D2D1_POINT_2F&>(vec2);
}

inline const D2D1_POINT_2F* ConvertToPoint2F(const Vec2* vec2)
{
    return reinterpret_cast<const D2D1_POINT_2F*>(vec2);
}

inline D2D1_POINT_2F* ConvertToPoint2F(Vec2* vec2)
{
    return reinterpret_cast<D2D1_POINT_2F*>(vec2);
}

//
// SizeF
//

inline D2D1_SIZE_F const& ConvertToSizeF(Vec2 const& vec2)
{
    return reinterpret_cast<D2D1_SIZE_F const&>(vec2);
}

inline D2D1_SIZE_F& ConvertToSizeF(Vec2& vec2)
{
    return reinterpret_cast<D2D1_SIZE_F&>(vec2);
}

inline const D2D1_SIZE_F* ConvertToSizeF(const Vec2* vec2)
{
    return reinterpret_cast<const D2D1_SIZE_F*>(vec2);
}

inline D2D1_SIZE_F* ConvertToSizeF(Vec2* vec2)
{
    return reinterpret_cast<D2D1_SIZE_F*>(vec2);
}

//
// RectF
//

inline D2D1_RECT_F const& ConvertToRectF(Rect const& rect)
{
    return reinterpret_cast<D2D1_RECT_F const&>(rect);
}

inline D2D1_RECT_F& ConvertToRectF(Rect& rect)
{
    return reinterpret_cast<D2D1_RECT_F&>(rect);
}

inline const D2D1_RECT_F* ConvertToRectF(const Rect* rect)
{
    return reinterpret_cast<const D2D1_RECT_F*>(rect);
}

inline D2D1_RECT_F* ConvertToRectF(Rect* rect)
{
    return reinterpret_cast<D2D1_RECT_F*>(rect);
}

//
// ColorF
//
inline D2D1_COLOR_F const& ConvertToColorF(Color const& color)
{
    return reinterpret_cast<D2D1_COLOR_F const&>(color);
}

inline D2D1_COLOR_F& ConvertToColorF(Color& color)
{
    return reinterpret_cast<D2D1_COLOR_F&>(color);
}

inline const D2D1_COLOR_F* ConvertToColorF(const Color* color)
{
    return reinterpret_cast<const D2D1_COLOR_F*>(color);
}

inline D2D1_COLOR_F* ConvertToColorF(Color* color)
{
    return reinterpret_cast<D2D1_COLOR_F*>(color);
}

//
// MatrixF
//

inline D2D1_MATRIX_3X2_F const& ConvertToMatrix3x2F(Matrix3x2 const& matrix)
{
    return reinterpret_cast<D2D1_MATRIX_3X2_F const&>(matrix);
}

inline D2D1_MATRIX_3X2_F& ConvertToMatrix3x2F(Matrix3x2& matrix)
{
    return reinterpret_cast<D2D1_MATRIX_3X2_F&>(matrix);
}

inline const D2D1_MATRIX_3X2_F* ConvertToMatrix3x2F(const Matrix3x2* matrix)
{
    return reinterpret_cast<const D2D1_MATRIX_3X2_F*>(matrix);
}

inline D2D1_MATRIX_3X2_F* ConvertToMatrix3x2F(Matrix3x2* matrix)
{
    return reinterpret_cast<D2D1_MATRIX_3X2_F*>(matrix);
}

// Converts a length in device-independent pixels (DIPs) to a length in physical pixels.
inline float ConvertDipsToPixels(float dips, float dpi)
{
    static const float dips_per_inch = 96.0f;
    return math::Floor(dips * dpi / dips_per_inch + 0.5f);  // Round to nearest integer.
}
}  // namespace DX
}  // namespace kiwano
