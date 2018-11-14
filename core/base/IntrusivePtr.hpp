// Copyright (c) 2016-2018 Easy2D - Nomango
// 
// Permission is hereby granted, free of charge, to any person obtaining lhs copy
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
#include "macros.h"

namespace easy2d
{
	template <typename T>
	class IntrusivePtr
	{
		using ElemType = T;

		ElemType* ptr_{ nullptr };

	public:
		IntrusivePtr() {}

		IntrusivePtr(nullptr_t) {}

		IntrusivePtr(ElemType* p) : ptr_(p)
		{
			IntrusivePtrAddRef(ptr_);
		}

		IntrusivePtr(const IntrusivePtr& other) : ptr_(other.ptr_)
		{
			IntrusivePtrAddRef(ptr_);
		}

		template <typename U>
		IntrusivePtr(const IntrusivePtr<U>& other) : ptr_(other.Get())
		{
			IntrusivePtrAddRef(ptr_);
		}

		IntrusivePtr(IntrusivePtr&& other)
		{
			ptr_ = other.ptr_;
			other.ptr_ = nullptr;
		}

		~IntrusivePtr()
		{
			IntrusivePtrRelease(ptr_);
		}

		inline ElemType* Get() const { return ptr_; }

		inline void Swap(IntrusivePtr& other)
		{
			::std::swap(ptr_, other.ptr_);
		}

		inline ElemType* operator ->() const
		{
			E2D_WARNING_IF(!ptr_ || ptr_->GetRefCount() <= 0,
				"Invalid pointer!");
			return ptr_;
		}

		inline ElemType& operator *() const
		{
			E2D_WARNING_IF(!ptr_ || ptr_->GetRefCount() <= 0,
				"Invalid pointer!");
			return *ptr_;
		}

		inline operator bool() const { return ptr_ != nullptr; }

		inline bool operator !() const { return ptr_ == 0; }

		inline IntrusivePtr& operator =(const IntrusivePtr& other)
		{
			IntrusivePtr(other).Swap(*this);
			return *this;
		}

		inline IntrusivePtr& operator =(IntrusivePtr&& other)
		{
			IntrusivePtrRelease(ptr_);
			ptr_ = other.ptr_;
			other.ptr_ = nullptr;
			return *this;
		}

		inline IntrusivePtr& operator =(ElemType* p)
		{
			IntrusivePtr(p).Swap(*this);
			return *this;
		}

		inline IntrusivePtr& operator =(nullptr_t)
		{
			IntrusivePtr{}.Swap(*this);
			return *this;
		}
	};

	template<class T, class U>
	inline bool operator==(IntrusivePtr<T> const& lhs, IntrusivePtr<U> const& rhs)
	{
		return lhs.Get() == rhs.Get();
	}

	template<class T, class U>
	inline bool operator!=(IntrusivePtr<T> const& lhs, IntrusivePtr<U> const& rhs)
	{
		return lhs.Get() != rhs.Get();
	}

	template<class T, class U>
	inline bool operator<(IntrusivePtr<T> const& lhs, IntrusivePtr<U> const& rhs)
	{
		return lhs.Get() < rhs.Get();
	}

	template<class T>
	inline bool operator==(IntrusivePtr<T> const& lhs, T* rhs)
	{
		return lhs.Get() == rhs;
	}

	template<class T>
	inline bool operator!=(IntrusivePtr<T> const& lhs, T* rhs)
	{
		return lhs.Get() != rhs;
	}

	template<class T>
	inline bool operator==(T* lhs, IntrusivePtr<T> const& rhs)
	{
		return lhs == rhs.Get();
	}

	template<class T>
	inline bool operator!=(T* lhs, IntrusivePtr<T> const& rhs)
	{
		return lhs != rhs.Get();
	}

	template<class T>
	inline bool operator==(IntrusivePtr<T> const& lhs, nullptr_t)
	{
		return !static_cast<bool>(lhs);
	}

	template<class T>
	inline bool operator!=(IntrusivePtr<T> const& lhs, nullptr_t)
	{
		return static_cast<bool>(lhs);
	}

	template<class T>
	inline bool operator==(nullptr_t, IntrusivePtr<T> const& rhs)
	{
		return !static_cast<bool>(rhs);
	}

	template<class T>
	inline bool operator!=(nullptr_t, IntrusivePtr<T> const& rhs)
	{
		return static_cast<bool>(rhs);
	}

	template<class T>
	inline IntrusivePtr<T> make_intrusive(T* ptr)
	{
		return IntrusivePtr<T>(ptr);
	}
}
