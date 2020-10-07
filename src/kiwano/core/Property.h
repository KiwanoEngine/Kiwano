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
#include <kiwano/core/BitOperator.h>
#include <kiwano/core/Flag.h>
#include <kiwano/core/Function.h>

namespace kiwano
{

template <typename _Ty, typename _NotifierTy = void>
class Property;


template <typename _Ty>
class Property<_Ty, void>
{
public:
    typedef _Ty  value_type;
    typedef void notifier_type;

    inline Property(_Ty* ptr)
        : ptr_(ptr)
    {
    }

    inline _Ty Get() const
    {
        return *ptr_;
    }

    inline void Set(const _Ty& value)
    {
        *ptr_ = value;
    }

private:
    _Ty* ptr_;
};


template <typename _Ty, typename _FlagTy>
class Property<_Ty, Flag<_FlagTy>>
{
public:
    typedef _Ty           value_type;
    typedef Flag<_FlagTy> notifier_type;

    inline Property(_Ty* ptr, Flag<_FlagTy>* flag, _FlagTy flag_value)
        : ptr_(ptr)
        , flag_(flag)
        , flag_value_(flag_value)
    {
    }

    inline _Ty Get() const
    {
        return *ptr_;
    }

    inline void Set(const _Ty& value)
    {
        *ptr_ = value;
        if (flag_)
        {
            flag_->Set(flag_value_);
        }
    }

private:
    _Ty*           ptr_;
    Flag<_FlagTy>* flag_;
    _FlagTy        flag_value_;
};


template <typename _Ty>
class Property<_Ty, Function<void()>>
{
public:
    typedef _Ty              value_type;
    typedef Function<void()> notifier_type;

    inline Property(_Ty* ptr, const Function<void()>& notifier)
        : ptr_(ptr)
        , notifier_(notifier)
    {
    }

    inline _Ty Get() const
    {
        return *ptr_;
    }

    inline void Set(const _Ty& value)
    {
        *ptr_ = value;
        if (notifier_)
        {
            notifier_();
        }
    }

private:
    _Ty*             ptr_;
    Function<void()> notifier_;
};


template <typename _Ty, typename _NotifierTy>
class Property
{
public:
    static_assert(std::is_arithmetic<_NotifierTy>::value, "_NotifierTy must be an arithmetic type");

    typedef _Ty         value_type;
    typedef _NotifierTy notifier_type;

    inline Property(_Ty* ptr, _NotifierTy* notifier, _NotifierTy notify_value)
        : ptr_(ptr)
        , notifier_(notifier)
        , notify_value_(notify_value)
    {
    }

    inline _Ty Get() const
    {
        return *ptr_;
    }

    inline void Set(const _Ty& value)
    {
        *ptr_ = value;
        if (notifier_)
        {
            bits::Set(*notifier_, notify_value_);
        }
    }

private:
    _Ty*         ptr_;
    _NotifierTy* notifier_;
    _NotifierTy  notify_value_;
};

}
