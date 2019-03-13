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
	class E2D_API String
	{
	public:
		using value_type = wchar_t;
		using size_type = size_t;
		using iterator = value_type * ;
		using const_iterator = const value_type*;
		using reference = value_type & ;
		using const_reference = const value_type &;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		using traits = std::char_traits<value_type>;
		using allocator = std::allocator<value_type>;

		String();
		String(const wchar_t* cstr, bool const_str = true);
		String(const wchar_t* cstr, size_type count);
		String(size_type count, wchar_t ch);
		String(std::wstring const& str);
		String(String const& rhs);
		String(String const& rhs, size_type pos, size_type count = npos);
		String(String && rhs);
		~String();

		template <typename _Iter>
		String(_Iter first, _Iter last) : String() { assign_iter(first, last); }

		inline const wchar_t*	c_str() const			{ return const_str_ ? const_str_ : L""; }
		inline wchar_t			at(size_t i) const		{ return (*this)[i]; }
		inline size_type		size() const			{ return size_; }
		inline size_type		length() const			{ return size(); }
		inline size_type		capacity() const		{ return capacity_; }
		inline size_type		max_size() const		{ return (static_cast<size_type>(-1) / sizeof(value_type)); }
		inline bool				empty() const			{ return size_ == 0; }
		inline void				clear()					{ discard_const_data(); if (str_) { str_[0] = value_type(); } size_ = 0; }

		void reserve(const size_type new_cap = 0);
		inline void resize(const size_type new_size, const wchar_t ch = value_type())	{ check_operability(); if (new_size < size_) str_[size_ = new_size] = value_type(); else append(new_size - size_, ch); }

		int compare(const wchar_t* const str) const;
		inline int compare(String const& str) const																	{ return compare(str.c_str()); }

		String& append(size_type count, wchar_t ch);
		String& append(const wchar_t* cstr, size_type count);
		String& append(String const& other, size_type pos, size_type count = npos);
		inline String& append(const wchar_t* cstr)																	{ return append(cstr, traits::length(cstr)); }
		inline String& append(std::wstring const& str)																{ return append(str.c_str()); }
		inline String& append(String const& other)																	{ return append(other.const_str_, 0, npos); }

		size_type find(const wchar_t ch, size_type offset = 0) const;
		size_type find(const wchar_t* const str, size_type offset, size_type count) const;
		inline size_type find(String const& str, size_type offset = 0) const										{ return find(str.c_str(), offset, str.size()); }
		inline size_type find(const wchar_t* const str, size_type offset = 0) const									{ return find(str, offset, traits::length(str)); }

		size_type find_first_of(const wchar_t* const str, size_type offset, size_type count) const;
		inline size_type find_first_of(const wchar_t ch, size_type offset = 0) const								{ return find(ch, offset); }
		inline size_type find_first_of(String const& str, size_type offset = 0) const								{ return find_first_of(str.c_str(), offset, str.size()); }
		inline size_type find_first_of(const wchar_t* const str, size_type offset = 0) const						{ return find_first_of(str, offset, traits::length(str)); }

		size_type find_last_of(const wchar_t ch, size_type pos = npos) const;
		size_type find_last_of(const wchar_t* const str, size_type pos, size_type count) const;
		inline size_type find_last_of(String const& str, size_type pos = npos) const								{ return find_first_of(str.c_str(), pos, str.size()); }
		inline size_type find_last_of(const wchar_t* const str, size_type pos = npos) const							{ return find_first_of(str, pos, traits::length(str)); }

		String& replace(size_type pos, size_type count, const wchar_t* cstr, size_type count2);
		String& replace(size_type pos, size_type count, size_type count2, const wchar_t ch);
		inline String& replace(size_type pos, size_type count, const String& str)									{ return replace(pos, count, str.c_str(), str.size()); }
		inline String& replace(size_type pos, size_type count, const wchar_t* cstr)									{ return replace(pos, count, cstr, traits::length(cstr)); }
		inline String& replace(const_iterator first, const_iterator last, const String& str)						{ return replace(first, last, str.c_str(), str.size()); }
		inline String& replace(const_iterator first, const_iterator last, const wchar_t* cstr)						{ return replace(first, last, cstr, traits::length(cstr)); }
		inline String& replace(const_iterator first, const_iterator last, const wchar_t* cstr, size_type count2)	{ return replace(first - cbegin(), last - first, cstr, traits::length(cstr)); }
		inline String& replace(const_iterator first, const_iterator last, size_type count2, const wchar_t ch)		{ return replace(first - cbegin(), last - first, count2, ch); }

		String& assign(size_type count, const wchar_t ch);
		String& assign(const wchar_t* cstr, size_type count);
		inline String& assign(const wchar_t* cstr, bool const_str = true)					{ String(cstr, const_str).swap(*this); return *this; }
		inline String& assign(std::wstring const& str)										{ String{ str }.swap(*this); return *this; }
		inline String& assign(String const& rhs)											{ String{ rhs }.swap(*this); return *this; }
		inline String& assign(String const& rhs, size_type pos, size_type count = npos)		{ String(rhs, pos, count).swap(*this); return *this; }

		template <typename _Iter>
		inline String& assign(_Iter first, _Iter last)										{ assign_iter(first, last); return(*this); }

		String& erase(size_type offset = 0, size_type count = npos);
		iterator erase(const const_iterator where)											{ size_type off = where - cbegin(); erase(off, 1); return begin() + off; }
		iterator erase(const const_iterator first, const const_iterator last)				{ size_type off = first - cbegin(); erase(first - cbegin(), last - first); return begin() + off; }

		String substr(size_type pos = 0, size_type count = npos) const						{ return String(*this, pos, count); }

		String& insert(size_type index, size_type count, wchar_t ch);
		String& insert(size_type index, const wchar_t* s, size_type count);
		String& insert(size_type index, const String& str, size_type off, size_type count = npos);
		inline String& insert(size_type index, const wchar_t* s)							{ return insert(index, s, traits::length(s)); }
		inline String& insert(size_type index, const String& str)							{ return insert(index, str, 0, str.size()); }
		inline iterator insert(const_iterator pos, size_type count, wchar_t ch)				{ size_type off = pos - cbegin(); insert(off, count, ch); return begin() + off; }
		inline iterator insert(const_iterator pos, wchar_t ch)								{ return insert(pos, 1, ch); }

		inline void push_back(const wchar_t ch)		{ append(1, ch); }
		inline wchar_t pop_back()					{ if (empty()) throw std::out_of_range("pop_back() called on empty string"); check_operability(); wchar_t ch = str_[--size_]; str_[size_] = value_type(); return ch; }

		std::string to_string() const;
		std::wstring to_wstring() const;

		void swap(String& rhs);

		size_t hash() const;

	public:
		inline iterator					begin()			{ check_operability(); return iterator(str_); }
		inline const_iterator			begin() const	{ return const_iterator(const_str_); }
		inline const_iterator			cbegin() const	{ return begin(); }
		inline iterator					end()			{ check_operability(); return iterator(str_ + size_); }
		inline const_iterator			end() const		{ return const_iterator(const_str_ + size_); }
		inline const_iterator			cend() const	{ return end(); }
		inline reverse_iterator			rbegin()		{ check_operability(); return reverse_iterator(end()); }
		inline const_reverse_iterator	rbegin() const	{ return const_reverse_iterator(end()); }
		inline const_reverse_iterator	crbegin() const	{ return rbegin(); }
		inline reverse_iterator			rend()			{ check_operability(); return reverse_iterator(begin()); }
		inline const_reverse_iterator	rend() const	{ return const_reverse_iterator(begin()); }
		inline const_reverse_iterator	crend() const	{ return rend(); }
		inline reference				front()			{ if (empty()) throw std::out_of_range("front() called on empty string"); check_operability(); return str_[0]; }
		inline const_reference			front() const	{ if (empty()) throw std::out_of_range("front() called on empty string"); return const_str_[0]; }
		inline reference				back()			{ if (empty()) throw std::out_of_range("back() called on empty string"); check_operability(); return str_[size_ - 1]; }
		inline const_reference			back() const	{ if (empty()) throw std::out_of_range("back() called on empty string"); return const_str_[size_ - 1]; }

	public:
		inline wchar_t operator[](size_type off) const	{ if(off >= size_) throw std::out_of_range("string subscript out of range"); return const_str_[off]; }
		inline wchar_t& operator[](size_type off)		{ if (off >= size_) throw std::out_of_range("string subscript out of range"); check_operability(); return str_[off]; }

	public:
		inline const String operator+(const wchar_t ch) const			{ return String{ *this }.append(1, ch); }
		inline const String operator+(const wchar_t* cstr) const		{ return String{ *this }.append(cstr); }
		inline const String operator+(std::wstring const& str) const	{ return String{ *this }.append(str); }
		inline const String operator+(String const& rhs) const			{ return String{ *this }.append(rhs); }

		inline String& operator+=(const wchar_t ch)						{ return append(1, ch); }
		inline String& operator+=(const wchar_t* cstr)					{ return append(cstr); }
		inline String& operator+=(std::wstring const& str)				{ return append(str); }
		inline String& operator+=(String const& rhs)					{ return append(rhs); }

	public:
		inline String& operator=(const wchar_t* cstr)					{ if (const_str_ != cstr) String{ cstr }.swap(*this); return *this; }
		inline String& operator=(std::wstring const& str)				{ String{ str }.swap(*this); return *this; }
		inline String& operator=(String const& rhs)						{ if (this != &rhs) String{ rhs }.swap(*this); return *this; }
		inline String& operator=(String && rhs)							{ if (this != &rhs) String{ rhs }.swap(*this); return *this; }

		inline bool operator!=(String const& rhs)						{ return compare(rhs) != 0; }
		inline bool operator!=(const wchar_t* cstr)						{ return compare(cstr) != 0; }

	public:
		static const String::size_type npos;

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

		static allocator allocator_;
	};


	//
	// operator== for String
	//

	inline bool operator==(String const& lhs, String const& rhs) { return lhs.compare(rhs) == 0; }
	inline bool operator==(const wchar_t* lhs, String const& rhs) { return rhs.compare(rhs) == 0; }
	inline bool operator==(String const& lhs, const wchar_t* rhs) { return lhs.compare(rhs) == 0; }

	//
	// operator+ for String
	//

	inline String operator+(const wchar_t* lhs, String const& rhs) { return String{ lhs } + rhs; }

	//
	// operator<> for String
	//

	inline bool operator<(String const& lhs, String const& rhs) { return lhs.compare(rhs) < 0; }
	inline bool operator>(String const& lhs, String const& rhs) { return lhs.compare(rhs) > 0; }
	inline bool operator<=(String const& lhs, String const& rhs) { return lhs.compare(rhs) <= 0; }
	inline bool operator>=(String const& lhs, String const& rhs) { return lhs.compare(rhs) >= 0; }

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
		using SizeType = typename String::size_type;

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
			::swprintf_s(&str[0], len + 1, fmt, std::forward<_Args>(args)...);
			return str;
		}
		return String{};
	}

	namespace __to_string_detail
	{
		template<typename _Ty>
		inline String FloatingToString(const wchar_t *fmt, _Ty val)
		{
			static_assert(std::is_floating_point_v<_Ty>, "_Ty must be floating point");

			return format_wstring(fmt, val);
		}

		template <typename _Ty>
		inline String IntegralToString(const _Ty val)
		{
			static_assert(std::is_integral_v<_Ty>, "_Ty must be integral");

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
	struct hash<easy2d::String>
	{
		size_t operator()(const easy2d::String& key) const
		{
			return key.hash();
		}
	};
}
