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
#include <functional>

#ifdef E2D_DEBUG
#	define E2D_DEBUG_CHECK_LIST(list_ptr) list_ptr->Check()
#else
#	define E2D_DEBUG_CHECK_LIST __noop
#endif

namespace easy2d
{
	template <typename T> class IntrusiveList;

	template <typename T>
	class IntrusiveListItem
	{
		T prev_;
		T next_;

		template <typename U>
		friend class IntrusiveList;

	public:
		using ItemType = T;

		IntrusiveListItem() : prev_(), next_() {}

		T const& PrevItem() const { return prev_; }

		T& PrevItem() { return prev_; }

		T const& NextItem() const { return next_; }

		T& NextItem() { return next_; }
	};


	template <typename T>
	class IntrusiveList
	{
		T first_;
		T last_;

	public:
		using ItemType = T;

		IntrusiveList() : first_(), last_() {}

		~IntrusiveList() { Clear(); }

		T const& First() const { return first_; }

		T& First() { return first_; }

		T const& Last() const { return last_; }

		T& Last() { return last_; }

		bool IsEmpty() const { return !first_; }

		void PushBack(T const& child)
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

			E2D_DEBUG_CHECK_LIST(this);
		}

		void PushFront(T const& child)
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

			E2D_DEBUG_CHECK_LIST(this);
		}

		void InsertBefore(T const& child, T const& before)
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

			E2D_DEBUG_CHECK_LIST(this);
		}

		void InsertAfter(T const& child, T const& after)
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

			E2D_DEBUG_CHECK_LIST(this);
		}

		void Remove(T const& child)
		{
#ifdef E2D_DEBUG
			T tmp = first_;
			while (tmp != child)
			{
				E2D_ASSERT((tmp != last_) && "The node to be removed is not in this list");
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

			E2D_DEBUG_CHECK_LIST(this);
		}

		void Clear()
		{
			T p = first_;
			while (p)
			{
				T tmp = p;
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

#ifdef E2D_DEBUG

	private:
		void Check()
		{
			if (!first_)
				return;

			int pos = 0;
			T p = first_;
			T tmp = p;
			do
			{
				tmp = p;
				p = p->next_;
				++pos;

				if (p)
				{
					E2D_ASSERT(p->prev_ == tmp && "Check list failed");
				}
				else
				{
					E2D_ASSERT(tmp == last_ && "Check list failed");
				}
			} while (p);
		}

#endif
	};
}

#undef E2D_DEBUG_CHECK_LIST
