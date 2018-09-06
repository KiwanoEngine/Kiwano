#include "..\e2dimpl.h"

e2d::SystemException::SystemException() E2D_NOEXCEPT
	: Exception("Unknown system exception")
{
}

e2d::SystemException::SystemException(const char * message) E2D_NOEXCEPT
	: Exception(message)
{
}
