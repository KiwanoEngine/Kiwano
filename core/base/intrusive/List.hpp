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
#include <stdexcept>
#include <functional>

#undef DEBUG_CHECK_LIST
#ifdef E2D_DEBUG
#	define DEBUG_CHECK_LIST(list_ptr) list_ptr->Check()
#else
#	define DEBUG_CHECK_LIST __noop
#endif

namespace easy2d
{
	namespace intrusive
	{
		template <typename T> class List;

		template <typename T>
		class ListItem
		{
			T prev_;
			T next_;

			template <typename U>
			friend class List;

		public:
			using ItemType = T;

			ListItem() : prev_(), next_() {}

			T const& PrevItem() const { return prev_; }

			T& PrevItem() { return prev_; }

			T const& NextItem() const { return next_; }

			T& NextItem() { return next_; }
		};


		template <typename T>
		class List
		{
			T first_;
			T last_;

		public:
			using ItemType = T;

			List() : first_(), last_() {}

			~List() { Clear(); }

			T const& First() const { return first_; }

			T& First() { return first_; }

			T const& Last() const { return last_; }

			T& Last() { return last_; }

			bool IsEmpty() const { return !first_; }

			void PushBack(T& child)
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

				DEBUG_CHECK_LIST(this);
			}

			void PushFront(T& child)
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

				DEBUG_CHECK_LIST(this);
			}

			void Insert(T& child, T& before)
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

				DEBUG_CHECK_LIST(this);
			}

			void Remove(T& child)
			{
#ifdef E2D_DEBUG
				T tmp = first_;
				while (tmp != child)
				{
					if (tmp == last_)
						throw std::logic_error("The node to be removed is not in this list");
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

				DEBUG_CHECK_LIST(this);
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

			void Sort(std::function<bool(T const&, T const&)> const& if_lt)
			{
				if (IsEmpty() || first_ == last_)
					return;

				std::vector<ItemType> temp_vec;
				for (ItemType p = first_; p; p = p->NextItem())
				{
					temp_vec.push_back(p);
				}
				std::sort(temp_vec.begin(), temp_vec.end(), if_lt);

				size_t size = temp_vec.size();
				for (size_t i = 0; i < size_; ++i)
				{
					if (i == 0)
						temp_vec[i]->prev_ = ItemType();
					else
					{
						temp_vec[i]->prev_ = temp_vec[i - 1];
						temp_vec[i - 1]->next_ = temp_vec[i];
					}
					if (i == size_ - 1)
						temp_vec[i]->next_ = ItemType();
					else
					{
						temp_vec[i]->next_ = temp_vec[i + 1];
						temp_vec[i + 1]->prev_ = temp_vec[i];
					}
				}
				first_ = *temp_vec.begin();
				last_ = *temp_vec.rbegin();

				DEBUG_CHECK_LIST(this);
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
						if (p->prev_ != tmp)
							throw std::logic_error("Check list failed");
					}
					else
					{
						if (tmp != last_)
							throw std::logic_error("Check list failed");
					}
				} while (p);
			}

#endif
		};
	}
}

#undef DEBUG_CHECK_LIST
