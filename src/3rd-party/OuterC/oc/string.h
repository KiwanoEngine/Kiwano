// Copyright (c) 2019-2020 OuterC - Nomango

#pragma once
#include <string>
#include <algorithm>
#include <codecvt>
#include <ostream>
#include <istream>
#include <cstring>
#include <cstdio>

namespace oc
{

//
// basic_string<>
//   Lightweight std::basic_string<>-like class
//   When using basic_string<> with a c-style string (char* or wchar_t*), constructor and operator=() just hold
//   a pointer to the character array but don't copy its content, considering performance issues.
//   Use assign() and basic_string<>::cstr() to work fine with c-style strings.
//
template <typename _CharTy>
class basic_string
{
private:
	// Iterator
	template <typename _Ty>
	struct iterator_impl
	{
		using iterator_category	= typename std::iterator_traits<_Ty*>::iterator_category;
		using value_type		= typename std::iterator_traits<_Ty*>::value_type;
		using difference_type	= typename std::iterator_traits<_Ty*>::difference_type;
		using pointer			= typename std::iterator_traits<_Ty*>::pointer;
		using reference			= typename std::iterator_traits<_Ty*>::reference;

		// disable warning 4996
		using _Unchecked_type = _Ty;

		inline iterator_impl(pointer core = nullptr) : base_(core) {}

		inline reference operator*() const									{ return *base_; }
		inline pointer core() const											{ return base_; }

		inline iterator_impl& operator++()									{ ++base_; return (*this); }
		inline iterator_impl operator++(int)								{ iterator_impl old = (*this); ++(*this); return old; }

		inline iterator_impl& operator--()									{ --base_; return (*this); }
		inline iterator_impl operator--(int)								{ iterator_impl old = (*this); --(*this); return old; }

		inline const iterator_impl operator+(difference_type off) const		{ return iterator_impl(base_ + off); }
		inline const iterator_impl operator-(difference_type off) const		{ return iterator_impl(base_ - off); }

		inline iterator_impl& operator+=(difference_type off)				{ base_ += off; return (*this); }
		inline iterator_impl& operator-=(difference_type off)				{ base_ -= off; return (*this); }

		inline difference_type operator-(iterator_impl const& other) const	{ return base_ - other.base_; }

		inline bool operator==(iterator_impl const& other) const			{ return base_ == other.base_; }
		inline bool operator!=(iterator_impl const& other) const			{ return !(*this == other); }

		inline bool operator<(iterator_impl const& other) const				{ return base_ < other.base_; }
		inline bool operator<=(iterator_impl const& other) const			{ return base_ <= other.base_; }
		inline bool operator>(iterator_impl const& other) const				{ return base_ > other.base_; }
		inline bool operator>=(iterator_impl const& other) const			{ return base_ >= other.base_; }

		inline reference operator[](difference_type off)					{ return *(base_ + off); }
		inline const reference operator[](difference_type off) const		{ return *(base_ + off); }

		inline operator bool() const										{ return base_ != nullptr; }

	private:
		pointer base_{ nullptr };
	};

public:
	using value_type				= _CharTy;
	using char_type					= value_type;
	using size_type					= size_t;
	using reference					= value_type &;
	using const_reference			= const value_type &;
	using iterator					= iterator_impl<value_type>;
	using const_iterator			= iterator_impl<const value_type>;
	using reverse_iterator			= std::reverse_iterator<iterator>;
	using const_reverse_iterator	= std::reverse_iterator<const_iterator>;
	using traits_type				= std::char_traits<value_type>;
	using allocator_type			= std::allocator<value_type>;

	basic_string();
	basic_string(const char_type* cstr, bool const_str = true);
	basic_string(const char_type* cstr, size_type count);
	basic_string(size_type count, char_type ch);
	basic_string(std::basic_string<char_type> const& str);
	basic_string(basic_string const& rhs);
	basic_string(basic_string const& rhs, size_type pos, size_type count = npos);
	basic_string(basic_string && rhs) noexcept;
	~basic_string();

	template <typename _Iter>
	basic_string(_Iter first, _Iter last) : basic_string()																{ assign_iter(first, last); }

	inline const char_type*	c_str() const																				{ return empty() ? empty_cstr : const_str_; }
	inline const char_type*	data() const																				{ return empty() ? empty_cstr : const_str_; }
	inline char_type		at(size_type i) const																		{ return (*this)[i]; }
	inline size_type		size() const																				{ return size_; }
	inline size_type		length() const																				{ return size(); }
	inline size_type		capacity() const																			{ return capacity_; }
	inline size_type		max_size() const																			{ return (static_cast<size_type>(-1) / sizeof(value_type)); }
	inline bool				empty() const																				{ return size_ == 0; }
	inline void				clear()																						{ discard_const_data(); if (str_) { str_[0] = value_type(); } size_ = 0; }

	void					reserve(const size_type new_cap = 0);
	inline void				resize(const size_type new_size, const char_type ch = value_type())							{ check_operability(); if (new_size < size_) str_[size_ = new_size] = value_type(); else append(new_size - size_, ch); }

	int						compare(const char_type* const str) const;
	inline int				compare(basic_string const& str) const														{ return compare(str.c_str()); }

	basic_string&			append(size_type count, char_type ch);
	basic_string&			append(const char_type* cstr, size_type count);
	basic_string&			append(basic_string const& other, size_type pos, size_type count = npos);
	inline basic_string&	append(const char_type* cstr)																{ return append(cstr, traits_type::length(cstr)); }
	inline basic_string&	append(basic_string const& other)															{ return append(other.const_str_, 0, npos); }
	inline basic_string&	append(std::basic_string<char_type> const& other)											{ return append(other.c_str()); }

	size_type				find(const char_type ch, size_type offset = 0) const;
	size_type				find(const char_type* const str, size_type offset, size_type count) const;
	inline size_type		find(basic_string const& str, size_type offset = 0) const									{ return find(str.c_str(), offset, str.size()); }
	inline size_type		find(const char_type* const str, size_type offset = 0) const								{ return find(str, offset, traits_type::length(str)); }

	size_type				find_first_of(const char_type* const str, size_type offset, size_type count) const;
	inline size_type		find_first_of(const char_type ch, size_type offset = 0) const								{ return find(ch, offset); }
	inline size_type		find_first_of(basic_string const& str, size_type offset = 0) const							{ return find_first_of(str.c_str(), offset, str.size()); }
	inline size_type		find_first_of(const char_type* const str, size_type offset = 0) const						{ return find_first_of(str, offset, traits_type::length(str)); }

