// Copyright (c) 2019-2020 Kiwano - Nomango
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
#include <kiwano/core/Common.h>
#include <kiwano/math/Math.h>
#include <kiwano/2d/animation/EaseFunc.h>

namespace kiwano
{

#define KGE_INTERPOLATOR_NUMRIC_INTERPOLATE_IMPL(TYPE)        \
    inline TYPE Interpolate(TYPE start, TYPE end, float frac) \
    {                                                         \
        return NumricInterpolate(start, end, Fraction(frac)); \
    }

class KGE_API Interpolator
{
public:
    inline Interpolator() {}

    inline Interpolator(const EaseFunc& ease_func)
        : ease_func_(ease_func)
    {
    }

    KGE_INTERPOLATOR_NUMRIC_INTERPOLATE_IMPL(int)
    KGE_INTERPOLATOR_NUMRIC_INTERPOLATE_IMPL(unsigned)
    KGE_INTERPOLATOR_NUMRIC_INTERPOLATE_IMPL(float)
    KGE_INTERPOLATOR_NUMRIC_INTERPOLATE_IMPL(double)

    template <typename _Ty>
    inline _Ty Interpolate(_Ty start, _Ty end, float frac)
    {
        if (NumricInterpolate(0, 1, Fraction(frac)) == 1)
            return end;
        return start;
    }

private:
    inline float Fraction(float frac)
    {
        if (ease_func_)
        {
            return ease_func_(frac);
        }
        return frac;
    }

    template <typename _Ty>
    inline _Ty NumricInterpolate(_Ty start, _Ty end, float frac)
    {
        return start + static_cast<_Ty>(math::Floor(static_cast<float>(end - start) * frac));
    }

private:
    EaseFunc ease_func_;
};

}
