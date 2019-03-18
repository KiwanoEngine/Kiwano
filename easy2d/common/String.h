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
#include <string>
#include <cstring>
#include <cstdio>

namespace easy2d
{
	//
	// String
	// Lightweight std::wstring<>-like class
	//
	class String
	{
	public:
		// Iterator
		template <typename _Ty>
		struct _Iterator
		{
			using iterator_category	= typename std::iterator_traits<_Ty*>::iterator_category;
			using value_type		= typename std::iterator_traits<_Ty*>::value_type;
			using difference_type	= typename std::iterator_traits<_Ty*>::difference_type;
			using pointer			= typename std::iterator_traits<_Ty*>::pointer;
			using reference			= typename std::iterator_traits<_Ty*>::reference;

			// disable warning 4996
			using _Unchecked_type = _Ty;

			inline _Iterator(pointer base = nullptr) : base_(base) {}

			inline reference operator*() const								{ return *base_; }
			inline pointer base() const										{ return base_; }

			inline _Iterator& operator++()									{ ++base_; return (*this); }
			inline _Iterator operator++(int)								{ _Iterator old = (*this); ++(*this); return old; }

			inline _Iterator& operator--()									{ --base_; return (*this); }
			inline _Iterator operator--(int)								{ _Iterator old = (*this); --(*this); return old; }

			inline const _Iterator operator+(difference_type off) const		{ return _Iterator(base_ + off); }
			inline const _Iterator operator-(difference_type off) const		{ return _Iterator(base_ - off); }

			inline _Iterator& operator+=(difference_type off)				{ base_ += off; return (*this); }
			inline _Iterator& operator-=(difference_type off)				{ base_ -= off; return (*this); }

			inline difference_type operator-(_Iterator const& other) const	{ return base_ - other.base_; }

			inline bool operator==(_Iterator const& other) const			{ return base_ == other.base_; }
			inline bool operator!=(_Iterator const& other) const			{ return !(*this == other); }

			inline bool operator<(_Iterator const& other) const				{ return base_ < other.base_; }
			inline bool operator<=(_Iterator const& other) const			{ return base_ <= other.base_; }
			inline bool operator>(_Iterator const& other) const				{ return base_ > other.base_; }
			inline bool operator>=(_Iterator const& other) const			{ return base_ >= other.base_; }

			inline reference operator[](difference_type off)				{ return *(base_ + off); }
			inline const reference operator[](difference_type off) const	{ return *(base_ + off); }

			inline operator bool() const									{ return base_ != nullptr; }

		private:
			pointer base_{ nullptr };
		};

	public:
		using value_type = wchar_t;
		using size_type = size_t;
		using reference = value_type &;
		using const_reference = const value_type &;
		using iterator = _Iterator<value_type>;
		using const_iterator = _Iterator<const value_type>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		using traits = std::char_traits<value_type>;
		using allocator = std::allocator<value_type>;

		String();
		String(const wchar_t* cstr, bool const_str = true);
		String(const wchar_t* cstr, size_type count);
		String(size_type count, wchar_t ch);
		String(const char* cstr);
		String(std::string const& str);
		String(std::wstring const& str);
		String(String const& rhs);
		String(String const& rhs, size_type pos, size_type count = npos);
		String(String && rhs);
		~String();

		template <typename _Iter>
		String(_Iter first, _Iter last) : String()																		{ assign_iter(first, last); }

		inline const wchar_t*	c_str() const																			{ return const_str_ ? const_str_ : L""; }
		inline wchar_t			at(size_t i) const																		{ return (*this)[i]; }
		inline size_type		size() const																			{ return size_; }
		inline size_type		length() const																			{ return size(); }
		inline size_type		capacity() const																		{ return capacity_; }
		inline size_type		max_size() const																		{ return (static_cast<size_type>(-1) / sizeof(value_type)); }
		inline bool				empty() const																			{ return size_ == 0; }
		inline void				clear()																					{ discard_const_data(); if (str_) { str_[0] = value_type(); } size_ = 0; }

		void					reserve(const size_type new_cap = 0);
		inline void				resize(const size_type new_size, const wchar_t ch = value_type())						{ check_operability(); if (new_size < size_) str_[size_ = new_size] = value_type(); else append(new_size - size_, ch); }

		int						compare(const wchar_t* const str) const;
		inline int				compare(String const& str) const														{ return compare(str.c_str()); }

		String&					append(size_type count, wchar_t ch);
		String&					append(const wchar_t* cstr, size_type count);
		String&					append(String const& other, size_type pos, size_type count = npos);
		inline String&			append(const wchar_t* cstr)																{ return append(cstr, traits::length(cstr)); }
		inline String&			append(std::wstring const& str)															{ return append(str.c_str()); }
		inline String&			append(String const& other)																{ return append(other.const_str_, 0, npos); }

