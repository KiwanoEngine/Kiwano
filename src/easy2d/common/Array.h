// Copyright (c) 2016-2018 Easy2D - Nomango
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
#include "../macros.h"
#include <type_traits>

namespace easy2d
{
	//
	// Array
	// Lightweight std::vector<>-like class
	//
	template<typename _Ty, typename _Alloc = std::allocator<_Ty>, bool _TyIsClass = std::is_class<_Ty>::value>
	class Array;

	template<typename _Ty, typename _Alloc>
	class __ArrayBase
	{
	public:
		using value_type = _Ty;
		using size_type = int;
		using allocator_type = _Alloc;
		using iterator = value_type*;
		using const_iterator = const value_type*;
		using reference = value_type &;
		using const_reference = const value_type &;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	public:
		inline bool				empty() const						{ return size_ == 0; }
		inline size_type		size() const						{ return size_; }
		inline size_type		size_in_bytes() const				{ return size_ * ((size_type)sizeof(_Ty)); }
		inline size_type		capacity() const					{ return capacity_; }
		inline reference		operator[](size_type off)			{ check_offset(off, "vector subscript out of range"); return data_[off]; }
		inline const_reference  operator[](size_type off) const		{ check_offset(off, "vector subscript out of range"); return data_[off]; }

		
		inline bool				contains(const _Ty& v) const		{ const _Ty* data = data_;  const _Ty* data_end = data_ + size_; while (data < data_end) if (*data++ == v) return true; return false; }
		inline size_type		index_of(const_iterator it) const	{ check_offset(it - cbegin(), "invalid array position"); const ptrdiff_t off = it - data_; return (size_type)off; }

		inline iterator					begin()						{ return iterator(data_); }
		inline const_iterator			begin() const				{ return const_iterator(data_); }
		inline const_iterator			cbegin() const				{ return begin(); }
		inline iterator					end()						{ return iterator(data_ + size_); }
		inline const_iterator			end() const					{ return const_iterator(data_ + size_); }
		inline const_iterator			cend() const				{ return end(); }
		inline reverse_iterator			rbegin()					{ return reverse_iterator(end()); }
		inline const_reverse_iterator	rbegin() const				{ return const_reverse_iterator(end()); }
		inline const_reverse_iterator	crbegin() const				{ return rbegin(); }
		inline reverse_iterator			rend()						{ return reverse_iterator(begin()); }
		inline const_reverse_iterator	rend() const				{ return const_reverse_iterator(begin()); }
		inline const_reverse_iterator	crend() const				{ return rend(); }
		inline reference				front()						{ if (empty()) throw std::out_of_range("front() called on empty array"); return data_[0]; }
		inline const_reference			front() const				{ if (empty()) throw std::out_of_range("front() called on empty array"); return data_[0]; }
		inline reference				back()						{ if (empty()) throw std::out_of_range("back() called on empty array"); return data_[size_ - 1]; }
		inline const_reference			back() const				{ if (empty()) throw std::out_of_range("back() called on empty array"); return data_[size_ - 1]; }

	protected:
		inline void check_offset(const size_type off, const char* const msg) const { if (off < 0 || off >= size_) throw std::out_of_range(msg); }

	protected:
		size_type	size_;
		size_type	capacity_;
		_Ty*		data_;
		_Alloc		allocator_;
	};


