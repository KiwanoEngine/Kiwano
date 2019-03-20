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
#include <memory>
#include <type_traits>
#include <exception>

namespace easy2d
{
	//
	// Array
	// Lightweight std::vector<>-like class
	//
	template<typename _Ty, typename _Alloc, typename _Manager>
	class Array;


	//
	// ArrayManager<> with memory operations
	//
	template<typename _Ty, typename _Alloc, bool _IsClassType = std::is_class<_Ty>::value>
	struct __ArrayManager;


	//
	// Array<>
	//
	template<
		typename _Ty,
		typename _Alloc = std::allocator<_Ty>,
		typename _Manager = __ArrayManager<_Ty, _Alloc>>
	class Array
	{
	public:
		using value_type				= _Ty;
		using size_type					= std::size_t;
		using iterator					= value_type * ;
		using const_iterator			= const value_type*;
		using reference					= value_type & ;
		using const_reference			= const value_type &;
		using reverse_iterator			= std::reverse_iterator<iterator>;
		using const_reverse_iterator	= std::reverse_iterator<const_iterator>;
		using allocator_type			= typename _Alloc;
		using manager					= typename _Manager;
		using initializer_list			= std::initializer_list<value_type>;

	public:
		inline Array()																	: size_(0), capacity_(0), data_(nullptr) { }
		inline Array(size_type count)													: Array() { reserve(count); }
		inline Array(size_type count, const _Ty& val)									: Array() { assign(count, val); }
		inline Array(initializer_list list)												: Array() { assign(list); }
		inline Array(const Array& src)													: Array() { assign(src); }
		inline Array(Array&& src)														: Array() { swap(src); }
		inline ~Array()																	{ destroy(); }

		template <typename _Iter>
		inline Array(_Iter first, _Iter last)											: Array() { assign(first, last); }

		inline Array&		operator=(const Array& src)									{ if (&src != this) { resize(src.size_); manager::copy_data(begin(), src.cbegin(), size_); } return (*this); }
		inline Array&		operator=(Array&& src)										{ swap(src); return *this; }
		inline Array&		operator=(initializer_list list)							{ if (list.size()) { assign(list.begin(), list.end()); } else clear(); return (*this); }

		inline Array&		assign(size_type count, const _Ty& val)						{ if (count > 0) { resize(count); manager::copy_data(begin(), count, val); } else clear(); return (*this); }
		inline Array&		assign(const Array& src)									{ return operator=(src); }
		inline Array&		assign(initializer_list list)								{ return operator=(list); }

		template <typename _Iter>
		inline void			assign(_Iter first, _Iter last)								{ auto diff = std::distance(first, last); resize((size_type)diff); auto data = begin(); while (first != last) (*data++) = (*first++); }

		inline void			clear()														{ destroy(); size_ = capacity_ = 0; data_ = nullptr; }
		inline void			swap(Array& rhs)											{ std::swap(size_, rhs.size_); std::swap(capacity_, rhs.capacity_); std::swap(data_, rhs.data_); }

		inline void			resize(size_type new_size)									{ resize(new_size, _Ty()); }
		inline void			resize(size_type new_size, const _Ty& v);
		inline void			reserve(size_type new_capacity);

		inline void			push_back(const _Ty& val)									{ resize(size_ + 1, val); }
		inline void			pop_back()													{ if (empty()) throw std::out_of_range("pop() called on empty vector"); resize(size_ - 1); }
		inline void			push_front(const _Ty& val)									{ if (size_ == 0) push_back(val); else insert(begin(), val); }

		inline iterator		erase(const_iterator where)									{ return erase(where, where + 1); }
		inline iterator		erase(const_iterator first, const_iterator last);

		inline iterator		insert(const_iterator where, const _Ty& v);

		inline bool						empty() const									{ return size_ == 0; }
		inline size_type				size() const									{ return size_; }
		inline size_type				size_in_bytes() const							{ return size_ * ((size_type)sizeof(_Ty)); }
		inline size_type				capacity() const								{ return capacity_; }
		inline reference				operator[](size_type off)						{ if (off < 0 || off >= size_) throw std::out_of_range("vector subscript out of range"); return data_[off]; }
		inline const_reference			operator[](size_type off) const					{ if (off < 0 || off >= size_) throw std::out_of_range("vector subscript out of range"); return data_[off]; }

		
		inline bool						contains(const _Ty& v) const					{ auto data = cbegin();  const auto data_end = cend(); while (data != data_end) if (*(data++) == v) return true; return false; }
		inline size_type				index_of(const_iterator it) const				{ check_offset(it - cbegin(), "invalid array position"); return it - data_; }

		inline iterator					begin()											{ return iterator(data_); }
		inline const_iterator			begin() const									{ return const_iterator(data_); }
		inline const_iterator			cbegin() const									{ return begin(); }
		inline iterator					end()											{ return iterator(data_ + size_); }
		inline const_iterator			end() const										{ return const_iterator(data_ + size_); }
		inline const_iterator			cend() const									{ return end(); }
		inline reverse_iterator			rbegin()										{ return reverse_iterator(end()); }
		inline const_reverse_iterator	rbegin() const									{ return const_reverse_iterator(end()); }
		inline const_reverse_iterator	crbegin() const									{ return rbegin(); }
		inline reverse_iterator			rend()											{ return reverse_iterator(begin()); }
		inline const_reverse_iterator	rend() const									{ return const_reverse_iterator(begin()); }
		inline const_reverse_iterator	crend() const									{ return rend(); }
		inline reference				front()											{ if (empty()) throw std::out_of_range("front() called on empty array"); return data_[0]; }
		inline const_reference			front() const									{ if (empty()) throw std::out_of_range("front() called on empty array"); return data_[0]; }
		inline reference				back()											{ if (empty()) throw std::out_of_range("back() called on empty array"); return data_[size_ - 1]; }
		inline const_reference			back() const									{ if (empty()) throw std::out_of_range("back() called on empty array"); return data_[size_ - 1]; }

