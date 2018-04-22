#include "..\e2dcommon.h"

e2d::Function::Function()
	: m_func(nullptr) 
{}

e2d::Function::Function(std::nullptr_t)
	: m_func(nullptr)
{
}

e2d::Function::Function(std::function<void()> func)
	: m_func(func)
{
}

void e2d::Function::operator()(void) const
{
	if (m_func)
	{
		m_func();
	}
}

e2d::Function::operator bool() const
{
	return static_cast<bool>(m_func);
}