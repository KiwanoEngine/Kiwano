#include "..\etransition.h"
#include "..\enode.h"

e2d::TransitionMove::TransitionMove(double duration, MOVE_DIRECT direct)
	: Transition(duration)
	, m_Direct(direct)
{
}

void e2d::TransitionMove::_update()
{
	this->_calcRateOfProgress();

	if (m_pPrevScene) m_pPrevScene->getRoot()->setPos(m_Vec * m_fRateOfProgress);
	m_pNextScene->getRoot()->setPos(m_NextPos + m_Vec * m_fRateOfProgress);

	if (m_fRateOfProgress >= 1)
	{
		this->_stop();
	}
}

void e2d::TransitionMove::_init()
{
	if (m_Direct == TransitionMove::UP)
	{
		m_Vec = Vector(0, -Window::getHeight());
		m_NextPos = Point(0, Window::getHeight());
	}
	else if (m_Direct == TransitionMove::DOWN)
	{
		m_Vec = Vector(0, Window::getHeight());
		m_NextPos = Point(0, -Window::getHeight());
	}
	else if (m_Direct == TransitionMove::LEFT)
	{
		m_Vec = Vector(-Window::getWidth(), 0);
		m_NextPos = Point(Window::getWidth(), 0);
	}
	else if (m_Direct == TransitionMove::RIGHT)
	{
		m_Vec = Vector(Window::getWidth(), 0);
		m_NextPos = Point(-Window::getWidth(), 0);
	}

	if (m_pPrevScene) m_pPrevScene->getRoot()->setPos(0, 0);
	m_pNextScene->getRoot()->setPos(m_NextPos);
}

void e2d::TransitionMove::_reset()
{
	if (m_pPrevScene) m_pPrevScene->getRoot()->setPos(0, 0);
	m_pNextScene->getRoot()->setPos(0, 0);
}