	private:
		inline size_type	grow_capacity(size_type sz) const							{ size_type new_capacity = capacity_ ? (capacity_ + capacity_ / 2) : 8; return new_capacity > sz ? new_capacity : sz; }
		inline void			check_offset(const size_type off) const						{ if (off < 0 || off >= size_) throw std::out_of_range("invalid vector position"); }

		inline void			destroy()													{ manager::destroy(data_, size_); manager::deallocate(data_, capacity_); }
	protected:
		size_type	size_;
		size_type	capacity_;
		_Ty*		data_;
	};

	template<typename _Ty, typename _Alloc, typename _Manager>
	inline void Array<_Ty, _Alloc, _Manager>::resize(size_type new_size, const _Ty& val)
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

	template<typename _Ty, typename _Alloc, typename _Manager>
	inline void Array<_Ty, _Alloc, _Manager>::reserve(size_type new_capacity)
	{
		if (new_capacity <= capacity_)
			return;

		auto new_data = manager::allocate(new_capacity);
		if (data_)
		{
			manager::construct(new_data, size_/* only construct needed size */);
			manager::copy_data(new_data, data_, size_);
			/* destroy old memory, but not resize */
			destroy();
		}
		data_ = new_data;
		capacity_ = new_capacity;
	}

	template<typename _Ty, typename _Alloc, typename _Manager>
	inline typename Array<_Ty, _Alloc, _Manager>::iterator
		Array<_Ty, _Alloc, _Manager>::erase(const_iterator first, const_iterator last)
	{
		const auto off = first - begin();
		const auto count = last - first;

		if (count != 0)
		{
			check_offset(off);

			manager::move_data(begin() + off, begin() + off + count, size_ - off - count);
			resize(size_ - count);  // do destruction
		}
		return begin() + off;
	}

	template<typename _Ty, typename _Alloc, typename _Manager>
	inline typename Array<_Ty, _Alloc, _Manager>::iterator
		Array<_Ty, _Alloc, _Manager>::insert(const_iterator where, const _Ty& v)
	{
		const auto off = where - begin();
		const auto insert_at = begin() + off;

		check_offset(off);
		
		resize(size_ + 1);
		manager::move_data(insert_at + 1, insert_at, size_ - off - 1);
		data_[off] = v;
		return begin() + off;
	}

	//
	// ArrayManager for common type
	//
	template<typename _Ty, typename _Alloc>
	struct __ArrayManager<_Ty, _Alloc, false>
	{
		using value_type		= _Ty;
		using size_type			= int;
		using allocator_type	= typename _Alloc;

		static inline void copy_data(value_type* dest, const value_type* src, size_type count)	{ if (src == dest) return; ::memcpy(dest, src, (size_t)count * sizeof(value_type)); }
		static inline void copy_data(value_type* dest, size_type count, const value_type& val)	{ ::memset(dest, (int)val, (size_t)count * sizeof(value_type)); }
		static inline void move_data(value_type* dest, const value_type* src, size_type count)	{ if (src == dest) return; ::memmove(dest, src, (size_t)count * sizeof(value_type)); }

		static inline value_type* allocate(size_type count)										{ return get_allocator().allocate(count); }
		static inline void deallocate(value_type*& ptr, size_type count)						{ if (ptr) { get_allocator().deallocate(ptr, count); ptr = nullptr; } }

		static inline void construct(value_type* ptr, size_type count)							{ }
		static inline void construct(value_type* ptr, size_type count, const value_type& val)	{ while (count) { --count; *(ptr + count) = val; } }
		static inline void destroy(value_type* ptr, size_type count)							{ }

	private:
		static allocator_type& get_allocator()
		{
			static allocator_type allocator_;
			return allocator_;
		}
	};

	//
	// ArrayManager for class
	//
	template<typename _Ty, typename _Alloc>
	struct __ArrayManager<_Ty, _Alloc, true>
	{
		using value_type		= _Ty;
		using size_type			= int;
		using allocator_type	= typename _Alloc;

		static inline void copy_data(value_type* dest, const value_type* src, size_type count)		{ if (src == dest) return; while (count--) (*dest++) = (*src++); }
		static inline void copy_data(value_type* dest, size_type count, const value_type& val)		{ while (count--) (*dest++) = val; }
		static inline void move_data(value_type* dest, const value_type* src, size_type count)
		{
			if (src == dest) return;
			if (dest > src && dest < src + count)
			{
				src = src + count - 1;
				dest = dest + count - 1;
				while (count--)
					(*dest--) = (*src--);
			}
			else
			{
				while (count--)
					(*dest++) = (*src++);
			}
		}

		static inline value_type* allocate(size_type count)										{ return get_allocator().allocate(count); }
		static inline void deallocate(value_type*& ptr, size_type count)						{ if (ptr) { get_allocator().deallocate(ptr, count); ptr = nullptr; } }

		static inline void construct(value_type* ptr, size_type count)							{ construct(ptr, count, value_type()); }
		static inline void construct(value_type* ptr, size_type count, const value_type& val)	{ while (count) get_allocator().construct(ptr + (--count), val); }
		static inline void destroy(value_type* ptr, size_type count)							{ while (count) get_allocator().destroy(ptr + (--count)); }

	private:
		static allocator_type& get_allocator()
		{
			static allocator_type allocator_;
			return allocator_;
		}
	};

}
