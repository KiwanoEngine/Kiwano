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

template <typename _Ty> class intrusive_list;

template <typename _Ty>
class intrusive_list_item
{
	_Ty prev_;
	_Ty next_;

	template <typename U>
	friend class intrusive_list;

public:
	using value_type = _Ty;

	intrusive_list_item() : prev_(), next_() {}

	_Ty const& prev_item() const	{ return prev_; }

	_Ty& prev_item()				{ return prev_; }

	_Ty const& next_item() const	{ return next_; }

	_Ty& next_item()				{ return next_; }
};


template <typename _Ty>
class intrusive_list
{
	_Ty first_;
	_Ty last_;

public:
	using value_type = _Ty;

	intrusive_list()				: first_(), last_() {}

	~intrusive_list()				{ clear(); }

	_Ty const& first_item() const		{ return first_; }

	_Ty& first_item()					{ return first_; }

	_Ty const& last_item() const		{ return last_; }

	_Ty& last_item()					{ return last_; }

	bool empty() const				{ return !first_; }

	void push_back(_Ty const& child)
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

	void push_front(_Ty const& child)
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

	void insert_before(_Ty const& child, _Ty const& before)
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

	void insert_after(_Ty const& child, _Ty const& after)
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

	void remove(_Ty const& child)
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
	};

}  // inline namespace core

}  // namespace kiwano

#undef KGE_DEBUG_CHECK_LIST
#undef KGE_DEBUG_ENABLE_LIST_CHECK
