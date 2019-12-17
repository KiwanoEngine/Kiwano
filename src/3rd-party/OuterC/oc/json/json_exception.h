// Copyright (c) 2019-2020 OuterC - Nomango

#pragma once
#include <stdexcept>
#include "json_value.h"

namespace oc
{

class json_exception
	: public std::runtime_error
{
public:
	json_exception(const char* message)
		: std::runtime_error(message)
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

} // namespace oc
