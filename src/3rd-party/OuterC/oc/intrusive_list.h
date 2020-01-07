// Copyright (c) 2019-2020 OuterC - Nomango

#pragma once
#include <type_traits>
#include <iterator>
#include <stdexcept>
#include "macros.h"

namespace oc
{

template <typename _Ty, typename _PTy = typename std::pointer_traits<_Ty>::pointer>
class intrusive_list;

template <typename _Ty, typename _PTy = typename std::pointer_traits<_Ty>::pointer>
class intrusive_list_item
{
public:
	using pointer_type			= _PTy;
	using const_pointer_type	= const _PTy;

	intrusive_list_item()					: prev_(nullptr), next_(nullptr) {}
	intrusive_list_item(pointer_type rhs)	: prev_(nullptr), next_(nullptr) { if (rhs) { prev_ = rhs->prev_; next_ = rhs->next_; } }

	const_pointer_type	prev_item() const	{ return prev_; }
	pointer_type		prev_item()			{ return prev_; }
	const_pointer_type	next_item() const	{ return next_; }
	pointer_type		next_item()			{ return next_; }

private:
	pointer_type prev_;
	pointer_type next_;

	friend class intrusive_list<_Ty, _PTy>;
};


template <typename _Ty, typename _PTy>
class intrusive_list
{
public:
	using pointer_type			= _PTy;
	using const_pointer_type	= const _PTy;

	intrusive_list()							: first_(), last_() {}
	~intrusive_list()							{ clear(); }

	const_pointer_type	first_item() const		{ return first_; }
	pointer_type		first_item()			{ return first_; }
	const_pointer_type	last_item() const		{ return last_; }
	pointer_type		last_item()				{ return last_; }

	inline bool empty() const
	{
		return first_ == nullptr;
	}

	void push_back(pointer_type child)
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
	}

	void push_front(pointer_type child)
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
	}

	void insert_before(pointer_type child, pointer_type before)
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
	}

	void insert_after(pointer_type child, pointer_type after)
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
	}

	void remove(pointer_type child)
	{
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
	}

	void clear()
	{
		pointer_type p = first_;
		while (p)
		{
			pointer_type tmp = p;
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

	void check_list()
	{
		if (!first_)
			return;

		int pos = 0;
		pointer_type p = first_;
		pointer_type tmp = p;
		do
		{
			tmp = p;
			p = p->next_;
			++pos;

			if (p)
			{
				OC_ASSERT(p->prev_ == tmp && "Check list failed");
			}
			else
			{
				OC_ASSERT(tmp == last_ && "Check list failed");
			}
		} while (p);
	}

public:
	// Iterator
	template <typename _PTy>
	struct iterator_impl
	{
		using iterator_category		= std::bidirectional_iterator_tag;
		using pointer_type			= _PTy;
		using const_pointer_type	= const _PTy;

		inline iterator_impl(pointer_type ptr = nullptr, bool is_end = false)	: base_(ptr), is_end_(is_end) {}

		inline pointer_type		operator*() const								{ OC_ASSERT(base_ && !is_end_); return base_; }
		inline iterator_impl&	operator++()									{ OC_ASSERT(base_ && !is_end_); pointer_type next = base_->next_item(); if (next) base_ = next; else is_end_ = true; return (*this); }
		inline iterator_impl	operator++(int)									{ iterator_impl old = (*this); ++(*this); return old; }
		inline iterator_impl&	operator--()									{ OC_ASSERT(base_); if (is_end_) is_end_ = false; else base_ = pointer_type(base_->prev_item()); return (*this); }
		inline iterator_impl	operator--(int)									{ iterator_impl old = (*this); --(*this); return old; }
		inline bool				operator==(iterator_impl const& other) const	{ return base_ == other.base_ && is_end_ == other.is_end_; }
		inline bool				operator!=(iterator_impl const& other) const	{ return !(*this == other); }
		inline					operator bool() const							{ return base_ != nullptr && !is_end_; }

	private:
		bool is_end_;
		pointer_type base_;
	};

	using iterator					= iterator_impl<pointer_type>;
	using const_iterator			= iterator_impl<const pointer_type>;
	using reverse_iterator			= std::reverse_iterator<iterator>;
	using const_reverse_iterator	= std::reverse_iterator<const_iterator>;

	inline iterator					begin()			{ return iterator(first_item(), first_item() == nullptr); }
	inline const_iterator			begin() const	{ return const_iterator(first_item(), first_item() == nullptr); }
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
	inline pointer_type				front()			{ if (empty()) throw std::out_of_range("front() called on empty intrusive_list"); return first_item(); }
	inline const_pointer_type		front() const	{ if (empty()) throw std::out_of_range("front() called on empty intrusive_list"); return first_item(); }
	inline pointer_type				back()			{ if (empty()) throw std::out_of_range("back() called on empty intrusive_list"); return last_item(); }
	inline const_pointer_type		back() const	{ if (empty()) throw std::out_of_range("back() called on empty intrusive_list"); return last_item(); }

private:
	pointer_type first_;
	pointer_type last_;
};

}  // namespace oc