	//
	// Array<> for common type
	//
	template<typename _Ty, typename _Alloc>
	class Array<_Ty, _Alloc, false>
		: public __ArrayBase<_Ty, _Alloc>
	{
	public:
		inline Array()																{ size_ = capacity_ = 0; data_ = nullptr; }
		inline Array(std::initializer_list<_Ty> const& list)						{ size_ = capacity_ = 0; data_ = nullptr; operator=(list); }
		inline Array(const Array<_Ty>& src)											{ size_ = capacity_ = 0; data_ = nullptr; operator=(src); }
		inline Array(Array<_Ty>&& src)												{ size_ = capacity_ = 0; data_ = nullptr; swap(src); }
		inline ~Array()																{ destroy(); }

		template <typename _Iter>
		inline Array(_Iter first, _Iter last)										{ size_ = capacity_ = 0; data_ = nullptr; assign_iter(first, last); }

		inline Array<_Ty>&		operator=(const Array<_Ty>& src)					{ if (&src == this) { return *this; } clear(); reserve(src.size_); size_ = src.size_; ::memcpy(data_, src.data_, (size_t)size_ * sizeof(_Ty)); return *this; }
		inline Array<_Ty>&		operator=(Array<_Ty>&& src)							{ swap(src); return *this; }
		inline Array<_Ty>&		operator=(std::initializer_list<_Ty> const& list)	{ clear(); auto new_size =(size_type)list.size(); if (new_size == 0) return; reserve(new_size); size_ = new_size; assign_iter(list.begin(), list.end()); return *this; }

		inline void				clear()												{ size_ = 0; }
		inline void				swap(Array<_Ty>& rhs)								{ size_type rhs_size = rhs.size_; rhs.size_ = size_; size_ = rhs_size; size_type rhs_cap = rhs.capacity_; rhs.capacity_ = capacity_; capacity_ = rhs_cap; _Ty* rhs_data = rhs.data_; rhs.data_ = data_; data_ = rhs_data; }

		inline void				resize(size_type new_size)							{ if (new_size > capacity_) { reserve(grow_capacity(new_size)); } size_ = new_size; }
		inline void				resize(size_type new_size, const _Ty& v)			{ if (new_size > capacity_) reserve(grow_capacity(new_size)); if (new_size > size_) for (int n = size_; n < new_size; n++) ::memcpy(&data_[n], &v, sizeof(v)); size_ = new_size; }
		inline void				reserve(size_type new_capacity)						{ if (new_capacity <= capacity_) return; _Ty* new_data = allocator_.allocate(new_capacity); if (data_) { ::memcpy(new_data, data_, (size_t)size_ * sizeof(_Ty)); allocator_.deallocate(data_, capacity_); } data_ = new_data; capacity_ = new_capacity; }

		inline void				push_back(const _Ty& v)								{ if (size_ == capacity_) reserve(grow_capacity(size_ + 1)); ::memcpy(&data_[size_], &v, sizeof(v)); size_++; }
		inline void				pop_back()											{ if (empty()) throw std::out_of_range("pop() called on empty vector"); size_--; }
		inline void				push_front(const _Ty& v)							{ if (size_ == 0) push_back(v); else insert(data_, v); }

		inline iterator			erase(const_iterator it)							{ const auto off = it - data_; check_offset(off, "invalid vector position"); ::memmove(data_ + off, data_ + off + 1, ((size_t)size_ - (size_t)off - 1) * sizeof(_Ty)); size_--; return data_ + off; }
		inline iterator			erase(const_iterator it, const_iterator it_last)	{ const auto count = it_last - it; const auto off = it - data_; if (count == 0) return it; check_offset(off, "invalid vector position");  ::memmove(data_ + off, data_ + off + count, ((size_t)size_ - (size_t)off - count) * sizeof(_Ty)); size_ -= (size_type)count; return data_ + off; }

		inline iterator			insert(const_iterator it, const _Ty& v)				{ const auto off = it - data_; check_offset(off, "invalid vector position"); if (size_ == capacity_) reserve(grow_capacity(size_ + 1)); if (off < (size_type)size_) ::memmove(data_ + off + 1, data_ + off, ((size_t)size_ - (size_t)off) * sizeof(_Ty)); ::memcpy(&data_[off], &v, sizeof(v)); size_++; return data_ + off; }

	private:
		inline size_type		grow_capacity(size_type sz) const					{ size_type new_capacity = capacity_ ? (capacity_ + capacity_ / 2) : 8; return new_capacity > sz ? new_capacity : sz; }
		inline void				destroy()											{ if (data_) allocator_.deallocate(data_, capacity_); size_ = capacity_ = 0; data_ = nullptr; }

		template <typename _Iter>
		inline void				assign_iter(_Iter first, _Iter last)				{ auto diff = std::distance(first, last); if (diff == 0) return; reserve(diff); size_ = diff; for (size_type index = 0; first != last; ++first, ++index) data_[index] = (*first); }
	};


