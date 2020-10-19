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
#include <kiwano/core/Function.h>
#include <kiwano/math/Vec2.hpp>
#include <kiwano/math/Rect.hpp>
#include <kiwano/math/Transform.hpp>

namespace kiwano
{
namespace math
{


template <typename _Method = void>
struct InterpolateMethod;

template <>
struct InterpolateMethod<void>
{
    inline float operator()(float frac) const noexcept
    {
        return frac;
    }
};

template <>
struct InterpolateMethod<float (*)(float)>
{
    float (*method)(float);

    InterpolateMethod(float (*method)(float)) noexcept
        : method(method)
    {
    }

    inline float operator()(float frac) const noexcept
    {
        if (method)
            return method(frac);
        return frac;
    }
};

template <>
struct InterpolateMethod<Function<float(float)>>
{
    Function<float(float)> method;

    InterpolateMethod(const Function<float(float)>& method) noexcept
        : method(method)
    {
    }

    inline float operator()(float frac) const noexcept
    {
        if (method)
            return method(frac);
        return frac;
    }
};

template <typename _Ty>
class Interpolator;

template <typename _Ty>
class Interpolator
{
public:
    template <typename _Method = void>
    inline _Ty Interpolate(_Ty start, _Ty end, float frac, const InterpolateMethod<_Method>& method = {})
    {
        if (frac >= 1)
            return end;
        return start + static_cast<_Ty>(static_cast<float>(end - start) * method(frac));
    }
};

template <typename _Ty>
class Interpolator<Vec2T<_Ty>>
{
public:
    template <typename _Method = void>
    inline Vec2T<_Ty> Interpolate(const Vec2T<_Ty>& start, const Vec2T<_Ty>& end, float frac,
                                  const InterpolateMethod<_Method>& method = {})
    {
        if (frac >= 1)
            return end;

        Interpolator<_Ty> fi;
        return Vec2T<_Ty>{
            fi.Interpolate(start.x, end.x, frac, method),
            fi.Interpolate(start.y, end.y, frac, method)
        };
    }
};

template <typename _Ty>
class Interpolator<RectT<_Ty>>
{
public:
    template <typename _Method = void>
    inline RectT<_Ty> Interpolate(const RectT<_Ty>& start, const RectT<_Ty>& end, float frac,
                                  const InterpolateMethod<_Method>& method = {})
    {
        if (frac >= 1)
            return end;

        Interpolator<Vec2T<_Ty>> vi;
        return RectT<_Ty>{
            vi.Interpolate(start.left_top, end.left_top, frac, method),
            vi.Interpolate(start.right_bottom, end.right_bottom, frac, method)
        };
    }
};

template <typename _Ty>
class Interpolator<TransformT<_Ty>>
{
public:
    template <typename _Method = void>
    inline TransformT<_Ty> Interpolate(const TransformT<_Ty>& start, const TransformT<_Ty>& end, float frac,
                                       const InterpolateMethod<_Method>& method = {})
    {
        if (frac >= 1)
            return end;

        Interpolator<_Ty>        fi;
        Interpolator<Vec2T<_Ty>> vi;

        TransformT<_Ty> transform;
        transform.rotation = fi.Interpolate(start.rotation, end.rotation, frac, method);
        transform.position = vi.Interpolate(start.position, end.position, frac, method);
        transform.scale    = vi.Interpolate(start.scale, end.scale, frac, method);
        transform.skew     = vi.Interpolate(start.skew, end.skew, frac, method);
        return transform;
    }
};

}  // namespace math
}  // namespace kiwano