		size_type				find(const wchar_t ch, size_type offset = 0) const;
		size_type				find(const wchar_t* const str, size_type offset, size_type count) const;
		inline size_type		find(String const& str, size_type offset = 0) const										{ return find(str.c_str(), offset, str.size()); }
		inline size_type		find(const wchar_t* const str, size_type offset = 0) const								{ return find(str, offset, traits::length(str)); }

		size_type				find_first_of(const wchar_t* const str, size_type offset, size_type count) const;
		inline size_type		find_first_of(const wchar_t ch, size_type offset = 0) const								{ return find(ch, offset); }
		inline size_type		find_first_of(String const& str, size_type offset = 0) const							{ return find_first_of(str.c_str(), offset, str.size()); }
		inline size_type		find_first_of(const wchar_t* const str, size_type offset = 0) const						{ return find_first_of(str, offset, traits::length(str)); }

		size_type				find_last_of(const wchar_t ch, size_type pos = npos) const;
		size_type				find_last_of(const wchar_t* const str, size_type pos, size_type count) const;
		inline size_type		find_last_of(String const& str, size_type pos = npos) const								{ return find_first_of(str.c_str(), pos, str.size()); }
		inline size_type		find_last_of(const wchar_t* const str, size_type pos = npos) const						{ return find_first_of(str, pos, traits::length(str)); }

		String&					replace(size_type pos, size_type count, const wchar_t* cstr, size_type count2);
		String&					replace(size_type pos, size_type count, size_type count2, const wchar_t ch);
		inline String&			replace(size_type pos, size_type count, const String& str)								{ return replace(pos, count, str.c_str(), str.size()); }
		inline String&			replace(size_type pos, size_type count, const wchar_t* cstr)							{ return replace(pos, count, cstr, traits::length(cstr)); }
		inline String&			replace(const_iterator first, const_iterator last, const String& str)					{ return replace(first, last, str.c_str(), str.size()); }
		inline String&			replace(const_iterator first, const_iterator last, const wchar_t* cstr)					{ return replace(first, last, cstr, traits::length(cstr)); }
		inline String&			replace(const_iterator first, const_iterator last, const wchar_t* cstr, size_type count){ return replace(first - cbegin(), last - first, cstr, count); }
		inline String&			replace(const_iterator first, const_iterator last, size_type count2, const wchar_t ch)	{ return replace(first - cbegin(), last - first, count2, ch); }

		String&					assign(size_type count, const wchar_t ch);
		String&					assign(const wchar_t* cstr, size_type count);
		inline String&			assign(const wchar_t* cstr, bool const_str = true)										{ String(cstr, const_str).swap(*this); return *this; }
		inline String&			assign(std::wstring const& str)															{ String{ str }.swap(*this); return *this; }
		inline String&			assign(String const& rhs)																{ String{ rhs }.swap(*this); return *this; }
		inline String&			assign(String const& rhs, size_type pos, size_type count = npos)						{ String(rhs, pos, count).swap(*this); return *this; }

		template <typename _Iter>
		inline String&			assign(_Iter first, _Iter last)															{ assign_iter(first, last); return(*this); }

		String&					erase(size_type offset = 0, size_type count = npos);
		iterator				erase(const const_iterator where)														{ size_type off = where - cbegin(); erase(off, 1); return begin().base() + off; }
		iterator				erase(const const_iterator first, const const_iterator last)							{ size_type off = first - cbegin(); erase(first - cbegin(), last - first); return begin().base() + off; }

		String					substr(size_type pos = 0, size_type count = npos) const									{ return String(*this, pos, count); }

		String&					insert(size_type index, size_type count, wchar_t ch);
		String&					insert(size_type index, const wchar_t* s, size_type count);
		String&					insert(size_type index, const String& str, size_type off, size_type count = npos);
		inline String&			insert(size_type index, const wchar_t* s)												{ return insert(index, s, traits::length(s)); }
		inline String&			insert(size_type index, const String& str)												{ return insert(index, str, 0, str.size()); }
		inline iterator			insert(const_iterator pos, size_type count, wchar_t ch)									{ size_type off = pos - cbegin(); insert(off, count, ch); return begin().base() + off; }
		inline iterator			insert(const_iterator pos, wchar_t ch)													{ return insert(pos, 1, ch); }

		inline void				push_back(const wchar_t ch)																{ append(1, ch); }
		inline wchar_t			pop_back()																				{ if (empty()) throw std::out_of_range("pop_back() called on empty string"); check_operability(); wchar_t ch = str_[--size_]; str_[size_] = value_type(); return ch; }

