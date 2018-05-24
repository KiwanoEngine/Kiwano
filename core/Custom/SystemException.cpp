#include "..\e2dcustom.h"

e2d::SystemException::SystemException() throw()
	: Exception(L"未知的系统异常")
{
}

e2d::SystemException::SystemException(const String& message) throw()
	: Exception(message)
{
}

