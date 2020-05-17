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
#include <typeinfo>
#include <type_traits>
#include <stdexcept>
#include <functional>

namespace kiwano
{
namespace details
{

template <typename _Ty, typename _Ret, typename... _Args>
struct IsCallableHelper
{
    template <typename _Uty, _Ret (_Uty::*)(_Args...)>
    struct ClassMember;

    template <typename _Uty, _Ret (_Uty::*)(_Args...) const>
    struct ClassConstMember;

    template <typename _Uty>
    static int Test(...);

    template <typename _Uty>
    static char Test(ClassMember<_Uty, &_Uty::operator()>*);

    template <typename _Uty>
    static char Test(ClassConstMember<_Uty, &_Uty::operator()>*);

    template <
        typename _Uty,
        typename _Uret = typename std::decay<decltype(std::declval<_Uty>().operator()(std::declval<_Args>()...))>::type,
        typename       = typename std::enable_if<std::is_convertible<_Ret, _Uret>::value>::type>
    static char Test(int);

    static constexpr bool value = sizeof(Test<_Ty>(0)) == sizeof(char);
};

template <typename _Ty, typename _Ret, typename... _Args>
struct IsCallable : public std::bool_constant<IsCallableHelper<_Ty, _Ret, _Args...>::value>
{
};

//
// Callable
//

template <typename _Ret, typename... _Args>
class Callable
{
public:
    virtual ~Callable() {}

    virtual void Retain()                      = 0;
    virtual void Release()                     = 0;
    virtual _Ret Invoke(_Args&&... args) const = 0;

    virtual const std::type_info& TargetType() const noexcept = 0;

    virtual const void* Target(const std::type_info& type) const noexcept = 0;
};

template <typename _Ret, typename... _Args>
class RefCountCallable : public Callable<_Ret, _Args...>
{
public:
    RefCountCallable()
        : ref_count_(0)
    {
    }

    virtual void Retain() override
    {
        ++ref_count_;
    }

    virtual void Release() override
    {
        --ref_count_;
        if (ref_count_ <= 0)
        {
            delete this;
        }
    }

private:
    int ref_count_;
};

template <typename _Ty, typename _Ret, typename... _Args>
class ProxyCallable : public RefCountCallable<_Ret, _Args...>
{
public:
    ProxyCallable(_Ty&& val)
        : callee_(std::move(val))
    {
    }

    virtual _Ret Invoke(_Args&&... args) const override
    {
        return std::invoke(callee_, std::forward<_Args>(args)...);
    }

    virtual const std::type_info& TargetType() const noexcept
    {
        return typeid(_Ty);
    }

    virtual const void* Target(const std::type_info& type) const noexcept
    {
        if (type == this->TargetType())
            return &callee_;
        return nullptr;
    }

    static inline Callable<_Ret, _Args...>* Make(_Ty&& val)
    {
        return new (std::nothrow) ProxyCallable<_Ty, _Ret, _Args...>(std::move(val));
    }

private:
    _Ty callee_;
};

template <typename _Ty, typename _Ret, typename... _Args>
class ProxyMemCallable : public RefCountCallable<_Ret, _Args...>
{
public:
    typedef _Ret (_Ty::*_FuncType)(_Args...);

    virtual _Ret Invoke(_Args&&... args) const override
    {
        return std::invoke(func_, ptr_, std::forward<_Args>(args)...);
    }

    virtual const std::type_info& TargetType() const noexcept
    {
        return typeid(ProxyMemCallable);
    }

    virtual const void* Target(const std::type_info& type) const noexcept
    {
        if (type == this->TargetType())
            return this;
        return nullptr;
    }

    static inline Callable<_Ret, _Args...>* Make(_Ty* ptr, _FuncType func)
    {
        return new (std::nothrow) ProxyMemCallable<_Ty, _Ret, _Args...>(ptr, func);
    }

protected:
    ProxyMemCallable(_Ty* ptr, _FuncType func)
        : ptr_(ptr)
        , func_(func)
    {
    }

protected:
    _Ty*      ptr_;
    _FuncType func_;
};

template <typename _Ty, typename _Ret, typename... _Args>
class ProxyConstMemCallable : public RefCountCallable<_Ret, _Args...>
{
public:
    typedef _Ret (_Ty::*_FuncType)(_Args...) const;

    virtual _Ret Invoke(_Args&&... args) const override
    {
        return std::invoke(func_, ptr_, std::forward<_Args>(args)...);
    }

    virtual const std::type_info& TargetType() const noexcept
    {
        return typeid(ProxyConstMemCallable);
    }

    virtual const void* Target(const std::type_info& type) const noexcept
    {
        if (type == this->TargetType())
            return this;
        return nullptr;
    }