		size_type				copy(wchar_t* cstr, size_type count, size_type pos = 0) const;

		std::string				to_string() const;
		std::wstring			to_wstring() const;

		void					swap(String& rhs);
		size_t					hash() const;

	public:
		inline iterator					begin()							{ check_operability(); return iterator(str_); }
		inline const_iterator			begin() const					{ return const_iterator(const_str_); }
		inline const_iterator			cbegin() const					{ return begin(); }
		inline iterator					end()							{ check_operability(); return iterator(str_ + size_); }
		inline const_iterator			end() const						{ return const_iterator(const_str_ + size_); }
		inline const_iterator			cend() const					{ return end(); }
		inline reverse_iterator			rbegin()						{ check_operability(); return reverse_iterator(end()); }
		inline const_reverse_iterator	rbegin() const					{ return const_reverse_iterator(end()); }
		inline const_reverse_iterator	crbegin() const					{ return rbegin(); }
		inline reverse_iterator			rend()							{ check_operability(); return reverse_iterator(begin()); }
		inline const_reverse_iterator	rend() const					{ return const_reverse_iterator(begin()); }
		inline const_reverse_iterator	crend() const					{ return rend(); }
		inline reference				front()							{ if (empty()) throw std::out_of_range("front() called on empty string"); check_operability(); return str_[0]; }
		inline const_reference			front() const					{ if (empty()) throw std::out_of_range("front() called on empty string"); return const_str_[0]; }
		inline reference				back()							{ if (empty()) throw std::out_of_range("back() called on empty string"); check_operability(); return str_[size_ - 1]; }
		inline const_reference			back() const					{ if (empty()) throw std::out_of_range("back() called on empty string"); return const_str_[size_ - 1]; }

	public:
		inline wchar_t		operator[](size_type off) const				{ if(off >= size_) throw std::out_of_range("string subscript out of range"); return const_str_[off]; }
		inline wchar_t&		operator[](size_type off)					{ if (off >= size_) throw std::out_of_range("string subscript out of range"); check_operability(); return str_[off]; }

	public:
		inline const String	operator+(const wchar_t ch) const			{ return String{ *this }.append(1, ch); }
		inline const String	operator+(const wchar_t* cstr) const		{ return String{ *this }.append(cstr); }
		inline const String	operator+(std::wstring const& str) const	{ return String{ *this }.append(str); }
		inline const String	operator+(String const& rhs) const			{ return String{ *this }.append(rhs); }

		inline String&		operator+=(const wchar_t ch)				{ return append(1, ch); }
		inline String&		operator+=(const wchar_t* cstr)				{ return append(cstr); }
		inline String&		operator+=(std::wstring const& str)			{ return append(str); }
		inline String&		operator+=(String const& rhs)				{ return append(rhs); }

	public:
		inline String&		operator=(const wchar_t* cstr)				{ if (const_str_ != cstr) String{ cstr }.swap(*this); return *this; }
		inline String&		operator=(std::wstring const& str)			{ String{ str }.swap(*this); return *this; }
		inline String&		operator=(String const& rhs)				{ if (this != &rhs) String{ rhs }.swap(*this); return *this; }
		inline String&		operator=(String && rhs)					{ if (this != &rhs) String{ rhs }.swap(*this); return *this; }

		inline bool			operator!=(String const& rhs)				{ return compare(rhs) != 0; }
		inline bool			operator!=(const wchar_t* cstr)				{ return compare(cstr) != 0; }

	public:
		static const String::size_type npos = static_cast<size_type>(-1);

		static inline allocator& get_allocator()
		{
			static allocator allocator_;
			return allocator_;
		}

	private:
		wchar_t* allocate(size_type count);
		void deallocate(wchar_t*& ptr, size_type count);

		void destroy();

		void discard_const_data();
		void check_operability();
		void check_offset(size_type offset) const							{ if (offset > size()) throw std::out_of_range("invalid string position"); }
		size_type clamp_suffix_size(size_type off, size_type count) const	{ return std::min(size() - off, count); }

		template <typename _Iter>
		void assign_iter(_Iter first, _Iter last)
		{
			size_type diff = static_cast<size_type>(std::distance(first, last));
			if (diff == 0)
				return;

			discard_const_data();
			if (diff > capacity_)
			{
				destroy();
				str_ = allocate(diff + 1);
				capacity_ = diff;
			}
			size_ = diff;

			for (size_type index = 0; first != last; ++first, ++index)
			{
				traits::assign(str_[index], traits::to_char_type(*first));
			}
			traits::assign(str_[size_], value_type());
		}

	private:
		union
		{
			struct
			{
				value_type* str_;
			};

			struct
			{
				const value_type* const_str_;
			};
		};
		size_type size_;
		size_type capacity_;
		const bool operable_;
	};


