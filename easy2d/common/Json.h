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
#include "helper.h"
#include <cstdint>
#include <cctype>
#include <array>
#include <iosfwd>

namespace easy2d
{
	template <
		template <class _Kty, class _Ty, class ..._Args> typename _ObjectTy = easy2d::Map,
		template <class _Kty, class ..._Args> typename _ArrayTy = easy2d::Array,
		typename _StringTy = easy2d::String,
		typename _IntegerTy = std::int32_t,
		typename _FloatTy = double,
		typename _BooleanTy = bool,
		template <class _Ty> typename _Allocator = std::allocator>
		class basic_json;

	using Json = basic_json<>;


	//
	// details of basic_json
	//

#define E2D_DECLARE_BASIC_JSON_TEMPLATE\
	template <\
		template <class _Kty, class _Ty, class ..._Args> typename _ObjectTy, \
		template <class _Kty, class ..._Args> typename _ArrayTy, \
		typename _StringTy, \
		typename _IntegerTy, \
		typename _FloatTy, \
		typename _BooleanTy, \
		template <class _Ty> typename _Allocator>

#define E2D_DECLARE_BASIC_JSON_TPL_ARGS \
	_ObjectTy, _ArrayTy, _StringTy, _IntegerTy, _FloatTy, _BooleanTy, _Allocator


	//
	// json value type
	//

	enum class JsonType
	{
		Integer,
		Float,
		String,
		Array,
		Object,
		Boolean,
		Null,
	};


	//
	// is_basic_json
	//

	template <typename>
	struct is_basic_json
		: ::std::false_type
	{
	};

	E2D_DECLARE_BASIC_JSON_TEMPLATE
	struct is_basic_json< basic_json<E2D_DECLARE_BASIC_JSON_TPL_ARGS> >
		: ::std::true_type
	{
	};

	//
	// exceptions
	//

	class json_exception
		: public std::exception
	{
	public:
		json_exception(const char* message) : std::exception(message) {}
	};

	class json_type_error
		: public json_exception
	{
	public:
		json_type_error() : json_exception("invalid json type") {}
	};

	class json_invalid_key
		: public json_exception
	{
	public:
		json_invalid_key() : json_exception("invalid basic_json key") {}
	};

	class json_invalid_iterator
		: public json_exception
	{
	public:
		json_invalid_iterator() : json_exception("invalid basic_json iterator") {}
	};

	class json_parse_error
		: public json_exception
	{
	public:
		json_parse_error() : json_exception("parse json data error") {}
	};


	//
	// basic_json
	//

	E2D_DECLARE_BASIC_JSON_TEMPLATE
	class basic_json
	{
	public:
		using size_type = std::size_t;
		using string_type = typename _StringTy;
		using integer_type = typename _IntegerTy;
		using float_type = typename _FloatTy;
		using boolean_type = typename _BooleanTy;
		using array_type = typename _ArrayTy<basic_json, _Allocator<basic_json>>;
		using object_type = typename _ObjectTy<string_type, basic_json, std::less<string_type>, _Allocator<std::pair<const string_type, basic_json>>>;
		using initializer_list = std::initializer_list<basic_json>;

		struct JsonValue
		{
			JsonType type;
			union
			{
				object_type* object;
				array_type* vector;
				string_type* string;
				integer_type number_integer;
				float_type number_float;
				boolean_type boolean;
			} data;

			JsonValue()
			{
				type = JsonType::Null;
				data.object = nullptr;
			}

			JsonValue(std::nullptr_t)
			{
				type = JsonType::Null;
				data.object = nullptr;
			}

			JsonValue(const object_type& value)
			{
				type = JsonType::Object;
				data.object = create<object_type>(value);
			}

			JsonValue(const array_type& value)
			{
				type = JsonType::Array;
				data.vector = create<array_type>(value);
			}

			JsonValue(const string_type& value)
			{
				type = JsonType::String;
				data.string = create<string_type>(value);
			}

			template <typename _CharT>
			JsonValue(const _CharT* str)
			{
				type = JsonType::String;
				data.string = create<string_type>(str);
			}

			JsonValue(const integer_type value)
			{
				type = JsonType::Integer;
				data.number_integer = value;
			}

			JsonValue(const float_type value)
			{
				type = JsonType::Float;
				data.number_float = value;
			}

			JsonValue(const boolean_type value)
			{
				type = JsonType::Boolean;
				data.boolean = value;
			}

			JsonValue(const JsonType value_type)
			{
				type = value_type;
				switch (type)
				{
				case JsonType::Object:
					data.object = create<object_type>();
					break;
				case JsonType::Array:
					data.vector = create<array_type>();
					break;
				case JsonType::String:
					data.string = create<string_type>();
					break;
				case JsonType::Integer:
					data.number_integer = integer_type(0);
					break;
				case JsonType::Float:
					data.number_float = float_type(0.0);
					break;
				case JsonType::Boolean:
					data.boolean = boolean_type(false);
					break;
				default:
					data.object = nullptr;
					break;
				}
			}

			JsonValue(JsonValue const& other)
			{
				type = other.type;
				
				switch (other.type)
				{
				case JsonType::Object:
					data.object = create<object_type>(*other.data.object);
					break;
				case JsonType::Array:
					data.vector = create<array_type>(*other.data.vector);
					break;
				case JsonType::String:
					data.string = create<string_type>(*other.data.string);
					break;
				case JsonType::Integer:
					data.number_integer = other.data.number_integer;
					break;
				case JsonType::Float:
					data.number_float = other.data.number_float;
					break;
				case JsonType::Boolean:
					data.boolean = other.data.boolean;
					break;
				default:
					data.object = nullptr;
					break;
				}
			}

			JsonValue(JsonValue&& other)
			{
				type = other.type;
				data = other.data;
				other.type = JsonType::Null;
				other.data.object = nullptr;
			}

