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
}

bool ActionCallback::_exec(LARGE_INTEGER nNow)
{
	if (!m_bStop)
	{
		m_Callback();
	}
	return true;
}

void ActionCallback::_reset()
{
}
