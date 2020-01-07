// Copyright (c) 2019-2020 OuterC - Nomango

#pragma once
#include <memory>
#include <stdexcept>
#include "vector/details.h"

namespace oc
{

//
// vector<>
// Lightweight std::vector<>-like class
//
template <typename _Ty, typename _Alloc = std::allocator<_Ty>>
class vector
{
public:
	using value_type				= _Ty;
	using size_type					= size_t;
	using reference					= value_type&;
	using const_reference			= const value_type&;
	using iterator					= value_type*;
	using const_iterator			= const value_type*;
	using reverse_iterator			= std::reverse_iterator<iterator>;
	using const_reverse_iterator	= std::reverse_iterator<const_iterator>;
	using allocator_type			= typename _Alloc;
	using manager					= typename __vector_details::vector_memory_manager<_Ty, _Alloc>;
	using initializer_list			= std::initializer_list<value_type>;

public:
	inline vector()															: size_(0), capacity_(0), data_(nullptr) { }
	inline vector(size_type count)											: vector() { reserve(count); }
	inline vector(size_type count, const _Ty& val)							: vector() { assign(count, val); }
	inline vector(initializer_list list)									: vector() { assign(list); }
	inline vector(const vector& src)										: vector() { assign(src); }
	inline vector(vector&& src) noexcept									: vector() { swap(src); }
	inline ~vector()														{ destroy(); }

	template <typename _Iter>
	inline vector(_Iter first, _Iter last)									: vector() { assign(first, last); }

	inline vector&		operator=(const vector& src)						{ if (&src != this) { resize(src.size_); manager::copy_n(begin(), src.cbegin(), size_); } return (*this); }
	inline vector&		operator=(vector&& src) noexcept					{ swap(src); return *this; }
	inline vector&		operator=(initializer_list list)					{ if (list.size()) { assign(list.begin(), list.end()); } else clear(); return (*this); }

	inline vector&		assign(size_type count, const _Ty& val)				{ if (count > 0) { resize(count); manager::fill_n(begin(), count, val); } else clear(); return (*this); }
	inline vector&		assign(const vector& src)							{ return operator=(src); }
	inline vector&		assign(initializer_list list)						{ return operator=(list); }

	template <typename _Iter>
	inline void			assign(_Iter first, _Iter last)						{ auto diff = std::distance(first, last); resize((size_type)diff); auto data = begin(); while (first != last) (*data++) = (*first++); }

	inline void			clear()												{ destroy(); size_ = capacity_ = 0; data_ = nullptr; }
	inline void			swap(vector& rhs) noexcept							{ std::swap(size_, rhs.size_); std::swap(capacity_, rhs.capacity_); std::swap(data_, rhs.data_); }

	inline void			resize(size_type new_size)							{ resize(new_size, _Ty()); }
	void				resize(size_type new_size, const _Ty& v);
	void				reserve(size_type new_capacity);

	inline void			push_back(const _Ty& val)							{ resize(size_ + 1, val); }
	inline void			pop_back()											{ if (empty()) throw std::out_of_range("pop() called on empty vector"); resize(size_ - 1); }
	inline void			push_front(const _Ty& val)							{ if (size_ == 0) push_back(val); else insert(begin(), val); }

	inline iterator		erase(const_iterator where)							{ return erase(where, where + 1); }
	iterator			erase(const_iterator first, const_iterator last);

	iterator			insert(const_iterator where, const _Ty& v);

	inline bool						empty() const							{ return size_ == 0; }
	inline size_type				size() const							{ return size_; }
	inline size_type				size_in_bytes() const					{ return size_ * ((size_type)sizeof(_Ty)); }
	inline size_type				capacity() const						{ return capacity_; }
	inline reference				operator[](size_type off)				{ if (off < 0 || off >= size_) throw std::out_of_range("vector subscript out of range"); return data_[off]; }
	inline const_reference			operator[](size_type off) const			{ if (off < 0 || off >= size_) throw std::out_of_range("vector subscript out of range"); return data_[off]; }

		
	inline bool						contains(const _Ty& v) const			{ auto data = cbegin();  const auto data_end = cend(); while (data != data_end) if (*(data++) == v) return true; return false; }
	inline size_type				index_of(const_iterator it) const		{ check_offset(it - cbegin(), "invalid array position"); return it - data_; }

