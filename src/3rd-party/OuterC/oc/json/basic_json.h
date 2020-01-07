// Copyright (c) 2019-2020 OuterC - Nomango

#pragma once
#include "json_exception.h"
#include "json_value.h"
#include "json_iterator.h"
#include "json_serializer.h"
#include "json_parser.h"
#include "json_value_getter.h"

namespace oc
{

#define OC_DECLARE_BASIC_JSON_TEMPLATE\
	template <\
	template <class _Kty, class _Ty, class ..._Args> typename _ObjectTy, \
	template <class _Kty, class ..._Args> typename _ArrayTy, \
	typename _StringTy, \
	typename _IntegerTy, \
	typename _FloatTy, \
	typename _BooleanTy, \
	template <class _Ty> typename _Allocator>

#define OC_DECLARE_BASIC_JSON_TPL_ARGS \
	_ObjectTy, _ArrayTy, _StringTy, _IntegerTy, _FloatTy, _BooleanTy, _Allocator

	
OC_DECLARE_BASIC_JSON_TEMPLATE
class basic_json;


//
// is_basic_json
//

template <typename>
struct is_basic_json
	: std::false_type
{
};

OC_DECLARE_BASIC_JSON_TEMPLATE
struct is_basic_json< basic_json<OC_DECLARE_BASIC_JSON_TPL_ARGS> >
	: std::true_type
{
};


OC_DECLARE_BASIC_JSON_TEMPLATE
class basic_json
{
	friend struct __json_detail::iterator_impl<basic_json>;
	friend struct __json_detail::iterator_impl<const basic_json>;
	friend struct __json_detail::json_serializer<basic_json>;
	friend struct __json_detail::json_parser<basic_json>;
	friend struct __json_detail::json_value_getter<basic_json>;

public:
	template <typename _Ty>
	using allocator_type			= _Allocator<_Ty>;
	using size_type					= size_t;
	using difference_type			= ptrdiff_t;
	using string_type				= _StringTy;
	using char_type					= typename _StringTy::value_type;
	using integer_type				= _IntegerTy;
	using float_type				= _FloatTy;
	using boolean_type				= _BooleanTy;
	using array_type				= typename _ArrayTy<basic_json, allocator_type<basic_json>>;
	using object_type				= typename _ObjectTy<string_type, basic_json, std::less<string_type>, allocator_type<std::pair<const string_type, basic_json>>>;
	using initializer_list			= std::initializer_list<basic_json>;

	using iterator					= __json_detail::iterator_impl<basic_json>;
	using const_iterator			= __json_detail::iterator_impl<const basic_json>;
	using reverse_iterator			= std::reverse_iterator<iterator>;
	using const_reverse_iterator	= std::reverse_iterator<const_iterator>;

public:
	basic_json() {}

	basic_json(std::nullptr_t) {}

	basic_json(const json_type type) : value_(type) {}

	basic_json(basic_json const& other) : value_(other.value_) {}

	basic_json(basic_json&& other) noexcept : value_(std::move(other.value_))
	{
		// invalidate payload
		other.value_.type = json_type::NIL;
		other.value_.data.object = nullptr;
	}

	basic_json(string_type const& value) : value_(value) {}

	template <
		typename _CompatibleTy,
		typename std::enable_if<std::is_constructible<string_type, _CompatibleTy>::value, int>::type = 0>
	basic_json(const _CompatibleTy& value)
	{
		value_.type = json_type::STRING;
		value_.data.string = value_.template create<string_type>(value);
	}

	basic_json(array_type const& arr)
		: value_(arr)
	{
	}

	basic_json(object_type const& object)
		: value_(object)
	{
	}

	basic_json(integer_type value)
		: value_(value)
	{
	}

	template <
		typename _IntegerTy,
		typename std::enable_if<std::is_integral<_IntegerTy>::value, int>::type = 0>
	basic_json(_IntegerTy value)
		: value_(static_cast<integer_type>(value))
	{
	}