	size_type				find_last_of(const char_type ch, size_type pos = npos) const;
	size_type				find_last_of(const char_type* const str, size_type pos, size_type count) const;
	inline size_type		find_last_of(basic_string const& str, size_type pos = npos) const							{ return find_first_of(str.c_str(), pos, str.size()); }
	inline size_type		find_last_of(const char_type* const str, size_type pos = npos) const						{ return find_first_of(str, pos, traits_type::length(str)); }

	basic_string&			replace(size_type pos, size_type count, const char_type* cstr, size_type count2);
	basic_string&			replace(size_type pos, size_type count, size_type count2, const char_type ch);
	inline basic_string&	replace(size_type pos, size_type count, const basic_string& str)							{ return replace(pos, count, str.c_str(), str.size()); }
	inline basic_string&	replace(size_type pos, size_type count, const char_type* cstr)								{ return replace(pos, count, cstr, traits_type::length(cstr)); }
	inline basic_string&	replace(const_iterator first, const_iterator last, const basic_string& str)					{ return replace(first, last, str.c_str(), str.size()); }
	inline basic_string&	replace(const_iterator first, const_iterator last, const char_type* cstr)					{ return replace(first, last, cstr, traits_type::length(cstr)); }
	inline basic_string&	replace(const_iterator first, const_iterator last, const char_type* cstr, size_type count)	{ return replace(first - cbegin(), last - first, cstr, count); }
	inline basic_string&	replace(const_iterator first, const_iterator last, size_type count2, const char_type ch)	{ return replace(first - cbegin(), last - first, count2, ch); }

	basic_string&			assign(size_type count, const char_type ch);
	basic_string&			assign(const char_type* cstr, size_type count);
	inline basic_string&	assign(const char_type* cstr)																{ basic_string(cstr, false).swap(*this); return *this; }
	inline basic_string&	assign(basic_string const& rhs)																{ basic_string{ rhs }.swap(*this); return *this; }
	inline basic_string&	assign(std::basic_string<char_type> const& rhs)												{ basic_string{ rhs }.swap(*this); return *this; }
	basic_string&			assign(basic_string const& rhs, size_type pos, size_type count = npos);

	template <typename _Iter>
	inline basic_string&	assign(_Iter first, _Iter last)																{ assign_iter(first, last); return(*this); }

	basic_string&			erase(size_type offset = 0, size_type count = npos);
	iterator				erase(const const_iterator where)															{ size_type off = where - cbegin(); erase(off, 1); return begin().core() + off; }
	iterator				erase(const const_iterator first, const const_iterator last)								{ size_type off = first - cbegin(); erase(first - cbegin(), last - first); return begin().core() + off; }

	basic_string			substr(size_type pos = 0, size_type count = npos) const										{ return basic_string(*this, pos, count); }

	basic_string&			insert(size_type index, size_type count, char_type ch);
	basic_string&			insert(size_type index, const char_type* s, size_type count);
	basic_string&			insert(size_type index, const basic_string& str, size_type off, size_type count = npos);
	inline basic_string&	insert(size_type index, const char_type* s)													{ return insert(index, s, traits_type::length(s)); }
	inline basic_string&	insert(size_type index, const basic_string& str)											{ return insert(index, str, 0, str.size()); }
	inline iterator			insert(const_iterator pos, size_type count, char_type ch)									{ size_type off = pos - cbegin(); insert(off, count, ch); return begin().core() + off; }
	inline iterator			insert(const_iterator pos, char_type ch)													{ return insert(pos, 1, ch); }

	inline void				push_back(const char_type ch)																{ append(1, ch); }
	inline char_type		pop_back()																					{ if (empty()) throw std::out_of_range("pop_back() called on empty string"); check_operability(); char_type ch = str_[--size_]; str_[size_] = value_type(); return ch; }

	size_type				copy(char_type* cstr, size_type count, size_type pos = 0) const;

	void					swap(basic_string& rhs) noexcept;
	size_type				hash() const;

public:
	static basic_string parse(int val);
	static basic_string parse(unsigned int val);
	static basic_string parse(long val);
	static basic_string parse(unsigned long val);
	static basic_string parse(long long val);
	static basic_string parse(unsigned long long val);
	static basic_string parse(float val);
	static basic_string parse(double val);
	static basic_string parse(long double val);

	template <typename ..._Args>
	static basic_string format(const char_type* fmt, _Args&&... args);

	static inline basic_string cstr(const char_type* cstr)			{ return basic_string(cstr, false); }

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
	inline char_type			operator[](size_type off) const						{ if (off >= size_) throw std::out_of_range("string subscript out of range"); return const_str_[off]; }
	inline char_type&			operator[](size_type off)							{ if (off >= size_) throw std::out_of_range("string subscript out of range"); check_operability(); return str_[off]; }

public:
	inline const basic_string	operator+(const char_type ch) const					{ return basic_string{ *this }.append(1, ch); }
	inline const basic_string	operator+(const char_type* cstr) const				{ return basic_string{ *this }.append(cstr); }
	inline const basic_string	operator+(basic_string const& rhs) const			{ return basic_string{ *this }.append(rhs); }

	inline basic_string&		operator+=(const char_type ch)						{ return append(1, ch); }
	inline basic_string&		operator+=(const char_type* cstr)					{ return append(cstr); }
	inline basic_string&		operator+=(basic_string const& rhs)					{ return append(rhs); }

public:
	inline basic_string&		operator=(const char_type* cstr)					{ if (const_str_ != cstr) basic_string{ cstr }.swap(*this); return *this; }
	inline basic_string&		operator=(std::basic_string<char_type> const& rhs)	{ basic_string{ rhs }.swap(*this); return *this; }
	inline basic_string&		operator=(basic_string const& rhs)					{ if (this != &rhs) basic_string{ rhs }.swap(*this); return *this; }
	inline basic_string&		operator=(basic_string && rhs) noexcept				{ if (this != &rhs) basic_string{ rhs }.swap(*this); return *this; }

public:
	static const size_type npos;
	static const char_type empty_cstr[1];

