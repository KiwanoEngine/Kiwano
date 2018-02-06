#include "..\eactions.h"

e2d::EActionCallback::EActionCallback(const std::function<void()>& callback) :
	m_Callback(callback)
{
}

e2d::EActionCallback * e2d::EActionCallback::clone() const
{
	return new EActionCallback(m_Callback);
}

void e2d::EActionCallback::_init()
{
	// 执行回调函数的动作不需要初始化
}

void e2d::EActionCallback::_update()
{
	m_Callback();
	this->stop();
}