			~JsonValue()
			{
				switch (type)
				{
				case JsonType::Object:
					destroy<object_type>(data.object);
					break;
				case JsonType::Array:
					destroy<array_type>(data.vector);
					break;
				case JsonType::String:
					destroy<string_type>(data.string);
					break;
				default:
					break;
				}
			}

			void swap(JsonValue& other)
			{
				std::swap(type, other.type);
				std::swap(data, other.data);
			}

			template <typename _Ty, typename ..._Args>
			inline _Ty* create(_Args&&... args)
			{
				using Allocator = _Allocator<_Ty>;
				using AllocatorTraits = std::allocator_traits<Allocator>;

				Allocator allocator;
				_Ty* ptr = AllocatorTraits::allocate(allocator, 1);
				AllocatorTraits::construct(allocator, ptr, std::forward<_Args>(args)...);
				return ptr;
			}

			template <typename _Ty>
			inline void destroy(_Ty* ptr)
			{
				using Allocator = _Allocator<_Ty>;
				using AllocatorTraits = std::allocator_traits<Allocator>;

				Allocator allocator;
				AllocatorTraits::destroy(allocator, ptr);
				AllocatorTraits::deallocate(allocator, ptr, 1);
			}

			inline JsonValue& operator=(JsonValue const& other)
			{
				JsonValue{ other }.swap(*this);
				return (*this);
			}

			inline JsonValue& operator=(JsonValue && other)
			{
				JsonValue{ std::forward<JsonValue &&>(other) }.swap(*this);
				return (*this);
			}
		};

	public:
		basic_json() {}

		basic_json(std::nullptr_t) {}

		basic_json(const JsonType type) : value_(type) {}

		basic_json(basic_json const& other) : value_(other.value_) {}

		basic_json(string_type const& value) : value_(value) {}

