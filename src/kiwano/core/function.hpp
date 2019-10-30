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
#include <stdexcept>
#include <type_traits>

namespace kiwano
{

namespace core
{
//
// function is a light weight ::std::function<>-like class
//

namespace __function_detail
{
	//
	// is_callable
	//

	namespace __callable_detail
	{
		template <typename _Ty, typename _Ret, typename... _Args>
		struct helper
		{
			template <typename _Uty> static int test(...);

			template <typename _Uty, _Ret(_Uty::*)(_Args...)> struct class_mem;
			template <typename _Uty> static char test(class_mem<_Uty, &_Uty::operator()>*);

			template <typename _Uty, _Ret(_Uty::*)(_Args...) const> struct class_const_mem;
			template <typename _Uty> static char test(class_const_mem<_Uty, &_Uty::operator()>*);

			template<
				typename _Uty,
				typename _Uret = typename ::std::decay<decltype(::std::declval<_Uty>().operator()(::std::declval<_Args>()...))>::type,
				typename = typename ::std::enable_if<::std::is_convertible<_Ret, _Uret>::value>::type>
			static char test(int);

			static constexpr bool value = sizeof(test<_Ty>(0)) == sizeof(char);
		};
	}

	template<typename _Ty, typename _Ret, typename... _Args>
	struct is_callable
		: public ::std::bool_constant<__callable_detail::helper<_Ty, _Ret, _Args...>::value>
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
}

//
// exceptions
//
class bad_function_call : public ::std::exception
{
public:
	bad_function_call() {}

	virtual const char* what() const override
	{
		return "bad function call";
	}
};


//
// function details
//
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
		::std::swap(callable_, rhs.callable_);
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

}  // namespace core

}  // namespace kiwano

namespace kiwano
{
	template<typename _Ty,
		typename _Uty,
		typename = typename std::enable_if<
			std::is_same<_Ty, _Uty>::value || std::is_base_of<_Ty, _Uty>::value, int
		>::type,
		typename _Ret,
		typename... _Args
	>
	inline core::function<_Ret(_Args...)> Closure(_Uty* ptr, _Ret(_Ty::* func)(_Args...))
	{
		return core::function<_Ret(_Args...)>(ptr, func);
	}

	template<typename _Ty,
		typename _Uty,
		typename = typename std::enable_if<
			std::is_same<_Ty, _Uty>::value || std::is_base_of<_Ty, _Uty>::value, int
		>::type,
		typename _Ret,
		typename... _Args
	>
	inline core::function<_Ret(_Args...)> Closure(_Uty* ptr, _Ret(_Ty::* func)(_Args...) const)
	{
		return core::function<_Ret(_Args...)>(ptr, func);
	}
}
