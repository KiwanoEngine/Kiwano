#include "..\e2dcustom.h"


e2d::Exception::Exception()
	: _message()
{
}

e2d::Exception::Exception(String message)
	: _message(message)
{
}

e2d::Exception::Exception(Exception const& other)
	: _message(other._message)
{
}

e2d::Exception& e2d::Exception::operator=(Exception const& other)
{
	if (this == &other)
	{
		return *this;
	}

	_message = other._message;
	return *this;
}

e2d::Exception::~Exception()
{
}

e2d::String e2d::Exception::msg() const
{
	return _message;
}
