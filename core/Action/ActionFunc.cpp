#include "..\e2daction.h"

e2d::ActionFunc::ActionFunc(const Function& func) :
	m_Callback(func)
{
}

e2d::ActionFunc * e2d::ActionFunc::clone() const
{
	return new ActionFunc(m_Callback);
}

void e2d::ActionFunc::_init()
{
	// 执行函数对象的动作不需要初始化
}

void e2d::ActionFunc::_update()
{
	m_Callback();
	this->stop();
}