	basic_json(float_type value)
		: value_(value)
	{
	}

	template <
		typename _FloatingTy,
		typename std::enable_if<std::is_floating_point<_FloatingTy>::value, int>::type = 0>
	basic_json(_FloatingTy value)
		: value_(static_cast<float_type>(value))
	{
	}

	basic_json(boolean_type value)
		: value_(value)
	{
	}

	basic_json(initializer_list const& init_list)
	{
		bool is_an_object = std::all_of(init_list.begin(), init_list.end(), [](const basic_json& json)
		{
			return (json.is_array() && json.size() == 2 && json[0].is_string());
		});

		if (is_an_object)
		{
			value_ = json_type::OBJECT;

			std::for_each(init_list.begin(), init_list.end(), [this](const basic_json& json)
			{
				value_.data.object->emplace(
					*((*json.value_.data.vector)[0].value_.data.string),
					(*json.value_.data.vector)[1]
				);
			});
		}
		else
		{
			value_ = json_type::VECTOR;
			value_.data.vector->reserve(init_list.size());
			value_.data.vector->assign(init_list.begin(), init_list.end());
		}
	}

	static inline basic_json object(initializer_list const& init_list)
	{
		if (init_list.size() != 2 || !(*init_list.begin()).is_string())
		{
			throw json_type_error("cannot create object from initializer_list");
		}

		basic_json json;
		json.value_ = json_type::OBJECT;
		json.value_.data.object->emplace(*((*init_list.begin()).value_.data.string), *(init_list.begin() + 1));
		return json;
	}

	static inline basic_json array(initializer_list const& init_list)
	{
		basic_json json;
		json.value_ = json_type::VECTOR;

		if (init_list.size())
		{
			json.value_.data.vector->reserve(init_list.size());
			json.value_.data.vector->assign(init_list.begin(), init_list.end());
		}
		return json;
	}

	inline bool is_object() const				{ return value_.type == json_type::OBJECT; }

	inline bool is_array() const				{ return value_.type == json_type::VECTOR; }

	inline bool is_string() const				{ return value_.type == json_type::STRING; }

	inline bool is_boolean() const				{ return value_.type == json_type::BOOL; }

	inline bool is_integer() const				{ return value_.type == json_type::INTEGER; }

	inline bool is_float() const				{ return value_.type == json_type::FLOAT; }

	inline bool is_number() const				{ return is_integer() || is_float(); }

	inline bool is_null() const					{ return value_.type == json_type::NIL; }

	inline json_type type() const				{ return value_.type; }

	inline string_type type_name() const
	{
		switch (type())
		{
		case json_type::OBJECT:
			return string_type(L"object");
		case json_type::VECTOR:
			return string_type(L"array");
		case json_type::STRING:
			return string_type(L"string");
		case json_type::INTEGER:
			return string_type(L"integer");
		case json_type::FLOAT:
			return string_type(L"float");
		case json_type::BOOL:
			return string_type(L"boolean");
		case json_type::NIL:
			return string_type(L"null");
		}
		return string_type();
	}

	inline void swap(basic_json& rhs) { value_.swap(rhs.value_); }

public:

	inline iterator					begin()				{ iterator iter(this); iter.set_begin(); return iter; }
	inline const_iterator			begin() const		{ return cbegin(); }
	inline const_iterator			cbegin() const		{ const_iterator iter(this); iter.set_begin(); return iter; }
	inline iterator					end()				{ iterator iter(this); iter.set_end(); return iter; }
	inline const_iterator			end() const			{ return cend(); }
	inline const_iterator			cend() const		{ const_iterator iter(this); iter.set_end(); return iter; }
	inline reverse_iterator			rbegin()			{ return reverse_iterator(end()); }
	inline const_reverse_iterator	rbegin() const		{ return const_reverse_iterator(end()); }
	inline const_reverse_iterator	crbegin() const		{ return rbegin(); }
	inline reverse_iterator			rend()				{ return reverse_iterator(begin()); }
	inline const_reverse_iterator	rend() const		{ return const_reverse_iterator(begin()); }
	inline const_reverse_iterator	crend() const		{ return rend(); }

public:
	inline size_type size() const
	{
		switch (type())
		{
		case json_type::NIL:
			return 0;
		case json_type::VECTOR:
			return value_.data.vector->size();
		case json_type::OBJECT:
			return value_.data.object->size();
		default:
			return 1;
		}
	}