	inline iterator					begin()									{ return iterator(data_); }
	inline const_iterator			begin() const							{ return const_iterator(data_); }
	inline const_iterator			cbegin() const							{ return begin(); }
	inline iterator					end()									{ return iterator(data_ + size_); }
	inline const_iterator			end() const								{ return const_iterator(data_ + size_); }
	inline const_iterator			cend() const							{ return end(); }
	inline reverse_iterator			rbegin()								{ return reverse_iterator(end()); }
	inline const_reverse_iterator	rbegin() const							{ return const_reverse_iterator(end()); }
	inline const_reverse_iterator	crbegin() const							{ return rbegin(); }
	inline reverse_iterator			rend()									{ return reverse_iterator(begin()); }
	inline const_reverse_iterator	rend() const							{ return const_reverse_iterator(begin()); }
	inline const_reverse_iterator	crend() const							{ return rend(); }
	inline reference				front()									{ if (empty()) throw std::out_of_range("front() called on empty array"); return data_[0]; }
	inline const_reference			front() const							{ if (empty()) throw std::out_of_range("front() called on empty array"); return data_[0]; }
	inline reference				back()									{ if (empty()) throw std::out_of_range("back() called on empty array"); return data_[size_ - 1]; }
	inline const_reference			back() const							{ if (empty()) throw std::out_of_range("back() called on empty array"); return data_[size_ - 1]; }

private:
	inline size_type				grow_capacity(size_type sz) const		{ size_type new_capacity = capacity_ ? (capacity_ + capacity_ / 2) : 8; return new_capacity > sz ? new_capacity : sz; }
	inline void						check_offset(const size_type off) const	{ if (off < 0 || off >= size_) throw std::out_of_range("invalid vector position"); }

	inline void						destroy()								{ manager::destroy(data_, size_); manager::deallocate(data_, capacity_); }
protected:
	size_type	size_;
	size_type	capacity_;
	_Ty*		data_;
};

template<typename _Ty, typename _Alloc>
void vector<_Ty, _Alloc>::resize(size_type new_size, const _Ty& val)
{
	if (new_size > size_)
	{
		if (new_size > capacity_)
		{
			reserve(grow_capacity(new_size));
		}
		manager::construct(begin() + size_, new_size - size_, val);
	}
	else
	{
		manager::destroy(begin() + new_size, size_ - new_size);
	}
	size_ = new_size;
}

template<typename _Ty, typename _Alloc>
void vector<_Ty, _Alloc>::reserve(size_type new_capacity)
{
	if (new_capacity <= capacity_)
		return;

	auto new_data = manager::allocate(new_capacity);
	if (data_)
	{
		manager::construct_n(new_data, data_, size_/* only construct needed size */);
		/* destroy old memory, but not resize */
		destroy();
	}
	data_ = new_data;
	capacity_ = new_capacity;
}

template<typename _Ty, typename _Alloc>
typename vector<_Ty, _Alloc>::iterator
	vector<_Ty, _Alloc>::erase(const_iterator first, const_iterator last)
{
	const auto off = first - begin();
	const auto count = last - first;

	if (count != 0)
	{
		check_offset(off);

		manager::move(begin() + off, begin() + off + count, size_ - off - count);
		resize(size_ - count);  // do destruction
	}
	return begin() + off;
}

template<typename _Ty, typename _Alloc>
typename vector<_Ty, _Alloc>::iterator
	vector<_Ty, _Alloc>::insert(const_iterator where, const _Ty& v)
{
	const auto off = where - begin();
	const auto insert_at = begin() + off;

	check_offset(off);
		
	resize(size_ + 1);
	manager::move(insert_at + 1, insert_at, size_ - off - 1);
	data_[off] = v;
	return begin() + off;
}

}  // namespace oc
