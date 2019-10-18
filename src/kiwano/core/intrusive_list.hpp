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
#include <kiwano/macros.h>

// #define KGE_DEBUG_ENABLE_LIST_CHECK

#ifdef KGE_DEBUG_ENABLE_LIST_CHECK
#	define KGE_DEBUG_CHECK_LIST(list_ptr) list_ptr->check_list()
#else
#	define KGE_DEBUG_CHECK_LIST __noop
#endif

namespace kiwano
{

inline namespace core
{

template <typename _Ty>
class intrusive_list;

template <typename _Ty>
class intrusive_list_item
{
public:
	using value_type		= _Ty;
	using reference			= value_type&;
	using const_reference	= const value_type&;

	intrusive_list_item()				: prev_(), next_() {}
	intrusive_list_item(_Ty const& rhs)	: prev_(rhs->prev_), next_(rhs->next_) {}

	const_reference	prev_item() const	{ return prev_; }
	reference		prev_item()			{ return prev_; }
	const_reference	next_item() const	{ return next_; }
	reference		next_item()			{ return next_; }

private:
	_Ty prev_;
	_Ty next_;

	template <typename U>
	friend class intrusive_list;
};


template <typename _Ty>
class intrusive_list
{
public:
	using value_type		= _Ty;
	using reference			= value_type&;
	using const_reference	= const value_type&;

	intrusive_list()						: first_(), last_() {}
	~intrusive_list()						{ clear(); }

	const_reference	first_item() const		{ return first_; }
	reference		first_item()			{ return first_; }
	const_reference	last_item() const		{ return last_; }
	reference		last_item()				{ return last_; }
	bool			empty() const			{ return !first_; }

	void push_back(const_reference child)
	{
		if (child->prev_)
			child->prev_->next_ = child->next_;
		if (child->next_)
			child->next_->prev_ = child->prev_;

		child->prev_ = last_;
		child->next_ = nullptr;

		if (first_)
		{
			last_->next_ = child;
		}
		else
		{
			first_ = child;
		}

		last_ = child;

		KGE_DEBUG_CHECK_LIST(this);
	}

	void push_front(const_reference child)
	{
		if (child->prev_)
			child->prev_->next_ = child->next_;
		if (child->next_)
			child->next_->prev_ = child->prev_;

		child->prev_ = nullptr;
		child->next_ = first_;

		if (first_)
		{
			first_->prev_ = child;
		}
		else
		{
			last_ = child;
		}

		first_ = child;

		KGE_DEBUG_CHECK_LIST(this);
	}

	void insert_before(const_reference child, const_reference before)
	{
		if (child->prev_)
			child->prev_->next_ = child->next_;
		if (child->next_)
			child->next_->prev_ = child->prev_;

		if (before->prev_)
			before->prev_->next_ = child;
		else
			first_ = child;

		child->prev_ = before->prev_;
		child->next_ = before;
		before->prev_ = child;

		KGE_DEBUG_CHECK_LIST(this);
	}

	void insert_after(const_reference child, const_reference after)
	{
		if (child->prev_)
			child->prev_->next_ = child->next_;
		if (child->next_)
			child->next_->prev_ = child->prev_;

		if (after->next_)
			after->next_->prev_ = child;
		else
			last_ = child;

		child->next_ = after->next_;
		child->prev_ = after;
		after->next_ = child;

		KGE_DEBUG_CHECK_LIST(this);
	}

	void remove(const_reference child)
	{
#ifdef KGE_DEBUG_ENABLE_LIST_CHECK
		_Ty tmp = first_;
		while (tmp != child)
		{
			KGE_ASSERT((tmp != last_) && "The actor to be removed is not in this list");
			tmp = tmp->next_;
		}
#endif

		if (child->next_)
		{
			child->next_->prev_ = child->prev_;
		}
		else
		{
			last_ = child->prev_;
		}

		if (child->prev_)
		{
			child->prev_->next_ = child->next_;
		}
		else
		{
			first_ = child->next_;
		}

		child->prev_ = nullptr;
		child->next_ = nullptr;

		KGE_DEBUG_CHECK_LIST(this);
	}

	void clear()
	{
		_Ty p = first_;
		while (p)
		{
			_Ty tmp = p;
			p = p->next_;
			if (tmp)
			{
				tmp->next_ = nullptr;
				tmp->prev_ = nullptr;
			}
		}
		first_ = nullptr;
		last_ = nullptr;
	}

#ifdef KGE_DEBUG_ENABLE_LIST_CHECK

private:
	void check_list()
	{
		if (!first_)
			return;

		int pos = 0;
		_Ty p = first_;
		_Ty tmp = p;
		do
		{
			tmp = p;
			p = p->next_;
			++pos;

			if (p)
			{
				KGE_ASSERT(p->prev_ == tmp && "Check list failed");
			}
			else
			{
				KGE_ASSERT(tmp == last_ && "Check list failed");
			}
		} while (p);
	}

#endif

private:
	// Iterator
	template <typename _Ty>
	struct iterator_impl
	{
		using iterator_category	= std::bidirectional_iterator_tag;
		using value_type		= _Ty;
		using reference			= _Ty&;
		using difference_type	= ptrdiff_t;

		// disable warning 4996
		using _Unchecked_type = _Ty;

		inline iterator_impl(value_type base = nullptr, bool is_end = false)	: base_(base), is_end_(is_end) {}

		inline _Ty				operator*() const								{ KGE_ASSERT(base_); return static_cast<_Ty>(const_cast<reference>(base_)); }
		inline iterator_impl&	operator++()									{ KGE_ASSERT(base_ && !is_end_); value_type next = base_->next_item(); if (next) base_ = next; else is_end_ = true; return (*this);}
		inline iterator_impl	operator++(int)									{ iterator_impl old = (*this); ++(*this); return old; }
		inline iterator_impl&	operator--()									{ KGE_ASSERT(base_); if (is_end_) is_end_ = false; else base_ = value_type(base_->prev_item()); return (*this); }
		inline iterator_impl	operator--(int)									{ iterator_impl old = (*this); --(*this); return old; }
		inline bool				operator==(iterator_impl const& other) const	{ return base_ == other.base_; }
		inline bool				operator!=(iterator_impl const& other) const	{ return !(*this == other); }
		inline					operator bool() const							{ return base_ != nullptr; }

	private:
		bool is_end_;
		value_type base_;
	};

public:
	using reference					= value_type&;
	using const_reference			= const value_type&;
	using iterator					= iterator_impl<value_type>;
	using const_iterator			= iterator_impl<const value_type>;
	using reverse_iterator			= std::reverse_iterator<iterator>;
	using const_reverse_iterator	= std::reverse_iterator<const_iterator>;

	inline iterator					begin()			{ return iterator(first_item()); }
	inline const_iterator			begin() const	{ return const_iterator(first_item()); }
	inline const_iterator			cbegin() const	{ return begin(); }
	inline iterator					end()			{ return iterator(last_item(), true); }
	inline const_iterator			end() const		{ return const_iterator(last_item(), true); }
	inline const_iterator			cend() const	{ return end(); }
	inline reverse_iterator			rbegin()		{ return reverse_iterator(end()); }
	inline const_reverse_iterator	rbegin() const	{ return const_reverse_iterator(end()); }
	inline const_reverse_iterator	crbegin() const	{ return rbegin(); }
	inline reverse_iterator			rend()			{ return reverse_iterator(begin()); }
	inline const_reverse_iterator	rend() const	{ return const_reverse_iterator(begin()); }
	inline const_reverse_iterator	crend() const	{ return rend(); }
	inline reference				front()			{ if (empty()) throw std::out_of_range("front() called on empty intrusive_list"); return first_item(); }
	inline const_reference			front() const	{ if (empty()) throw std::out_of_range("front() called on empty intrusive_list"); return first_item(); }
	inline reference				back()			{ if (empty()) throw std::out_of_range("back() called on empty intrusive_list"); return last_item(); }
	inline const_reference			back() const	{ if (empty()) throw std::out_of_range("back() called on empty intrusive_list"); return last_item(); }

private:
	value_type first_;
	value_type last_;
};

}  // inline namespace core

}  // namespace kiwano

#undef KGE_DEBUG_CHECK_LIST
#undef KGE_DEBUG_ENABLE_LIST_CHECK