	inline bool empty() const
	{
		if (is_null())
			return true;

		if (is_object())
			return value_.data.object->empty();

		if (is_array())
			return value_.data.vector->empty();

		return false;
	}

	template <typename _Kty>
	inline const_iterator find(_Kty && key) const
	{
		if (is_object())
		{
			const_iterator iter;
			iter.object_iter = value_.data.object->find(std::forward<_Kty>(key));
			return iter;
		}
		return cend();
	}

	template <typename _Kty>
	inline size_type count(_Kty && key) const
	{
		return is_object() ? value_.data.object->count(std::forward<_Kty>(key)) : 0;
	}

	inline size_type erase(const typename object_type::key_type& key)
	{
		if (!is_object())
		{
			throw json_invalid_key("cannot use erase() with non-object value");
		}
		return value_.data.object->erase(key);
	}

	inline void erase(const size_type index)
	{
		if (!is_array())
		{
			throw json_invalid_key("cannot use erase() with non-array value");
		}
		value_.data.vector->erase(value_.data.vector->begin() + static_cast<difference_type>(index));
	}

	template<
		class _IteratorTy,
		typename std::enable_if<
			std::is_same<_IteratorTy, iterator>::value ||
			std::is_same<_IteratorTy, const_iterator>::value, int
		>::type = 0>
	inline _IteratorTy erase(_IteratorTy pos)
	{
		_IteratorTy result = end();

		switch (type())
		{
		case json_type::OBJECT:
		{
			result.object_iter = value_.data.object->erase(pos.object_iter);
			break;
		}

		case json_type::VECTOR:
		{
			result.array_iter = value_.data.vector->erase(pos.array_iter);
			break;
		}

		default:
			throw json_invalid_iterator("cannot use erase() with non-object & non-array value");
		}

		return result;
	}

	template<
		class _IteratorTy,
		typename std::enable_if<
			std::is_same<_IteratorTy, iterator>::value ||
			std::is_same<_IteratorTy, const_iterator>::value, int
		>::type = 0>
	inline _IteratorTy erase(_IteratorTy first, _IteratorTy last)
	{
		_IteratorTy result = end();

		switch (type())
		{
		case json_type::OBJECT:
		{
			result.object_iter = value_.data.object->erase(first.object_iter, last.object_iter);
			break;
		}

		case json_type::VECTOR:
		{
			result.array_iter = value_.data.vector->erase(first.array_iter, last.array_iter);
			break;
		}

		default:
			throw json_invalid_iterator("cannot use erase() with non-object & non-array value");
		}

		return result;
	}

	inline void push_back(basic_json&& json)
	{
		if (!is_null() && !is_array())
		{
			throw json_type_error("cannot use push_back() with non-array value");
		}

		if (is_null())
		{
			value_ = json_type::VECTOR;
		}

		value_.data.vector->push_back(std::move(json));
	}

	inline basic_json& operator+=(basic_json&& json)
	{
		push_back(std::move(json));
		return (*this);
	}

	inline void clear()
	{
		switch (type())
		{
		case json_type::INTEGER:
		{
			value_.data.number_integer = 0;
			break;
		}

		case json_type::FLOAT:
		{
			value_.data.number_float = static_cast<float_type>(0.0);
			break;
		}

		case json_type::BOOL:
		{
			value_.data.boolean = false;
			break;
		}

		case json_type::STRING:
		{
			value_.data.string->clear();
			break;
		}

		case json_type::VECTOR:
		{
			value_.data.vector->clear();
			break;
		}

		case json_type::OBJECT:
		{
			value_.data.object->clear();
			break;
		}

		default:
			break;
		}
	}

public:
	// GET value functions