	static inline allocator_type& get_allocator()
	{
		static allocator_type allocator_;
		return allocator_;
	}

private:
	char_type* allocate(size_type count);
	void deallocate(char_type*& ptr, size_type count);

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
			traits_type::assign(str_[index], traits_type::to_char_type(*first));
		}
		traits_type::assign(str_[size_], value_type());
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

// static members
template <typename _CharTy>
const typename basic_string<_CharTy>::size_type basic_string<_CharTy>::npos = static_cast<typename basic_string<_CharTy>::size_type>(-1);

template <typename _CharTy>
const typename basic_string<_CharTy>::char_type basic_string<_CharTy>::empty_cstr[1] = { 0 };


//
// operator== for basic_string
//

template <typename _CharTy>
inline bool operator==(basic_string<_CharTy> const& lhs, basic_string<_CharTy> const& rhs)						{ return lhs.compare(rhs) == 0; }
	
template <typename _CharTy>
inline bool operator==(const typename basic_string<_CharTy>::char_type* lhs, basic_string<_CharTy> const& rhs)	{ return rhs.compare(lhs) == 0; }
	
template <typename _CharTy>
inline bool operator==(basic_string<_CharTy> const& lhs, const typename basic_string<_CharTy>::char_type* rhs)	{ return lhs.compare(rhs) == 0; }

//
// operator!= for basic_string
//

template <typename _CharTy>
inline bool operator!=(basic_string<_CharTy> const& lhs, basic_string<_CharTy> const& rhs)						{ return lhs.compare(rhs) != 0; }
	
template <typename _CharTy>
inline bool operator!=(const typename basic_string<_CharTy>::char_type* lhs, basic_string<_CharTy> const& rhs)	{ return rhs.compare(lhs) != 0; }
	
template <typename _CharTy>
inline bool operator!=(basic_string<_CharTy> const& lhs, const typename basic_string<_CharTy>::char_type* rhs)	{ return lhs.compare(rhs) != 0; }

//
// operator+ for basic_string
//

template <typename _CharTy>
inline basic_string<_CharTy> operator+(const typename basic_string<_CharTy>::char_type* lhs, basic_string<_CharTy> const& rhs)	{ return basic_string<_CharTy>{ lhs } + rhs; }

//
// operator<> for basic_string
//

template <typename _CharTy>
inline bool operator<(basic_string<_CharTy> const& lhs, basic_string<_CharTy> const& rhs)		{ return lhs.compare(rhs) < 0; }
	
template <typename _CharTy>
inline bool operator>(basic_string<_CharTy> const& lhs, basic_string<_CharTy> const& rhs)		{ return lhs.compare(rhs) > 0; }
	
template <typename _CharTy>
inline bool operator<=(basic_string<_CharTy> const& lhs, basic_string<_CharTy> const& rhs)		{ return lhs.compare(rhs) <= 0; }
	
template <typename _CharTy>
inline bool operator>=(basic_string<_CharTy> const& lhs, basic_string<_CharTy> const& rhs)		{ return lhs.compare(rhs) >= 0; }

//
// operator<<>> for basic_string
//

template <typename _CharTy>
std::basic_ostream<typename basic_string<_CharTy>::char_type>& operator<<(std::basic_ostream<typename basic_string<_CharTy>::char_type>& os, const basic_string<_CharTy>& str);
	
template <typename _CharTy>
std::basic_istream<typename basic_string<_CharTy>::char_type>& operator>>(std::basic_istream<typename basic_string<_CharTy>::char_type>& is, basic_string<_CharTy>& str);


//
// to_string functions
//

template <typename _CharTy>
basic_string<_CharTy> to_basic_string(int val);
	
template <typename _CharTy>
basic_string<_CharTy> to_basic_string(unsigned int val);
	
template <typename _CharTy>
basic_string<_CharTy> to_basic_string(long val);
	
template <typename _CharTy>
basic_string<_CharTy> to_basic_string(unsigned long val);
	
template <typename _CharTy>
basic_string<_CharTy> to_basic_string(long long val);
	
template <typename _CharTy>
basic_string<_CharTy> to_basic_string(unsigned long long val);
	
template <typename _CharTy>
basic_string<_CharTy> to_basic_string(float val);
	
template <typename _CharTy>
basic_string<_CharTy> to_basic_string(double val);
	
template <typename _CharTy>
basic_string<_CharTy> to_basic_string(long double val);

//
// format_string
//

template <typename ..._Args>
basic_string<char> format_string(const char* const fmt, _Args&& ... args);

template <typename ..._Args>
basic_string<wchar_t> format_string(const wchar_t* const fmt, _Args&& ... args);


//
// string && wstring
//
using string = ::oc::basic_string<char>;
using wstring = ::oc::basic_string<wchar_t>;


inline string to_string(int val)					{ return to_basic_string<char>(val); }
inline string to_string(unsigned int val)			{ return to_basic_string<char>(val); }
inline string to_string(long val)					{ return to_basic_string<char>(val); }
inline string to_string(unsigned long val)			{ return to_basic_string<char>(val); }
inline string to_string(long long val)				{ return to_basic_string<char>(val); }
inline string to_string(unsigned long long val)		{ return to_basic_string<char>(val); }
inline string to_string(float val)					{ return to_basic_string<char>(val); }
inline string to_string(double val)					{ return to_basic_string<char>(val); }
inline string to_string(long double val)			{ return to_basic_string<char>(val); }

inline wstring to_wstring(int val)					{ return to_basic_string<wchar_t>(val); }
inline wstring to_wstring(unsigned int val)			{ return to_basic_string<wchar_t>(val); }
inline wstring to_wstring(long val)					{ return to_basic_string<wchar_t>(val); }
inline wstring to_wstring(unsigned long val)		{ return to_basic_string<wchar_t>(val); }
inline wstring to_wstring(long long val)			{ return to_basic_string<wchar_t>(val); }
inline wstring to_wstring(unsigned long long val)	{ return to_basic_string<wchar_t>(val); }
inline wstring to_wstring(float val)				{ return to_basic_string<wchar_t>(val); }
inline wstring to_wstring(double val)				{ return to_basic_string<wchar_t>(val); }
inline wstring to_wstring(long double val)			{ return to_basic_string<wchar_t>(val); }


}  // namespace oc

namespace oc
{

//
// details of basic_string
//
namespace __string_details
{
	template <typename _Traits, typename _SizeTy>
	_SizeTy TraitsFind(
		const typename _Traits::char_type* first, _SizeTy first_size, _SizeTy offset,
		const typename _Traits::char_type* second, _SizeTy count)
	{
		if (count > first_size || offset > first_size - count)
		{
			return static_cast<_SizeTy>(-1);
		}

		if (count == 0)
		{
			return offset;
		}

		const auto matches_end = first + (first_size - count) + 1;
		for (auto iter = first + offset; ; ++iter)
		{
			iter = typename _Traits::find(iter, static_cast<_SizeTy>(matches_end - iter), *second);
			if (!iter)
			{
				return static_cast<_SizeTy>(-1);
			}

			if (typename _Traits::compare(iter, second, count) == 0)
			{
				return static_cast<_SizeTy>(iter - first);
			}
		}
	}

