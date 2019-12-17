// Copyright (c) 2019-2020 OuterC - Nomango

#pragma once
#include <iosfwd>

namespace oc
{
namespace __json_detail
{

template <typename _CharTy>
struct output_adapter
{
	using char_type = _CharTy;
	using char_traits = std::char_traits<char_type>;

	virtual void write(const _CharTy ch) = 0;
	virtual void write(const _CharTy* str, uint32_t size) = 0;
	virtual void write(const _CharTy* str)
	{
		const auto size = char_traits::length(str);
		write(str, static_cast<uint32_t>(size));
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

	virtual void write(const char_type* str, uint32_t size) override
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

	virtual void write(const char_type* str, uint32_t size) override
	{
		stream_.write(str, static_cast<size_type>(size));
	}

private:
	std::basic_ostream<char_type>& stream_;
};

} // namespace __json_detail

} // namespace oc
