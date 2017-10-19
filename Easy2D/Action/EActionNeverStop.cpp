#include "..\eactions.h"

e2d::EActionLoop::EActionLoop(EAction * action) :
	m_Action(action)
{
	m_Action->retain();
}

e2d::EActionLoop::~EActionLoop()
{
	SafeRelease(&m_Action);
}

e2d::EActionLoop * e2d::EActionLoop::clone() const
{
	return new EActionLoop(m_Action->clone());
}

void e2d::EActionLoop::_init()
{
	EAction::_init();
	m_Action->m_pTarget = m_pTarget;
	m_Action->_init();
}

void e2d::EActionLoop::_callOn()
{
	m_Action->_callOn();

	if (m_Action->_isEnding())
	{
		m_Action->_reset();
	}
}

void e2d::EActionLoop::_reset()
{
	EAction::_reset();
}
