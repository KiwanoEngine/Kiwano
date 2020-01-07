// Copyright (c) 2019-2020 OuterC - Nomango

#pragma once
#include "json_output_adapter.h"
#include <array>
#include <memory>

namespace oc
{

namespace __json_detail
{

template <typename _CharTy>
struct json_literaler
{
	using char_type		= _CharTy;
	using string_type	= const char_type*;

	static inline string_type value_separator()		{ return ",\n"; }

	static inline string_type empty_object()		{ return "{}"; }
	static inline string_type object_begin()		{ return "{\n"; }
	static inline string_type object_key_begin()	{ return "\": "; }

	static inline string_type empty_array()			{ return "[]"; }
	static inline string_type array_begin()			{ return "[\n"; }

	static inline string_type literal_true()		{ return "true"; }
	static inline string_type literal_false()		{ return "false"; }
	static inline string_type literal_null()		{ return "null"; }

	static inline string_type escape_t()			{ return "\\t"; }
	static inline string_type escape_r()			{ return "\\r"; }
	static inline string_type escape_n()			{ return "\\n"; }
	static inline string_type escape_b()			{ return "\\b"; }
	static inline string_type escape_f()			{ return "\\f"; }
	static inline string_type escape_quote()		{ return "\\\""; }
	static inline string_type escape_slash()		{ return "\\\\"; }

	template <typename _FloatTy>
	static inline void sprint_float(char_type* buff, const _FloatTy value)
	{
		const auto digits = std::numeric_limits<_FloatTy>::max_digits10;
		const auto len = ::_scprintf("%.*g", digits, value);
		if (len)
		{
			buff[0] = '\0';
			::sprintf_s(buff, size_t(len) + 1, "%.*g", digits, value);
		}
		else
		{
			buff[0] = '0';
			buff[1] = '.';
			buff[2] = '0';
			buff[3] = '\0';
		}
	}
};

template <>
struct json_literaler<wchar_t>
{
	using char_type		= wchar_t;
	using string_type	= const char_type*;

	static inline string_type value_separator()		{ return L",\n"; }

	static inline string_type empty_object()		{ return L"{}"; }
	static inline string_type object_begin()		{ return L"{\n"; }
	static inline string_type object_key_begin()	{ return L"\":"; }

	static inline string_type empty_array()			{ return L"[]"; }
	static inline string_type array_begin()			{ return L"[\n"; }

	static inline string_type literal_true()		{ return L"true"; }
	static inline string_type literal_false()		{ return L"false"; }
	static inline string_type literal_null()		{ return L"null"; }

	static inline string_type escape_t()			{ return L"\\t"; }
	static inline string_type escape_r()			{ return L"\\r"; }
	static inline string_type escape_n()			{ return L"\\n"; }
	static inline string_type escape_b()			{ return L"\\b"; }
	static inline string_type escape_f()			{ return L"\\f"; }
	static inline string_type escape_quote()		{ return L"\\\""; }
	static inline string_type escape_slash()		{ return L"\\\\"; }

	template <typename _FloatTy>
	static inline void sprint_float(char_type* buff, const _FloatTy value)
	{
		const auto digits = std::numeric_limits<_FloatTy>::max_digits10;
		const auto len = ::_scwprintf(L"%.*g", digits, value);
		if (len)
		{
			buff[0] = '\0';
			::swprintf_s(buff, size_t(len) + 1, L"%.*g", digits, value);
		}
		else
		{
			buff[0] = '0';
			buff[1] = '.';
			buff[2] = '0';
			buff[3] = '\0';
		}
	}
};

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
	using literaler		= json_literaler<char_type>;

	json_serializer(output_adapter<char_type>* out, const char_type indent_char)
		: out(out)
		, indent_char(indent_char)
		, indent_string(32, indent_char)
	{}