	template <typename _Traits, typename _SizeTy>
	_SizeTy TraitsFindLastOf(
		const typename _Traits::char_type* first, const _SizeTy first_size, const _SizeTy pos,
		const typename _Traits::char_type* second, const _SizeTy count)
	{
		if (count != 0 && first_size != 0)
		{
			for (auto iter = first + std::min(pos, first_size - 1); ; --iter)
			{
				if (typename _Traits::find(second, count, *iter))
				{
					return static_cast<_SizeTy>(iter - first);
				}
				if (iter == first)
				{
					break;
				}
			}
		}
		return static_cast<_SizeTy>(-1);
	}
}


template <typename _CharTy>
inline basic_string<_CharTy>::basic_string()
	: str_(nullptr)
	, size_(0)
	, capacity_(0)
	, operable_(true)
{
}

template <typename _CharTy>
inline basic_string<_CharTy>::basic_string(const char_type* cstr, bool const_str)
	: operable_(!const_str)
	, size_(0)
	, capacity_(0)
	, str_(nullptr)
{
	if (cstr == nullptr)
		return;

	if (operable_)
	{
		assign(cstr, traits_type::length(cstr));
	}
	else
	{
		const_str_ = cstr;
		size_ = traits_type::length(cstr);
		capacity_ = size_;
	}
}

template <typename _CharTy>
inline basic_string<_CharTy>::basic_string(const char_type* cstr, size_type count)
	: basic_string()
{
	assign(cstr, count);
}

template <typename _CharTy>
inline basic_string<_CharTy>::basic_string(size_type count, char_type ch)
	: basic_string()
{
	assign(count, ch);
}

template <typename _CharTy>
inline basic_string<_CharTy>::basic_string(basic_string const& rhs)
	: basic_string(rhs.const_str_, !rhs.operable_)
{
}

template <typename _CharTy>
inline basic_string<_CharTy>::basic_string(basic_string const& rhs, size_type pos, size_type count)
	: basic_string()
{
	assign(rhs, pos, count);
}

template <typename _CharTy>
inline basic_string<_CharTy>::basic_string(std::basic_string<char_type> const& str)
	: basic_string(str.c_str(), false)
{
}

template <typename _CharTy>
inline basic_string<_CharTy>::basic_string(basic_string&& rhs) noexcept
	: str_(rhs.str_)
	, size_(rhs.size_)
	, capacity_(rhs.capacity_)
	, operable_(rhs.operable_)
{
	rhs.str_ = nullptr;
	rhs.size_ = rhs.capacity_ = 0;
}

template <typename _CharTy>
inline basic_string<_CharTy>::~basic_string()
{
	destroy();
}

template <typename _CharTy>
inline basic_string<_CharTy>& basic_string<_CharTy>::assign(size_type count, const char_type ch)
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

		traits_type::assign(str_, size_, ch);
		traits_type::assign(*(str_ + size_), value_type());
	}
	else
	{
		clear();
	}
	return (*this);
}

template <typename _CharTy>
inline basic_string<_CharTy>& basic_string<_CharTy>::assign(const char_type* cstr, size_type count)
{
	discard_const_data();

	if (cstr && count)
	{
		if (count > capacity_)
		{
			destroy();
			str_ = allocate(count + 1);
			capacity_ = count;
		}
		size_ = count;

		traits_type::move(str_, cstr, size_);
		traits_type::assign(str_[size_], value_type());
	}
	else
	{
		clear();
	}
	return (*this);
}

template <typename _CharTy>
inline basic_string<_CharTy>& basic_string<_CharTy>::assign(basic_string const& rhs, size_type pos, size_type count)
{
	if (count == 0 || pos > rhs.size())
	{
		clear();
		return (*this);
	}

	discard_const_data();

	count = rhs.clamp_suffix_size(pos, count);

	if (count > capacity_)
	{
		destroy();
		str_ = allocate(count + 1);
		capacity_ = count;
	}
	size_ = count;

	traits_type::move(str_, rhs.begin().core() + pos, size_);
	traits_type::assign(str_[size_], value_type());
	return (*this);
}

template <typename _CharTy>
inline basic_string<_CharTy>& basic_string<_CharTy>::erase(size_type offset, size_type count)
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
	iterator erase_at = begin().core() + offset;
	traits_type::move(erase_at.core(), erase_at.core() + count, new_size - offset + 1);
	return (*this);
}

template <typename _CharTy>
inline basic_string<_CharTy>& basic_string<_CharTy>::insert(size_type index, size_type count, char_type ch)
{
	if (count == 0)
		return (*this);

	if (index >= size())
		return append(count, ch);

	check_operability();

	char_type* const old_ptr = str_;
	const size_type old_size = size_;
	const size_type old_capacity = capacity_;
	const size_type suffix_size = old_size - index + 1;

	size_ = old_size + count;

	if (size_ > old_capacity)
	{
		capacity_ = size_;
		char_type* new_ptr = allocate(capacity_ + 1);

		char_type* const insert_at = new_ptr + index;
		traits_type::move(new_ptr, old_ptr, index);							// (0) - (index)
		traits_type::assign(insert_at, count, ch);							// (index) - (index + count)
		traits_type::move(insert_at + count, old_ptr + index, suffix_size);	// (index + count) - (old_size - index)

		deallocate(str_, old_capacity + 1);
		str_ = new_ptr;
	}
	else
	{
		char_type* const insert_at = old_ptr + index;
		traits_type::move(insert_at + count, old_ptr + index, suffix_size);
		traits_type::assign(insert_at, count, ch);
	}

	return (*this);
}

template <typename _CharTy>
inline basic_string<_CharTy>& basic_string<_CharTy>::insert(size_type index, const char_type* cstr, size_type count)
{
	if (count == 0)
		return (*this);

	if (index >= size())
		return append(cstr, count);

	check_operability();

	char_type* const old_ptr = str_;
	const size_type old_size = size_;
	const size_type old_capacity = capacity_;
	const size_type suffix_size = old_size - index + 1;

	size_ = old_size + count;

	if (size_ > old_capacity)
	{
		capacity_ = size_;
		char_type* new_ptr = allocate(capacity_ + 1);

		char_type* const insert_at = new_ptr + index;
		traits_type::move(new_ptr, old_ptr, index);							// (0) - (index)
		traits_type::move(insert_at, cstr, count);							// (index) - (index + count)
		traits_type::move(insert_at + count, old_ptr + index, suffix_size);	// (index + count) - (old_size - index)

		deallocate(str_, old_capacity + 1);
		str_ = new_ptr;
	}
	else
	{
		char_type* const insert_at = old_ptr + index;
		traits_type::move(insert_at + count, old_ptr + index, suffix_size);
		traits_type::move(insert_at, cstr, count);
	}

	return (*this);
}