	inline bool get_value(boolean_type& val) const
	{
		if (is_boolean())
		{
			val = value_.data.boolean;
			return true;
		}
		return false;
	}

	inline bool get_value(integer_type& val) const
	{
		if (is_integer())
		{
			val = value_.data.number_integer;
			return true;
		}
		return false;
	}

	inline bool get_value(float_type& val) const
	{
		if (is_float())
		{
			val = value_.data.number_float;
			return true;
		}
		return false;
	}

	template <
		typename _IntegerTy,
		typename std::enable_if<std::is_integral<_IntegerTy>::value, int>::type = 0>
	inline bool get_value(_IntegerTy& val) const
	{
		if (is_integer())
		{
			val = static_cast<_IntegerTy>(value_.data.number_integer);
			return true;
		}
		return false;
	}

	template <
		typename _FloatingTy,
		typename std::enable_if<std::is_floating_point<_FloatingTy>::value, int>::type = 0>
	inline bool get_value(_FloatingTy& val) const
	{
		if (is_float())
		{
			val = static_cast<_FloatingTy>(value_.data.number_float);
			return true;
		}
		return false;
	}

	inline bool get_value(array_type& val) const
	{
		if (is_array())
		{
			val.assign((*value_.data.vector).begin(), (*value_.data.vector).end());
			return true;
		}
		return false;
	}

	inline bool get_value(string_type& val) const
	{
		if (is_string())
		{
			val.assign(*value_.data.string);
			return true;
		}
		return false;
	}

	inline bool get_value(object_type& val) const
	{
		if (is_object())
		{
			val.assign(*value_.data.object);
			return true;
		}
		return false;
	}

	boolean_type as_bool() const
	{
		if (!is_boolean()) throw json_type_error("json value must be boolean");
		return value_.data.boolean;
	}

	integer_type as_int() const
	{
		if (!is_integer()) throw json_type_error("json value must be integer");
		return value_.data.number_integer;
	}

	float_type as_float() const
	{
		if (!is_float()) throw json_type_error("json value must be float");
		return value_.data.number_float;
	}

	const array_type& as_array() const
	{
		if (!is_array()) throw json_type_error("json value must be array");
		return *value_.data.vector;
	}

	const string_type& as_string() const
	{
		if (!is_string()) throw json_type_error("json value must be string");
		return *value_.data.string;
	}

	const object_type& as_object() const
	{
		if (!is_object()) throw json_type_error("json value must be object");
		return *value_.data.object;
	}

	template <typename _Ty>
	_Ty get() const
	{
		_Ty value;
		__json_detail::json_value_getter<basic_json>::assign(*this, value);
		return value;
	}

public:
	// operator= functions

	inline basic_json& operator=(basic_json const& other)
	{
		value_ = other.value_;
		return (*this);
	}

	inline basic_json& operator=(basic_json&& other) noexcept
	{
		value_ = std::move(other.value_);
		return (*this);
	}

	inline basic_json& operator=(std::nullptr_t)
	{
		value_ = nullptr;
		return (*this);
	}

public:
	// operator[] functions

	inline basic_json& operator[](size_type index)
	{
		if (is_null())
		{
			value_ = json_type::VECTOR;
		}

		if (!is_array())
		{
			throw json_invalid_key("operator[] called on a non-array object");
		}

		if (index >= value_.data.vector->size())
		{
			value_.data.vector->insert(value_.data.vector->end(),
				index - value_.data.vector->size() + 1,
				basic_json()
			);
		}
		return (*value_.data.vector)[index];
	}

