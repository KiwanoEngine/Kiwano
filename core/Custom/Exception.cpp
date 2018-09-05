#include "..\e2dimpl.h"


e2d::Exception::Exception() E2D_NOEXCEPT
	: message_()
{
}

e2d::Exception::Exception(const char * message) E2D_NOEXCEPT
	: message_(message)
{
}

e2d::Exception::Exception(Exception const& other) E2D_NOEXCEPT
	: message_(other.message_)
{
}

e2d::Exception& e2d::Exception::operator=(Exception const& other) E2D_NOEXCEPT
{
	if (this == &other)
	{
		return *this;
	}

	message_ = other.message_;
	return *this;
}

e2d::Exception::~Exception() E2D_NOEXCEPT
{
}

const char * e2d::Exception::GetMsg() const
{
	return message_;
}