	//
	// operator== for String
	//

	inline bool operator==(String const& lhs, String const& rhs)	{ return lhs.compare(rhs) == 0; }
	inline bool operator==(const wchar_t* lhs, String const& rhs)	{ return rhs.compare(rhs) == 0; }
	inline bool operator==(String const& lhs, const wchar_t* rhs)	{ return lhs.compare(rhs) == 0; }

	//
	// operator+ for String
	//

	inline String operator+(const wchar_t* lhs, String const& rhs)	{ return String{ lhs } + rhs; }

	//
	// operator<> for String
	//

	inline bool operator<(String const& lhs, String const& rhs)		{ return lhs.compare(rhs) < 0; }
	inline bool operator>(String const& lhs, String const& rhs)		{ return lhs.compare(rhs) > 0; }
	inline bool operator<=(String const& lhs, String const& rhs)	{ return lhs.compare(rhs) <= 0; }
	inline bool operator>=(String const& lhs, String const& rhs)	{ return lhs.compare(rhs) >= 0; }

	//
	// operator<<>> for String
	//

	std::basic_ostream<String::traits::char_type, String::traits>& operator<<(std::basic_ostream<String::traits::char_type, String::traits>& os, const String & str);
	std::basic_istream<String::traits::char_type, String::traits>& operator>>(std::basic_istream<String::traits::char_type, String::traits>& is, String & str);

	//
	// to_string functions
	//

	String to_wstring(int val);
	String to_wstring(unsigned int val);
	String to_wstring(long val);
	String to_wstring(unsigned long val);
	String to_wstring(long long val);
	String to_wstring(unsigned long long val);
	String to_wstring(float val);
	String to_wstring(double val);
	String to_wstring(long double val);

	//
	// format_wstring
	//

	template<typename ..._Args>
	inline String format_wstring(const wchar_t* const fmt, _Args&&... args);
}

namespace easy2d
{
	//
	// details of String
	//

	namespace __string_details
	{
		template<class _Traits>
		size_t TraitsFind(
			const typename _Traits::char_type* first, size_t first_size, size_t offset,
			const typename _Traits::char_type* second, size_t count)
		{
			if (count > first_size || offset > first_size - count)
			{
				return static_cast<size_t>(-1);
			}

			if (count == 0)
			{
				return offset;
			}

			const auto matches_end = first + (first_size - count) + 1;
			for (auto iter = first + offset; ; ++iter)
			{
				iter = typename _Traits::find(iter, static_cast<size_t>(matches_end - iter), *second);
				if (!iter)
				{
					return static_cast<size_t>(-1);
				}

				if (typename _Traits::compare(iter, second, count) == 0)
				{
					return static_cast<size_t>(iter - first);
				}
			}
		}

		template<class _Traits>
		size_t TraitsFindLastOf(
			const typename _Traits::char_type* first, const size_t first_size, const size_t pos,
			const typename _Traits::char_type* second, const size_t count)
		{
			if (count != 0 && first_size != 0)
			{
				for (auto iter = first + std::min(pos, first_size - 1); ; --iter)
				{
					if (typename _Traits::find(second, count, *iter))
					{
						return static_cast<size_t>(iter - first);
					}
					if (iter == first)
					{
						break;
					}
				}
			}
			return static_cast<size_t>(-1);
		}
	}

	inline String::String()
		: str_(nullptr)
		, size_(0)
		, capacity_(0)
		, operable_(true)
	{
	}

	inline String::String(const wchar_t * cstr, bool const_str)
		: operable_(!const_str)
		, size_(0)
		, capacity_(0)
		, str_(nullptr)
	{
		if (operable_)
		{
			assign(cstr, traits::length(cstr));
		}
		else
		{
			const_str_ = cstr;
			size_ = traits::length(cstr);
			capacity_ = size_;
		}
	}

	inline String::String(const wchar_t * cstr, size_type count)
		: String()
	{
		assign(cstr, count);
	}

	inline String::String(size_type count, wchar_t ch)
		: String()
	{
		assign(count, ch);
	}

	inline String::String(const char * cstr)
		: String()
	{
		if (cstr)
		{
			int len = ::MultiByteToWideChar(CP_ACP, 0, cstr, -1, nullptr, 0);
			if (len)
			{
				wchar_t* temp = allocate(len + 1);
				temp[0] = 0;

				if (::MultiByteToWideChar(CP_ACP, 0, cstr, -1, temp, len + 1))
				{
					str_ = temp;
					capacity_ = size_ = static_cast<size_type>(len);
				}
			}
		}
	}

	inline String::String(std::string const & str)
		: String(str.c_str())
	{
	}

