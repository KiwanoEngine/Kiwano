#include "..\eactions.h"

e2d::ActionNeverStop::ActionNeverStop(EAction * action) :
	m_Action(action)
{
	m_Action->retain();
}

e2d::ActionNeverStop::~ActionNeverStop()
{
	SafeRelease(&m_Action);
}

e2d::ActionNeverStop * e2d::ActionNeverStop::copy() const
{
	return new ActionNeverStop(m_Action->copy());
}

void e2d::ActionNeverStop::_init()
{
	EAction::_init();
	m_Action->m_pTarget = m_pTarget;
	m_Action->_init();
}

void e2d::ActionNeverStop::_exec()
{
	m_Action->_exec();

	if (m_Action->isEnding())
	{
		m_Action->_reset();
	}
}

void e2d::ActionNeverStop::_reset()
{
	EAction::_reset();
}