template <typename _CharTy>
inline basic_string<_CharTy>& basic_string<_CharTy>::insert(size_type index, const basic_string& str, size_type off, size_type count)
{
	if (count == 0 || off > str.size())
		return (*this);

	if (index >= size())
		return append(str, off, count);

	check_operability();

	count = clamp_suffix_size(off, count);

	char_type* const old_ptr = str_;
	const size_type old_size = size_;
	const size_type old_capacity = capacity_;
	const size_type suffix_size = old_size - index + 1;

	size_ = old_size + count;

	if (size_ > old_capacity)
	{
		capacity_ = size_;
		char_type* new_ptr = allocate(capacity_ + 1);

		char_type* const insert_at = new_ptr + index;
		traits_type::move(new_ptr, old_ptr, index);							// (0) - (index)
		traits_type::move(insert_at, str.begin().core() + off, count);		// (index) - (index + count)
		traits_type::move(insert_at + count, old_ptr + index, suffix_size);	// (index + count) - (old_size - index)

		deallocate(str_, old_capacity + 1);
		str_ = new_ptr;
	}
	else
	{
		char_type* const insert_at = old_ptr + index;
		traits_type::move(insert_at + count, old_ptr + index, suffix_size);
		traits_type::move(insert_at, str.begin().core() + off, count);
	}

	return (*this);
}

template <typename _CharTy>
inline basic_string<_CharTy>& basic_string<_CharTy>::append(size_type count, char_type ch)
{
	check_operability();

	size_type new_size = size_ + count;
	size_type new_cap = new_size + 1;
	char_type* new_str = allocate(new_cap);

	traits_type::move(new_str, str_, size_);
	traits_type::assign(new_str + size_, count, ch);
	traits_type::assign(new_str[new_size], value_type());

	destroy();

	str_ = new_str;
	size_ = new_size;
	capacity_ = new_cap;
	return (*this);
}

template <typename _CharTy>
inline basic_string<_CharTy>& basic_string<_CharTy>::append(const char_type* cstr, size_type count)
{
	check_operability();

	size_type new_size = size_ + count;
	size_type new_cap = new_size + 1;
	char_type* new_str = allocate(new_cap);

	traits_type::move(new_str, str_, size_);
	traits_type::move(new_str + size_, cstr, count);
	traits_type::assign(new_str[new_size], value_type());

	destroy();

	str_ = new_str;
	size_ = new_size;
	capacity_ = new_cap;
	return (*this);
}

template <typename _CharTy>
inline basic_string<_CharTy>& basic_string<_CharTy>::append(basic_string const& other, size_type pos, size_type count)
{
	check_operability();

	if (pos >= other.size())
		return (*this);

	count = other.clamp_suffix_size(pos, count);

	size_type new_size = size_ + count;
	size_type new_cap = new_size + 1;
	char_type* new_str = allocate(new_cap);

	traits_type::move(new_str, str_, size_);
	traits_type::move(new_str + size_, other.begin().core() + pos, count);
	traits_type::assign(new_str[new_size], value_type());

	destroy();

	str_ = new_str;
	size_ = new_size;
	capacity_ = new_cap;
	return (*this);
}

template <typename _CharTy>
inline void basic_string<_CharTy>::reserve(const size_type new_cap)
{
	if (new_cap <= capacity_)
		return;

	check_operability();

	char_type* new_str = allocate(new_cap);
	traits_type::move(new_str, str_, capacity_);

	destroy();

	str_ = new_str;
	capacity_ = new_cap;
}

template <typename _CharTy>
inline typename basic_string<_CharTy>::size_type basic_string<_CharTy>::hash() const
{
	static size_type fnv_prime = 16777619U;
	size_type fnv_offset_basis = 2166136261U;

	for (size_type index = 0; index < size_; ++index)
	{
		fnv_offset_basis ^= static_cast<size_type>(const_str_[index]);
		fnv_offset_basis *= fnv_prime;
	}
	return fnv_offset_basis;
}

template <typename _CharTy>
inline int basic_string<_CharTy>::compare(const char_type* const str) const
{
	size_type count1 = size();
	size_type count2 = traits_type::length(str);
	size_type rlen = std::min(count1, count2);

	int ret = traits_type::compare(const_str_, str, rlen);
	if (ret != 0)
		return ret;

	if (count1 < count2)
		return -1;

	if (count1 > count2)
		return 1;

	return 0;
}

template <typename _CharTy>
inline typename basic_string<_CharTy>::size_type basic_string<_CharTy>::find(const char_type ch, size_type offset) const
{
	if (offset >= size_)
		return basic_string<_CharTy>::npos;

	const_iterator citer = traits_type::find(cbegin().core() + offset, size_, ch);
	return citer ? (citer - cbegin()) : basic_string<_CharTy>::npos;
}

template <typename _CharTy>
inline typename basic_string<_CharTy>::size_type basic_string<_CharTy>::find(const char_type* const str, size_type offset, size_type count) const
{
	if (offset >= size_)
		return basic_string<_CharTy>::npos;
	return __string_details::TraitsFind<traits_type, size_type>(const_str_, size_, offset, str, count);
}

template <typename _CharTy>
inline typename basic_string<_CharTy>::size_type basic_string<_CharTy>::find_first_of(const char_type* const str, size_type offset, size_type count) const
{
	if (offset >= size_)
		return basic_string<_CharTy>::npos;

	const_iterator citer = std::find_first_of(cbegin().core() + offset, cend().core(), str, str + count);
	return (citer != cend()) ? (citer - cbegin()) : basic_string<_CharTy>::npos;
}

template <typename _CharTy>
inline typename basic_string<_CharTy>::size_type basic_string<_CharTy>::find_last_of(const char_type ch, size_type pos) const
{
	if (pos == 0 || pos > size_ || pos == npos)
		return npos;

	const_reverse_iterator criter = std::find(crbegin(), crend(), ch);
	return (criter != crend()) ? (criter.core() - cbegin()) : basic_string<_CharTy>::npos;
}

template <typename _CharTy>
inline typename basic_string<_CharTy>::size_type basic_string<_CharTy>::find_last_of(const char_type* const str, size_type pos, size_type count) const
{
	if (pos == 0 || pos > size_ || pos == npos)
		return npos;

	return __string_details::TraitsFindLastOf<traits_type, size_type>(const_str_, size_, pos, str, count);
}

