#include "..\easy2d.h"

ActionNeverStop::ActionNeverStop(Action * action) :
	m_Action(action)
{
	m_Action->retain();
}

ActionNeverStop::~ActionNeverStop()
{
	SafeRelease(m_Action);
}

ActionNeverStop * ActionNeverStop::copy() const
{
	return new ActionNeverStop(m_Action->copy());
}

void ActionNeverStop::_init()
{
	m_Action->m_pParent = m_pParent;
	m_Action->_init();
}

bool ActionNeverStop::_exec(LARGE_INTEGER nNow)
{
	if (m_bStop) return true;
	if (!m_bRunning) return false;

	if (m_Action->_exec(nNow))
	{
		m_Action->_reset();
	}
	// ÓÀ²»½áÊø
	return false;
}

void ActionNeverStop::_reset()
{
	m_Action->_reset();
}
