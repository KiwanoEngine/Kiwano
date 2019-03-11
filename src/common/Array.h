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

namespace easy2d
{
	//
	// Array
	// Lightweight std::vector<>-like class
	//
	template<typename _Ty, typename _Alloc = std::allocator<_Ty>>
	class Array
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

		inline Array()												{ size_ = capacity_ = 0; data_ = nullptr; }
		inline Array(std::initializer_list<_Ty> const& list)		{ size_ = capacity_ = 0; data_ = nullptr; operator=(list); }
		inline Array(const Array<_Ty>& src)							{ size_ = capacity_ = 0; data_ = nullptr; operator=(src); }
		inline Array(Array<_Ty>&& src)								{ size_ = capacity_ = 0; data_ = nullptr; operator=(std::forward<Array<_Ty>&&>(src)); }
		inline ~Array()												{ if (data_) _destroy_all(); }

		inline Array<_Ty>& operator=(const Array<_Ty>& src)						{ if (&src == this) { return *this; } clear(); reserve(src.size_); std::uninitialized_copy(src.begin(), src.end(), begin()); size_ = src.size_; return *this; }
		inline Array<_Ty>& operator=(Array<_Ty>&& src)							{ clear(); size_ = src.size_; capacity_ = src.capacity_; data_ = src.data_; src.size_ = src.capacity_ = 0; src.data_ = nullptr; return *this; }
		inline Array<_Ty>& operator=(std::initializer_list<_Ty> const& list)	{ clear(); reserve((int)list.size()); std::uninitialized_copy(list.begin(), list.end(), begin()); size_ = (int)list.size(); return *this; }
	
		inline bool				empty() const						{ return size_ == 0; }
		inline int				size() const						{ return size_; }
		inline int				size_in_bytes() const				{ return size_ * ((int)sizeof(_Ty)); }
		inline int				capacity() const					{ return capacity_; }
		inline reference		operator[](int i)					{ if (i < 0 || i >= size_) throw std::out_of_range("vector subscript out of range"); return data_[i]; }
		inline const_reference  operator[](int i) const				{ if (i < 0 || i >= size_) throw std::out_of_range("vector subscript out of range"); return data_[i]; }

		inline void						clear()						{ if (data_) { _destroy_all(); } }
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
		inline void						swap(Array<_Ty>& rhs)		{ int rhs_size = rhs.size_; rhs.size_ = size_; size_ = rhs_size; int rhs_cap = rhs.capacity_; rhs.capacity_ = capacity_; capacity_ = rhs_cap; _Ty* rhs_data = rhs.data_; rhs.data_ = data_; data_ = rhs_data; }

		inline void				resize(int new_size)				{ if (new_size > capacity_) { reserve(_grow_capacity(new_size)); } _Ty tmp; std::uninitialized_fill_n(data_ + size_, new_size - size_, tmp); size_ = new_size; }
		inline void				resize(int new_size, const _Ty& v)	{ if (new_size > capacity_) reserve(_grow_capacity(new_size)); if (new_size > size_) std::uninitialized_fill_n(data_ + size_, new_size - size_, v); size_ = new_size; }
		inline void				reserve(int new_capacity)			{ if (new_capacity <= capacity_) return; _Ty* new_data = allocator_.allocate(new_capacity); if (data_) { ::memcpy(new_data, data_, (size_t)size_ * sizeof(_Ty)); allocator_.deallocate(data_, capacity_); } data_ = new_data; capacity_ = new_capacity; }

		inline void				push_back(const _Ty& v)								{ if (size_ == capacity_) reserve(_grow_capacity(size_ + 1)); allocator_.construct(data_ + size_, v); size_++; }
		inline void				pop_back()											{ if (empty()) throw std::out_of_range("vector empty before pop"); size_--; }
		inline void				push_front(const _Ty& v)							{ if (size_ == 0) push_back(v); else insert(data_, v); }
		inline iterator			erase(const_iterator it)							{ E2D_ASSERT(it >= data_ && it < data_ + size_); allocator_.destroy(it); const ptrdiff_t off = it - data_; ::memmove(data_ + off, data_ + off + 1, ((size_t)size_ - (size_t)off - 1) * sizeof(_Ty)); size_--; return data_ + off; }
		inline iterator			erase(const_iterator it, const_iterator it_last)	{ E2D_ASSERT(it >= data_ && it < data_ + size_ && it_last > it && it_last <= data_ + size_); _destroy(it, it_last); const ptrdiff_t count = it_last - it; const ptrdiff_t off = it - data_; ::memmove(data_ + off, data_ + off + count, ((size_t)size_ - (size_t)off - count) * sizeof(_Ty)); size_ -= (int)count; return data_ + off; }
		inline iterator			insert(const_iterator it, const _Ty& v)				{ E2D_ASSERT(it >= data_ && it <= data_ + size_); const ptrdiff_t off = it - data_; if (size_ == capacity_) reserve(_grow_capacity(size_ + 1)); if (off < (int)size_) ::memmove(data_ + off + 1, data_ + off, ((size_t)size_ - (size_t)off) * sizeof(_Ty)); allocator_.construct(data_ + off, v); size_++; return data_ + off; }
		inline bool				contains(const _Ty& v) const						{ const _Ty* data = data_;  const _Ty* data_end = data_ + size_; while (data < data_end) if (*data++ == v) return true; return false; }
		inline int				index_of(const_iterator it) const					{ E2D_ASSERT(it >= data_ && it <= data_ + size_); const ptrdiff_t off = it - data_; return (int)off; }

	private:
		inline int				_grow_capacity(int sz) const						{ int new_capacity = capacity_ ? (capacity_ + capacity_ / 2) : 8; return new_capacity > sz ? new_capacity : sz; }
		inline void				_destroy(iterator it, iterator it_last)				{ E2D_ASSERT(it >= data_ && it < data_ + size_ && it_last > it && it_last <= data_ + size_); for (; it != it_last; ++it) allocator_.destroy(it); }
		inline void				_destroy_all()										{ E2D_ASSERT((data_ && capacity_) || (!data_ && !capacity_)); if (size_) _destroy(begin(), end()); if (data_ && capacity_) allocator_.deallocate(data_, capacity_); size_ = capacity_ = 0; data_ = nullptr; }

	private:
		int size_;
		int capacity_;
		_Ty* data_;

		typename _Alloc allocator_;
	};
}