	//
	// Array<> for class
	//
	template<typename _Ty, typename _Alloc>
	class Array<_Ty, _Alloc, true>
		: public __ArrayBase<_Ty, _Alloc>
	{
	public:
		using value_type				= _Ty;
		using allocator_type			= _Alloc;
		using iterator					= value_type*;
		using const_iterator			= const value_type*;
		using reference					= value_type &;
		using const_reference			= const value_type &;
		using reverse_iterator			= std::reverse_iterator<iterator>;
		using const_reverse_iterator	= std::reverse_iterator<const_iterator>;

		inline Array()																{ size_ = capacity_ = 0; data_ = nullptr; }
		inline Array(std::initializer_list<_Ty> const& list)						{ size_ = capacity_ = 0; data_ = nullptr; operator=(list); }
		inline Array(const Array<_Ty>& src)											{ size_ = capacity_ = 0; data_ = nullptr; operator=(src); }
		inline Array(Array<_Ty>&& src)												{ size_ = capacity_ = 0; data_ = nullptr; operator=(std::forward<Array<_Ty>&&>(src)); }
		inline ~Array()																{ if (data_) destroy_all(); }

		inline Array<_Ty>&		operator=(const Array<_Ty>& src)					{ if (&src == this) { return *this; } clear(); assign_iter(src.begin(), src.end()); return *this; }
		inline Array<_Ty>&		operator=(Array<_Ty>&& src)							{ clear(); size_ = src.size_; capacity_ = src.capacity_; data_ = src.data_; src.size_ = src.capacity_ = 0; src.data_ = nullptr; return *this; }
		inline Array<_Ty>&		operator=(std::initializer_list<_Ty> const& list)	{ clear(); assign_iter(list.begin(), list.end()); return *this; }

		inline void				clear()												{ if (data_) { destroy_all(); } }
		inline void				swap(Array<_Ty>& rhs)								{ size_type rhs_size = rhs.size_; rhs.size_ = size_; size_ = rhs_size; size_type rhs_cap = rhs.capacity_; rhs.capacity_ = capacity_; capacity_ = rhs_cap; _Ty* rhs_data = rhs.data_; rhs.data_ = data_; data_ = rhs_data; }

		inline void				resize(size_type new_size)							{ if (new_size > capacity_) { reserve(grow_capacity(new_size)); } _Ty tmp; std::uninitialized_fill_n(data_ + size_, new_size - size_, tmp); size_ = new_size; }
		inline void				resize(size_type new_size, const _Ty& v)			{ if (new_size > capacity_) reserve(grow_capacity(new_size)); if (new_size > size_) std::uninitialized_fill_n(data_ + size_, new_size - size_, v); size_ = new_size; }
		inline void				reserve(size_type new_capacity)						{ if (new_capacity <= capacity_) return; _Ty* new_data = allocator_.allocate(new_capacity); if (data_) { ::memcpy(new_data, data_, (size_t)size_ * sizeof(_Ty)); allocator_.deallocate(data_, capacity_); } data_ = new_data; capacity_ = new_capacity; }

		inline void				push_back(const _Ty& v)								{ if (size_ == capacity_) reserve(grow_capacity(size_ + 1)); allocator_.construct(data_ + size_, v); size_++; }
		inline void				pop_back()											{ if (empty()) throw std::out_of_range("vector empty before pop"); size_--; }
		inline void				push_front(const _Ty& v)							{ if (size_ == 0) push_back(v); else insert(data_, v); }
		inline iterator			erase(const_iterator it)							{ E2D_ASSERT(it >= data_ && it < data_ + size_); allocator_.destroy(it); const ptrdiff_t off = it - data_; ::memmove(data_ + off, data_ + off + 1, ((size_t)size_ - (size_t)off - 1) * sizeof(_Ty)); size_--; return data_ + off; }
		inline iterator			erase(const_iterator it, const_iterator it_last)	{ E2D_ASSERT(it >= data_ && it < data_ + size_ && it_last > it && it_last <= data_ + size_); destroy(it, it_last); const ptrdiff_t count = it_last - it; const ptrdiff_t off = it - data_; ::memmove(data_ + off, data_ + off + count, ((size_t)size_ - (size_t)off - count) * sizeof(_Ty)); size_ -= (size_type)count; return data_ + off; }
		inline iterator			insert(const_iterator it, const _Ty& v)				{ E2D_ASSERT(it >= data_ && it <= data_ + size_); const ptrdiff_t off = it - data_; if (size_ == capacity_) reserve(grow_capacity(size_ + 1)); if (off < (size_type)size_) ::memmove(data_ + off + 1, data_ + off, ((size_t)size_ - (size_t)off) * sizeof(_Ty)); allocator_.construct(data_ + off, v); size_++; return data_ + off; }

	private:
		inline size_type		grow_capacity(size_type sz) const					{ size_type new_capacity = capacity_ ? (capacity_ + capacity_ / 2) : 8; return new_capacity > sz ? new_capacity : sz; }
		inline void				destroy(iterator it, iterator it_last)				{ E2D_ASSERT(it >= data_ && it < data_ + size_ && it_last > it && it_last <= data_ + size_); for (; it != it_last; ++it) allocator_.destroy(it); }
		inline void				destroy_all()										{ E2D_ASSERT((data_ && capacity_) || (!data_ && !capacity_)); if (size_) destroy(begin(), end()); if (data_ && capacity_) allocator_.deallocate(data_, capacity_); size_ = capacity_ = 0; data_ = nullptr; }
		
		template <typename _Iter>
		inline void				assign_iter(_Iter first, _Iter last)				{ auto diff = std::distance(first, last); if (diff == 0) return; resize(diff); for (size_type index = 0; first != last; ++first, ++index) data_[index] = (*first); }
	};
}
