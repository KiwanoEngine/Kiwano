// Copyright (c) 2016-2018 Kiwano - Nomango
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
#include <string>
#include <type_traits>

namespace kiwano
{

/// \~chinese
/// @brief �����ַ�������
template <typename CharTy>
using BasicString = std::basic_string<CharTy>;

/// \~chinese
/// @brief �ַ�������
using String = BasicString<char>;

/// \~chinese
/// @brief ���ַ�������
using WideString = BasicString<wchar_t>;

/// \~chinese
/// @brief �����ַ�����ͼ
template <typename CharTy>
class BasicStringView;

/// \~chinese
/// @brief �ַ�����ͼ
using StringView = BasicStringView<char>;

/// \~chinese
/// @brief ���ַ�����ͼ
using WideStringView = BasicStringView<wchar_t>;

namespace strings
{

/// \~chinese
/// @brief ��ʽ���ַ���
String Format(const char* format, ...);

/// \~chinese
/// @brief ��ʽ���ַ���
WideString Format(const wchar_t* format, ...);

/// \~chinese
/// @brief ��ʽ���ַ���
String FormatArgs(const char* format, va_list args);

/// \~chinese
/// @brief ��ʽ���ַ���
WideString FormatArgs(const wchar_t* format, va_list args);

/// \~chinese
/// @brief ���ַ���תխ�ַ���
String WideToNarrow(WideStringView str);

/// \~chinese
/// @brief խ�ַ���ת���ַ���
WideString NarrowToWide(StringView str);

/// \~chinese
/// @brief ���ַ���ת utf8 �ַ���
String WideToUTF8(WideStringView str);

/// \~chinese
/// @brief utf8 �ַ���ת���ַ���
WideString UTF8ToWide(StringView str);

}  // namespace strings

/// \~chinese
/// @brief �������ַ�����ͼ
template <typename CharTy>
class BasicStringView
{
public:
    using value_type      = CharTy;
    using pointer         = CharTy*;
    using const_pointer   = const CharTy*;
    using reference       = CharTy&;
    using const_reference = const CharTy&;
    using traits_type     = std::char_traits<CharTy>;
    using size_type       = std::size_t;
    using string_type     = BasicString<CharTy>;

    BasicStringView()
        : ptr_(nullptr)
        , count_(0)
    {
    }

    BasicStringView(const_pointer c_str)
    {
        ptr_ = c_str;
        if (c_str)
        {
            count_ = traits_type::length(c_str);
        }
        else
        {
            count_ = 0;
        }
    }

    BasicStringView(const_pointer c_str, size_type count)
    {
        ptr_ = c_str;
        if (c_str)
        {
            count_ = count;
        }
        else
        {
            count_ = 0;
        }
    }

    BasicStringView(const string_type& str)
        : ptr_(str.c_str())
        , count_(str.length())
    {
    }

    BasicStringView(const BasicStringView& rhs)
        : ptr_(rhs.ptr_)
        , count_(rhs.count_)
    {
    }

    inline const value_type* data() const
    {
        return ptr_;
    }

    inline bool empty() const
    {
        return !ptr_ || !count_;
    }

    inline size_type find(value_type ch) const
    {
        const auto ptr = traits_type::find(ptr_, count_, ch);
        if (ptr)
        {
            return ptr - ptr_;
        }
        return string_type::npos;
    }

    inline BasicStringView substr(size_type pos, size_type count = string_type::npos) const
    {
        if (pos >= count_)
            return BasicStringView();

        if (count == string_type::npos)
            return BasicStringView(ptr_ + pos, count_ - pos);

        KGE_ASSERT(pos + count <= count_);
        return BasicStringView(ptr_ + pos, count);
    }

    inline size_type size() const
    {
        return count_;
    }

    inline value_type at(size_type index) const
    {
        return operator[](index);
    }

    inline value_type operator[](size_type index) const
    {
        if (empty() || index >= count_)
            throw std::out_of_range("operator[] out of index");
        return ptr_[index];
    }

    inline BasicStringView& operator=(const BasicStringView& rhs)
    {
        ptr_   = rhs.ptr_;
        count_ = rhs.count_;
        return *this;
    }

    friend bool operator==(const BasicStringView& lhs, const BasicStringView& rhs)
    {
        return lhs.size() == rhs.size() ? std::char_traits<value_type>::compare(lhs.data(), rhs.data(), lhs.size()) == 0
                                        : false;
    }

    inline operator string_type() const
    {
        return string_type(ptr_, count_);
    }

public:
    //
    // Iterators for BasicStringView
    //
    class Iterator
    {
        const value_type* ptr_;
        size_type         pos_;
        size_type         count_;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = value_type;
        using pointer           = value_type*;
        using reference         = value_type&;
        using difference_type   = ptrdiff_t;

