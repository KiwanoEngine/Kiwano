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
	Action::_init();
	m_Action->m_pTargetSprite = m_pTargetSprite;
	m_Action->_init();
}

void ActionNeverStop::_exec(LARGE_INTEGER nNow)
{
	m_Action->_exec(nNow);

	if (m_Action->isEnding())
	{
		m_Action->_reset();
	}
}

void ActionNeverStop::_reset()
{
	Action::_reset();
}
