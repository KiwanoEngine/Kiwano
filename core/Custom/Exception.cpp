#include "..\e2dcustom.h"


e2d::Exception::Exception() throw()
	: _message()
{
}

e2d::Exception::Exception(const String& message) throw()
	: _message(message)
{
}

e2d::Exception::Exception(Exception const& other) throw()
	: _message(other._message)
{
}

e2d::Exception& e2d::Exception::operator=(Exception const& other) throw()
{
	if (this == &other)
	{
		return *this;
	}

	_message = other._message;
	return *this;
}

e2d::Exception::~Exception() throw()
{
}

e2d::String e2d::Exception::msg() const
{
	return _message;
}