	inline basic_json& operator[](size_type index) const
	{
		if (!is_array())
		{
			throw json_invalid_key("operator[] called on a non-array type");
		}

		if (index >= value_.data.vector->size())
		{
			throw std::out_of_range("operator[] index out of range");
		}
		return (*value_.data.vector)[index];
	}

	inline basic_json& operator[](const typename object_type::key_type& key)
	{
		if (is_null())
		{
			value_ = json_type::OBJECT;
		}

		if (!is_object())
		{
			throw json_invalid_key("operator[] called on a non-object type");
		}
		return (*value_.data.object)[key];
	}

	inline basic_json& operator[](const typename object_type::key_type& key) const
	{
		if (!is_object())
		{
			throw json_invalid_key("operator[] called on a non-object object");
		}

		auto iter = value_.data.object->find(key);
		if (iter == value_.data.object->end())
		{
			throw std::out_of_range("operator[] key out of range");
		}
		return iter->second;
	}

	template <typename _CharT>
	inline basic_json& operator[](_CharT* key)
	{
		if (is_null())
		{
			value_ = json_type::OBJECT;
		}

		if (!is_object())
		{
			throw json_invalid_key("operator[] called on a non-object object");
		}
		return (*value_.data.object)[key];
	}

	template <typename _CharT>
	inline basic_json& operator[](_CharT* key) const
	{
		if (!is_object())
		{
			throw json_invalid_key("operator[] called on a non-object object");
		}

		auto iter = value_.data.object->find(key);
		if (iter == value_.data.object->end())
		{
			throw std::out_of_range("operator[] key out of range");
		}
		return iter->second;
	}

public:
	// implicitly convert functions

	inline operator boolean_type () const
	{
		return as_bool();
	}

	inline operator integer_type () const
	{
		return as_int();
	}

	inline operator float_type () const
	{
		return as_float();
	}

	inline operator const array_type& () const
	{
		return as_array();
	}

	inline operator const string_type& () const
	{
		return as_string();
	}

	inline operator const object_type& () const
	{
		return as_object();
	}

public:
	// dumps functions

	string_type dump(
		const int indent = -1,
		const char_type indent_char = ' ',
		const bool char_escape = true) const
	{
		string_type result;
		__json_detail::string_output_adapter<string_type> adapter(result);
		dump(&adapter, indent, indent_char, char_escape);
		return result;
	}

	void dump(
		__json_detail::output_adapter<char_type>* adapter,
		const int indent = -1,
		const char_type indent_char = ' ',
		const bool char_escape = true) const
	{
		__json_detail::json_serializer<basic_json>(adapter, indent_char).dump(*this, (indent >= 0), char_escape, static_cast<uint32_t>(indent));
	}

	friend std::basic_ostream<char_type>& operator<<(std::basic_ostream<char_type>& out, const basic_json& json)
	{
		using char_type = typename std::basic_ostream<char_type>::char_type;

		const bool pretty_print = (out.width() > 0);
		const auto indentation = (pretty_print ? out.width() : 0);
		out.width(0);

		__json_detail::stream_output_adapter<char_type> adapter(out);
		__json_detail::json_serializer<basic_json>(&adapter, out.fill()).dump(json, pretty_print, true, static_cast<uint32_t>(indentation));
		return out;
	}

public:
	// parse functions

	static inline basic_json parse(const string_type& str)
	{
		__json_detail::string_input_adapter<string_type> adapter(str);
		return parse(&adapter);
	}

	static inline basic_json parse(const char_type* str)
	{
		__json_detail::buffer_input_adapter<char_type> adapter(str);
		return parse(&adapter);
	}

	static inline basic_json parse(std::FILE* file)
	{
		__json_detail::file_input_adapter<char_type> adapter(file);
		return parse(&adapter);
	}

	static inline basic_json parse(__json_detail::input_adapter<char_type>* adapter)
	{
		basic_json result;
		__json_detail::json_parser<basic_json>(adapter).parse(result);
		return result;
	}