	inline String::String(std::wstring const & str)
		: String(str.c_str(), false)
	{
	}

	inline String::String(String const & rhs)
		: String(rhs.const_str_, !rhs.operable_)
	{
	}

	inline String::String(String const & rhs, size_type pos, size_type count)
		: String()
	{
		assign(rhs, pos, count);
	}

	inline String::String(String && rhs)
		: String()
	{
		swap(rhs);
	}

	inline String::~String()
	{
		destroy();
	}

	inline String & String::assign(size_type count, const wchar_t ch)
	{
		discard_const_data();

		if (count != 0)
		{
			if (count > capacity_)
			{
				destroy();
				str_ = allocate(count + 1);
				capacity_ = count;
			}
			size_ = count;

			traits::assign(str_, count, ch);
			traits::assign(str_[size_], value_type());

		}
		else
		{
			clear();
		}
		return (*this);
	}

	inline String & String::assign(const wchar_t * cstr, size_type count)
	{
		discard_const_data();

		if (cstr)
		{
			if (count > capacity_)
			{
				destroy();
				str_ = allocate(count + 1);
				capacity_ = count;
			}
			size_ = count;

			traits::move(str_, cstr, size_);
			traits::assign(str_[size_], value_type());
		}
		else
		{
			clear();
		}
		return (*this);
	}

	inline String & String::erase(size_type offset, size_type count)
	{
		if (count == 0)
			return (*this);

		check_offset(offset);
		check_operability();

		count = clamp_suffix_size(offset, count);

		if (count == 0)
		{
			clear();
			return (*this);
		}

		size_type new_size = size_ - count;
		iterator erase_at = begin().base() + offset;
		traits::move(erase_at.base(), erase_at.base() + count, new_size - offset + 1);
		return (*this);
	}

	inline String & String::insert(size_type index, size_type count, wchar_t ch)
	{
		if (count == 0)
			return (*this);

		if (index >= size())
			return append(count, ch);

		check_operability();

		wchar_t* const old_ptr = str_;
		const size_type old_size = size_;
		const size_type old_capacity = capacity_;
		const size_type suffix_size = old_size - index + 1;

		size_ = old_size + count;

		if (size_ > old_capacity)
		{
			capacity_ = size_;
			wchar_t* new_ptr = allocate(capacity_ + 1);

			wchar_t* const insert_at = new_ptr + index;
			traits::move(new_ptr, old_ptr, index);							// (0) - (index)
			traits::assign(insert_at, count, ch);							// (index) - (index + count)
			traits::move(insert_at + count, old_ptr + index, suffix_size);	// (index + count) - (old_size - index)

			deallocate(str_, old_capacity);
			str_ = new_ptr;
		}
		else
		{
			wchar_t* const insert_at = old_ptr + index;
			traits::move(insert_at + count, old_ptr + index, suffix_size);
			traits::assign(insert_at, count, ch);
		}

		return (*this);
	}

	inline String & String::insert(size_type index, const wchar_t * cstr, size_type count)
	{
		if (count == 0)
			return (*this);

		if (index >= size())
			return append(cstr, count);

		check_operability();

		wchar_t* const old_ptr = str_;
		const size_type old_size = size_;
		const size_type old_capacity = capacity_;
		const size_type suffix_size = old_size - index + 1;

		size_ = old_size + count;

		if (size_ > old_capacity)
		{
			capacity_ = size_;
			wchar_t* new_ptr = allocate(capacity_ + 1);

			wchar_t* const insert_at = new_ptr + index;
			traits::move(new_ptr, old_ptr, index);							// (0) - (index)
			traits::move(insert_at, cstr, count);							// (index) - (index + count)
			traits::move(insert_at + count, old_ptr + index, suffix_size);	// (index + count) - (old_size - index)

			deallocate(str_, old_capacity);
			str_ = new_ptr;
		}
		else
		{
			wchar_t* const insert_at = old_ptr + index;
			traits::move(insert_at + count, old_ptr + index, suffix_size);
			traits::move(insert_at, cstr, count);
		}

		return (*this);
	}

	inline String & String::insert(size_type index, const String & str, size_type off, size_type count)
	{
		if (count == 0 || off > str.size())
			return (*this);

		if (index >= size())
			return append(str, off, count);

		check_operability();

		count = clamp_suffix_size(off, count);

		wchar_t* const old_ptr = str_;
		const size_type old_size = size_;
		const size_type old_capacity = capacity_;
		const size_type suffix_size = old_size - index + 1;

		size_ = old_size + count;

		if (size_ > old_capacity)
		{
			capacity_ = size_;
			wchar_t* new_ptr = allocate(capacity_ + 1);

			wchar_t* const insert_at = new_ptr + index;
			traits::move(new_ptr, old_ptr, index);							// (0) - (index)
			traits::move(insert_at, str.begin().base() + off, count);				// (index) - (index + count)
			traits::move(insert_at + count, old_ptr + index, suffix_size);	// (index + count) - (old_size - index)

			deallocate(str_, old_capacity);
			str_ = new_ptr;
		}
		else
		{
			wchar_t* const insert_at = old_ptr + index;
			traits::move(insert_at + count, old_ptr + index, suffix_size);
			traits::move(insert_at, str.begin().base() + off, count);
		}

		return (*this);
	}

