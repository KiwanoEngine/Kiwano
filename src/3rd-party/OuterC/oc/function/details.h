// Copyright (c) 2019-2020 OuterC - Nomango

#pragma once
#include <type_traits>

namespace oc
{
namespace __function_detail
{

template <typename _Ty, typename _Ret, typename... _Args>
struct is_callable_helper
{
	template <typename _Uty, _Ret(_Uty::*)(_Args...)>
	struct class_mem;

	template <typename _Uty, _Ret(_Uty::*)(_Args...) const>
	struct class_const_mem;

	template <typename _Uty>
	static int test(...);

	template <typename _Uty>
	static char test(class_mem<_Uty, &_Uty::operator()>*);

	template <typename _Uty>
	static char test(class_const_mem<_Uty, &_Uty::operator()>*);

	template<
		typename _Uty,
		typename _Uret = typename std::decay<decltype(std::declval<_Uty>().operator()(std::declval<_Args>()...))>::type,
		typename = typename std::enable_if<std::is_convertible<_Ret, _Uret>::value>::type
	>
	static char test(int);

	static constexpr bool value = sizeof(test<_Ty>(0)) == sizeof(char);
};

template<typename _Ty, typename _Ret, typename... _Args>
struct is_callable
	: public ::std::bool_constant<is_callable_helper<_Ty, _Ret, _Args...>::value>
{
};

//
// callable
//

template<typename _Ret, typename... _Args>
class callable
{
public:
	virtual ~callable() {}

	virtual void retain() = 0;
	virtual void release() = 0;
	virtual _Ret invoke(_Args... args) const = 0;
};

template<typename _Ret, typename... _Args>
class ref_count_callable
	: public callable<_Ret, _Args...>
{
public:
	ref_count_callable() : ref_count_(0) {}

	virtual void retain() override
	{
		++ref_count_;
	}

	virtual void release() override
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

template<typename _Ty, typename _Ret, typename... _Args>
class proxy_callable
	: public ref_count_callable<_Ret, _Args...>
{
public:
	proxy_callable(_Ty&& val)
		: callee_(::std::move(val))
	{
	}

	virtual _Ret invoke(_Args... args) const override
	{
		return callee_(::std::forward<_Args&&>(args)...);
	}

	static inline callable<_Ret, _Args...>* make(_Ty&& val)
	{
		return new (::std::nothrow) proxy_callable<_Ty, _Ret, _Args...>(::std::move(val));
	}

private:
	_Ty callee_;
};

template<typename _Ty, typename _Ret, typename... _Args>
class proxy_mem_callable
	: public ref_count_callable<_Ret, _Args...>
{
public:
	typedef _Ret(_Ty::* _FuncType)(_Args...);

	virtual _Ret invoke(_Args... args) const override
	{
		return (static_cast<_Ty*>(ptr_)->*func_)(::std::forward<_Args>(args)...);
	}

	static inline callable<_Ret, _Args...>* make(void* ptr, _FuncType func)
	{
		return new (::std::nothrow) proxy_mem_callable<_Ty, _Ret, _Args...>(ptr, func);
	}

protected:
	proxy_mem_callable(void* ptr, _FuncType func)
		: ptr_(ptr)
		, func_(func)
	{
	}

protected:
	void* ptr_;
	_FuncType func_;
};

template<typename _Ty, typename _Ret, typename... _Args>
class proxy_const_mem_callable
	: public ref_count_callable<_Ret, _Args...>
{
public:
	typedef _Ret(_Ty::* _FuncType)(_Args...) const;

	virtual _Ret invoke(_Args... args) const override
	{
		return (static_cast<_Ty*>(ptr_)->*func_)(::std::forward<_Args>(args)...);
	}

	static inline callable<_Ret, _Args...>* make(void* ptr, _FuncType func)
	{
		return new (::std::nothrow) proxy_const_mem_callable<_Ty, _Ret, _Args...>(ptr, func);
	}

protected:
	proxy_const_mem_callable(void* ptr, _FuncType func)
		: ptr_(ptr)
		, func_(func)
	{
	}

protected:
	void* ptr_;
	_FuncType func_;
};

}  // namespace __function_detail
}  // namespace oc
