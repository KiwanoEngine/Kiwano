#include "..\eactions.h"

e2d::ActionCallback::ActionCallback(const VoidFunction& callback) :
	m_Callback(callback)
{
}

e2d::ActionCallback * e2d::ActionCallback::clone() const
{
	return new ActionCallback(m_Callback);
}

void e2d::ActionCallback::_init()
{
	// 执行回调函数的动作不需要初始化
}

void e2d::ActionCallback::_update()
{
	m_Callback();
	this->stop();
}
