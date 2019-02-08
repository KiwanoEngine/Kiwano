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
#include <utility>

namespace easy2d
{
	template <typename T>
	class IntrusivePtr
	{
		T* ptr_{ nullptr };

	public:
		using Type = T;

		IntrusivePtr() E2D_NOEXCEPT {}

		IntrusivePtr(nullptr_t) E2D_NOEXCEPT {}

		IntrusivePtr(Type* p) E2D_NOEXCEPT : ptr_(p)
		{
			IntrusivePtrAddRef(ptr_);
		}

		IntrusivePtr(const IntrusivePtr& other) E2D_NOEXCEPT
			: ptr_(other.ptr_)
		{
			IntrusivePtrAddRef(ptr_);
		}

		template <typename U>
		IntrusivePtr(const IntrusivePtr<U>& other) E2D_NOEXCEPT
			: ptr_(other.Get())
		{
			IntrusivePtrAddRef(ptr_);
		}

		IntrusivePtr(IntrusivePtr&& other) E2D_NOEXCEPT
		{
			ptr_ = other.ptr_;
			other.ptr_ = nullptr;
		}

		~IntrusivePtr() E2D_NOEXCEPT
		{
			IntrusivePtrRelease(ptr_);
		}

		inline Type* Get() const E2D_NOEXCEPT { return ptr_; }

		inline void Reset() E2D_NOEXCEPT
		{
			IntrusivePtr{}.Swap(*this);
		}

		inline void Swap(IntrusivePtr& other) E2D_NOEXCEPT
		{
			std::swap(ptr_, other.ptr_);
		}

		inline Type* operator ->() const
		{
			E2D_ASSERT(ptr_ != nullptr && "Invalid pointer");
			return ptr_;
		}

		inline Type& operator *() const
		{
			E2D_ASSERT(ptr_ != nullptr && "Invalid pointer");
			return *ptr_;
		}

		inline Type** operator &()
		{
			E2D_ASSERT(ptr_ == nullptr && "Memory leak");
			return &ptr_;
		}

		inline operator bool() const E2D_NOEXCEPT { return ptr_ != nullptr; }

		inline bool operator !() const E2D_NOEXCEPT { return ptr_ == 0; }

		inline IntrusivePtr& operator =(const IntrusivePtr& other) E2D_NOEXCEPT
		{
			if (other.ptr_ != ptr_)
				IntrusivePtr(other).Swap(*this);
			return *this;
		}

		inline IntrusivePtr& operator =(IntrusivePtr&& other) E2D_NOEXCEPT
		{
			IntrusivePtrRelease(ptr_);
			ptr_ = other.ptr_;
			other.ptr_ = nullptr;
			return *this;
		}

		inline IntrusivePtr& operator =(Type* p) E2D_NOEXCEPT
		{
			if (p != ptr_)
				IntrusivePtr(p).Swap(*this);
			return *this;
		}

		inline IntrusivePtr& operator =(nullptr_t) E2D_NOEXCEPT
		{
			if (nullptr != ptr_)
				IntrusivePtr{}.Swap(*this);
			return *this;
		}
	};

	template<class T, class U>
	inline bool operator==(IntrusivePtr<T> const& lhs, IntrusivePtr<U> const& rhs) E2D_NOEXCEPT
	{
		return lhs.Get() == rhs.Get();
	}

	template<class T, class U>
	inline bool operator!=(IntrusivePtr<T> const& lhs, IntrusivePtr<U> const& rhs) E2D_NOEXCEPT
	{
		return lhs.Get() != rhs.Get();
	}

	template<class T, class U>
	inline bool operator<(IntrusivePtr<T> const& lhs, IntrusivePtr<U> const& rhs) E2D_NOEXCEPT
	{
		return lhs.Get() < rhs.Get();
	}

	template<class T>
	inline bool operator==(IntrusivePtr<T> const& lhs, T* rhs) E2D_NOEXCEPT
	{
		return lhs.Get() == rhs;
	}

	template<class T>
	inline bool operator!=(IntrusivePtr<T> const& lhs, T* rhs) E2D_NOEXCEPT
	{
		return lhs.Get() != rhs;
	}

	template<class T>
	inline bool operator==(T* lhs, IntrusivePtr<T> const& rhs) E2D_NOEXCEPT
	{
		return lhs == rhs.Get();
	}

	template<class T>
	inline bool operator!=(T* lhs, IntrusivePtr<T> const& rhs) E2D_NOEXCEPT
	{
		return lhs != rhs.Get();
	}

	template<class T>
	inline bool operator==(IntrusivePtr<T> const& lhs, nullptr_t) E2D_NOEXCEPT
	{
		return !static_cast<bool>(lhs);
	}

	template<class T>
	inline bool operator!=(IntrusivePtr<T> const& lhs, nullptr_t) E2D_NOEXCEPT
	{
		return static_cast<bool>(lhs);
	}

	template<class T>
	inline bool operator==(nullptr_t, IntrusivePtr<T> const& rhs) E2D_NOEXCEPT
	{
		return !static_cast<bool>(rhs);
	}

	template<class T>
	inline bool operator!=(nullptr_t, IntrusivePtr<T> const& rhs) E2D_NOEXCEPT
	{
		return static_cast<bool>(rhs);
	}

	// template class cannot specialize std::swap,
	// so implement a swap function in easy2d namespace
	template<class T>
	inline void swap(IntrusivePtr<T>& lhs, IntrusivePtr<T>& rhs) E2D_NOEXCEPT
	{
		lhs.Swap(rhs);
	}

}
