#include "..\e2dcommon.h"

e2d::Function::Function()
	: _func(nullptr) 
{}

e2d::Function::Function(std::nullptr_t)
	: _func(nullptr)
{
}

e2d::Function::Function(std::function<void()> func)
	: _func(func)
{
}

void e2d::Function::operator()(void) const
{
	if (_func)
	{
		_func();
	}
}

e2d::Function::operator bool() const
{
	return static_cast<bool>(_func);
}