    static inline Callable<_Ret, _Args...>* Make(_Ty* ptr, _FuncType func)
    {
        return new (std::nothrow) ProxyConstMemCallable<_Ty, _Ret, _Args...>(ptr, func);
    }

protected:
    ProxyConstMemCallable(_Ty* ptr, _FuncType func)
        : ptr_(ptr)
        , func_(func)
    {
    }

protected:
    _Ty*      ptr_;
    _FuncType func_;
};

}  // namespace details

template <typename _Ty>
class Function;

template <typename _Ret, typename... _Args>
class Function<_Ret(_Args...)>
{
public:
    Function()
        : callable_(nullptr)
    {
    }

    Function(std::nullptr_t)
        : callable_(nullptr)
    {
    }

    Function(const Function& rhs)
        : callable_(nullptr)
    {
        SetCallable(rhs.callable_);
    }

    Function(Function&& rhs) noexcept
        : callable_(rhs.callable_)
    {
        rhs.callable_ = nullptr;
    }

    Function(_Ret (*func)(_Args...))
        : callable_(nullptr)
    {
        SetCallable(details::ProxyCallable<_Ret (*)(_Args...), _Ret, _Args...>::Make(std::move(func)));
    }

    template <typename _Ty,
              typename = typename std::enable_if<details::IsCallable<_Ty, _Ret, _Args...>::value, int>::type>
    Function(_Ty val)
        : callable_(nullptr)
    {
        SetCallable(details::ProxyCallable<_Ty, _Ret, _Args...>::Make(std::move(val)));
    }

    template <typename _Ty, typename _Uty,
              typename = typename std::enable_if<std::is_same<_Ty, _Uty>::value || std::is_base_of<_Ty, _Uty>::value,
                                                 int>::type>
    Function(_Uty* ptr, _Ret (_Ty::*func)(_Args...))
        : callable_(nullptr)
    {
        SetCallable(details::ProxyMemCallable<_Ty, _Ret, _Args...>::Make(ptr, func));
    }

    template <typename _Ty, typename _Uty,
              typename = typename std::enable_if<std::is_same<_Ty, _Uty>::value || std::is_base_of<_Ty, _Uty>::value,
                                                 int>::type>
    Function(_Uty* ptr, _Ret (_Ty::*func)(_Args...) const)
        : callable_(nullptr)
    {
        SetCallable(details::ProxyConstMemCallable<_Ty, _Ret, _Args...>::Make(ptr, func));
    }

    ~Function()
    {
        SetCallable(nullptr);
    }

    inline _Ret operator()(_Args... args) const
    {
        if (!callable_)
            throw std::bad_function_call();
        return callable_->Invoke(std::forward<_Args>(args)...);
    }

    inline operator bool() const
    {
        return !!callable_;
    }

    inline Function& operator=(const Function& rhs)
    {
        SetCallable(rhs.callable_);
        return (*this);
    }

    inline Function& operator=(Function&& rhs)
    {
        SetCallable(nullptr);
        callable_     = rhs.callable_;
        rhs.callable_ = nullptr;
        return (*this);
    }

    inline void swap(const Function& rhs)
    {
        std::swap(callable_, rhs.callable_);
    }

    const std::type_info& target_type() const noexcept
    {
        return callable_->TargetType();
    }

    template <class _Fx>
    _Fx* target() noexcept
    {
        return reinterpret_cast<_Fx*>(const_cast<void*>(callable_->Target(typeid(_Fx))));
    }

    template <class _Fx>
    const _Fx* target() const noexcept
    {
        return reinterpret_cast<const _Fx*>(callable_->Target(typeid(_Fx)));
    }

private:
    inline void SetCallable(details::Callable<_Ret, _Args...>* callable)
    {
        if (callable_ != callable)
        {
            if (callable_)
            {
                callable_->Release();
                callable_ = nullptr;
            }

            callable_ = callable;
            if (callable_)
            {
                callable_->Retain();
            }
        }
    }

private:
    details::Callable<_Ret, _Args...>* callable_;
};

template <
    typename _Ty, typename _Uty,
    typename = typename std::enable_if<std::is_same<_Ty, _Uty>::value || std::is_base_of<_Ty, _Uty>::value, int>::type,
    typename _Ret, typename... _Args>
inline Function<_Ret(_Args...)> Closure(_Uty* ptr, _Ret (_Ty::*func)(_Args...))
{
    return Function<_Ret(_Args...)>(ptr, func);
}

template <
    typename _Ty, typename _Uty,
    typename = typename std::enable_if<std::is_same<_Ty, _Uty>::value || std::is_base_of<_Ty, _Uty>::value, int>::type,
    typename _Ret, typename... _Args>
inline Function<_Ret(_Args...)> Closure(_Uty* ptr, _Ret (_Ty::*func)(_Args...) const)
{
    return Function<_Ret(_Args...)>(ptr, func);
}

template <typename _Ret, typename... _Args>
inline void swap(kiwano::Function<_Ret(_Args...)>& lhs, kiwano::Function<_Ret(_Args...)>& rhs) noexcept
{
    lhs.swap(rhs);
}

}  // namespace kiwano