	inline String & String::append(size_type count, wchar_t ch)
	{
		check_operability();

		size_t new_size = size_ + count;
		size_t new_cap = new_size + 1;
		wchar_t* new_str = allocate(new_cap);

		traits::move(new_str, str_, size_);
		traits::assign(new_str + size_, count, ch);
		traits::assign(new_str[new_size], value_type());

		destroy();

		str_ = new_str;
		size_ = new_size;
		capacity_ = new_cap;
		return (*this);
	}

	inline String & String::append(const wchar_t * cstr, size_type count)
	{
		check_operability();

		size_t new_size = size_ + count;
		size_t new_cap = new_size + 1;
		wchar_t* new_str = allocate(new_cap);

		traits::move(new_str, str_, size_);
		traits::move(new_str + size_, cstr, count);
		traits::assign(new_str[new_size], value_type());

		destroy();

		str_ = new_str;
		size_ = new_size;
		capacity_ = new_cap;
		return (*this);
	}

	inline String & String::append(String const & other, size_type pos, size_type count)
	{
		check_operability();

		if (pos >= other.size())
			return (*this);

		count = other.clamp_suffix_size(pos, count);

		size_t new_size = size_ + count;
		size_t new_cap = new_size + 1;
		wchar_t* new_str = allocate(new_cap);

		traits::move(new_str, str_, size_);
		traits::move(new_str + size_, other.begin().base() + pos, count);
		traits::assign(new_str[new_size], value_type());

		destroy();

		str_ = new_str;
		size_ = new_size;
		capacity_ = new_cap;
		return (*this);
	}

	inline void String::reserve(const size_type new_cap)
	{
		if (new_cap <= capacity_)
			return;

		check_operability();

		wchar_t* new_str = allocate(new_cap);
		traits::move(new_str, str_, capacity_);

		destroy();

		str_ = new_str;
		capacity_ = new_cap;
	}

	inline size_t String::hash() const
	{
		static size_t fnv_prime = 16777619U;
		size_t fnv_offset_basis = 2166136261U;

		for (size_t index = 0; index < size_; ++index)
		{
			fnv_offset_basis ^= static_cast<size_t>(const_str_[index]);
			fnv_offset_basis *= fnv_prime;
		}
		return fnv_offset_basis;
	}

	inline int String::compare(const wchar_t * const str) const
	{
		size_type count1 = size();
		size_type count2 = traits::length(str);
		size_type rlen = std::min(count1, count2);

		int ret = traits::compare(const_str_, str, rlen);
		if (ret != 0)
			return ret;

		if (count1 < count2)
			return -1;

		if (count1 > count2)
			return 1;

		return 0;
	}

	inline String::size_type String::find(const wchar_t ch, size_type offset) const
	{
		if (offset >= size_)
			return String::npos;

		const_iterator citer = traits::find(cbegin().base() + offset, size_, ch);
		return citer ? (citer - cbegin()) : String::npos;
	}

	inline String::size_type String::find(const wchar_t * const str, size_type offset, size_type count) const
	{
		if (offset >= size_)
			return String::npos;
		return __string_details::TraitsFind<String::traits>(const_str_, size_, offset, str, count);
	}

	inline String::size_type String::find_first_of(const wchar_t * const str, size_type offset, size_type count) const
	{
		if (offset >= size_)
			return String::npos;

		const_iterator citer = std::find_first_of(cbegin().base() + offset, cend().base(), str, str + count);
		return (citer != cend()) ? (citer - cbegin()) : String::npos;
	}

	inline String::size_type String::find_last_of(const wchar_t ch, size_type pos) const
	{
		if (pos == 0 || pos > size_ || pos == npos)
			return npos;

		const_reverse_iterator criter = std::find(crbegin(), crend(), ch);
		return (criter != crend()) ? (criter.base() - cbegin()) : String::npos;
	}

	inline String::size_type String::find_last_of(const wchar_t * const str, size_type pos, size_type count) const
	{
		if (pos == 0 || pos > size_ || pos == npos)
			return npos;

		return __string_details::TraitsFindLastOf<String::traits>(const_str_, size_, pos, str, count);
	}

