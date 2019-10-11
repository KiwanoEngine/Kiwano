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
#include <kiwano/macros.h>
#include <utility>
#include <type_traits>

namespace kiwano
{

inline namespace core
{

template <typename _Ty, typename _ManagerTy>
class intrusive_ptr
{
public:
	using value_type			= _Ty;
	using pointer_type			= _Ty*;
	using const_pointer_type	= const _Ty*;
	using manager_type			= _ManagerTy;

	intrusive_ptr() noexcept {}

	intrusive_ptr(nullptr_t) noexcept {}

	intrusive_ptr(pointer_type p) noexcept : ptr_(p)
	{
		typename manager_type::AddRef(ptr_);
	}

	intrusive_ptr(const_pointer_type p) noexcept : ptr_(p)
	{
		typename manager_type::AddRef(ptr_);
	}

	intrusive_ptr(const intrusive_ptr& other) noexcept
		: ptr_(other.ptr_)
	{
		typename manager_type::AddRef(ptr_);
	}

	template <typename _UTy>
	intrusive_ptr(const intrusive_ptr<_UTy, manager_type>& other) noexcept
		: ptr_(other.get())
	{
		typename manager_type::AddRef(ptr_);
	}

	intrusive_ptr(intrusive_ptr&& other) noexcept
	{
		ptr_ = other.ptr_;
		other.ptr_ = nullptr;
	}

	~intrusive_ptr() noexcept
	{
		typename manager_type::Release(ptr_);
	}

	inline pointer_type get() const noexcept { return ptr_; }

	inline void reset(pointer_type ptr = nullptr) noexcept
	{
		intrusive_ptr{ ptr }.swap(*this);
	}

	inline void swap(intrusive_ptr& other) noexcept
	{
		std::swap(ptr_, other.ptr_);
	}

	inline pointer_type operator ->() const
	{
		KGE_ASSERT(ptr_ != nullptr && "Invalid pointer");
		return ptr_;
	}

	inline value_type& operator *() const
	{
		KGE_ASSERT(ptr_ != nullptr && "Invalid pointer");
		return *ptr_;
	}

	inline pointer_type* operator &()
	{
		KGE_ASSERT(ptr_ == nullptr && "Memory leak");
		return &ptr_;
	}

	inline operator bool() const noexcept { return ptr_ != nullptr; }

	inline bool operator !() const noexcept { return ptr_ == 0; }

	inline intrusive_ptr& operator =(const intrusive_ptr& other) noexcept
	{
		if (other.ptr_ != ptr_)
			intrusive_ptr(other).swap(*this);
		return *this;
	}

	inline intrusive_ptr& operator =(intrusive_ptr&& other) noexcept
	{
		typename manager_type::Release(ptr_);
		ptr_ = other.ptr_;
		other.ptr_ = nullptr;
		return *this;
	}

	inline intrusive_ptr& operator =(pointer_type p) noexcept
	{
		if (p != ptr_)
			intrusive_ptr(p).swap(*this);
		return *this;
	}

	inline intrusive_ptr& operator =(nullptr_t) noexcept
	{
		if (nullptr != ptr_)
			intrusive_ptr{}.swap(*this);
		return *this;
	}

private:
	pointer_type ptr_{ nullptr };
};

template <class _Ty, class _UTy, class manager_type>
inline bool operator==(intrusive_ptr<_Ty, manager_type> const& lhs, intrusive_ptr<_UTy, manager_type> const& rhs) noexcept
{
	return lhs.get() == rhs.get();
}

template <class _Ty, class _UTy, class manager_type>
inline bool operator!=(intrusive_ptr<_Ty, manager_type> const& lhs, intrusive_ptr<_UTy, manager_type> const& rhs) noexcept
{
	return lhs.get() != rhs.get();
}

template <class _Ty, class _UTy, class manager_type>
inline bool operator<(intrusive_ptr<_Ty, manager_type> const& lhs, intrusive_ptr<_UTy, manager_type> const& rhs) noexcept
{
	return lhs.get() < rhs.get();
}

template <class _Ty, class manager_type>
inline bool operator==(intrusive_ptr<_Ty, manager_type> const& lhs, _Ty* rhs) noexcept
{
	return lhs.get() == rhs;
}

template <class _Ty, class manager_type>
inline bool operator!=(intrusive_ptr<_Ty, manager_type> const& lhs, _Ty* rhs) noexcept
{
	return lhs.get() != rhs;
}

template <class _Ty, class manager_type>
inline bool operator==(_Ty* lhs, intrusive_ptr<_Ty, manager_type> const& rhs) noexcept
{
	return lhs == rhs.get();
}

template <class _Ty, class manager_type>
inline bool operator!=(_Ty* lhs, intrusive_ptr<_Ty, manager_type> const& rhs) noexcept
{
	return lhs != rhs.get();
}

template <class _Ty, class manager_type>
inline bool operator==(intrusive_ptr<_Ty, manager_type> const& lhs, nullptr_t) noexcept
{
	return !static_cast<bool>(lhs);
}

template <class _Ty, class manager_type>
inline bool operator!=(intrusive_ptr<_Ty, manager_type> const& lhs, nullptr_t) noexcept
{
	return static_cast<bool>(lhs);
}

template <class _Ty, class manager_type>
inline bool operator==(nullptr_t, intrusive_ptr<_Ty, manager_type> const& rhs) noexcept
{
	return !static_cast<bool>(rhs);
}

template <class _Ty, class manager_type>
inline bool operator!=(nullptr_t, intrusive_ptr<_Ty, manager_type> const& rhs) noexcept
{
	return static_cast<bool>(rhs);
}

// template class cannot specialize std::swap,
// so implement a swap Function in kiwano namespace
template <class _Ty, class manager_type>
inline void swap(intrusive_ptr<_Ty, manager_type>& lhs, intrusive_ptr<_Ty, manager_type>& rhs) noexcept
{
	lhs.swap(rhs);
}

}  // inline namespace core

}  // namespace kiwano