        inline Iterator(pointer ptr, size_type pos, size_type count)
            : ptr_(ptr)
            , pos_(pos)
            , count_(count)
        {
        }

        inline Iterator(const Iterator& rhs)
            : ptr_(rhs.ptr_)
            , pos_(rhs.pos_)
            , count_(rhs.count_)
        {
        }

        inline Iterator& operator=(const Iterator& rhs)
        {
            ptr_   = rhs.ptr_;
            pos_   = rhs.pos_;
            count_ = rhs.count_;
            return *this;
        }

        inline const value_type& operator*() const
        {
            KGE_ASSERT(pos_ < count_);
            return ptr_[pos_];
        }

        inline const value_type* operator->() const
        {
            return std::pointer_traits<pointer>::pointer_to(**this);
        }

        inline Iterator& operator+=(size_type count)
        {
            KGE_ASSERT(pos_ + count >= 0 && pos_ + count <= count_);
            pos_ += count;
            return (*this);
        }

        inline Iterator& operator-=(size_type count)
        {
            KGE_ASSERT(pos_ - count >= 0 && pos_ - count <= count_);
            pos_ -= count;
            return (*this);
        }

        inline const Iterator operator+(size_type count) const
        {
            Iterator iter(*this);
            iter += count;
            return iter;
        }

        inline const Iterator& operator-(size_type count) const
        {
            Iterator iter(*this);
            iter -= count;
            return iter;
        }

        inline Iterator& operator++()
        {
            KGE_ASSERT(pos_ < count_);
            ++pos_;
            return (*this);
        }

        inline Iterator operator++(int)
        {
            Iterator old = (*this);
            ++(*this);
            return old;
        }

        inline Iterator& operator--()
        {
            KGE_ASSERT(pos_ > 0);
            --pos_;
            return (*this);
        }

        inline Iterator operator--(int)
        {
            Iterator old = (*this);
            --(*this);
            return old;
        }

        inline const value_type& operator[](size_type index) const
        {
            Iterator iter = (*this + index);
            return iter.ptr_[iter.pos_];
        }

        inline difference_type operator-(const Iterator& other) const
        {
            KGE_ASSERT(ptr_ == other.ptr_ && count_ == other.count_);
            return static_cast<difference_type>(pos_ - other.pos_);
        }

        inline bool operator==(const Iterator& other) const
        {
            return ptr_ == other.ptr_ && pos_ == other.pos_ && count_ == other.count_;
        }

        inline bool operator!=(const Iterator& other) const
        {
            return !(*this == other);
        }

        inline bool operator<(const Iterator& other) const
        {
            return ptr_ < other.ptr_ || pos_ < other.pos_ || count_ < other.count_;
        }

        inline bool operator<=(const Iterator& other) const
        {
            return (*this < other) || (*this == other);
        }

        inline bool operator>(const Iterator& other) const
        {
            return !(*this <= other);
        }

        inline bool operator>=(const Iterator& other) const
        {
            return !(*this < other);
        }

        inline operator bool() const
        {
            return ptr_ != nullptr && pos_ != count_;
        }
    };

    using const_iterator         = Iterator;
    using iterator               = const_iterator;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using reverse_iterator       = const_reverse_iterator;

    inline const_iterator begin() const
    {
        return const_iterator(ptr_, 0, count_);
    }

    inline const_iterator cbegin() const
    {
        return begin();
    }

    inline const_iterator end() const
    {
        return const_iterator(ptr_, count_, count_);
    }

    inline const_iterator cend() const
    {
        return end();
    }

    inline const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(end());
    }

    inline const_reverse_iterator crbegin() const
    {
        return rbegin();
    }

    inline const_reverse_iterator rend() const
    {
        return const_reverse_iterator(begin());
    }

    inline const_reverse_iterator crend() const
    {
        return rend();
    }

    inline const value_type& front() const
    {
        if (empty())
            throw std::out_of_range("front() called on empty list");
        return ptr_[0];
    }

    inline const value_type& back() const
    {
        if (empty())
            throw std::out_of_range("back() called on empty list");
        return ptr_[count_];
    }

private:
    const value_type* ptr_;
    size_type         count_;
};

}  // namespace kiwano

namespace std
{

template <class _Char>
struct hash<::kiwano::BasicStringView<_Char>>
{
    inline size_t operator()(const ::kiwano::BasicStringView<_Char>& v) const
    {
        std::hash<_Char> hasher;

        size_t result = 0;
        for (size_t i = 0; i < v.size(); ++i)
            result = (result << 1) ^ hasher(v[i]);
        return result;
    }
};

}  // namespace std
