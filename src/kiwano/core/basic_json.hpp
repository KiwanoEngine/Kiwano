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
#include <cstdint>
#include <cctype>
#include <memory>
#include <array>
#include <iosfwd>
#include <stdexcept>

namespace kiwano
{

inline namespace core
{

//
// details of basic_json
//

#define KGE_DECLARE_BASIC_JSON_TEMPLATE\
	template <\
	template <class _Kty, class _Ty, class ..._Args> typename _ObjectTy, \
	template <class _Kty, class ..._Args> typename _ArrayTy, \
	typename _StringTy, \
	typename _IntegerTy, \
	typename _FloatTy, \
	typename _BooleanTy, \
	template <class _Ty> typename _Allocator>

#define KGE_DECLARE_BASIC_JSON_TPL_ARGS \
	_ObjectTy, _ArrayTy, _StringTy, _IntegerTy, _FloatTy, _BooleanTy, _Allocator

	
KGE_DECLARE_BASIC_JSON_TEMPLATE
class basic_json;

//
// json value type
//

enum class JsonType
{
	Integer,
	Float,
	String,
	Vector,
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

KGE_DECLARE_BASIC_JSON_TEMPLATE
struct is_basic_json< basic_json<KGE_DECLARE_BASIC_JSON_TPL_ARGS> >
	: ::std::true_type
{
};

//
// exceptions
//

class json_exception
	: public ::std::runtime_error
{
public:
	json_exception(const char* message)
		: ::std::runtime_error(message)
	{}
};

class json_type_error
	: public json_exception
{
public:
	json_type_error(const char* message) : json_exception(message) {}
};

class json_invalid_key
	: public json_exception
{
public:
	json_invalid_key(const char* message) : json_exception(message) {}
};

class json_invalid_iterator
	: public json_exception
{
public:
	json_invalid_iterator(const char* message) : json_exception(message) {}
};

class json_parse_error
	: public json_exception
{
public:
	json_parse_error(const char* message) : json_exception(message) {}
};


namespace __json_detail
{
	//
	// json_value
	//

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

		json_value()
		{
			type = JsonType::Null;
			data.object = nullptr;
		}

		json_value(::std::nullptr_t)
		{
			type = JsonType::Null;
			data.object = nullptr;
		}

		json_value(const object_type& value)
		{
			type = JsonType::Object;
			data.object = create<object_type>(value);
		}

		json_value(const array_type& value)
		{
			type = JsonType::Vector;
			data.vector = create<array_type>(value);
		}

		json_value(const string_type& value)
		{
			type = JsonType::String;
			data.string = create<string_type>(value);
		}

		template <typename _CharT>
		json_value(const _CharT* str)
		{
			type = JsonType::String;
			data.string = create<string_type>(str);
		}

		json_value(const integer_type value)
		{
			type = JsonType::Integer;
			data.number_integer = value;
		}

		json_value(const float_type value)
		{
			type = JsonType::Float;
			data.number_float = value;
		}

		json_value(const boolean_type value)
		{
			type = JsonType::Boolean;
			data.boolean = value;
		}

		json_value(const JsonType value_type)
		{
			type = value_type;
			switch (type)
			{
			case JsonType::Object:
				data.object = create<object_type>();
				break;
			case JsonType::Vector:
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

		json_value(json_value const& other)
		{
			type = other.type;
				
			switch (other.type)
			{
			case JsonType::Object:
				data.object = create<object_type>(*other.data.object);
				break;
			case JsonType::Vector:
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

		json_value(json_value&& other) noexcept
		{
			type = other.type;
			data = other.data;
			other.type = JsonType::Null;
			other.data.object = nullptr;
		}

		~json_value()
		{
			clear();
		}

		void swap(json_value& other)
		{
			::std::swap(type, other.type);
			::std::swap(data, other.data);
		}

		void clear()
		{
			switch (type)
			{
			case JsonType::Object:
				destroy<object_type>(data.object);
				break;
			case JsonType::Vector:
				destroy<array_type>(data.vector);
				break;
			case JsonType::String:
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
			using allocator_traits = ::std::allocator_traits<allocator_type>;

			allocator_type allocator;
			_Ty* ptr = allocator_traits::allocate(allocator, 1);
			allocator_traits::construct(allocator, ptr, ::std::forward<_Args>(args)...);
			return ptr;
		}

		template <typename _Ty>
		inline void destroy(_Ty* ptr)
		{
			using allocator_type = typename _BasicJsonTy::template allocator_type<_Ty>;
			using allocator_traits = ::std::allocator_traits<allocator_type>;

			allocator_type allocator;
			allocator_traits::destroy(allocator, ptr);
			allocator_traits::deallocate(allocator, ptr, 1);
		}

		inline json_value& operator=(json_value const& other)
		{
			json_value{ other }.swap(*this);
			return (*this);
		}

		inline json_value& operator=(json_value && other) noexcept
		{
			clear();
			type = other.type;
			data = ::std::move(other.data);
			// invalidate payload
			other.type = JsonType::Null;
			other.data.object = nullptr;
			return (*this);
		}
	};
} // end of namespace __json_detail

namespace __json_detail
{
	//
	// iterator for basic_json
	//

	struct primitive_iterator
	{
		using difference_type = ::std::ptrdiff_t;

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
		using difference_type	= ::std::ptrdiff_t;
		using iterator_category = ::std::bidirectional_iterator_tag;
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
				case JsonType::Object:
					object_iter = rhs.object_iter;
					break;
				case JsonType::Vector:
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
			case JsonType::Object:
				return (object_iter->second);
			case JsonType::Vector:
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
			case JsonType::Object:
				return &(object_iter->second);
			case JsonType::Vector:
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
				case JsonType::Object:
				{
					object_iter = data_->value_.data.object->begin();
					break;
				}
				case JsonType::Vector:
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
				case JsonType::Object:
				{
					object_iter = data_->value_.data.object->end();
					break;
				}
				case JsonType::Vector:
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
				case JsonType::Object:
				{
					::std::advance(object_iter, 1);
					break;
				}
				case JsonType::Vector:
				{
					::std::advance(array_iter, 1);
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
				case JsonType::Object:
				{
					::std::advance(object_iter, -1);
					break;
				}
				case JsonType::Vector:
				{
					::std::advance(array_iter, -1);
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
				case JsonType::Object:
				{
					throw json_invalid_iterator("cannot use offsets with object type");
					break;
				}
				case JsonType::Vector:
				{
					::std::advance(array_iter, off);
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
				case JsonType::Object:
				{
					return object_iter == other.object_iter;
				}
				case JsonType::Vector:
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
			case JsonType::Object:
				throw json_invalid_iterator("cannot compare iterators with object type");
			case JsonType::Vector:
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
			case JsonType::Object:
				if (object_iter == data_->value_.data.object->end())
				{
					throw ::std::out_of_range("iterator out of range");
				}
				break;
			case JsonType::Vector:
				if (array_iter == data_->value_.data.vector->end())
				{
					throw ::std::out_of_range("iterator out of range");
				}
				break;
			default:
				if (primitive_iter == 1)
				{
					throw ::std::out_of_range("iterator out of range");
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
} // end of namespace __json_detail

namespace __json_detail
{
	//
	// output_adapter
	//

	template <typename _CharTy>
	struct output_adapter
	{
		using char_type = _CharTy;
		using char_traits = ::std::char_traits<char_type>;

		virtual void write(const _CharTy ch) = 0;
		virtual void write(const _CharTy* str, ::std::size_t size) = 0;
		virtual void write(const _CharTy* str)
		{
			const auto size = char_traits::length(str);
			write(str, static_cast<::std::size_t>(size));
		}
	};

	template <typename _StringTy>
	struct string_output_adapter
		: public output_adapter<typename _StringTy::value_type>
	{
		using char_type = typename _StringTy::value_type;
		using size_type = typename _StringTy::size_type;
		using char_traits = ::std::char_traits<char_type>;

		string_output_adapter(_StringTy& str) : str_(str) {}

		virtual void write(const char_type ch) override
		{
			str_.push_back(ch);
		}

		virtual void write(const char_type* str, ::std::size_t size) override
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
		using size_type = typename ::std::streamsize;
		using char_traits = ::std::char_traits<char_type>;

		stream_output_adapter(::std::basic_ostream<char_type>& stream) : stream_(stream) {}

		virtual void write(const char_type ch) override
		{
			stream_.put(ch);
		}

		virtual void write(const char_type* str, ::std::size_t size) override
		{
			stream_.write(str, static_cast<size_type>(size));
		}

	private:
		::std::basic_ostream<char_type>& stream_;
	};
} // end of namespace __json_detail

namespace __json_detail
{
	//
	// json_serializer
	//

	template <typename _BasicJsonTy>
	struct json_serializer
	{
		using string_type	= typename _BasicJsonTy::string_type;
		using char_type		= typename _BasicJsonTy::char_type;
		using integer_type	= typename _BasicJsonTy::integer_type;
		using float_type	= typename _BasicJsonTy::float_type;
		using boolean_type	= typename _BasicJsonTy::boolean_type;
		using array_type	= typename _BasicJsonTy::array_type;
		using object_type	= typename _BasicJsonTy::object_type;

		json_serializer(output_adapter<char_type>* out, const wchar_t indent_char)
			: out(out)
			, indent_char(indent_char)
			, indent_string(32, indent_char)
			, number_buffer()
		{}

		void dump(
			const _BasicJsonTy& json,
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
					for (::std::size_t i = 0; i < size; ++i, ++iter)
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
					for (::std::size_t i = 0; i < size; ++i, ++iter)
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

			case JsonType::Vector:
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
					for (::std::size_t i = 0; i < size; ++i, ++iter)
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
					for (::std::size_t i = 0; i < size; ++i, ++iter)
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
				dump_string(*json.value_.data.string);
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

			auto uval = static_cast<::std::make_unsigned_t<integer_type>>(val);

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
			const auto digits = ::std::numeric_limits<float_type>::max_digits10;
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

		void dump_string(const string_type & val)
		{
			for (const auto& ch : val)
			{
				switch (ch)
				{
				case '\t':
				{
					out->write(L"\\t");
					break;
				}

				case '\r':
				{
					out->write(L"\\r");
					break;
				}

				case '\n':
				{
					out->write(L"\\n");
					break;
				}

				case '\b':
				{
					out->write(L"\\b");
					break;
				}

				case '\f':
				{
					out->write(L"\\f");
					break;
				}

				case '\"':
				{
					out->write(L"\\\"");
					break;
				}

				case '\\':
				{
					out->write(L"\\\\");
					break;
				}

				default:
				{
					const auto char_byte = static_cast<uint16_t>(ch);
					if ((char_byte > 0x1F) && (char_byte < 0x7F))
					{
						out->write(ch);
					}
					else
					{
						wchar_t escaped[7] = { 0 };
						::swprintf_s(escaped, 7, L"\\u%04x", char_byte);
						out->write(escaped);
					}
					break;
				}
				}
			}
		}

	private:
		output_adapter<char_type>* out;
		char_type indent_char;
		string_type indent_string;
		::std::array<char_type, 21> number_buffer;
	};
} // end of namespace __json_detail

namespace __json_detail
{
	//
	// input_adapter
	//

	template <typename _CharTy>
	struct input_adapter
	{
		using char_type = _CharTy;
		using char_traits = ::std::char_traits<char_type>;

		virtual typename char_traits::int_type get_char() = 0;
		virtual ~input_adapter() = default;
	};

	template <typename _CharTy>
	struct file_input_adapter
		: public input_adapter<_CharTy>
	{
		using char_type = typename input_adapter<_CharTy>::char_type;
		using char_traits = typename input_adapter<_CharTy>::char_traits;

		file_input_adapter(::std::FILE* file) : file(file) {}

		virtual typename char_traits::int_type get_char() override
		{
			return ::std::fgetc(file);
		}

	private:
		::std::FILE* file;
	};

	template <typename _CharTy>
	struct stream_input_adapter
		: public input_adapter<_CharTy>
	{
		using char_type = typename input_adapter<_CharTy>::char_type;
		using char_traits = typename input_adapter<_CharTy>::char_traits;

		stream_input_adapter(::std::basic_istream<char_type>& stream) : stream(stream), streambuf(*stream.rdbuf()) {}

		virtual typename char_traits::int_type get_char() override
		{
			auto ch = streambuf.sbumpc();
			if (ch == EOF)
			{
				stream.clear(stream.rdstate() | ::std::ios::eofbit);
			}
			return ch;
		}

		virtual ~stream_input_adapter()
		{
			stream.clear(stream.rdstate() & ::std::ios::eofbit);
		}

	private:
		::std::basic_istream<char_type>& stream;
		::std::basic_streambuf<char_type>& streambuf;
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
		::std::size_t index;
	};
} // end of namespace __json_detail

namespace __json_detail
{
	//
	// json_lexer & json_parser
	//

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

	template <typename _BasicJsonTy>
	struct json_lexer
	{
		using string_type	= typename _BasicJsonTy::string_type;
		using char_type		= typename _BasicJsonTy::char_type;
		using integer_type	= typename _BasicJsonTy::integer_type;
		using float_type	= typename _BasicJsonTy::float_type;
		using boolean_type	= typename _BasicJsonTy::boolean_type;
		using array_type	= typename _BasicJsonTy::array_type;
		using object_type	= typename _BasicJsonTy::object_type;
		using char_traits	= ::std::char_traits<char_type>;

		json_lexer(input_adapter<char_type>* adapter) : adapter(adapter)
		{
			// read first char
			read_next();
		}

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
			for (::std::size_t i = 0; text[i] != '\0'; ++i)
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
			if (current != '\"')
				return token_type::parse_error;

			string_buffer.clear();

			while (true)
			{
				const auto ch = read_next();
				switch (ch)
				{
				case char_traits::eof():
				{
					// unexpected end
					return token_type::parse_error;
				}

				case '\"':
				{
					// skip last `\"`
					read_next();
					return token_type::value_string;
				}

				case 0x00:
				case 0x01:
				case 0x02:
				case 0x03:
				case 0x04:
				case 0x05:
				case 0x06:
				case 0x07:
				case 0x08:
				case 0x09:
				case 0x0A:
				case 0x0B:
				case 0x0C:
				case 0x0D:
				case 0x0E:
				case 0x0F:
				case 0x10:
				case 0x11:
				case 0x12:
				case 0x13:
				case 0x14:
				case 0x15:
				case 0x16:
				case 0x17:
				case 0x18:
				case 0x19:
				case 0x1A:
				case 0x1B:
				case 0x1C:
				case 0x1D:
				case 0x1E:
				case 0x1F:
				{
					// invalid control character
					return token_type::parse_error;
				}

				case '\\':
				{
					switch (read_next())
					{
					case '\"':
						string_buffer.push_back('\"');
						break;
					case '\\':
						string_buffer.push_back('\\');
						break;
					case '/':
						string_buffer.push_back('/');
						break;
					case 'b':
						string_buffer.push_back('\b');
						break;
					case 'f':
						string_buffer.push_back('\f');
						break;
					case 'n':
						string_buffer.push_back('\n');
						break;
					case 'r':
						string_buffer.push_back('\r');
						break;
					case 't':
						string_buffer.push_back('\t');
						break;

					case 'u':
					{
						// unicode escapes
						uint16_t byte = 0;

						for (const auto factor : { 12, 8, 4, 0 })
						{
							const auto n = read_next();
							if (n >= L'0' && n <= L'9')
							{
								byte += ((n - L'0') << factor);
							}
							else if (n >= L'A' && n <= L'F')
							{
								byte += ((n - L'A' + 10) << factor);
							}
							else if (n >= L'a' && n <= L'f')
							{
								byte += ((n - L'a' + 10) << factor);
							}
							else
							{
								// '\u' must be followed by 4 hex digits
								return token_type::parse_error;
							}
						}

						string_buffer.push_back(char_traits::to_char_type(byte));
						break;
					}

					default:
					{
						return token_type::parse_error;
					}
					}
					break;
				}

				default:
				{
					if (ch > 0x1F && ch < 0x7F)
					{
						string_buffer.push_back(char_traits::to_char_type(ch));
						break;
					}
					else
					{
						return token_type::parse_error;
					}
				}

				}
			}
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
			if (::std::isdigit(current))
			{
				number_value = static_cast<float_type>(current - '0');

				while (true)
				{
					const auto ch = read_next();
					if (ch == '.')
						return scan_float();

					if (ch == 'e' || ch == 'E')
						return scan_exponent();

					if (::std::isdigit(ch))
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

			if (::std::isdigit(read_next()))
			{
				float_type fraction = static_cast<float_type>(0.1);
				number_value += static_cast<float_type>(current - '0') * fraction;

				while (true)
				{
					const auto ch = read_next();
					if (ch == 'e' || ch == 'E')
						return scan_exponent();

					if (::std::isdigit(ch))
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

			if ((::std::isdigit(current) && current != '0') || (current == '-') || (current == '+'))
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
				while (::std::isdigit(read_next()))
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


	template <typename _BasicJsonTy>
	struct json_parser
	{
		using string_type	= typename _BasicJsonTy::string_type;
		using char_type		= typename _BasicJsonTy::char_type;
		using integer_type	= typename _BasicJsonTy::integer_type;
		using float_type	= typename _BasicJsonTy::float_type;
		using boolean_type	= typename _BasicJsonTy::boolean_type;
		using array_type	= typename _BasicJsonTy::array_type;
		using object_type	= typename _BasicJsonTy::object_type;
		using char_traits	= ::std::char_traits<char_type>;

		json_parser(input_adapter<char_type>* adapter)
			: lexer(adapter)
			, last_token(token_type::uninitialized)
		{}

		void parse(_BasicJsonTy& json)
		{
			parse_value(json);

			if (get_token() != token_type::end_of_input)
				throw json_parse_error("unexpected token, expect end");
		}

	private:
		token_type get_token()
		{
			last_token = lexer.scan();
			return last_token;
		}

		void parse_value(_BasicJsonTy& json)
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
				json = JsonType::Vector;
				while (true)
				{
					json.value_.data.vector->push_back(_BasicJsonTy());
					parse_value(json.value_.data.vector->back());

					// read ','
					if (get_token() != token_type::value_separator)
						break;
				}
				if (last_token != token_type::end_array)
					throw json_parse_error("unexpected token in array");
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

					_BasicJsonTy object;
					parse_value(object);
					json.value_.data.object->insert(::std::make_pair(key, object));

					// read ','
					if (get_token() != token_type::value_separator)
						break;
				}
				if (last_token != token_type::end_object)
					throw json_parse_error("unexpected token in object");
				break;

			default:
				// unexpected token
				throw json_parse_error("unexpected token");
				break;
			}
		}

	private:
		json_lexer<_BasicJsonTy> lexer;
		token_type last_token;
	};
} // end of namespace __json_detail

namespace __json_detail
{
	//
	// json_value_getter
	//

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
			typename ::std::enable_if<::std::is_integral<_IntegerTy>::value, int>::type = 0>
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
			typename ::std::enable_if<::std::is_floating_point<_FloatingTy>::value, int>::type = 0>
		static inline void assign(const _BasicJsonTy& json, _FloatingTy& value)
		{
			if (!json.is_float()) throw json_type_error("json value type must be float");
			value = static_cast<_FloatingTy>(json.value_.data.number_float);
		}
	};
} // end of namespace __json_detail

//
// basic_json
//

KGE_DECLARE_BASIC_JSON_TEMPLATE
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
	using size_type					= ::std::size_t;
	using difference_type			= ::std::ptrdiff_t;
	using string_type				= _StringTy;
	using char_type					= typename _StringTy::value_type;
	using integer_type				= _IntegerTy;
	using float_type				= _FloatTy;
	using boolean_type				= _BooleanTy;
	using array_type				= typename _ArrayTy<basic_json, allocator_type<basic_json>>;
	using object_type				= typename _ObjectTy<string_type, basic_json, ::std::less<string_type>, allocator_type<::std::pair<const string_type, basic_json>>>;
	using initializer_list			= ::std::initializer_list<basic_json>;

	using iterator					= __json_detail::iterator_impl<basic_json>;
	using const_iterator			= __json_detail::iterator_impl<const basic_json>;
	using reverse_iterator			= ::std::reverse_iterator<iterator>;
	using const_reverse_iterator	= ::std::reverse_iterator<const_iterator>;

public:
	basic_json() {}

	basic_json(::std::nullptr_t) {}

	basic_json(const JsonType type) : value_(type) {}

	basic_json(basic_json const& other) : value_(other.value_) {}

	basic_json(basic_json&& other) noexcept : value_(::std::move(other.value_))
	{
		// invalidate payload
		other.value_.type = JsonType::Null;
		other.value_.data.object = nullptr;
	}

	basic_json(string_type const& value) : value_(value) {}

	template <
		typename _CompatibleTy,
		typename ::std::enable_if<::std::is_constructible<string_type, _CompatibleTy>::value, int>::type = 0>
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

	basic_json(integer_type value)
		: value_(value)
	{
	}

	template <
		typename _IntegerTy,
		typename ::std::enable_if<::std::is_integral<_IntegerTy>::value, int>::type = 0>
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
		typename ::std::enable_if<::std::is_floating_point<_FloatingTy>::value, int>::type = 0>
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
		bool is_an_object = ::std::all_of(init_list.begin(), init_list.end(), [](const basic_json& json)
		{
			return (json.is_array() && json.size() == 2 && json[0].is_string());
		});

		if (is_an_object)
		{
			value_ = JsonType::Object;

			::std::for_each(init_list.begin(), init_list.end(), [this](const basic_json& json)
			{
				value_.data.object->emplace(
					*((*json.value_.data.vector)[0].value_.data.string),
					(*json.value_.data.vector)[1]
				);
			});
		}
		else
		{
			value_ = JsonType::Vector;
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
		json.value_ = JsonType::Object;
		json.value_.data.object->emplace(*((*init_list.begin()).value_.data.string), *(init_list.begin() + 1));
		return json;
	}

	static inline basic_json array(initializer_list const& init_list)
	{
		basic_json json;
		json.value_ = JsonType::Vector;

		if (init_list.size())
		{
			json.value_.data.vector->reserve(init_list.size());
			json.value_.data.vector->assign(init_list.begin(), init_list.end());
		}
		return json;
	}

	inline bool is_object() const				{ return value_.type == JsonType::Object; }

	inline bool is_array() const				{ return value_.type == JsonType::Vector; }

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
		case JsonType::Vector:
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
		case JsonType::Null:
			return 0;
		case JsonType::Vector:
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

	template <typename _Kty>
	inline const_iterator find(_Kty && key) const
	{
		if (is_object())
		{
			const_iterator iter;
			iter.object_iter = value_.data.object->find(::std::forward<_Kty>(key));
			return iter;
		}
		return cend();
	}

	template <typename _Kty>
	inline size_type count(_Kty && key) const
	{
		return is_object() ? value_.data.object->count(::std::forward<_Kty>(key)) : 0;
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
		typename ::std::enable_if<
			::std::is_same<_IteratorTy, iterator>::value ||
			::std::is_same<_IteratorTy, const_iterator>::value, int
		>::type = 0>
	inline _IteratorTy erase(_IteratorTy pos)
	{
		_IteratorTy result = end();

		switch (type())
		{
		case JsonType::Object:
		{
			result.object_iter = value_.data.object->erase(pos.object_iter);
			break;
		}

		case JsonType::Vector:
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
		typename ::std::enable_if<
			::std::is_same<_IteratorTy, iterator>::value ||
			::std::is_same<_IteratorTy, const_iterator>::value, int
		>::type = 0>
	inline _IteratorTy erase(_IteratorTy first, _IteratorTy last)
	{
		_IteratorTy result = end();

		switch (type())
		{
		case JsonType::Object:
		{
			result.object_iter = value_.data.object->erase(first.object_iter, last.object_iter);
			break;
		}

		case JsonType::Vector:
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
			value_ = JsonType::Vector;
		}

		value_.data.vector->push_back(::std::move(json));
	}

	inline basic_json& operator+=(basic_json&& json)
	{
		push_back(::std::move(json));
		return (*this);
	}

	inline void clear()
	{
		switch (type())
		{
		case JsonType::Integer:
		{
			value_.data.number_integer = 0;
			break;
		}

		case JsonType::Float:
		{
			value_.data.number_float = static_cast<float_type>(0.0);
			break;
		}

		case JsonType::Boolean:
		{
			value_.data.boolean = false;
			break;
		}

		case JsonType::String:
		{
			value_.data.string->clear();
			break;
		}

		case JsonType::Vector:
		{
			value_.data.vector->clear();
			break;
		}

		case JsonType::Object:
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
		typename ::std::enable_if<::std::is_integral<_IntegerTy>::value, int>::type = 0>
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
		typename ::std::enable_if<::std::is_floating_point<_FloatingTy>::value, int>::type = 0>
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
		value_ = ::std::move(other.value_);
		return (*this);
	}

	inline basic_json& operator=(::std::nullptr_t)
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
			value_ = JsonType::Vector;
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
			throw ::std::out_of_range("operator[] index out of range");
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
			throw ::std::out_of_range("operator[] key out of range");
		}
		return iter->second;
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
			throw ::std::out_of_range("operator[] key out of range");
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

	friend ::std::basic_ostream<char_type>& operator<<(::std::basic_ostream<char_type>& out, const basic_json& json)
	{
		using char_type = typename ::std::basic_ostream<char_type>::char_type;

		const bool pretty_print = (out.width() > 0);
		const auto indentation = (pretty_print ? out.width() : 0);
		out.width(0);

		__json_detail::stream_output_adapter<char_type> adapter(out);
		__json_detail::json_serializer<basic_json>(&adapter, out.fill()).dump(json, pretty_print, static_cast<unsigned int>(indentation));
		return out;
	}

	string_type dump(
		const int indent = -1,
		const char_type indent_char = ' ') const
	{
		string_type result;
		__json_detail::string_output_adapter<string_type> adapter(result);
		dump(&adapter, indent, indent_char);
		return result;
	}

	void dump(
		__json_detail::output_adapter<char_type>* adapter,
		const int indent = -1,
		const char_type indent_char = ' ') const
	{
		if (indent >= 0)
		{
			__json_detail::json_serializer<basic_json>(adapter, indent_char).dump(*this, true, static_cast<unsigned int>(indent));
		}
		else
		{
			__json_detail::json_serializer<basic_json>(adapter, indent_char).dump(*this, false, 0);
		}
	}

public:
	// parse functions

	friend ::std::basic_istream<char_type>&
		operator>>(::std::basic_istream<char_type>& in, basic_json& json)
	{
		__json_detail::stream_input_adapter<char_type> adapter(in);
		__json_detail::json_parser<basic_json>(&adapter).parse(json);
		return in;
	}

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

	static inline basic_json parse(::std::FILE* file)
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
			case JsonType::Vector:
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
			case JsonType::Vector:
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

} // inline namespace core

} // namespace kiwano

#undef KGE_DECLARE_BASIC_JSON_TEMPLATE
#undef KGE_DECLARE_BASIC_JSON_TPL_ARGS
