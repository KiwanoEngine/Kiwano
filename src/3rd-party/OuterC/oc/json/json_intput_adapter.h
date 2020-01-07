// Copyright (c) 2019-2020 OuterC - Nomango

#pragma once
#include <iosfwd>

namespace oc
{
namespace __json_detail
{

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
	uint32_t index;
};

} // namespace __json_detail

} // namespace oc
