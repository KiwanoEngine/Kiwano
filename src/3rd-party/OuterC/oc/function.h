// Copyright (c) 2019-2020 OuterC - Nomango

#pragma once
#include "function/details.h"
#include <stdexcept>

namespace oc
{

class bad_function_call : public ::std::exception
{
public:
	bad_function_call() {}

	virtual const char* what() const override
	{
		return "bad function call";
	}
};


template<typename _Ty>
class function;

template<typename _Ret, typename... _Args>
class function<_Ret(_Args...)>
{
public:
	function()
		: callable_(nullptr)
	{
	}

	function(std::nullptr_t)
		: callable_(nullptr)
	{
	}

	function(const function& rhs)
		: callable_(rhs.callable_)
	{
		if (callable_) callable_->retain();
	}

	function(function&& rhs) noexcept
		: callable_(rhs.callable_)
	{
		rhs.callable_ = nullptr;
	}

	function(_Ret(*func)(_Args...))
	{
		callable_ = __function_detail::proxy_callable<_Ret(*)(_Args...), _Ret, _Args...>::make(::std::move(func));
		if (callable_) callable_->retain();
	}

	template<
		typename _Ty,
		typename = typename ::std::enable_if<__function_detail::is_callable<_Ty, _Ret, _Args...>::value, int>::type>
	function(_Ty val)
	{
		callable_ = __function_detail::proxy_callable<_Ty, _Ret, _Args...>::make(::std::move(val));
		if (callable_) callable_->retain();
	}

	template<typename _Ty,
		typename _Uty,
		typename = typename ::std::enable_if<::std::is_same<_Ty, _Uty>::value || ::std::is_base_of<_Ty, _Uty>::value, int>::type>
	function(_Uty* ptr, _Ret(_Ty::* func)(_Args...))
	{
		callable_ = __function_detail::proxy_mem_callable<_Ty, _Ret, _Args...>::make(ptr, func);
		if (callable_) callable_->retain();
	}

	template<typename _Ty,
		typename _Uty,
		typename = typename ::std::enable_if<::std::is_same<_Ty, _Uty>::value || ::std::is_base_of<_Ty, _Uty>::value, int>::type>
	function(_Uty* ptr, _Ret(_Ty::* func)(_Args...) const)
	{
		callable_ = __function_detail::proxy_const_mem_callable<_Ty, _Ret, _Args...>::make(ptr, func);
		if (callable_) callable_->retain();
	}

	~function()
	{
		tidy();
	}

	inline void swap(const function& rhs)
	{
		std::swap(callable_, rhs.callable_);
	}

	inline _Ret operator()(_Args... args) const
	{
		if (!callable_)
			throw bad_function_call();
		return callable_->invoke(::std::forward<_Args>(args)...);
	}

	inline operator bool() const
	{
		return !!callable_;
	}

	inline function& operator=(const function& rhs)
	{
		tidy();
		callable_ = rhs.callable_;
		if (callable_) callable_->retain();
		return (*this);
	}

	inline function& operator=(function&& rhs)
	{
		tidy();
		callable_ = rhs.callable_;
		rhs.callable_ = nullptr;
		return (*this);
	}

private:
	inline void tidy()
	{
		if (callable_)
		{
			callable_->release();
			callable_ = nullptr;
		}
	}

private:
	__function_detail::callable<_Ret, _Args...>* callable_;
};

template<typename _Ty,
	typename _Uty,
	typename = typename std::enable_if<
		std::is_same<_Ty, _Uty>::value || std::is_base_of<_Ty, _Uty>::value, int
	>::type,
	typename _Ret,
	typename... _Args
>
inline function<_Ret(_Args...)> closure(_Uty* ptr, _Ret(_Ty::* func)(_Args...))
{
	return function<_Ret(_Args...)>(ptr, func);
}

template<typename _Ty,
	typename _Uty,
	typename = typename std::enable_if<
		std::is_same<_Ty, _Uty>::value || std::is_base_of<_Ty, _Uty>::value, int
	>::type,
	typename _Ret,
	typename... _Args
>
inline function<_Ret(_Args...)> closure(_Uty* ptr, _Ret(_Ty::* func)(_Args...) const)
{
	return function<_Ret(_Args...)>(ptr, func);
}


template<typename _Ret, typename... _Args>
inline void swap(oc::function<_Ret(_Args...)>& lhs, oc::function<_Ret(_Args...)>& rhs) noexcept
{
	lhs.swap(rhs);
}

}  // namespace oc
