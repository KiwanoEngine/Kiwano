#include "..\eactions.h"

e2d::EActionNeverStop::EActionNeverStop(EAction * action) :
	m_Action(action)
{
	m_Action->retain();
}

e2d::EActionNeverStop::~EActionNeverStop()
{
	SafeRelease(&m_Action);
}

e2d::EActionNeverStop * e2d::EActionNeverStop::copy() const
{
	return new EActionNeverStop(m_Action->copy());
}

void e2d::EActionNeverStop::_init()
{
	EAction::_init();
	m_Action->m_pTarget = m_pTarget;
	m_Action->_init();
}

void e2d::EActionNeverStop::_exec()
{
	m_Action->_exec();

	if (m_Action->isEnding())
	{
		m_Action->_reset();
	}
}

void e2d::EActionNeverStop::_reset()
{
	EAction::_reset();
}
