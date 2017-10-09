#include "..\easy2d.h"

ActionCallback::ActionCallback(const std::function<void()>& callback) :
	m_Callback(callback)
{
}

ActionCallback::~ActionCallback()
{
}

ActionCallback * ActionCallback::copy() const
{
	return new ActionCallback(m_Callback);
}

void ActionCallback::_init()
{
	Action::_init();
}

void ActionCallback::_exec(std::chrono::steady_clock::time_point nNow)
{
	m_Callback();
	this->stop();
}

void ActionCallback::_reset()
{
	Action::_reset();
}