template <typename _CharTy>
inline basic_string<_CharTy>& basic_string<_CharTy>::replace(size_type pos, size_type count, const char_type* cstr, size_type count2)
{
	check_offset(pos);
	check_operability();

	count = clamp_suffix_size(pos, count);
	if (count == count2)
	{
		traits_type::move(str_ + pos, cstr, count2);
		return (*this);
	}

	char_type* new_ptr = nullptr;
	char_type* const old_ptr = str_;
	const size_type old_size = size_;
	const size_type old_capacity = capacity_;
	const size_type suffix_size = old_size - count - pos + 1;

	if (count < count2 && (old_size + count2 - count) > capacity_)
	{
		const size_type growth = count2 - count;
		size_ = old_size + growth;
		capacity_ = size_;
		new_ptr = allocate(capacity_ + 1);

		traits_type::move(new_ptr, old_ptr, pos);
	}
	else
	{
		size_ = old_size - (count - count2);
	}

	char_type* const insert_at = (new_ptr ? new_ptr : old_ptr) + pos;
	traits_type::move(insert_at, cstr, count2);
	traits_type::move(insert_at + count2, old_ptr + count, suffix_size);

	if (new_ptr)
	{
		deallocate(str_, old_capacity + 1);
		str_ = new_ptr;
	}

	return (*this);
}

template <typename _CharTy>
inline basic_string<_CharTy>& basic_string<_CharTy>::replace(size_type pos, size_type count, size_type count2, const char_type ch)
{
	check_offset(pos);
	check_operability();

	count = clamp_suffix_size(pos, count);
	if (count == count2)
	{
		traits_type::assign(str_ + pos, count2, ch);
		return (*this);
	}

	char_type* new_ptr = nullptr;
	char_type* const old_ptr = str_;
	const size_type old_size = size_;
	const size_type old_capacity = capacity_;
	const size_type suffix_size = old_size - count - pos + 1;

	if (count < count2 && (old_size + count2 - count) > capacity_)
	{
		const size_type growth = count2 - count;
		size_ = old_size + growth;
		capacity_ = size_;
		new_ptr = allocate(capacity_ + 1);

		traits_type::move(new_ptr, old_ptr, pos);
	}
	else
	{
		size_ = old_size - (count - count2);
	}

	char_type* const insert_at = (new_ptr ? new_ptr : old_ptr) + pos;
	traits_type::assign(insert_at, count2, ch);
	traits_type::move(insert_at + count2, old_ptr + count, suffix_size);

	if (new_ptr)
	{
		deallocate(str_, old_capacity + 1);
		str_ = new_ptr;
	}

	return (*this);
}

template <typename _CharTy>
inline typename basic_string<_CharTy>::size_type basic_string<_CharTy>::copy(char_type* cstr, size_type count, size_type pos) const
{
	if (count == 0 || cstr == const_str_)
		return 0;

	check_offset(pos);

	count = clamp_suffix_size(pos, count);
	traits_type::move(cstr, cbegin().core() + pos, count);
	return count;
}

template <typename _CharTy>
inline typename basic_string<_CharTy>::char_type* basic_string<_CharTy>::allocate(size_type count)
{
	return get_allocator().allocate(count);
}

template <typename _CharTy>
inline void basic_string<_CharTy>::deallocate(char_type*& ptr, size_type count)
{
	get_allocator().deallocate(ptr, count);
	ptr = nullptr;
}

template <typename _CharTy>
inline void basic_string<_CharTy>::destroy()
{
	if (operable_ && str_)
	{
		deallocate(str_, capacity_ + 1);
	}
	else
	{
		const_str_ = nullptr;
	}
	size_ = capacity_ = 0;
}

template <typename _CharTy>
inline void basic_string<_CharTy>::swap(basic_string& rhs) noexcept
{
	std::swap(const_str_, rhs.const_str_);
	std::swap(size_, rhs.size_);
	std::swap(capacity_, rhs.capacity_);

	// swap const datas
	std::swap(*const_cast<bool*>(&operable_), *const_cast<bool*>(&rhs.operable_));
}

template <typename _CharTy>
inline void basic_string<_CharTy>::discard_const_data()
{
	if (!operable_)
	{
		// force to enable operability
		*const_cast<bool*>(&operable_) = true;
		const_str_ = nullptr;
		capacity_ = size_ = 0;
	}
}

template <typename _CharTy>
inline void basic_string<_CharTy>::check_operability()
{
	if (!operable_)
	{
		// create a new string, then swap it with self
		basic_string(const_str_, false).swap(*this);
	}
}

//
// details of basic_string<>::parese
//

template <typename _CharTy>
inline basic_string<_CharTy> basic_string<_CharTy>::parse(int val)					{ return ::oc::to_basic_string<char_type>(val); }

template <typename _CharTy>
inline basic_string<_CharTy> basic_string<_CharTy>::parse(unsigned int val)			{ return ::oc::to_basic_string<char_type>(val); }

template <typename _CharTy>
inline basic_string<_CharTy> basic_string<_CharTy>::parse(long val)					{ return ::oc::to_basic_string<char_type>(val); }

template <typename _CharTy>
inline basic_string<_CharTy> basic_string<_CharTy>::parse(unsigned long val)		{ return ::oc::to_basic_string<char_type>(val); }

template <typename _CharTy>
inline basic_string<_CharTy> basic_string<_CharTy>::parse(long long val)			{ return ::oc::to_basic_string<char_type>(val); }

template <typename _CharTy>
inline basic_string<_CharTy> basic_string<_CharTy>::parse(unsigned long long val)	{ return ::oc::to_basic_string<char_type>(val); }

template <typename _CharTy>
inline basic_string<_CharTy> basic_string<_CharTy>::parse(float val)				{ return ::oc::to_basic_string<char_type>(val); }

template <typename _CharTy>
inline basic_string<_CharTy> basic_string<_CharTy>::parse(double val)				{ return ::oc::to_basic_string<char_type>(val); }

template <typename _CharTy>
inline basic_string<_CharTy> basic_string<_CharTy>::parse(long double val)			{ return ::oc::to_basic_string<char_type>(val); }

//
// details of basic_string::format
//

template <typename _CharTy>
template <typename ..._Args>
inline basic_string<_CharTy> basic_string<_CharTy>::format(const char_type* fmt, _Args&& ... args)
{
	return ::oc::format_string(fmt, std::forward<_Args>(args)...);
}

//
// details of operator<<>>
//