	void dump(
		const _BasicJsonTy& json,
		const bool pretty_print,
		const bool char_escape,
		const uint32_t indent_step,
		const uint32_t current_indent = 0)
	{
		switch (json.type())
		{
		case json_type::OBJECT:
		{
			auto& object = *json.value_.data.object;

			if (object.empty())
			{
				out->write(literaler::empty_object());
				return;
			}

			if (pretty_print)
			{
				out->write(literaler::object_begin());

				const auto new_indent = current_indent + indent_step;
				if (indent_string.size() < new_indent)
				{
					indent_string.resize(indent_string.size() * 2, indent_char);
				}

				auto iter = object.cbegin();
				const auto size = object.size();
				for (uint32_t i = 0; i < size; ++i, ++iter)
				{
					out->write(indent_string.c_str(), new_indent);
					out->write('\"');
					out->write(iter->first.c_str());
					out->write(literaler::object_key_begin());
					out->write(' ');
					dump(iter->second, pretty_print, char_escape, indent_step, new_indent);

					// not last element
					if (i != size - 1)
						out->write(literaler::value_separator());
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
				for (uint32_t i = 0; i < size; ++i, ++iter)
				{
					out->write('\"');
					out->write(iter->first.c_str());
					out->write(literaler::object_key_begin());
					dump(iter->second, pretty_print, char_escape, indent_step, current_indent);

					// not last element
					if (i != size - 1)
						out->write(',');
				}

				out->write('}');
			}

			return;
		}

		case json_type::VECTOR:
		{
			auto& vector = *json.value_.data.vector;

			if (vector.empty())
			{
				out->write(literaler::empty_array());
				return;
			}

			if (pretty_print)
			{
				out->write(literaler::array_begin());

				const auto new_indent = current_indent + indent_step;
				if (indent_string.size() < new_indent)
				{
					indent_string.resize(indent_string.size() * 2, indent_char);
				}

				auto iter = vector.cbegin();
				const auto size = vector.size();
				for (uint32_t i = 0; i < size; ++i, ++iter)
				{
					out->write(indent_string.c_str(), new_indent);
					dump(*iter, pretty_print, char_escape, indent_step, new_indent);

					// not last element
					if (i != size - 1)
						out->write(literaler::value_separator());
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
				for (uint32_t i = 0; i < size; ++i, ++iter)
				{
					dump(*iter, pretty_print, char_escape, indent_step, current_indent);
					// not last element
					if (i != size - 1)
						out->write(',');
				}

				out->write(']');
			}

			return;
		}

		case json_type::STRING:
		{
			out->write('\"');
			dump_string(*json.value_.data.string, char_escape);
			out->write('\"');
			return;
		}

		case json_type::BOOL:
		{
			if (json.value_.data.boolean)
			{
				out->write(literaler::literal_true());
			}
			else
			{
				out->write(literaler::literal_false());
			}
			return;
		}

		case json_type::INTEGER:
		{
			dump_integer(json.value_.data.number_integer);
			return;
		}

		case json_type::FLOAT:
		{
			dump_float(json.value_.data.number_float);
			return;
		}

		case json_type::NIL:
		{
			out->write(literaler::literal_null());
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

		if (number_buffer == nullptr)
			number_buffer.reset(new number_buffer_type);

		auto next = (*number_buffer).rbegin();
		*next = '\0';

		do
		{
			*(++next) = static_cast<char_type>('0' + uval % 10);
			uval /= 10;
		} while (uval != 0);

		if (val < 0)
			*(++next) = '-';

		out->write(&(*next));
	}

	void dump_float(float_type val)
	{
		if (number_buffer == nullptr)
			number_buffer.reset(new number_buffer_type);

		literaler::sprint_float((*number_buffer).data(), val);
		out->write((*number_buffer).data());
	}

	void dump_string(const string_type & val, const bool char_escape)
	{
		for (const auto& ch : val)
		{
			switch (ch)
			{
			case '\t':
			{
				out->write(literaler::escape_t());
				break;
			}

			case '\r':
			{
				out->write(literaler::escape_r());
				break;
			}

			case '\n':
			{
				out->write(literaler::escape_n());
				break;
			}

			case '\b':
			{
				out->write(literaler::escape_b());
				break;
			}

			case '\f':
			{
				out->write(literaler::escape_f());
				break;
			}

			case '\"':
			{
				out->write(literaler::escape_quote());
				break;
			}

			case '\\':
			{
				out->write(literaler::escape_slash());
				break;
			}

			default:
			{
				uint32_t char_byte = static_cast<uint32_t>(ch);
				if (char_byte > 0x1F && (!char_escape || char_byte < 0x7F))
				{
					out->write(ch);
				}
				else
				{
					char_type escaped[7] = { '\\', 'u', 0 };

					uint8_t index = 2;
					for (const auto factor : { 12, 8, 4, 0 })
					{
						char_type code = ((char_byte >> factor) & 0x0F);
						code += (code < 0x0A) ? '0' : 'a' - 0x0A;
						escaped[index++] = code;
					}
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

	using number_buffer_type = std::array<char_type, 21>;
	std::unique_ptr<number_buffer_type> number_buffer;
};

} // namespace __json_detail

} // namespace oc
