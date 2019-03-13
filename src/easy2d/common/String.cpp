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

#include "String.h"

namespace easy2d
{
	namespace
	{
		std::string StringWideCharToMultiByte(const wchar_t* wstr)
		{
			std::string ret;
			if (wstr)
			{
				int len = ::WideCharToMultiByte(CP_ACP, 0, wstr, -1, nullptr, 0, nullptr, FALSE);
				if (len)
				{
					char* str_tmp = new char[len + 1];
					str_tmp[0] = 0;

					len = ::WideCharToMultiByte(CP_ACP, 0, wstr, -1, str_tmp, len + 1, nullptr, FALSE);

					ret = str_tmp;
					delete[] str_tmp;
				}
			}
			return ret;
		}

		template<class _Traits>
		const size_t TraitsFind(const typename _Traits::char_type* first, size_t first_size, size_t offset,
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

	String::allocator String::allocator_;
	const String::size_type String::npos = static_cast<size_type>(-1);

	String::String()
		: str_(nullptr)
		, size_(0)
		, capacity_(0)
		, operable_(true)
	{
	}

	String::String(const wchar_t * cstr, bool const_str)
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

	String::String(const wchar_t * cstr, size_type count)
		: String()
	{
		assign(cstr, count);
	}

	String::String(size_type count, wchar_t ch)
		: String()
	{
		assign(count, ch);
	}

	String::String(std::wstring const & str)
		: String(str.c_str(), false)
	{
	}

	String::String(String const & rhs)
		: String(rhs.const_str_, !rhs.operable_)
	{
	}

	String::String(String const & rhs, size_type pos, size_type count)
		: String()
	{
		assign(rhs, pos, count);
	}

	String::String(String && rhs)
		: String()
	{
		swap(rhs);
	}

	String::~String()
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

	String & String::erase(size_type offset, size_type count)
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

	String & String::insert(size_type index, size_type count, wchar_t ch)
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

	String & String::insert(size_type index, const wchar_t * cstr, size_type count)
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

	String & String::insert(size_type index, const String & str, size_type off, size_type count)
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

	String & String::append(size_type count, wchar_t ch)
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

	String & String::append(const wchar_t * cstr, size_type count)
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

	String & String::append(String const & other, size_type pos, size_type count)
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

	size_t String::hash() const
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

	int String::compare(const wchar_t * const str) const
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

	String::size_type String::find(const wchar_t ch, size_type offset) const
	{
		if (offset >= size_)
			return String::npos;

		const_iterator citer = traits::find(cbegin().base() + offset, size_, ch);
		return citer ? (citer - cbegin()) : String::npos;
	}

	String::size_type String::find(const wchar_t * const str, size_type offset, size_type count) const
	{
		if (offset >= size_)
			return String::npos;
		return TraitsFind<String::traits>(const_str_, size_, offset, str, count);
	}

	String::size_type String::find_first_of(const wchar_t * const str, size_type offset, size_type count) const
	{
		if (offset >= size_)
			return String::npos;

		const_iterator citer = std::find_first_of(cbegin().base() + offset, cend().base(), str, str + count);
		return (citer != cend()) ? (citer - cbegin()) : String::npos;
	}

	String::size_type String::find_last_of(const wchar_t ch, size_type pos) const
	{
		if (pos == 0 || pos > size_ || pos == npos)
			return npos;

		const_reverse_iterator criter = std::find(crbegin(), crend(), ch);
		return (criter != crend()) ? (criter.base() - cbegin()) : String::npos;
	}

	String::size_type String::find_last_of(const wchar_t * const str, size_type pos, size_type count) const
	{
		if (pos == 0 || pos > size_ || pos == npos)
			return npos;

		return TraitsFindLastOf<String::traits>(const_str_, size_, pos, str, count);
	}

	String & String::replace(size_type pos, size_type count, const wchar_t * cstr, size_type count2)
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

	String & String::replace(size_type pos, size_type count, size_type count2, const wchar_t ch)
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

	String::size_type String::copy(wchar_t * cstr, size_type count, size_type pos) const
	{
		if (count == 0 || cstr == const_str_)
			return 0;

		check_offset(pos);

		count = clamp_suffix_size(pos, count);
		traits::move(cstr, cbegin().base() + pos, count);
		return count;
	}

	std::string String::to_string() const
	{
		return StringWideCharToMultiByte(str_);
	}

	std::wstring String::to_wstring() const
	{
		return std::wstring(const_str_);
	}

	wchar_t * String::allocate(size_type count)
	{
		return allocator_.allocate(count);
	}

	void String::deallocate(wchar_t*& ptr, size_type count)
	{
		allocator_.deallocate(ptr, count);
		ptr = nullptr;
	}

	void String::destroy()
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

	void String::swap(String & rhs)
	{
		std::swap(const_str_, rhs.const_str_);
		std::swap(size_, rhs.size_);
		std::swap(capacity_, rhs.capacity_);

		// swap const datas
		std::swap(*const_cast<bool*>(&operable_), *const_cast<bool*>(&rhs.operable_));
	}

	void String::discard_const_data()
	{
		if (!operable_)
		{
			// force to enable operability
			*const_cast<bool*>(&operable_) = true;
			const_str_ = nullptr;
			capacity_ = size_ = 0;
		}
	}

	void String::check_operability()
	{
		if (!operable_)
		{
			// create a new string, then swap it with self
			String(const_str_, false).swap(*this);
		}
	}

}