template <typename _CharTy>
inline std::basic_ostream<typename basic_string<_CharTy>::char_type>& operator<<(std::basic_ostream<typename basic_string<_CharTy>::char_type>& os, const basic_string<_CharTy>& str)
{
	using ostream = std::basic_ostream<typename basic_string<_CharTy>::char_type, typename basic_string<_CharTy>::traits_type>;
	using size_type = typename basic_string<_CharTy>::size_type;
	using traits = typename basic_string<_CharTy>::traits_type;

	const ostream::sentry ok(os);
	std::ios_base::iostate state = std::ios_base::goodbit;

	if (!ok)
	{
		state |= std::ios_base::badbit;
	}
	else
	{
		const auto str_size = str.size();
		size_type pad = (os.width() <= 0 || static_cast<size_type>(os.width()) <= str_size) ? 0 : static_cast<size_type>(os.width()) - str_size;

		try
		{
			if ((os.flags() & std::ios_base::adjustfield) != std::ios_base::left)
			{
				for (; 0 < pad; --pad)
				{
					if (traits::eq_int_type(traits::eof(), os.rdbuf()->sputc(os.fill())))
					{
						state |= std::ios_base::badbit;
						break;
					}
				}
			}

			if (state == std::ios_base::goodbit
				&& os.rdbuf()->sputn(str.data(), (std::streamsize)str_size) != (std::streamsize)str_size)
			{
				state |= std::ios_base::badbit;
			}
			else
			{
				for (; 0 < pad; --pad)
				{
					if (traits::eq_int_type(traits::eof(), os.rdbuf()->sputc(os.fill())))
					{
						state |= std::ios_base::badbit;
						break;
					}
				}
			}
			os.width(0);
		}
		catch (...)
		{
			os.setstate(std::ios_base::badbit, true);
		}
	}

	os.setstate(state);
	return (os);
}

