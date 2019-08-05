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

namespace kiwano
{
	//
	// Closure is a light weight std::function<>-like class
	//

	namespace __closure_detail
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
					typename _Uret = typename std::decay<decltype(std::declval<_Uty>().operator()(std::declval<_Args>()...))>::type,
					typename = typename std::enable_if<std::is_convertible<_Ret, _Uret>::value>::type>
				static char test(int);

				static constexpr bool value = sizeof(test<_Ty>(0)) == sizeof(char);
			};
		}

		template<typename _Ty, typename _Ret, typename... _Args>
		struct is_callable
			: public std::bool_constant<__callable_detail::helper<_Ty, _Ret, _Args...>::value>
		{
		};

		//
		// Callable
		//

		template<typename _Ret, typename... _Args>
		class Callable
		{
		public:
			virtual ~Callable() {}

			virtual void AddRef() = 0;
			virtual void Release() = 0;
			virtual _Ret Invoke(_Args... args) const = 0;
		};

		template<typename _Ret, typename... _Args>
		class RefCountCallable
			: public Callable<_Ret, _Args...>
		{
		public:
			RefCountCallable() : ref_count_(0) {}

			virtual void AddRef() override
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

		template<typename _Ty, typename _Ret, typename... _Args>
		class ProxyCallable
			: public RefCountCallable<_Ret, _Args...>
		{
		public:
			ProxyCallable(_Ty&& val)
				: callee_(std::move(val))
			{
			}

			virtual _Ret Invoke(_Args... args) const override
			{
				return callee_(std::forward<_Args>(args)...);
			}

			static inline Callable<_Ret, _Args...>* Make(_Ty&& val)
			{
				return new (std::nothrow) ProxyCallable<_Ty, _Ret, _Args...>(std::move(val));
			}

		private:
			_Ty callee_;
		};

		template<typename _Ty, typename _Ret, typename... _Args>
		class ProxyMemCallable
			: public RefCountCallable<_Ret, _Args...>
		{
		public:
			typedef _Ret(_Ty::* _FuncType)(_Args...);

			virtual _Ret Invoke(_Args... args) const override
			{
				return (static_cast<_Ty*>(ptr_)->*func_)(std::forward<_Args>(args)...);
			}

			static inline Callable<_Ret, _Args...>* Make(void* ptr, _FuncType func)
			{
				return new (std::nothrow) ProxyMemCallable<_Ty, _Ret, _Args...>(ptr, func);
			}

		protected:
			ProxyMemCallable(void* ptr, _FuncType func)
				: ptr_(ptr)
				, func_(func)
			{
			}

		protected:
			void* ptr_;
			_FuncType func_;
		};

		template<typename _Ty, typename _Ret, typename... _Args>
		class ProxyConstMemCallable
			: public RefCountCallable<_Ret, _Args...>
		{
		public:
			typedef _Ret(_Ty::* _FuncType)(_Args...) const;

			virtual _Ret Invoke(_Args... args) const override
			{
				return (static_cast<_Ty*>(ptr_)->*func_)(std::forward<_Args>(args)...);
			}

			static inline Callable<_Ret, _Args...>* Make(void* ptr, _FuncType func)
			{
				return new (std::nothrow) ProxyConstMemCallable<_Ty, _Ret, _Args...>(ptr, func);
			}

		protected:
			ProxyConstMemCallable(void* ptr, _FuncType func)
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
	class bad_function_call : public std::exception
	{
	public:
		bad_function_call() {}

		virtual const char* what() const override
		{
			return "bad function call";
		}
	};


	//
	// Closure details
	//
	template<typename _Ty>
	class Closure;

	template<typename _Ret, typename... _Args>
	class Closure<_Ret(_Args...)>
	{
	public:
		Closure()
			: callable_(nullptr)
		{
		}

		Closure(std::nullptr_t)
			: callable_(nullptr)
		{
		}

		Closure(const Closure& rhs)
			: callable_(rhs.callable_)
		{
			if (callable_) callable_->AddRef();
		}

		Closure(Closure&& rhs) noexcept
			: callable_(rhs.callable_)
		{
			rhs.callable_ = nullptr;
		}

		Closure(_Ret(*func)(_Args...))
		{
			callable_ = __closure_detail::ProxyCallable<_Ret(*)(_Args...), _Ret, _Args...>::Make(std::move(func));
			if (callable_) callable_->AddRef();
		}

		template<
			typename _Ty,
			typename = typename std::enable_if<__closure_detail::is_callable<_Ty, _Ret, _Args...>::value, int>::type>
		Closure(_Ty val)
		{
			callable_ = __closure_detail::ProxyCallable<_Ty, _Ret, _Args...>::Make(std::move(val));
			if (callable_) callable_->AddRef();
		}

		template<typename _Ty,
			typename _Uty,
			typename = typename std::enable_if<std::is_same<_Ty, _Uty>::value || std::is_base_of<_Ty, _Uty>::value, int>::type>
		Closure(_Uty* ptr, _Ret(_Ty::* func)(_Args...))
		{
			callable_ = __closure_detail::ProxyMemCallable<_Ty, _Ret, _Args...>::Make(ptr, func);
			if (callable_) callable_->AddRef();
		}

		template<typename _Ty,
			typename _Uty,
			typename = typename std::enable_if<std::is_same<_Ty, _Uty>::value || std::is_base_of<_Ty, _Uty>::value, int>::type>
		Closure(_Uty* ptr, _Ret(_Ty::* func)(_Args...) const)
		{
			callable_ = __closure_detail::ProxyConstMemCallable<_Ty, _Ret, _Args...>::Make(ptr, func);
			if (callable_) callable_->AddRef();
		}

		~Closure()
		{
			tidy();
		}

		inline void swap(const Closure& rhs)
		{
			std::swap(callable_, rhs.callable_);
		}

		inline _Ret operator()(_Args... args) const
		{
			if (!callable_)
				throw bad_function_call();
			return callable_->Invoke(std::forward<_Args>(args)...);
		}

		inline operator bool() const
		{
			return !!callable_;
		}

		inline Closure& operator=(const Closure& rhs)
		{
			tidy();
			callable_ = rhs.callable_;
			if (callable_) callable_->AddRef();
			return (*this);
		}

		inline Closure& operator=(Closure&& rhs)
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
				callable_->Release();
				callable_ = nullptr;
			}
		}

	private:
		__closure_detail::Callable<_Ret, _Args...>* callable_;
	};

	template<typename _Ty,
		typename _Uty,
		typename = typename std::enable_if<
			std::is_same<_Ty, _Uty>::value || std::is_base_of<_Ty, _Uty>::value, int
		>::type,
		typename _Ret,
		typename... _Args>
	inline Closure<_Ret(_Args...)> MakeClosure(_Uty* ptr, _Ret(_Ty::* func)(_Args...))
	{
		return Closure<_Ret(_Args...)>(ptr, func);
	}

	template<typename _Ty,
		typename _Uty,
		typename = typename std::enable_if<
			std::is_same<_Ty, _Uty>::value || std::is_base_of<_Ty, _Uty>::value, int
		>::type,
		typename _Ret,
		typename... _Args>
	inline Closure<_Ret(_Args...)> MakeClosure(_Uty* ptr, _Ret(_Ty::* func)(_Args...) const)
	{
		return Closure<_Ret(_Args...)>(ptr, func);
	}
}
