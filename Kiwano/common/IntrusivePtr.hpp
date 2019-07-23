// Copyright (c) 2016-2018 Kiwano - Nomango
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
#include "../macros.h"
#include <utility>
#include <type_traits>

namespace kiwano
{
	template <typename _Ty, typename _Manager>
	class IntrusivePtr
	{
		_Ty* ptr_{ nullptr };

	public:
		using Type = _Ty;

		IntrusivePtr() noexcept {}

		IntrusivePtr(nullptr_t) noexcept {}

		IntrusivePtr(Type* p) noexcept : ptr_(p)
		{
			typename _Manager::AddRef(ptr_);
		}

		IntrusivePtr(const Type* p) noexcept : ptr_(p)
		{
			typename _Manager::AddRef(ptr_);
		}

		IntrusivePtr(const IntrusivePtr& other) noexcept
			: ptr_(other.ptr_)
		{
			typename _Manager::AddRef(ptr_);
		}

		template <typename _UTy>
		IntrusivePtr(const IntrusivePtr<_UTy, _Manager>& other) noexcept
			: ptr_(other.Get())
		{
			typename _Manager::AddRef(ptr_);
		}

		IntrusivePtr(IntrusivePtr&& other) noexcept
		{
			ptr_ = other.ptr_;
			other.ptr_ = nullptr;
		}

		~IntrusivePtr() noexcept
		{
			typename _Manager::Release(ptr_);
		}

		inline Type* Get() const noexcept { return ptr_; }

		inline void Reset() noexcept
		{
			IntrusivePtr{}.Swap(*this);
		}

		inline void Swap(IntrusivePtr& other) noexcept
		{
			std::swap(ptr_, other.ptr_);
		}

		inline Type* operator ->() const
		{
			KGE_ASSERT(ptr_ != nullptr && "Invalid pointer");
			return ptr_;
		}

		inline Type& operator *() const
		{
			KGE_ASSERT(ptr_ != nullptr && "Invalid pointer");
			return *ptr_;
		}

		inline Type** operator &()
		{
			KGE_ASSERT(ptr_ == nullptr && "Memory leak");
			return &ptr_;
		}

		inline operator bool() const noexcept { return ptr_ != nullptr; }

		inline bool operator !() const noexcept { return ptr_ == 0; }

		inline IntrusivePtr& operator =(const IntrusivePtr& other) noexcept
		{
			if (other.ptr_ != ptr_)
				IntrusivePtr(other).Swap(*this);
			return *this;
		}

		inline IntrusivePtr& operator =(IntrusivePtr&& other) noexcept
		{
			typename _Manager::Release(ptr_);
			ptr_ = other.ptr_;
			other.ptr_ = nullptr;
			return *this;
		}

		inline IntrusivePtr& operator =(Type* p) noexcept
		{
			if (p != ptr_)
				IntrusivePtr(p).Swap(*this);
			return *this;
		}

		inline IntrusivePtr& operator =(nullptr_t) noexcept
		{
			if (nullptr != ptr_)
				IntrusivePtr{}.Swap(*this);
			return *this;
		}
	};

	template <class _Ty, class _UTy, class _Manager>
	inline bool operator==(IntrusivePtr<_Ty, _Manager> const& lhs, IntrusivePtr<_UTy, _Manager> const& rhs) noexcept
	{
		return lhs.Get() == rhs.Get();
	}

	template <class _Ty, class _UTy, class _Manager>
	inline bool operator!=(IntrusivePtr<_Ty, _Manager> const& lhs, IntrusivePtr<_UTy, _Manager> const& rhs) noexcept
	{
		return lhs.Get() != rhs.Get();
	}

	template <class _Ty, class _UTy, class _Manager>
	inline bool operator<(IntrusivePtr<_Ty, _Manager> const& lhs, IntrusivePtr<_UTy, _Manager> const& rhs) noexcept
	{
		return lhs.Get() < rhs.Get();
	}

	template <class _Ty, class _Manager>
	inline bool operator==(IntrusivePtr<_Ty, _Manager> const& lhs, _Ty* rhs) noexcept
	{
		return lhs.Get() == rhs;
	}

	template <class _Ty, class _Manager>
	inline bool operator!=(IntrusivePtr<_Ty, _Manager> const& lhs, _Ty* rhs) noexcept
	{
		return lhs.Get() != rhs;
	}

	template <class _Ty, class _Manager>
	inline bool operator==(_Ty* lhs, IntrusivePtr<_Ty, _Manager> const& rhs) noexcept
	{
		return lhs == rhs.Get();
	}

	template <class _Ty, class _Manager>
	inline bool operator!=(_Ty* lhs, IntrusivePtr<_Ty, _Manager> const& rhs) noexcept
	{
		return lhs != rhs.Get();
	}

	template <class _Ty, class _Manager>
	inline bool operator==(IntrusivePtr<_Ty, _Manager> const& lhs, nullptr_t) noexcept
	{
		return !static_cast<bool>(lhs);
	}

	template <class _Ty, class _Manager>
	inline bool operator!=(IntrusivePtr<_Ty, _Manager> const& lhs, nullptr_t) noexcept
	{
		return static_cast<bool>(lhs);
	}

	template <class _Ty, class _Manager>
	inline bool operator==(nullptr_t, IntrusivePtr<_Ty, _Manager> const& rhs) noexcept
	{
		return !static_cast<bool>(rhs);
	}

	template <class _Ty, class _Manager>
	inline bool operator!=(nullptr_t, IntrusivePtr<_Ty, _Manager> const& rhs) noexcept
	{
		return static_cast<bool>(rhs);
	}

	// template class cannot specialize std::swap,
	// so implement a swap function in kiwano namespace
	template <class _Ty, class _Manager>
	inline void swap(IntrusivePtr<_Ty, _Manager>& lhs, IntrusivePtr<_Ty, _Manager>& rhs) noexcept
	{
		lhs.Swap(rhs);
	}

}
