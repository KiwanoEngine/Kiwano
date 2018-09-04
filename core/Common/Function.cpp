#include "..\e2dcommon.h"

e2d::Function::Function()
	: func_(nullptr)
{}

e2d::Function::Function(std::nullptr_t)
	: func_(nullptr)
{
}

e2d::Function::Function(std::function<void()> func)
	: func_(func)
{
}

void e2d::Function::operator()(void) const
{
	if (func_)
	{
		func_();
	}
}

e2d::Function::operator bool() const
{
	return static_cast<bool>(func_);
}