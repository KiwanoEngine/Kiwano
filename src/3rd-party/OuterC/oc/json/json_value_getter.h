// Copyright (c) 2019-2020 OuterC - Nomango

#pragma once
#include "json_value.h"

namespace oc
{
namespace __json_detail
{

template <typename _BasicJsonTy>
struct json_value_getter
{
	using string_type	= typename _BasicJsonTy::string_type;
	using char_type		= typename _BasicJsonTy::char_type;
	using integer_type	= typename _BasicJsonTy::integer_type;
	using float_type	= typename _BasicJsonTy::float_type;
	using boolean_type	= typename _BasicJsonTy::boolean_type;
	using array_type	= typename _BasicJsonTy::array_type;
	using object_type	= typename _BasicJsonTy::object_type;

	static inline void assign(const _BasicJsonTy& json, object_type& value)
	{
		if (!json.is_object()) throw json_type_error("json value type must be object");
		value = *json.value_.data.object;
	}

	static inline void assign(const _BasicJsonTy& json, array_type& value)
	{
		if (!json.is_array()) throw json_type_error("json value type must be array");
		value = *json.value_.data.vector;
	}

	static inline void assign(const _BasicJsonTy& json, string_type& value)
	{
		if (!json.is_string()) throw json_type_error("json value type must be string");
		value = *json.value_.data.string;
	}

	static inline void assign(const _BasicJsonTy& json, boolean_type& value)
	{
		if (!json.is_boolean()) throw json_type_error("json value type must be boolean");
		value = json.value_.data.boolean;
	}

	static inline void assign(const _BasicJsonTy& json, integer_type& value)
	{
		if (!json.is_integer()) throw json_type_error("json value type must be integer");
		value = json.value_.data.number_integer;
	}

	template <
		typename _IntegerTy,
		typename std::enable_if<std::is_integral<_IntegerTy>::value, int>::type = 0>
	static inline void assign(const _BasicJsonTy& json, _IntegerTy& value)
	{
		if (!json.is_integer()) throw json_type_error("json value type must be integer");
		value = static_cast<_IntegerTy>(json.value_.data.number_integer);
	}

	static inline void assign(const _BasicJsonTy& json, float_type& value)
	{
		if (!json.is_float()) throw json_type_error("json value type must be float");
		value = json.value_.data.number_float;
	}

	template <
		typename _FloatingTy,
		typename std::enable_if<std::is_floating_point<_FloatingTy>::value, int>::type = 0>
	static inline void assign(const _BasicJsonTy& json, _FloatingTy& value)
	{
		if (!json.is_float()) throw json_type_error("json value type must be float");
		value = static_cast<_FloatingTy>(json.value_.data.number_float);
	}
};

} // namespace __json_detail

} // namespace oc
