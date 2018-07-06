#include "..\e2dcustom.h"

e2d::SystemException::SystemException() E2D_NOEXCEPT
	: Exception(L"未知的系统异常")
{
}

e2d::SystemException::SystemException(const String& message) E2D_NOEXCEPT
	: Exception(message)
{
}

