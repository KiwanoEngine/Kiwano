// Copyright (c) 2019-2020 OuterC - Nomango

#pragma once
#include <cctype>

namespace oc
{
namespace __json_detail
{

struct primitive_iterator
{
	using difference_type = ptrdiff_t;

	inline primitive_iterator(difference_type it = 0)						: it_(it) {}

	inline void set_begin()													{ it_ = 0; }
	inline void set_end()													{ it_ = 1; }

	inline primitive_iterator& operator++()									{ ++it_; return *this; }

	inline primitive_iterator operator++(int)								{ primitive_iterator old(it_); ++(*this); return old; }

	inline primitive_iterator& operator--()									{ --it_; return (*this); }
	inline primitive_iterator operator--(int)								{ primitive_iterator old = (*this); --(*this); return old; }

	inline bool operator==(primitive_iterator const& other) const			{ return it_ == other.it_; }
	inline bool operator!=(primitive_iterator const& other) const			{ return !(*this == other); }

	inline const primitive_iterator operator+(difference_type off) const	{ return primitive_iterator(it_ + off); }
	inline const primitive_iterator operator-(difference_type off) const	{ return primitive_iterator(it_ - off); }

	inline primitive_iterator& operator+=(difference_type off)				{ it_ += off; return (*this); }
	inline primitive_iterator& operator-=(difference_type off)				{ it_ -= off; return (*this); }

	inline difference_type operator-(primitive_iterator const& other) const	{ return it_ - other.it_; }

	inline bool operator<(primitive_iterator const& other) const			{ return it_ < other.it_; }
	inline bool operator<=(primitive_iterator const& other) const			{ return it_ <= other.it_; }
	inline bool operator>(primitive_iterator const& other) const			{ return it_ > other.it_; }
	inline bool operator>=(primitive_iterator const& other) const			{ return it_ >= other.it_; }

private:
	difference_type it_;
};

template <typename _BasicJsonTy>
struct iterator_impl
{
	friend _BasicJsonTy;

	using string_type	= typename _BasicJsonTy::string_type;
	using char_type		= typename _BasicJsonTy::char_type;
	using integer_type	= typename _BasicJsonTy::integer_type;
	using float_type	= typename _BasicJsonTy::float_type;
	using boolean_type	= typename _BasicJsonTy::boolean_type;
	using array_type	= typename _BasicJsonTy::array_type;
	using object_type	= typename _BasicJsonTy::object_type;

	using value_type		= _BasicJsonTy;
	using difference_type	= ptrdiff_t;
	using iterator_category = std::bidirectional_iterator_tag;
	using pointer			= value_type*;
	using reference			= value_type&;

	using array_iterator	= typename _BasicJsonTy::array_type::iterator;
	using object_iterator	= typename _BasicJsonTy::object_type::iterator;

	inline iterator_impl(pointer json = nullptr) : data_(json), primitive_iter(0), array_iter(), object_iter() {}

	inline iterator_impl(const iterator_impl& rhs) : iterator_impl()
	{
		operator=(rhs);
	}

	~iterator_impl() {}

	inline iterator_impl& operator=(const iterator_impl& rhs)
	{
		data_ = rhs.data_;
		if (data_)
		{
			switch (data_->type())
			{
			case json_type::OBJECT:
				object_iter = rhs.object_iter;
				break;
			case json_type::VECTOR:
				array_iter = rhs.array_iter;
				break;
			default:
				primitive_iter = rhs.primitive_iter;
				break;
			}
		}
		return (*this);
	}

	inline reference operator*() const
	{
		check_data();
		check_iterator();
		switch (data_->type())
		{
		case json_type::OBJECT:
			return (object_iter->second);
		case json_type::VECTOR:
			return (*array_iter);
		default:
			return *data_;
		}
	}

	inline pointer operator->() const
	{
		check_data();
		check_iterator();
		switch (data_->type())
		{
		case json_type::OBJECT:
			return &(object_iter->second);
		case json_type::VECTOR:
			return &(*array_iter);
		default:
			return data_;
		}
	}

	inline const typename object_type::key_type& key() const
	{
		check_data();
		check_iterator();
		if (!data_->is_object())
			throw json_invalid_iterator("cannot use key() with non-object type");
		return object_iter->first;
	}

	inline reference value() const
	{
		return operator*();
	}

