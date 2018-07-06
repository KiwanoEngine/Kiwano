#include "..\e2dcustom.h"


e2d::Exception::Exception() E2D_NOEXCEPT
	: _message()
{
}

e2d::Exception::Exception(const String& message) E2D_NOEXCEPT
	: _message(message)
{
}

e2d::Exception::Exception(Exception const& other) E2D_NOEXCEPT
	: _message(other._message)
{
}

e2d::Exception& e2d::Exception::operator=(Exception const& other) E2D_NOEXCEPT
{
	if (this == &other)
	{
		return *this;
	}

	_message = other._message;
	return *this;
}

e2d::Exception::~Exception() E2D_NOEXCEPT
{
}

e2d::String e2d::Exception::msg() const
{
	return _message;
}