	inline String & String::replace(size_type pos, size_type count, const wchar_t * cstr, size_type count2)
	{
		check_offset(pos);
		check_operability();

		count = clamp_suffix_size(pos, count);
		if (count == count2)
		{
			traits::move(str_ + pos, cstr, count2);
			return (*this);
		}

		wchar_t* new_ptr = nullptr;
		wchar_t* const old_ptr = str_;
		const size_type old_size = size_;
		const size_type old_capacity = capacity_;
		const size_type suffix_size = old_size - count - pos + 1;

		if (count < count2 && (old_size + count2 - count) > capacity_)
		{
			const size_type growth = count2 - count;
			size_ = old_size + growth;
			capacity_ = size_;
			new_ptr = allocate(capacity_ + 1);

			traits::move(new_ptr, old_ptr, pos);
		}
		else
		{
			size_ = old_size - (count - count2);
		}

		wchar_t* const insert_at = (new_ptr ? new_ptr : old_ptr) + pos;
		traits::move(insert_at, cstr, count2);
		traits::move(insert_at + count2, old_ptr + count, suffix_size);

		if (new_ptr)
		{
			deallocate(str_, old_capacity);
			str_ = new_ptr;
		}

		return (*this);
	}

	inline String & String::replace(size_type pos, size_type count, size_type count2, const wchar_t ch)
	{
		check_offset(pos);
		check_operability();

		count = clamp_suffix_size(pos, count);
		if (count == count2)
		{
			traits::assign(str_ + pos, count2, ch);
			return (*this);
		}

		wchar_t* new_ptr = nullptr;
		wchar_t* const old_ptr = str_;
		const size_type old_size = size_;
		const size_type old_capacity = capacity_;
		const size_type suffix_size = old_size - count - pos + 1;

		if (count < count2 && (old_size + count2 - count) > capacity_)
		{
			const size_type growth = count2 - count;
			size_ = old_size + growth;
			capacity_ = size_;
			new_ptr = allocate(capacity_ + 1);

			traits::move(new_ptr, old_ptr, pos);
		}
		else
		{
			size_ = old_size - (count - count2);
		}

		wchar_t* const insert_at = (new_ptr ? new_ptr : old_ptr) + pos;
		traits::assign(insert_at, count2, ch);
		traits::move(insert_at + count2, old_ptr + count, suffix_size);

		if (new_ptr)
		{
			deallocate(str_, old_capacity);
			str_ = new_ptr;
		}

		return (*this);
	}

	inline String::size_type String::copy(wchar_t * cstr, size_type count, size_type pos) const
	{
		if (count == 0 || cstr == const_str_)
			return 0;

		check_offset(pos);

		count = clamp_suffix_size(pos, count);
		traits::move(cstr, cbegin().base() + pos, count);
		return count;
	}

	inline std::string String::to_string() const
	{
		std::string ret;
		if (const_str_)
		{
			int len = ::WideCharToMultiByte(CP_ACP, 0, const_str_, -1, nullptr, 0, nullptr, FALSE);
			if (len)
			{
				char* str_tmp = new char[len + 1];
				str_tmp[0] = 0;

				len = ::WideCharToMultiByte(CP_ACP, 0, const_str_, -1, str_tmp, len + 1, nullptr, FALSE);

				ret = str_tmp;
				delete[] str_tmp;
			}
		}
		return ret;
	}

	inline std::wstring String::to_wstring() const
	{
		return std::wstring(const_str_);
	}

	inline wchar_t * String::allocate(size_type count)
	{
		return get_allocator().allocate(count);
	}

	inline void String::deallocate(wchar_t*& ptr, size_type count)
	{
		get_allocator().deallocate(ptr, count);
		ptr = nullptr;
	}

	inline void String::destroy()
	{
		if (operable_ && str_)
		{
			deallocate(str_, capacity_);
		}
		else
		{
			const_str_ = nullptr;
		}
		size_ = capacity_ = 0;
	}

	inline void String::swap(String & rhs)
	{
		std::swap(const_str_, rhs.const_str_);
		std::swap(size_, rhs.size_);
		std::swap(capacity_, rhs.capacity_);

		// swap const datas
		std::swap(*const_cast<bool*>(&operable_), *const_cast<bool*>(&rhs.operable_));
	}

	inline void String::discard_const_data()
	{
		if (!operable_)
		{
			// force to enable operability
			*const_cast<bool*>(&operable_) = true;
			const_str_ = nullptr;
			capacity_ = size_ = 0;
		}
	}

	inline void String::check_operability()
	{
		if (!operable_)
		{
			// create a new string, then swap it with self
			String(const_str_, false).swap(*this);
		}
	}

	//
	// details of operator<<>>
	//