		template <
			typename _CompatibleTy,
			enable_if_t<std::is_constructible<string_type, _CompatibleTy>::value, int> = 0
		>
		basic_json(const _CompatibleTy& value)
		{
			value_.type = JsonType::String;
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

		template <
			typename _FloatingTy,
			enable_if_t<std::is_floating_point<_FloatingTy>::value, int> = 0>
		basic_json(_FloatingTy value)
			: value_(static_cast<float_type>(value))
		{
		}

		template <
			typename _IntegerTy,
			enable_if_t<std::is_integral<_IntegerTy>::value, int> = 0>
		basic_json(_IntegerTy value)
			: value_(static_cast<integer_type>(value))
		{
		}

		basic_json(boolean_type value)
			: value_(value)
		{
		}

		basic_json(initializer_list init_list)
		{
			bool is_an_object = std::all_of(init_list.begin(), init_list.end(), [](const basic_json& json)
			{
				return (json.is_array() && json.size() == 2 && json[0].is_string());
			});

			if (is_an_object)
			{
				value_ = JsonType::Object;

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
				value_ = JsonType::Array;
				value_.data.vector->reserve(init_list.size());
				value_.data.vector->assign(init_list.begin(), init_list.end());
			}
		}

		inline bool is_object() const				{ return value_.type == JsonType::Object; }

		inline bool is_array() const				{ return value_.type == JsonType::Array; }

		inline bool is_string() const				{ return value_.type == JsonType::String; }

		inline bool is_boolean() const				{ return value_.type == JsonType::Boolean; }

		inline bool is_integer() const				{ return value_.type == JsonType::Integer; }

		inline bool is_float() const				{ return value_.type == JsonType::Float; }

		inline bool is_number() const				{ return is_integer() || is_float(); }

		inline bool is_null() const					{ return value_.type == JsonType::Null; }

		inline JsonType type() const				{ return value_.type; }

		inline string_type type_name() const
		{
			switch (type())
			{
			case JsonType::Object:
				return string_type(L"object");
			case JsonType::Array:
				return string_type(L"array");
			case JsonType::String:
				return string_type(L"string");
			case JsonType::Integer:
				return string_type(L"integer");
			case JsonType::Float:
				return string_type(L"float");
			case JsonType::Boolean:
				return string_type(L"boolean");
			case JsonType::Null:
				return string_type(L"null");
			}
			return string_type();
		}

		inline size_type size() const
		{
			switch (type())
			{
			case JsonType::Null:
				return 0;
			case JsonType::Array:
				return value_.data.vector->size();
			case JsonType::Object:
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

		inline void clear() { value_ = nullptr; }

		inline void swap(basic_json& rhs) { value_.swap(rhs.value_); }

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
			enable_if_t<std::is_integral<_IntegerTy>::value, int> = 0>
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
			enable_if_t<std::is_floating_point<_FloatingTy>::value, int> = 0>
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
			if (!is_boolean()) throw json_type_error();
			return value_.data.boolean;
		}

		integer_type as_int() const
		{
			if (!is_integer()) throw json_type_error();
			return value_.data.number_integer;
		}

		float_type as_float() const
		{
			if (!is_float()) throw json_type_error();
			return value_.data.number_float;
		}

		const array_type& as_array() const
		{
			if (!is_array()) throw json_type_error();
			return *value_.data.vector;
		}

		const string_type& as_string() const
		{
			if (!is_string()) throw json_type_error();
			return *value_.data.string;
		}

		const object_type& as_object() const
		{
			if (!is_object()) throw json_type_error();
			return *value_.data.object;
		}

	public:
		// json_value_getter

		template <typename _Ty>
		_Ty get() const
		{
			_Ty value;
			json_value_getter::assign(*this, value);
			return value;
		}

		struct json_value_getter
		{
			template <typename _BasicJsonTy>
			static inline void assign(const _BasicJsonTy& json, object_type& value)
			{
				if (!json.is_object()) throw json_type_error();
				value = *json.value_.data.object;
			}

			template <typename _BasicJsonTy>
			static inline void assign(const _BasicJsonTy& json, array_type& value)
			{
				if (!json.is_array()) throw json_type_error();
				value = *json.value_.data.vector;
			}

			template <typename _BasicJsonTy>
			static inline void assign(const _BasicJsonTy& json, string_type& value)
			{
				if (!json.is_string()) throw json_type_error();
				value = *json.value_.data.string;
			}

			template <typename _BasicJsonTy>
			static inline void assign(const _BasicJsonTy& json, boolean_type& value)
			{
				if (!json.is_boolean()) throw json_type_error();
				value = json.value_.data.boolean;
			}

			template <typename _BasicJsonTy>
			static inline void assign(const _BasicJsonTy& json, integer_type& value)
			{
				if (!json.is_integer()) throw json_type_error();
				value = json.value_.data.number_integer;
			}

			template <
				typename _BasicJsonTy,
				typename _IntegerTy,
				enable_if_t<std::is_integral<_IntegerTy>::value, int> = 0>
			static inline void assign(const _BasicJsonTy& json, _IntegerTy& value)
			{
				if (!json.is_integer()) throw json_type_error();
				value = static_cast<_IntegerTy>(json.value_.data.number_integer);
			}

			template <
				typename _BasicJsonTy,
				typename _FloatingTy,
				enable_if_t<std::is_floating_point<_FloatingTy>::value, int> = 0>
			static inline void assign(const _BasicJsonTy& json, _FloatingTy& value)
			{
				if (!json.is_float()) throw json_type_error();
				value = static_cast<_FloatingTy>(json.value_.data.number_float);
			}
		};

		friend struct json_value_getter;

	public:
		// operator= functions

		inline basic_json& operator=(basic_json other)
		{
			other.swap(*this);
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
				value_ = JsonType::Array;
			}

			if (!is_array())
			{
				throw json_invalid_key();
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
				throw json_invalid_key();
			}

			if (index >= value_.data.vector->size())
			{
				throw json_invalid_key();
			}
			return (*value_.data.vector)[index];
		}

		inline basic_json& operator[](const typename object_type::key_type& key)
		{
			if (is_null())
			{
				value_ = JsonType::Object;
			}

			if (!is_object())
			{
				throw json_invalid_key();
			}
			return (*value_.data.object)[key];
		}

		inline basic_json& operator[](const typename object_type::key_type& key) const
		{
			if (!is_object())
			{
				throw json_invalid_key();
			}

			auto iter = value_.data.object->find(key);
			if (iter == value_.data.object->end())
			{
				throw json_invalid_key();
			}
			return iter.second;
		}

		template <typename _CharT>
		inline basic_json& operator[](_CharT* key)
		{
			if (is_null())
			{
				value_ = JsonType::Object;
			}

			if (!is_object())
			{
				throw json_invalid_key();
			}
			return (*value_.data.object)[key];
		}

		template <typename _CharT>
		inline basic_json& operator[](_CharT* key) const
		{
			if (!is_object())
			{
				throw json_invalid_key();
			}

			auto iter = value_.data.object->find(key);
			if (iter == value_.data.object->end())
			{
				throw json_invalid_key();
			}
			return iter.second;
		}

	public:
		// implicitly convert functions

		template <typename _Ty>
		inline operator _Ty() const
		{
			return get<_Ty>();
		}

	public:
		// iterator for basic_json

		struct original_iterator
		{
			using difference_type = std::ptrdiff_t;

			inline original_iterator(int it = 0)									: it_(it) {}

			inline void set_begin()													{ it_ = 0; }
			inline void set_end()													{ it_ = 1; }

			inline original_iterator& operator++()									{ ++it_; return *this; }

			inline original_iterator operator++(int)								{ original_iterator old(it_); ++(*this); return old; }

			inline original_iterator& operator--()									{ --it_; return (*this); }
			inline original_iterator operator--(int)								{ iterator_impl old = (*this); --(*this); return old; }

			inline bool operator==(original_iterator const& other) const			{ return it_ == other.it_ && it_ == other.it_; }
			inline bool operator!=(original_iterator const& other) const			{ return !(*this == other); }

			inline const original_iterator operator+(difference_type off) const		{ return original_iterator(it_ + off); }
			inline const original_iterator operator-(difference_type off) const		{ return original_iterator(it_ - off); }

			inline original_iterator& operator+=(difference_type off)				{ it_ += off; return (*this); }
			inline original_iterator& operator-=(difference_type off)				{ it_ -= off; return (*this); }

			inline difference_type operator-(original_iterator const& other) const	{ return it_ - other.it_; }

			inline bool operator<(original_iterator const& other) const				{ return it_ < other.it_; }
			inline bool operator<=(original_iterator const& other) const			{ return it_ <= other.it_; }
			inline bool operator>(original_iterator const& other) const				{ return it_ > other.it_; }
			inline bool operator>=(original_iterator const& other) const			{ return it_ >= other.it_; }

		private:
			int it_;
		};

		struct internal_iterator
		{
			typename array_type::iterator array_iter;
			typename object_type::iterator object_iter;
			original_iterator original_iter = 0;  // for other types
		};

		template <typename _Ty>
		struct iterator_impl
		{
			using value_type		= _Ty;
			using difference_type	= std::ptrdiff_t;
			using iterator_category = std::bidirectional_iterator_tag;
			using pointer			= value_type*;
			using reference			= value_type&;

			inline iterator_impl(pointer json = nullptr) : data_(json) {}

			inline reference operator*() const
			{
				check_data();
				check_iterator();
				switch (data_->type())
				{
				case JsonType::Object:
					return (it_.object_iter->second);
				case JsonType::Array:
					return (*it_.array_iter);
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
				case JsonType::Object:
					return &(it_.object_iter->second);
				case JsonType::Array:
					return &(*it_.array_iter);
				default:
					return data_;
				}
			}

			inline const typename object_type::key_type& key() const
			{
				check_data();
				check_iterator();
				if (!data_->is_object())
					throw json_invalid_iterator();
				return it_.object_iter->first;
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
					case JsonType::Object:
					{
						it_.object_iter = data_->value_.data.object->begin();
						break;
					}
					case JsonType::Array:
					{
						it_.array_iter = data_->value_.data.vector->begin();
						break;
					}
					default:
					{
						it_.original_iter.set_begin();
						break;
					}
				}
			}

			inline void set_end()
			{
				check_data();

				switch (data_->type())
				{
					case JsonType::Object:
					{
						it_.object_iter = data_->value_.data.object->end();
						break;
					}
					case JsonType::Array:
					{
						it_.array_iter = data_->value_.data.vector->end();
						break;
					}
					default:
					{
						it_.original_iter.set_end();
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
					case JsonType::Object:
					{
						std::advance(it_.object_iter, 1);
						break;
					}
					case JsonType::Array:
					{
						std::advance(it_.array_iter, 1);
						break;
					}
					default:
					{
						++it_.original_iter;
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
					case JsonType::Object:
					{
						std::advance(it_.object_iter, -1);
						break;
					}
					case JsonType::Array:
					{
						std::advance(it_.array_iter, -1);
						break;
					}
					default:
					{
						--it_.original_iter;
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
					case JsonType::Object:
					{
						throw json_invalid_iterator();
						break;
					}
					case JsonType::Array:
					{
						std::advance(it_.array_iter, off);
						break;
					}
					default:
					{
						it_.original_iter += off;
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

				if (data_->type() != other.data_->type())
					throw json_invalid_iterator();

				switch (data_->type())
				{
					case JsonType::Object:
					{
						return it_.object_iter == other.it_.object_iter;
					}
					case JsonType::Array:
					{
						return it_.array_iter == other.it_.array_iter;
					}
					default:
					{
						return it_.original_iter == other.it_.original_iter;
					}
				}
			}

			inline bool operator>(iterator_impl const& other) const { return other.operator<(*this); }
			inline bool operator>=(iterator_impl const& other) const { return !operator<(other); }
			inline bool operator<=(iterator_impl const& other) const { return !other.operator<(*this); }
			inline bool operator<(iterator_impl const& other) const
			{
				check_data();

				switch (data_->type())
				{
				case JsonType::Object:
					throw json_invalid_iterator();
				case JsonType::Array:
					return it_.array_iter < other.it_.array_iter;
				default:
					return it_.original_iter < other.it_.original_iter;
				}
			}

		private:
			inline void check_data() const
			{
				if (data_ == nullptr)
				{
					throw json_invalid_iterator();
				}
			}

			inline void check_iterator() const
			{
				switch (data_->type())
				{
				case JsonType::Object:
					if (it_.object_iter == data_->value_.data.object->end())
					{
						throw json_invalid_iterator();
					}
					break;
				case JsonType::Array:
					if (it_.array_iter == data_->value_.data.vector->end())
					{
						throw json_invalid_iterator();
					}
					break;
				default:
					if (it_.original_iter == 1)
					{
						throw json_invalid_iterator();
					}
					break;
				}
			}

		private:
			pointer data_;
			internal_iterator it_;
		};

	public:
		friend struct iterator_impl<basic_json>;
		friend struct iterator_impl<const basic_json>;

		using iterator					= iterator_impl<basic_json>;
		using const_iterator			= iterator_impl<const basic_json>;
		using reverse_iterator			= std::reverse_iterator<iterator>;
		using const_reverse_iterator	= std::reverse_iterator<const_iterator>;

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
		// compare functions

		friend bool operator==(const basic_json& lhs, const basic_json& rhs)
		{
			const auto lhs_type = lhs.type();
			const auto rhs_type = rhs.type();

			if (lhs_type == rhs_type)
			{
				switch (lhs_type)
				{
				case JsonType::Array:
					return (*lhs.value_.data.vector == *rhs.value_.data.vector);

				case JsonType::Object:
					return (*lhs.value_.data.object == *rhs.value_.data.object);

				case JsonType::Null:
					return true;

				case JsonType::String:
					return (*lhs.value_.data.string == *rhs.value_.data.string);

				case JsonType::Boolean:
					return (lhs.value_.data.boolean == rhs.value_.data.boolean);

				case JsonType::Integer:
					return (lhs.value_.data.number_integer == rhs.value_.data.number_integer);

				case JsonType::Float:
					return (lhs.value_.data.number_float == rhs.value_.data.number_float);

				default:
					return false;
				}
			}
			else if (lhs_type == JsonType::Integer && rhs_type == JsonType::Float)
			{
				return (static_cast<float_type>(lhs.value_.data.number_integer) == rhs.value_.data.number_float);
			}
			else if (lhs_type == JsonType::Float && rhs_type == JsonType::Integer)
			{
				return (lhs.value_.data.number_float == static_cast<float_type>(rhs.value_.data.number_integer));
			}

			return false;
		}

		template <typename _ScalarTy, enable_if_t<std::is_scalar<_ScalarTy>::value, int> = 0>
		friend bool operator==(const basic_json& lhs, const _ScalarTy rhs)
		{
			return lhs == basic_json(rhs);
		}

		template <typename _ScalarTy, enable_if_t<std::is_scalar<_ScalarTy>::value, int> = 0>
		friend bool operator==(const _ScalarTy lhs, const basic_json& rhs)
		{
			return basic_json(lhs) == rhs;
		}

		friend bool operator!=(const basic_json& lhs, const basic_json& rhs)
		{
			return !(lhs == rhs);
		}

		template <typename _ScalarTy, enable_if_t<std::is_scalar<_ScalarTy>::value, int> = 0>
		friend bool operator!=(const basic_json& lhs, const _ScalarTy rhs)
		{
			return lhs != basic_json(rhs);
		}

		template <typename _ScalarTy, enable_if_t<std::is_scalar<_ScalarTy>::value, int> = 0>
		friend bool operator!=(const _ScalarTy lhs, const basic_json& rhs)
		{
			return rhs != basic_json(lhs);
		}

		friend bool operator<(const basic_json& lhs, const basic_json& rhs)
		{
			const auto lhs_type = lhs.type();
			const auto rhs_type = rhs.type();

			if (lhs_type == rhs_type)
			{
				switch (lhs_type)
				{
				case JsonType::Array:
					return (*lhs.value_.data.vector) < (*rhs.value_.data.vector);

				case JsonType::Object:
					return (*lhs.value_.data.object) < (*rhs.value_.data.object);

				case JsonType::Null:
					return false;

				case JsonType::String:
					return (*lhs.value_.data.string) < (*rhs.value_.data.string);

				case JsonType::Boolean:
					return (lhs.value_.data.boolean < rhs.value_.data.boolean);

				case JsonType::Integer:
					return (lhs.value_.data.number_integer < rhs.value_.data.number_integer);

				case JsonType::Float:
					return (lhs.value_.data.number_float < rhs.value_.data.number_float);

				default:
					return false;
				}
			}
			else if (lhs_type == JsonType::Integer && rhs_type == JsonType::Float)
			{
				return (static_cast<float_type>(lhs.value_.data.number_integer) < rhs.value_.data.number_float);
			}
			else if (lhs_type == JsonType::Float && rhs_type == JsonType::Integer)
			{
				return (lhs.value_.data.number_float < static_cast<float_type>(rhs.value_.data.number_integer));
			}

			return false;
		}

		template <typename _ScalarTy, enable_if_t<std::is_scalar<_ScalarTy>::value, int> = 0>
		friend bool operator<(const basic_json& lhs, const _ScalarTy rhs)
		{
			return lhs < basic_json(rhs);
		}

		template <typename _ScalarTy, enable_if_t<std::is_scalar<_ScalarTy>::value, int> = 0>
		friend bool operator<(const _ScalarTy lhs, const basic_json& rhs)
		{
			return basic_json(lhs) < rhs;
		}

		friend bool operator<=(const basic_json& lhs, const basic_json& rhs)
		{
			return !(rhs < lhs);
		}

		template <typename _ScalarTy, enable_if_t<std::is_scalar<_ScalarTy>::value, int> = 0>
		friend bool operator<=(const basic_json& lhs, const _ScalarTy rhs)
		{
			return lhs <= basic_json(rhs);
		}

		template <typename _ScalarTy, enable_if_t<std::is_scalar<_ScalarTy>::value, int> = 0>
		friend bool operator<=(const _ScalarTy lhs, const basic_json& rhs)
		{
			return basic_json(lhs) <= rhs;
		}

		friend bool operator>(const basic_json& lhs, const basic_json& rhs)
		{
			return rhs < lhs;
		}

		template <typename _ScalarTy, enable_if_t<std::is_scalar<_ScalarTy>::value, int> = 0>
		friend bool operator>(const basic_json& lhs, const _ScalarTy rhs)
		{
			return lhs > basic_json(rhs);
		}

		template <typename _ScalarTy, enable_if_t<std::is_scalar<_ScalarTy>::value, int> = 0>
		friend bool operator>(const _ScalarTy lhs, const basic_json& rhs)
		{
			return basic_json(lhs) > rhs;
		}

		friend bool operator>=(const basic_json& lhs, const basic_json& rhs)
		{
			return !(lhs < rhs);
		}

		template <typename _ScalarTy, enable_if_t<std::is_scalar<_ScalarTy>::value, int> = 0>
		friend bool operator>=(const basic_json& lhs, const _ScalarTy rhs)
		{
			return lhs >= basic_json(rhs);
		}

		template <typename _ScalarTy, enable_if_t<std::is_scalar<_ScalarTy>::value, int> = 0>
		friend bool operator>=(const _ScalarTy lhs, const basic_json& rhs)
		{
			return basic_json(lhs) >= rhs;
		}

	public:
		// output_adapter

		template <typename _CharTy>
		struct output_adapter
		{
			using char_type = typename _StringTy::value_type;
			using size_type = typename _StringTy::size_type;
			using char_traits = std::char_traits<char_type>;

			virtual void write(const _CharTy ch) = 0;
			virtual void write(const _CharTy* str, std::size_t size) = 0;
			virtual void write(const _CharTy* str)
			{
				const auto size = char_traits::length(str);
				write(str, static_cast<size_type>(size));
			}
		};

		template <typename _StringTy>
		struct string_output_adapter
			: public output_adapter<typename _StringTy::value_type>
		{
			using char_type = typename _StringTy::value_type;
			using size_type = typename _StringTy::size_type;
			using char_traits = std::char_traits<char_type>;

			string_output_adapter(_StringTy& str) : str_(str) {}

			virtual void write(const char_type ch) override
			{
				str_.push_back(ch);
			}

			virtual void write(const char_type* str, std::size_t size) override
			{
				str_.append(str, static_cast<size_type>(size));
			}

		private:
			_StringTy& str_;
		};

		template <typename _CharTy>
		struct stream_output_adapter
			: public output_adapter<_CharTy>
		{
			using char_type = _CharTy;
			using size_type = typename std::streamsize;
			using char_traits = std::char_traits<char_type>;

			stream_output_adapter(std::basic_ostream<char_type>& stream) : stream_(stream) {}

			virtual void write(const char_type ch) override
			{
				stream_.put(ch);
			}

			virtual void write(const char_type* str, std::size_t size) override
			{
				stream_.write(str, static_cast<size_type>(size));
			}

		private:
			std::basic_ostream<char_type>& stream_;
		};

	public:
		friend std::basic_ostream<typename string_type::value_type>& operator<<(std::basic_ostream<typename string_type::value_type>& out, const basic_json& json)
		{
			using char_type = typename std::basic_ostream<typename string_type::value_type>::char_type;

			const bool pretty_print = (out.width() > 0);
			const auto indentation = (pretty_print ? out.width() : 0);
			out.width(0);

			stream_output_adapter<char_type> adapter(out);
			json_serializer(&adapter, out.fill()).dump(json, pretty_print, static_cast<unsigned int>(indentation));
			return out;
		}

		string_type dump(
			const int indent = -1,
			const typename string_type::value_type indent_char = ' ') const
		{
			string_type result;
			string_output_adapter<string_type> adapter(result);
			dump(&adapter, indent, indent_char);
			return result;
		}

		void dump(
			output_adapter<typename string_type::value_type>* adapter,
			const int indent = -1,
			const typename string_type::value_type indent_char = ' ') const
		{
			if (indent >= 0)
			{
				json_serializer(adapter, indent_char).dump(*this, true, static_cast<unsigned int>(indent));
			}
			else
			{
				json_serializer(adapter, indent_char).dump(*this, false, 0);
			}
		}

	public:
		// json_serializer

		struct json_serializer
		{
			using char_type = typename string_type::value_type;

			json_serializer(output_adapter<char_type>* out, const wchar_t indent_char)
				: out(out)
				, indent_char(indent_char)
				, indent_string(32, indent_char)
			{}

			void dump(
				const basic_json& json,
				const bool pretty_print,
				const unsigned int indent_step,
				const unsigned int current_indent = 0)
			{
				switch (json.type())
				{
				case JsonType::Object:
				{
					auto& object = *json.value_.data.object;

					if (object.empty())
					{
						out->write(L"{}");
						return;
					}

					if (pretty_print)
					{
						out->write(L"{\n");

						const auto new_indent = current_indent + indent_step;
						if (indent_string.size() < new_indent)
						{
							indent_string.resize(indent_string.size() * 2, indent_char);
						}

						auto iter = object.cbegin();
						const auto size = object.size();
						for (std::size_t i = 0; i < size; ++i, ++iter)
						{
							out->write(indent_string.c_str(), new_indent);
							out->write('\"');
							out->write(iter->first.c_str());
							out->write(L"\": ");
							dump(iter->second, true, indent_step, new_indent);

							// not last element
							if (i != size - 1)
								out->write(L",\n");
						}

						out->write('\n');
						out->write(indent_string.c_str(), current_indent);
						out->write('}');
					}
					else
					{
						out->write('{');

						auto iter = object.cbegin();
						const auto size = object.size();
						for (std::size_t i = 0; i < size; ++i, ++iter)
						{
							out->write('\"');
							out->write(iter->first.c_str());
							out->write(L"\":");
							dump(iter->second, false, indent_step, current_indent);
							
							// not last element
							if (i != size - 1)
								out->write(',');
						}

						out->write('}');
					}

					return;
				}

				case JsonType::Array:
				{
					auto& vector = *json.value_.data.vector;

					if (vector.empty())
					{
						out->write(L"[]");
						return;
					}

					if (pretty_print)
					{
						out->write(L"[\n");

						const auto new_indent = current_indent + indent_step;
						if (indent_string.size() < new_indent)
						{
							indent_string.resize(indent_string.size() * 2, indent_char);
						}

						auto iter = vector.cbegin();
						const auto size = vector.size();
						for (std::size_t i = 0; i < size; ++i, ++iter)
						{
							out->write(indent_string.c_str(), new_indent);
							dump(*iter, true, indent_step, new_indent);

							// not last element
							if (i != size - 1)
								out->write(L",\n");
						}

						out->write('\n');
						out->write(indent_string.c_str(), current_indent);
						out->write(']');
					}
					else
					{
						out->write('[');

						auto iter = vector.cbegin();
						const auto size = vector.size();
						for (std::size_t i = 0; i < size; ++i, ++iter)
						{
							dump(*iter, false, indent_step, current_indent);
							// not last element
							if (i != size - 1)
								out->write(',');
						}

						out->write(']');
					}

					return;
				}

				case JsonType::String:
				{
					out->write('\"');
					out->write(json.value_.data.string->c_str());
					out->write('\"');
					return;
				}

				case JsonType::Boolean:
				{
					if (json.value_.data.boolean)
					{
						out->write(L"true");
					}
					else
					{
						out->write(L"false");
					}
					return;
				}

				case JsonType::Integer:
				{
					dump_integer(json.value_.data.number_integer);
					return;
				}

				case JsonType::Float:
				{
					dump_float(json.value_.data.number_float);
					return;
				}

				case JsonType::Null:
				{
					out->write(L"null");
					return;
				}
				}
			}

			void dump_integer(integer_type val)
			{
				if (val == 0)
				{
					out->write('0');
					return;
				}

				auto uval = static_cast<std::make_unsigned_t<integer_type>>(val);

				if (val < 0)
					uval = 0 - uval;

				auto next = number_buffer.rbegin();
				*next = '\0';

				do
				{
					*(++next) = static_cast<wchar_t>('0' + uval % 10);
					uval /= 10;
				} while (uval != 0);

				if (val < 0)
					*(++next) = '-';

				out->write(&(*next));
			}

			void dump_float(float_type val)
			{
				const auto digits = std::numeric_limits<float_type>::max_digits10;
				const auto len = ::_scwprintf(L"%.*g", digits, val);
				if (len)
				{
					number_buffer[0] = '\0';
					::swprintf_s(&number_buffer[0], len + 1, L"%.*g", digits, val);
				}
				else
				{
					number_buffer[0] = '0';
					number_buffer[1] = '.';
					number_buffer[2] = '0';
					number_buffer[3] = '\0';
				}
				out->write(number_buffer.data());
			}

		private:
			output_adapter<char_type>* out;
			char_type indent_char;
			string_type indent_string;
			std::array<char_type, 21> number_buffer;
		};

		friend struct json_serializer;

	public:
		// input_adapter

		template <typename _CharTy>
		struct input_adapter
		{
			using char_type = _CharTy;
			using char_traits = std::char_traits<char_type>;

			virtual typename char_traits::int_type get_char() = 0;
			virtual ~input_adapter() = default;
		};

		template <typename _CharTy>
		struct file_input_adapter
			: public input_adapter<_CharTy>
		{
			using char_type = typename input_adapter<_CharTy>::char_type;
			using char_traits = typename input_adapter<_CharTy>::char_traits;

			file_input_adapter(std::FILE* file) : file(file) {}

			virtual typename char_traits::int_type get_char() override
			{
				return std::fgetc(file);
			}

		private:
			std::FILE* file;
		};

		template <typename _CharTy>
		struct stream_input_adapter
			: public input_adapter<_CharTy>
		{
			using char_type = typename input_adapter<_CharTy>::char_type;
			using char_traits = typename input_adapter<_CharTy>::char_traits;

			stream_input_adapter(std::basic_istream<char_type>& stream) : stream(stream), streambuf(*stream.rdbuf()) {}

			virtual typename char_traits::int_type get_char() override
			{
				auto ch = streambuf.sbumpc();
				if (ch == EOF)
				{
					stream.clear(stream.rdstate() | std::ios::eofbit);
				}
				return ch;
			}

			virtual ~stream_input_adapter()
			{
				stream.clear(stream.rdstate() & std::ios::eofbit);
			}

		private:
			std::basic_istream<char_type>& stream;
			std::basic_streambuf<char_type>& streambuf;
		};

		template <typename _StringTy>
		struct string_input_adapter
			: public input_adapter<typename _StringTy::value_type>
		{
			using char_type = typename input_adapter<typename _StringTy::value_type>::char_type;
			using char_traits = typename input_adapter<typename _StringTy::value_type>::char_traits;

			string_input_adapter(const _StringTy& str) : str(str), index(0) {}

			virtual typename char_traits::int_type get_char() override
			{
				if (index == str.size())
					return char_traits::eof();
				return str[index++];
			}

		private:
			const _StringTy& str;
			typename _StringTy::size_type index;
		};

		template <typename _CharTy>
		struct buffer_input_adapter
			: public input_adapter<_CharTy>
		{
			using char_type = typename input_adapter<_CharTy>::char_type;
			using char_traits = typename input_adapter<_CharTy>::char_traits;

			buffer_input_adapter(const _CharTy* str) : str(str), index(0) {}

			virtual typename char_traits::int_type get_char() override
			{
				if (str[index] == '\0')
					return char_traits::eof();
				return str[index++];
			}

		private:
			const char_type* str;
			std::size_t index;
		};

	public:
		// json_parser

		friend std::basic_istream<typename string_type::value_type>&
			operator>>(std::basic_istream<typename string_type::value_type>& in, basic_json& json)
		{
			using char_type = typename string_type::value_type;

			stream_input_adapter<char_type> adapter(in);
			json_parser(&adapter).parse(json);
			return in;
		}

		static inline basic_json parse(const string_type& str)
		{
			string_input_adapter<string_type> adapter(str);
			return parse(&adapter);
		}

		static inline basic_json parse(const typename string_type::value_type* str)
		{
			buffer_input_adapter<typename string_type::value_type> adapter(str);
			return parse(&adapter);
		}

		static inline basic_json parse(std::FILE* file)
		{
			using char_type = typename string_type::value_type;

			file_input_adapter<char_type> adapter(file);
			return parse(&adapter);
		}

		static inline basic_json parse(input_adapter<typename string_type::value_type>* adapter)
		{
			basic_json result;
			json_parser(adapter).parse(result);
			return result;
		}

		struct json_lexer
		{
			using char_type = typename string_type::value_type;
			using char_traits = std::char_traits<char_type>;

			json_lexer(input_adapter<char_type>* adapter) : adapter(adapter)
			{
				// read first char
				read_next();
			}

			enum class token_type
			{
				uninitialized,

				literal_true,
				literal_false,
				literal_null,

				value_string,
				value_integer,
				value_float,

				begin_array,
				end_array,

				begin_object,
				end_object,

				name_separator,
				value_separator,

				parse_error,

				end_of_input
			};

			typename char_traits::int_type read_next()
			{
				current = adapter->get_char();
				return current;
			}

			void skip_spaces()
			{
				while (current == ' ' || current == '\t' || current == '\n' || current == '\r')
				{
					read_next();
				}
			}

			token_type scan()
			{
				skip_spaces();

				token_type result = token_type::uninitialized;
				switch (current)
				{
				case '[':
					result = token_type::begin_array;
					break;
				case ']':
					result = token_type::end_array;
					break;
				case '{':
					result = token_type::begin_object;
					break;
				case '}':
					result = token_type::end_object;
					break;
				case ':':
					result = token_type::name_separator;
					break;
				case ',':
					result = token_type::value_separator;
					break;

				case 't':
					return scan_literal(L"true", token_type::literal_true);
				case 'f':
					return scan_literal(L"false", token_type::literal_false);
				case 'n':
					return scan_literal(L"null", token_type::literal_null);

				case '\"':
					return scan_string();

				case '-':
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					return scan_number();

				case '\0':
				case char_traits::eof():
					return token_type::end_of_input;

				// unexpected char
				default:
					return token_type::parse_error;
				}

				// skip current char
				read_next();

				return result;
			}

			token_type scan_literal(const char_type* text, token_type result)
			{
				for (std::size_t i = 0; text[i] != '\0'; ++i)
				{
					if (text[i] != char_traits::to_char_type(current))
					{
						return token_type::parse_error;
					}
					read_next();
				}
				return result;
			}

			token_type scan_string()
			{
				if (current == '\"')
				{
					string_buffer.clear();

					while (true)
					{
						const auto ch = read_next();
						if (ch == '\"')
						{
							// skip last \"
							read_next();
							break;
						}

						if (ch == '\0' || ch == char_traits::eof())
							return token_type::parse_error;

						string_buffer.push_back(char_traits::to_char_type(ch));
					}
					return token_type::value_string;
				}
				return token_type::parse_error;
			}

			token_type scan_number()
			{
				is_negative = false;
				number_value = static_cast<float_type>(0.0);

				if (current == '-')
				{
					return scan_negative();
				}

				if (current == '0')
				{
					return scan_zero();
				}

				return scan_integer();
			}

			token_type scan_negative()
			{
				if (current == '-')
				{
					is_negative = true;
					read_next();

					return scan_integer();
				}
				return token_type::parse_error;
			}

			token_type scan_zero()
			{
				if (current == '0')
				{
					if (read_next() == '.')
						return scan_float();
					else
						return token_type::value_integer;
				}
				return token_type::parse_error;
			}

			token_type scan_integer()
			{
				if (std::isdigit(current))
				{
					number_value = static_cast<float_type>(current - '0');

					while (true)
					{
						const auto ch = read_next();
						if (ch == '.')
							return scan_float();

						if (ch == 'e' || ch == 'E')
							return scan_exponent();

						if (std::isdigit(ch))
							number_value = number_value * 10 + (ch - '0');
						else
							break;
					}
					return token_type::value_integer;
				}
				return token_type::parse_error;
			}

			token_type scan_float()
			{
				if (current != '.')
					return token_type::parse_error;

				if (std::isdigit(read_next()))
				{
					float_type fraction = static_cast<float_type>(0.1);
					number_value += static_cast<float_type>(current - '0') * fraction;

					while (true)
					{
						const auto ch = read_next();
						if (ch == 'e' || ch == 'E')
							return scan_exponent();

						if (std::isdigit(ch))
						{
							fraction *= static_cast<float_type>(0.1);
							number_value += static_cast<float_type>(ch - '0') * fraction;
						}
						else
							break;
					}
					return token_type::value_float;
				}
				return token_type::parse_error;
			}

			token_type scan_exponent()
			{
				if (current != 'e' && current != 'E')
					return token_type::parse_error;

				// skip current char
				read_next();

				if ((std::isdigit(current) && current != '0') || (current == '-') || (current == '+'))
				{
					float_type base = 10;
					if (current == '+')
					{
						read_next();
					}
					else if (current == '-')
					{
						base = static_cast<float_type>(0.1);
						read_next();
					}

					unsigned int exponent = static_cast<unsigned int>(current - '0');
					while (std::isdigit(read_next()))
					{
						exponent = (exponent * 10) + static_cast<unsigned int>(current - '0');
					}

					float_type power = 1;
					for (; exponent; exponent >>= 1, base *= base)
						if (exponent & 1)
							power *= base;

					number_value *= power;
					return token_type::value_float;
				}
				return token_type::parse_error;
			}

			integer_type token_to_integer() const
			{
				integer_type integer = static_cast<integer_type>(number_value);
				return is_negative ? -integer : integer;
			}

			float_type token_to_float() const
			{
				return is_negative ? -number_value : number_value;
			}

			string_type token_to_string() const
			{
				return string_buffer;
			}

		private:
			input_adapter<char_type>* adapter;
			typename char_traits::int_type current;

			bool is_negative;
			float_type number_value;
			string_type string_buffer;
		};

		struct json_parser
		{
			using char_type = typename string_type::value_type;
			using char_traits = std::char_traits<char_type>;
			using token_type = typename json_lexer::token_type;

			json_parser(input_adapter<char_type>* adapter) : lexer(adapter), last_token(token_type::uninitialized) {}

			void parse(basic_json& json)
			{
				parse_value(json);

				if (get_token() != token_type::end_of_input)
					throw json_parse_error();
			}

		private:
			token_type get_token()
			{
				last_token = lexer.scan();
				return last_token;
			}

			void parse_value(basic_json& json)
			{
				switch (get_token())
				{
				case token_type::literal_true:
					json = JsonType::Boolean;
					json.value_.data.boolean = true;
					break;

				case token_type::literal_false:
					json = JsonType::Boolean;
					json.value_.data.boolean = false;
					break;

				case token_type::literal_null:
					json = JsonType::Null;
					break;

				case token_type::value_string:
					json = lexer.token_to_string();
					break;

				case token_type::value_integer:
					json = lexer.token_to_integer();
					break;

				case token_type::value_float:
					json = lexer.token_to_float();
					break;

				case token_type::begin_array:
					json = JsonType::Array;
					while (true)
					{
						json.value_.data.vector->push_back(basic_json());
						parse_value(json.value_.data.vector->back());

						// read ','
						if (get_token() != token_type::value_separator)
							break;
					}
					if (last_token != token_type::end_array)
						throw json_parse_error();
					break;

				case token_type::begin_object:
					json = JsonType::Object;
					while (true)
					{
						if (get_token() != token_type::value_string)
							break;
						string_type key = lexer.token_to_string();

						if (get_token() != token_type::name_separator)
							break;

						basic_json object;
						parse_value(object);
						json.value_.data.object->insert(std::make_pair(key, object));

						// read ','
						if (get_token() != token_type::value_separator)
							break;
					}
					if (last_token != token_type::end_object)
						throw json_parse_error();
					break;

				default:
					// unexpected token
					throw json_parse_error();
					break;
				}
			}

		private:
			json_lexer lexer;
			token_type last_token;
		};

		friend struct json_parser;

	private:
		JsonValue value_;
	};

#undef E2D_DECLARE_BASIC_JSON_TEMPLATE
#undef E2D_DECLARE_BASIC_JSON_TPL_ARGS

}

namespace std
{
	template<>
	struct hash<::easy2d::Json>
	{
		std::size_t operator()(const ::easy2d::Json& json) const
		{
			return hash<::easy2d::Json::string_type>{}(json.dump());
		}
	};

	template<>
	inline void swap<::easy2d::Json>(::easy2d::Json& lhs, ::easy2d::Json& rhs)
	{
		lhs.swap(rhs);
	}
}
