#include "..\eactions.h"

e2d::ActionCallback::ActionCallback(const std::function<void()>& callback) :
	m_Callback(callback)
{
}

e2d::ActionCallback::~ActionCallback()
{
}

e2d::ActionCallback * e2d::ActionCallback::copy() const
{
	return new ActionCallback(m_Callback);
}

void e2d::ActionCallback::_init()
{
	EAction::_init();
}

void e2d::ActionCallback::_exec()
{
	m_Callback();
	this->stop();
}

void e2d::ActionCallback::_reset()
{
	EAction::_reset();
}
