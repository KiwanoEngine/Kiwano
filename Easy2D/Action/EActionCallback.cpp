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

void e2d::EActionCallback::_callOn()
{
	m_Callback();
	this->stop();
}

void e2d::EActionCallback::_reset()
{
	EAction::_reset();
}