	inline void set_begin()
	{
		check_data();

		switch (data_->type())
		{
			case json_type::OBJECT:
			{
				object_iter = data_->value_.data.object->begin();
				break;
			}
			case json_type::VECTOR:
			{
				array_iter = data_->value_.data.vector->begin();
				break;
			}
			default:
			{
				primitive_iter.set_begin();
				break;
			}
		}
	}

	inline void set_end()
	{
		check_data();

		switch (data_->type())
		{
			case json_type::OBJECT:
			{
				object_iter = data_->value_.data.object->end();
				break;
			}
			case json_type::VECTOR:
			{
				array_iter = data_->value_.data.vector->end();
				break;
			}
			default:
			{
				primitive_iter.set_end();
				break;
			}
		}
	}

	inline iterator_impl operator++(int) { iterator_impl old = (*this); ++(*this); return old; }
	inline iterator_impl& operator++()
	{
		check_data();

		switch (data_->type())
		{
			case json_type::OBJECT:
			{
				std::advance(object_iter, 1);
				break;
			}
			case json_type::VECTOR:
			{
				std::advance(array_iter, 1);
				break;
			}
			default:
			{
				++primitive_iter;
				break;
			}
		}
		return *this;
	}

	inline iterator_impl operator--(int) { iterator_impl old = (*this); --(*this); return old; }
	inline iterator_impl& operator--()
	{
		check_data();

		switch (data_->type())
		{
			case json_type::OBJECT:
			{
				std::advance(object_iter, -1);
				break;
			}
			case json_type::VECTOR:
			{
				std::advance(array_iter, -1);
				break;
			}
			default:
			{
				--primitive_iter;
				break;
			}
		}
	}

	inline const iterator_impl operator-(difference_type off) const { return operator+(-off); }
	inline const iterator_impl operator+(difference_type off) const { iterator_impl ret(*this); ret += off; return ret; }

	inline iterator_impl& operator-=(difference_type off) { return operator+=(-off); }
	inline iterator_impl& operator+=(difference_type off)
	{
		check_data();

		switch (data_->type())
		{
			case json_type::OBJECT:
			{
				throw json_invalid_iterator("cannot use offsets with object type");
				break;
			}
			case json_type::VECTOR:
			{
				std::advance(array_iter, off);
				break;
			}
			default:
			{
				primitive_iter += off;
				break;
			}
		}
		return *this;
	}

	inline bool operator!=(iterator_impl const& other) const { return !(*this == other); }
	inline bool operator==(iterator_impl const& other) const
	{
		check_data();
		other.check_data();

		if (data_ != other.data_)
			throw json_invalid_iterator("cannot compare iterators of different objects");

		switch (data_->type())
		{
			case json_type::OBJECT:
			{
				return object_iter == other.object_iter;
			}
			case json_type::VECTOR:
			{
				return array_iter == other.array_iter;
			}
			default:
			{
				return primitive_iter == other.primitive_iter;
			}
		}
	}

	inline bool operator>(iterator_impl const& other) const		{ return other.operator<(*this); }
	inline bool operator>=(iterator_impl const& other) const	{ return !operator<(other); }
	inline bool operator<=(iterator_impl const& other) const	{ return !other.operator<(*this); }
	inline bool operator<(iterator_impl const& other) const
	{
		check_data();
		other.check_data();

		if (data_ != other.data_)
			throw json_invalid_iterator("cannot compare iterators of different objects");

		switch (data_->type())
		{
		case json_type::OBJECT:
			throw json_invalid_iterator("cannot compare iterators with object type");
		case json_type::VECTOR:
			return array_iter < other.array_iter;
		default:
			return primitive_iter < other.primitive_iter;
		}
	}

private:
	inline void check_data() const
	{
		if (data_ == nullptr)
		{
			throw json_invalid_iterator("iterator contains an empty object");
		}
	}

	inline void check_iterator() const
	{
		switch (data_->type())
		{
		case json_type::OBJECT:
			if (object_iter == data_->value_.data.object->end())
			{
				throw std::out_of_range("iterator out of range");
			}
			break;
		case json_type::VECTOR:
			if (array_iter == data_->value_.data.vector->end())
			{
				throw std::out_of_range("iterator out of range");
			}
			break;
		default:
			if (primitive_iter == 1)
			{
				throw std::out_of_range("iterator out of range");
			}
			break;
		}
	}

private:
	pointer data_;

	union
	{
		struct
		{
			array_iterator array_iter;
		};

		struct
		{
			object_iterator object_iter;
		};

		struct
		{
			primitive_iterator primitive_iter;  // for other types
		};
	};
};

} // namespace __json_detail

} // namespace oc
