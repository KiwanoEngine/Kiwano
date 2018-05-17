#include "..\e2daction.h"

e2d::CallFunc::CallFunc(const Function& func) :
	_func(func)
{
}

e2d::CallFunc * e2d::CallFunc::create(const Function & func)
{
	return Create<CallFunc>(func);
}

e2d::CallFunc * e2d::CallFunc::clone() const
{
	return Create<CallFunc>(_func);
}

e2d::CallFunc * e2d::CallFunc::reverse() const
{
	return Create<CallFunc>(_func);
}

void e2d::CallFunc::_init()
{
}

void e2d::CallFunc::_update()
{
	_func();
	this->stop();
}