	inline std::basic_ostream<String::traits::char_type, String::traits>&
		operator<<(std::basic_ostream<String::traits::char_type, String::traits>& os, const String & str)
	{
		return os << str.c_str();
	}

	inline std::basic_istream<String::traits::char_type, String::traits>&
		operator>>(std::basic_istream<String::traits::char_type, String::traits>& is, String & str)
	{
		using Ctype = std::ctype<wchar_t>;
		using IStream = std::basic_istream<wchar_t, String::traits>;
		using SizeType = String::size_type;

		std::ios_base::iostate state = std::ios_base::goodbit;
		bool changed = false;

		if (IStream::sentry(is))
		{
			const Ctype& ctype_fac = std::use_facet<Ctype>(is.getloc());
			str.erase();
			try
			{
				SizeType size = (0 < is.width() && static_cast<SizeType>(is.width()) < str.max_size()) ? static_cast<SizeType>(is.width()) : str.max_size();
				String::traits::int_type meta = is.rdbuf()->sgetc();

				for (; 0 < size; --size, meta = is.rdbuf()->snextc())
				{
					if (String::traits::eq_int_type(String::traits::eof(), meta))
					{
						state |= std::ios_base::eofbit;
						break;
					}
					else if (ctype_fac.is(Ctype::space, String::traits::to_char_type(meta)))
					{
						break;
					}
					else
					{
						str.push_back(String::traits::to_char_type(meta));
						changed = true;
					}
				}
			}
			catch (...)
			{
				is.setstate(std::ios_base::badbit, true);
			}
		}

		is.width(0);
		if (!changed)
			state |= std::ios_base::failbit;
		is.setstate(state);
		return is;
	}

	//
	// details of to_string functions
	//

	namespace __to_string_detail
	{
		template<typename _Ty>
		inline String FloatingToString(const wchar_t *fmt, _Ty val);

		template <typename _Ty>
		inline String IntegralToString(const _Ty val);
	}

	inline String to_wstring(int val)
	{
		return (__to_string_detail::IntegralToString(val));
	}

	inline String to_wstring(unsigned int val)
	{
		return (__to_string_detail::IntegralToString(val));
	}

	inline String to_wstring(long val)
	{
		return (__to_string_detail::IntegralToString(val));
	}

	inline String to_wstring(unsigned long val)
	{
		return (__to_string_detail::IntegralToString(val));
	}

	inline String to_wstring(long long val)
	{
		return (__to_string_detail::IntegralToString(val));
	}

	inline String to_wstring(unsigned long long val)
	{
		return (__to_string_detail::IntegralToString(val));
	}

	inline String to_wstring(float val)
	{
		return (__to_string_detail::FloatingToString(L"%f", val));
	}

	inline String to_wstring(double val)
	{
		return (__to_string_detail::FloatingToString(L"%f", val));
	}

	inline String to_wstring(long double val)
	{
		return (__to_string_detail::FloatingToString(L"%Lf", val));
	}

	template<typename ..._Args>
	inline String format_wstring(const wchar_t* const fmt, _Args&&... args)
	{
		const auto len = static_cast<String::size_type>(::_scwprintf(fmt, std::forward<_Args&&>(args)...));
		if (len)
		{
			String str(len, L'\0');
			::swprintf_s(&str[0], len + 1, fmt, std::forward<_Args&&>(args)...);
			return str;
		}
		return String{};
	}

	namespace __to_string_detail
	{
		template<typename _Ty>
		inline String FloatingToString(const wchar_t *fmt, _Ty val)
		{
			static_assert(std::is_floating_point<_Ty>::value, "_Ty must be floating point");

			return format_wstring(fmt, val);
		}

		template <typename _Ty>
		inline String IntegralToString(const _Ty val)
		{
			static_assert(std::is_integral<_Ty>::value, "_Ty must be integral");

			using _UTy = std::make_unsigned_t<_Ty>;
			using _Elem = String::traits::char_type;

			_Elem buffer[21];
			_Elem* const buffer_end = std::end(buffer);
			_Elem* next = buffer_end;
			auto uval = static_cast<_UTy>(val);

			if (val < 0)
				uval = 0 - uval;

			do
			{
				*--next = static_cast<_Elem>('0' + uval % 10);
				uval /= 10;
			} while (uval != 0);

			if (val < 0)
				*--next = static_cast<_Elem>('-');

			return String(next, buffer_end);
		}
	}
}

namespace std
{
	template<>
	struct hash<::easy2d::String>
	{
		inline size_t operator()(const easy2d::String& key) const
		{
			return key.hash();
		}
	};

	template<>
	inline void swap<::easy2d::String>(::easy2d::String& lhs, ::easy2d::String& rhs)
	{
		lhs.swap(rhs);
	}
}