template <typename _CharTy>
inline std::basic_istream<typename basic_string<_CharTy>::char_type>& operator>>(std::basic_istream<typename basic_string<_CharTy>::char_type>& is, basic_string<_CharTy>& str)
{
	using ctype = std::ctype<typename basic_string<_CharTy>::char_type>;
	using istream = std::basic_istream<typename basic_string<_CharTy>::char_type, typename basic_string<_CharTy>::traits_type>;
	using size_type = typename basic_string<_CharTy>::size_type;
	using traits = typename basic_string<_CharTy>::traits_type;

	bool changed = false;
	const istream::sentry ok(is);
	std::ios_base::iostate state = std::ios_base::goodbit;

	if (ok)
	{
		const ctype& ctype_fac = std::use_facet<ctype>(is.getloc());
		str.erase();
		try
		{
			size_type size = (0 < is.width() && static_cast<size_type>(is.width()) < str.max_size()) ? static_cast<size_type>(is.width()) : str.max_size();
			typename traits::int_type meta = is.rdbuf()->sgetc();

			for (; 0 < size; --size, meta = is.rdbuf()->snextc())
			{
				if (traits::eq_int_type(traits::eof(), meta))
				{
					state |= std::ios_base::eofbit;
					break;
				}
				else if (ctype_fac.is(ctype::space, traits::to_char_type(meta)))
				{
					break;
				}
				else
				{
					str.push_back(traits::to_char_type(meta));
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
	template <typename _CharTy>
	struct FloatingToString
	{
		// template <typename _Ty>
		// static basic_string<_CharTy> convert(const _Ty val);
	};

	template <typename _CharTy>
	struct IntegralToString
	{
		// template <typename _Ty>
		// static basic_string<_CharTy> convert(const _Ty val);
	};
}

template <typename _CharTy>
inline basic_string<_CharTy> to_basic_string(int val)
{
	return (__to_string_detail::IntegralToString<_CharTy>::convert(val));
}

template <typename _CharTy>
inline basic_string<_CharTy> to_basic_string(unsigned int val)
{
	return (__to_string_detail::IntegralToString<_CharTy>::convert(val));
}

template <typename _CharTy>
inline basic_string<_CharTy> to_basic_string(long val)
{
	return (__to_string_detail::IntegralToString<_CharTy>::convert(val));
}

template <typename _CharTy>
inline basic_string<_CharTy> to_basic_string(unsigned long val)
{
	return (__to_string_detail::IntegralToString<_CharTy>::convert(val));
}

template <typename _CharTy>
inline basic_string<_CharTy> to_basic_string(long long val)
{
	return (__to_string_detail::IntegralToString<_CharTy>::convert(val));
}

template <typename _CharTy>
inline basic_string<_CharTy> to_basic_string(unsigned long long val)
{
	return (__to_string_detail::IntegralToString<_CharTy>::convert(val));
}

template <typename _CharTy>
inline basic_string<_CharTy> to_basic_string(float val)
{
	return (__to_string_detail::FloatingToString<_CharTy>::convert(val));
}

template <typename _CharTy>
inline basic_string<_CharTy> to_basic_string(double val)
{
	return (__to_string_detail::FloatingToString<_CharTy>::convert(val));
}

template <typename _CharTy>
inline basic_string<_CharTy> to_basic_string(long double val)
{
	return (__to_string_detail::FloatingToString<_CharTy>::convert(val));
}

template <typename ..._Args>
inline basic_string<char> format_string(const char* const fmt, _Args&& ... args)
{
	using string_type = basic_string<char>;
	const auto len = static_cast<typename string_type::size_type>(::_scprintf(fmt, std::forward<_Args>(args)...));
	if (len)
	{
		string_type str(len, '\0');
		::sprintf_s(&str[0], len + 1, fmt, std::forward<_Args>(args)...);
		return str;
	}
	return string_type{};
}

template <typename ..._Args>
inline basic_string<wchar_t> format_string(const wchar_t* const fmt, _Args&& ... args)
{
	using string_type = basic_string<wchar_t>;
	const auto len = static_cast<typename string_type::size_type>(::_scwprintf(fmt, std::forward<_Args>(args)...));
	if (len)
	{
		string_type str(len, L'\0');
		::swprintf_s(&str[0], len + 1, fmt, std::forward<_Args>(args)...);
		return str;
	}
	return string_type{};
}

namespace __to_string_detail
{
	template <typename _Ty, typename _Elem>
	_Elem* __IntegerToStringBufferEnd(const _Ty val, _Elem* const buffer_end)
	{
		using _UTy = std::make_unsigned_t<_Ty>;

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
			* --next = static_cast<_Elem>('-');

		return next;
	}

	template <>
	struct IntegralToString<char>
	{
		template <typename _Ty>
		static basic_string<char> convert(const _Ty val)
		{
			static_assert(std::is_integral<_Ty>::value, "_Ty must be integral");

			using _Elem = typename basic_string<char>::traits_type::char_type;

			_Elem buffer[21];
			_Elem* const buffer_end = std::end(buffer);
			_Elem* buffer_begin = __IntegerToStringBufferEnd(val, buffer_end);

			return basic_string<char>(buffer_begin, buffer_end);
		}
	};

	template <>
	struct IntegralToString<wchar_t>
	{
		template <typename _Ty>
		static basic_string<wchar_t> convert(const _Ty val)
		{
			static_assert(std::is_integral<_Ty>::value, "_Ty must be integral");

			using _Elem = typename basic_string<wchar_t>::traits_type::char_type;

			_Elem buffer[21];
			_Elem* const buffer_end = std::end(buffer);
			_Elem* buffer_begin = __IntegerToStringBufferEnd(val, buffer_end);

			return basic_string<wchar_t>(buffer_begin, buffer_end);
		}
	};

	template<>
	struct FloatingToString<wchar_t>
	{
		static inline basic_string<wchar_t> convert(const float val)
		{
			return format_string(L"%g", val);
		}

		static inline basic_string<wchar_t> convert(const double val)
		{
			return format_string(L"%g", val);
		}

		static inline basic_string<wchar_t> convert(const long double val)
		{
			return format_string(L"%Lg", val);
		}
	};

	template<>
	struct FloatingToString<char>
	{
		static inline basic_string<char> convert(const float val)
		{
			return format_string("%g", val);
		}

		static inline basic_string<char> convert(const double val)
		{
			return format_string("%g", val);
		}

		static inline basic_string<char> convert(const long double val)
		{
			return format_string("%Lg", val);
		}
	};
}

}

namespace oc
{

template <typename _Codecvt, typename _Elem = wchar_t>
class string_convert
{
	enum : size_t { BUFFER_INCREASE = 8, BUFFER_MAX = 16 };

public:
	using byte_string  = ::oc::basic_string<char>;
	using wide_string  = ::oc::basic_string<_Elem>;
	using codecvt_type = _Codecvt;
	using state_type   = typename codecvt_type::state_type;
	using int_type     = typename wide_string::traits_type::int_type;

	string_convert()
		: string_convert(new codecvt_type)
	{
	}

	explicit string_convert(const codecvt_type* cvt)
		: state_{}
		, cvt_(cvt)
		, loc_()
		, conv_num_(0)
	{
		loc_ = std::locale(loc_, cvt_);
	}

	virtual ~string_convert() { }

	size_t converted() const noexcept { return conv_num_; }

	state_type state() const { return state_; }

	wide_string from_bytes(char _Byte)
	{
		return from_bytes(&_Byte, &_Byte + 1);
	}

	wide_string from_bytes(const char* ptr)
	{
		return from_bytes(ptr, ptr + std::strlen(ptr));
	}

	wide_string from_bytes(const byte_string& byte_str)
	{
		const char* ptr = byte_str.c_str();
		return from_bytes(ptr, ptr + byte_str.size());
	}

	wide_string from_bytes(const char* first, const char* last)
	{
		wide_string wbuf, wstr;
		const char* first_save = first;

		state_ = state_type{};

		wbuf.append(BUFFER_INCREASE, (_Elem) '\0');
		for (conv_num_ = 0; first != last; conv_num_ = static_cast<size_t>(first - first_save))
		{
			_Elem* dest = &*wbuf.begin();
			_Elem* dnext;

			switch (cvt_->in(state_, first, last, first, dest, dest + wbuf.size(), dnext))
			{
			case codecvt_type::partial:
			case codecvt_type::ok:
			{
				if (dest < dnext)
				{
					wstr.append(dest, static_cast<size_t>(dnext - dest));
				}
				else if (wbuf.size() < BUFFER_MAX)
				{
					wbuf.append(BUFFER_INCREASE, '\0');
				}
				else
				{
					throw std::range_error("bad conversion");
				}
				break;
			}

			case codecvt_type::noconv:
			{
				// no conversion, just copy code values
				for (; first != last; ++first)
				{
					wstr.push_back(static_cast<_Elem>(*first));
				}
				break;
			}

			default:
				throw std::range_error("bad conversion");
			}
		}
		return wstr;
	}

	byte_string to_bytes(_Elem _Char)
	{
		return to_bytes(&_Char, &_Char + 1);
	}

	byte_string to_bytes(const _Elem* _Wptr)
	{
		const _Elem* _Next = _Wptr;
		while ((int_type)* _Next != 0) { ++_Next; }

		return to_bytes(_Wptr, _Next);
	}

	byte_string to_bytes(const wide_string& _Wstr)
	{
		const _Elem* _Wptr = _Wstr.c_str();
		return to_bytes(_Wptr, _Wptr + _Wstr.size());
	}

	byte_string to_bytes(const _Elem* first, const _Elem* last)
	{
		byte_string bbuf, bstr;
		const _Elem* first_save = first;

		state_ = state_type{};

		bbuf.append(BUFFER_INCREASE, '\0');
		for (conv_num_ = 0; first != last; conv_num_ = static_cast<size_t>(first - first_save))
		{
			char* dest = &*bbuf.begin();
			char* dnext;

			switch (cvt_->out(state_, first, last, first, dest, dest + bbuf.size(), dnext))
			{
			case codecvt_type::partial:
			case codecvt_type::ok:
			{
				if (dest < dnext)
				{
					bstr.append(dest, (size_t)(dnext - dest));
				}
				else if (bbuf.size() < BUFFER_MAX)
				{
					bbuf.append(BUFFER_INCREASE, '\0');
				}
				else
				{
					throw (std::range_error("bad conversion"));
				}
				break;
			}

			case codecvt_type::noconv:
			{
				// no conversion, just copy code values
				for (; first != last; ++first) {
					bstr.push_back((char)(int_type)* first);
				}
				break;
			}

			default:
				throw (std::range_error("bad conversion"));
			}
		}
		return bstr;
	}

	string_convert(const string_convert&) = delete;
	string_convert& operator=(const string_convert&) = delete;

private:
	const codecvt_type*	cvt_;
	std::locale			loc_;
	state_type			state_;
	size_t				conv_num_;
};

class chs_codecvt
	: public std::codecvt_byname<wchar_t, char, mbstate_t>
{
public:
	chs_codecvt() : codecvt_byname("chs") {}

	static inline ::oc::wstring string_to_wide(::oc::string const& str)
	{
		string_convert<chs_codecvt> conv;
		return conv.from_bytes(str);
	}

	static inline ::oc::string wide_to_string(::oc::wstring const& str)
	{
		string_convert<chs_codecvt> conv;
		return conv.to_bytes(str);
	}
};

inline ::oc::wstring string_to_wide(::oc::string const& str)
{
	return oc::chs_codecvt::string_to_wide(str);
}

inline ::oc::string wide_to_string(::oc::wstring const& str)
{
	return oc::chs_codecvt::wide_to_string(str);
}

}  // namespace oc

namespace std
{

template<>
struct hash<::oc::string>
{
	inline size_t operator()(const ::oc::string& key) const
	{
		return key.hash();
	}
};

template<>
struct hash<::oc::wstring>
{
	inline size_t operator()(const ::oc::wstring& key) const
	{
		return key.hash();
	}
};

template<>
inline void swap<::oc::string>(::oc::string& lhs, ::oc::string& rhs) noexcept
{
	lhs.swap(rhs);
}

template<>
inline void swap<::oc::wstring>(::oc::wstring& lhs, ::oc::wstring& rhs) noexcept
{
	lhs.swap(rhs);
}

}