	friend std::basic_istream<char_type>&
		operator>>(std::basic_istream<char_type>& in, basic_json& json)
	{
		__json_detail::stream_input_adapter<char_type> adapter(in);
		__json_detail::json_parser<basic_json>(&adapter).parse(json);
		return in;
	}

public:
	// compare functions

	friend bool operator==(const basic_json& lhs, const basic_json& rhs)
	{
		const auto lhs_type = lhs.type();
		const auto rhs_type = rhs.type();

		if (lhs_type == rhs_type)
		{
			switch (lhs_type)
			{
			case json_type::VECTOR:
				return (*lhs.value_.data.vector == *rhs.value_.data.vector);

			case json_type::OBJECT:
				return (*lhs.value_.data.object == *rhs.value_.data.object);

			case json_type::NIL:
				return true;

			case json_type::STRING:
				return (*lhs.value_.data.string == *rhs.value_.data.string);

			case json_type::BOOL:
				return (lhs.value_.data.boolean == rhs.value_.data.boolean);

			case json_type::INTEGER:
				return (lhs.value_.data.number_integer == rhs.value_.data.number_integer);

			case json_type::FLOAT:
				return (lhs.value_.data.number_float == rhs.value_.data.number_float);

			default:
				return false;
			}
		}
		else if (lhs_type == json_type::INTEGER && rhs_type == json_type::FLOAT)
		{
			return (static_cast<float_type>(lhs.value_.data.number_integer) == rhs.value_.data.number_float);
		}
		else if (lhs_type == json_type::FLOAT && rhs_type == json_type::INTEGER)
		{
			return (lhs.value_.data.number_float == static_cast<float_type>(rhs.value_.data.number_integer));
		}

		return false;
	}

	friend bool operator!=(const basic_json& lhs, const basic_json& rhs)
	{
		return !(lhs == rhs);
	}

	friend bool operator<(const basic_json& lhs, const basic_json& rhs)
	{
		const auto lhs_type = lhs.type();
		const auto rhs_type = rhs.type();

		if (lhs_type == rhs_type)
		{
			switch (lhs_type)
			{
			case json_type::VECTOR:
				return (*lhs.value_.data.vector) < (*rhs.value_.data.vector);

			case json_type::OBJECT:
				return (*lhs.value_.data.object) < (*rhs.value_.data.object);

			case json_type::NIL:
				return false;

			case json_type::STRING:
				return (*lhs.value_.data.string) < (*rhs.value_.data.string);

			case json_type::BOOL:
				return (lhs.value_.data.boolean < rhs.value_.data.boolean);

			case json_type::INTEGER:
				return (lhs.value_.data.number_integer < rhs.value_.data.number_integer);

			case json_type::FLOAT:
				return (lhs.value_.data.number_float < rhs.value_.data.number_float);

			default:
				return false;
			}
		}
		else if (lhs_type == json_type::INTEGER && rhs_type == json_type::FLOAT)
		{
			return (static_cast<float_type>(lhs.value_.data.number_integer) < rhs.value_.data.number_float);
		}
		else if (lhs_type == json_type::FLOAT && rhs_type == json_type::INTEGER)
		{
			return (lhs.value_.data.number_float < static_cast<float_type>(rhs.value_.data.number_integer));
		}

		return false;
	}

	friend bool operator<=(const basic_json& lhs, const basic_json& rhs)
	{
		return !(rhs < lhs);
	}

	friend bool operator>(const basic_json& lhs, const basic_json& rhs)
	{
		return rhs < lhs;
	}

	friend bool operator>=(const basic_json& lhs, const basic_json& rhs)
	{
		return !(lhs < rhs);
	}

private:
	__json_detail::json_value<basic_json> value_;
};

} // namespace oc

#undef OC_DECLARE_BASIC_JSON_TEMPLATE
#undef OC_DECLARE_BASIC_JSON_TPL_ARGS
