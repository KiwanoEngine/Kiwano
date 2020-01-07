// Copyright (c) 2019-2020 OuterC - Nomango
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
#include <utility>
#include <type_traits>
#include <atomic>
#include "macros.h"

namespace oc
{

template <typename _Ty, typename _ProxyTy>
class intrusive_ptr
{
public:
	using value_type			= _Ty;
	using pointer_type			= _Ty*;
	using const_pointer_type	= const _Ty*;
	using reference_type		= _Ty&;
	using const_reference_type	= const _Ty&;
	using ref_proxy_type		= _ProxyTy;

	intrusive_ptr() noexcept												: ptr_(nullptr)		{}
	intrusive_ptr(std::nullptr_t) noexcept									: ptr_(nullptr)		{}
	intrusive_ptr(pointer_type p)											: ptr_(p)			{ typename ref_proxy_type::add_ref(ptr_); }
	intrusive_ptr(const intrusive_ptr& other)								: ptr_(other.ptr_)	{ typename ref_proxy_type::add_ref(ptr_); }
	intrusive_ptr(intrusive_ptr&& other) noexcept							: ptr_(nullptr)		{ swap(other); }
	~intrusive_ptr()														{ tidy(); }

	template <typename _UTy>
	intrusive_ptr(const intrusive_ptr<_UTy, ref_proxy_type>& other)			{ ptr_ = const_cast<pointer_type>(dynamic_cast<const_pointer_type>(other.get())); typename ref_proxy_type::add_ref(ptr_); }

	inline pointer_type			get() noexcept								{ return ptr_; }
	inline const_pointer_type	get() const noexcept						{ return ptr_; }
	inline void					reset(pointer_type ptr = nullptr)			{ if (ptr) intrusive_ptr(ptr).swap(*this); else tidy(); }
	inline void					swap(intrusive_ptr& other) noexcept			{ std::swap(ptr_, other.ptr_); }

	inline pointer_type			operator ->()								{ OC_ASSERT(ptr_ != nullptr && "Invalid pointer"); return ptr_; }
	inline const_pointer_type	operator ->() const							{ OC_ASSERT(ptr_ != nullptr && "Invalid pointer"); return ptr_; }
	inline reference_type		operator *()								{ OC_ASSERT(ptr_ != nullptr && "Invalid pointer"); return *ptr_; }
	inline const_reference_type	operator *() const							{ OC_ASSERT(ptr_ != nullptr && "Invalid pointer"); return *ptr_; }
	inline pointer_type*		operator &()								{ OC_ASSERT(ptr_ == nullptr && "Memory leak"); return &ptr_; }
	inline						operator bool() const noexcept				{ return ptr_ != nullptr; }
	inline bool					operator !() const noexcept					{ return ptr_ == 0; }

	inline intrusive_ptr&		operator=(const intrusive_ptr& other)		{ if (other.ptr_ != ptr_) intrusive_ptr(other).swap(*this); return (*this); }
	inline intrusive_ptr&		operator=(intrusive_ptr&& other) noexcept	{ if (other.ptr_ != ptr_) other.swap(*this); return (*this); }
	inline intrusive_ptr&		operator=(pointer_type p)					{ if (p != ptr_) intrusive_ptr(p).swap(*this); return (*this); }
	inline intrusive_ptr&		operator=(std::nullptr_t)					{ tidy(); return *this; }

private:
	void tidy()
	{
		typename ref_proxy_type::release(ptr_);
		ptr_ = nullptr;
	}

private:
	pointer_type ptr_;
};

template <class _Ty, class _UTy, class _ProxyTy>
inline bool operator==(intrusive_ptr<_Ty, _ProxyTy> const& lhs, intrusive_ptr<_UTy, _ProxyTy> const& rhs) noexcept
{
	return lhs.get() == rhs.get();
}

template <class _Ty, class _ProxyTy>
inline bool operator==(intrusive_ptr<_Ty, _ProxyTy> const& lhs, _Ty* rhs) noexcept
{
	return lhs.get() == rhs;
}

template <class _Ty, class _ProxyTy>
inline bool operator==(_Ty* lhs, intrusive_ptr<_Ty, _ProxyTy> const& rhs) noexcept
{
	return lhs == rhs.get();
}

template <class _Ty, class _ProxyTy>
inline bool operator==(intrusive_ptr<_Ty, _ProxyTy> const& lhs, std::nullptr_t) noexcept
{
	return !static_cast<bool>(lhs);
}

template <class _Ty, class _ProxyTy>
inline bool operator==(std::nullptr_t, intrusive_ptr<_Ty, _ProxyTy> const& rhs) noexcept
{
	return !static_cast<bool>(rhs);
}

template <class _Ty, class _UTy, class _ProxyTy>
inline bool operator!=(intrusive_ptr<_Ty, _ProxyTy> const& lhs, intrusive_ptr<_UTy, _ProxyTy> const& rhs) noexcept
{
	return !(lhs == rhs);
}

template <class _Ty, class _ProxyTy>
inline bool operator!=(intrusive_ptr<_Ty, _ProxyTy> const& lhs, _Ty* rhs) noexcept
{
	return lhs.get() != rhs;
}

template <class _Ty, class _ProxyTy>
inline bool operator!=(_Ty* lhs, intrusive_ptr<_Ty, _ProxyTy> const& rhs) noexcept
{
	return lhs != rhs.get();
}

template <class _Ty, class _ProxyTy>
inline bool operator!=(intrusive_ptr<_Ty, _ProxyTy> const& lhs, std::nullptr_t) noexcept
{
	return static_cast<bool>(lhs);
}

template <class _Ty, class _ProxyTy>
inline bool operator!=(std::nullptr_t, intrusive_ptr<_Ty, _ProxyTy> const& rhs) noexcept
{
	return static_cast<bool>(rhs);
}

template <class _Ty, class _UTy, class _ProxyTy>
inline bool operator<(intrusive_ptr<_Ty, _ProxyTy> const& lhs, intrusive_ptr<_UTy, _ProxyTy> const& rhs) noexcept
{
	return lhs.get() < rhs.get();
}

// template class cannot specialize std::swap,
// so implement a swap Function in oc namespace
template <class _Ty, class _ProxyTy>
inline void swap(intrusive_ptr<_Ty, _ProxyTy>& lhs, intrusive_ptr<_Ty, _ProxyTy>& rhs) noexcept
{
	lhs.swap(rhs);
}


class intrusive_ref
{
public:
	void add_ref()
	{
		++ref_count_;
	}

	void release()
	{
		--ref_count_;
		if (ref_count_ == 0)
		{
			delete this;
		}
	}

	int16_t get_ref() const
	{
		return ref_count_;
	}

protected:
	intrusive_ref()
		: ref_count_(0)
	{
	}

private:
	std::atomic<int16_t> ref_count_;
};


class intrusive_ref_proxy
{
public:
	static inline void add_ref(intrusive_ref* ptr) { if (ptr) ptr->add_ref(); }

	static inline void release(intrusive_ref* ptr) { if (ptr) ptr->release(); }
};

template<
	typename _Ty,
	typename = typename std::enable_if<std::is_base_of<intrusive_ref, _Ty>::value, int>::type>
using intrusive_ref_ptr = intrusive_ptr<_Ty, intrusive_ref_proxy>;

}  // namespace oc
