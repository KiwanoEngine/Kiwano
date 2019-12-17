// Copyright (c) 2019-2020 OuterC - Nomango

#pragma once
#include <type_traits>
#include <memory>

namespace oc
{

enum class json_type
{
	INTEGER,
	FLOAT,
	STRING,
	VECTOR,
	OBJECT,
	BOOL,
	NIL,
};

namespace __json_detail
{

template <typename _BasicJsonTy>
struct json_value
{
	using string_type	= typename _BasicJsonTy::string_type;
	using char_type		= typename _BasicJsonTy::char_type;
	using integer_type	= typename _BasicJsonTy::integer_type;
	using float_type	= typename _BasicJsonTy::float_type;
	using boolean_type	= typename _BasicJsonTy::boolean_type;
	using array_type	= typename _BasicJsonTy::array_type;
	using object_type	= typename _BasicJsonTy::object_type;

	json_type type;
	union
	{
		object_type* object;
		array_type* vector;
		string_type* string;
		integer_type number_integer;
		float_type number_float;
		boolean_type boolean;
	} data;

	json_value()
	{
		type = json_type::NIL;
		data.object = nullptr;
	}

	json_value(std::nullptr_t)
	{
		type = json_type::NIL;
		data.object = nullptr;
	}

	json_value(const object_type& value)
	{
		type = json_type::OBJECT;
		data.object = create<object_type>(value);
	}

	json_value(const array_type& value)
	{
		type = json_type::VECTOR;
		data.vector = create<array_type>(value);
	}

	json_value(const string_type& value)
	{
		type = json_type::STRING;
		data.string = create<string_type>(value);
	}

	template <typename _CharT>
	json_value(const _CharT* str)
	{
		type = json_type::STRING;
		data.string = create<string_type>(str);
	}

	json_value(const integer_type value)
	{
		type = json_type::INTEGER;
		data.number_integer = value;
	}

	json_value(const float_type value)
	{
		type = json_type::FLOAT;
		data.number_float = value;
	}

	json_value(const boolean_type value)
	{
		type = json_type::BOOL;
		data.boolean = value;
	}

	json_value(const json_type value_type)
	{
		type = value_type;
		switch (type)
		{
		case json_type::OBJECT:
			data.object = create<object_type>();
			break;
		case json_type::VECTOR:
			data.vector = create<array_type>();
			break;
		case json_type::STRING:
			data.string = create<string_type>();
			break;
		case json_type::INTEGER:
			data.number_integer = integer_type(0);
			break;
		case json_type::FLOAT:
			data.number_float = float_type(0.0);
			break;
		case json_type::BOOL:
			data.boolean = boolean_type(false);
			break;
		default:
			data.object = nullptr;
			break;
		}
	}

	json_value(json_value const& other)
	{
		type = other.type;

		switch (other.type)
		{
		case json_type::OBJECT:
			data.object = create<object_type>(*other.data.object);
			break;
		case json_type::VECTOR:
			data.vector = create<array_type>(*other.data.vector);
			break;
		case json_type::STRING:
			data.string = create<string_type>(*other.data.string);
			break;
		case json_type::INTEGER:
			data.number_integer = other.data.number_integer;
			break;
		case json_type::FLOAT:
			data.number_float = other.data.number_float;
			break;
		case json_type::BOOL:
			data.boolean = other.data.boolean;
			break;
		default:
			data.object = nullptr;
			break;
		}
	}

	json_value(json_value&& other) noexcept
	{
		type = other.type;
		data = other.data;
		other.type = json_type::NIL;
		other.data.object = nullptr;
	}

	~json_value()
	{
		clear();
	}

	void swap(json_value& other)
	{
		std::swap(type, other.type);
		std::swap(data, other.data);
	}

	void clear()
	{
		switch (type)
		{
		case json_type::OBJECT:
			destroy<object_type>(data.object);
			break;
		case json_type::VECTOR:
			destroy<array_type>(data.vector);
			break;
		case json_type::STRING:
			destroy<string_type>(data.string);
			break;
		default:
			break;
		}
	}

	template <typename _Ty, typename ..._Args>
	inline _Ty* create(_Args&&... args)
	{
		using allocator_type = typename _BasicJsonTy::template allocator_type<_Ty>;
		using allocator_traits = std::allocator_traits<allocator_type>;

		allocator_type allocator;
		_Ty* ptr = allocator_traits::allocate(allocator, 1);
		allocator_traits::construct(allocator, ptr, std::forward<_Args>(args)...);
		return ptr;
	}

	template <typename _Ty>
	inline void destroy(_Ty* ptr)
	{
		using allocator_type = typename _BasicJsonTy::template allocator_type<_Ty>;
		using allocator_traits = std::allocator_traits<allocator_type>;

		allocator_type allocator;
		allocator_traits::destroy(allocator, ptr);
		allocator_traits::deallocate(allocator, ptr, 1);
	}

	inline json_value& operator=(json_value const& other)
	{
		json_value{ other }.swap(*this);
		return (*this);
	}

	inline json_value& operator=(json_value&& other) noexcept
	{
		clear();
		type = other.type;
		data = std::move(other.data);
		// invalidate payload
		other.type = json_type::NIL;
		other.data.object = nullptr;
		return (*this);
	}
};

} // namespace __json_detail

} // namespace oc
