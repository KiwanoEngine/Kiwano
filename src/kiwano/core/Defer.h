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

namespace kiwano
{

class Defer
{
public:
    Defer() = default;

    Defer(const Function<void()>& func)
        : func_(func)
    {
    }

    Defer(Defer&& other) noexcept
        : func_(other.func_)
    {
        other.func_ = nullptr;
    }

    ~Defer()
    {
        if (func_)
            func_();
    }

private:
    Defer(const Defer&)            = delete;
    Defer& operator=(const Defer&) = delete;

    Function<void()> func_;
};

class __DeferHelper
{
public:
    Defer operator-(const Function<void()>& func) const
    {
        return Defer{ func };
    }
};

#define KGE_DEFER auto __KGE_DEFER_VAR(__defer_line_, __LINE__, __) = ::kiwano::__DeferHelper() -
#define __KGE_DEFER_VAR(a, b, c) __KGE_DEFER_TOKEN_CONNECT(a, b, c)
#define __KGE_DEFER_TOKEN_CONNECT(a, b, c) a##b##c

}  // namespace kiwano
