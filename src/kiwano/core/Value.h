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
#include <kiwano/core/Function.h>
#include <kiwano/core/BitOperator.h>
#include <kiwano/core/Flag.h>

namespace kiwano
{

template <typename _Ty, typename _FlagTy>
struct ValueObserver;

template <typename _Ty, typename _FlagTy>
struct ValueObserver
{
    static_assert(std::is_arithmetic<_FlagTy>::value, "_Ty must be an arithmetic type");

    inline ValueObserver(_FlagTy& observer, _FlagTy value)
        : observer(observer)
        , value(value)
    {
    }

    inline void Changed(const _Ty& old_val, const _Ty& new_val)
    {
        bits::Set(observer, value);
    }

    _FlagTy& observer;
    _FlagTy  value;
};

template <typename _Ty, typename _FlagTy>
struct ValueObserver<_Ty, Flag<_FlagTy>>
{
    inline ValueObserver(Flag<_FlagTy>& flag, _FlagTy flag_value)
        : flag(flag)
        , flag_value(flag_value)
    {
    }

    inline void Changed(const _Ty& old_val, const _Ty& new_val)
    {
        flag.Set(flag_value);
    }

    Flag<_FlagTy>& flag;
    _FlagTy        flag_value;
};

template <typename _Ty>
struct ValueObserver<_Ty, Function<void()>>
{
    inline ValueObserver(const Function<void()>& callback)
        : callback(callback)
    {
    }

    inline void Changed(const _Ty& old_val, const _Ty& new_val)
    {
        if (callback)
        {
            callback();
        }
    }

    Function<void()> callback;
};

template <typename _Ty, typename _ObserverTy = void>
class Value;

template <typename _Ty>
class Value<_Ty, void>
{
public:
    typedef _Ty  value_type;
    typedef void observer_type;

    inline Value(_Ty& ptr)
        : ptr_(ptr)
    {
    }

    inline const _Ty& Get() const
    {
        return ptr_;
    }

    inline void Set(const _Ty& value)
    {
        ptr_ = value;
    }

private:
    _Ty& ptr_;
};

template <typename _Ty, typename _ObserverTy>
class Value
{
public:
    typedef _Ty value_type;

    typedef ValueObserver<_Ty, _ObserverTy> observer_type;

    inline Value(_Ty& ptr, const observer_type& observer)
        : ptr_(ptr)
        , observer_(observer)
    {
    }

    inline const _Ty& Get() const
    {
        return ptr_;
    }

    inline void Set(const _Ty& value)
    {
        ptr_ = value;
        observer_.Changed(ptr_, value);
    }

private:
    _Ty&          ptr_;
    observer_type observer_;
};

}
