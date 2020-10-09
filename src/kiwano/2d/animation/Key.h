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
#include <kiwano/2d/animation/Interpolator.h>

namespace kiwano
{

template <typename _Ty, typename _NotifierTy>
class KeyValue
{
public:
    typedef _Ty                     value_type;
    typedef Value<_Ty, _NotifierTy> property_type;

    inline KeyValue(const property_type& prop, value_type end_value)
        : end_value_(end_value)
        , prop_(prop)
        , interpolator_()
    {
    }

    inline KeyValue(const property_type& prop, value_type end_value, const EaseFunc& ease_func)
        : end_value_(end_value)
        , prop_(prop)
        , interpolator_(ease_func)
    {
    }

    inline property_type GetProperty() const
    {
        return prop_;
    }

    inline value_type GetEndValue() const
    {
        return end_value_;
    }

    inline const Interpolator& GetInterpolator() const
    {
        return interpolator_;
    }

private:
    property_type prop_;
    value_type    end_value_;
    Interpolator  interpolator_;
};

}
