#include "..\etransitions.h"
#include "..\enodes.h"

e2d::ETransitionMove::ETransitionMove(float duration, MOVE_DIRECT direct)
	: ETransition(duration)
	, m_Direct(direct)
{
}

void e2d::ETransitionMove::_update()
{
	this->_calcRateOfProgress();

	if (m_pPrevScene) m_pPrevScene->getRoot()->setPos(m_Vec * m_fRateOfProgress);
	m_pNextScene->getRoot()->setPos(m_NextPos + m_Vec * m_fRateOfProgress);

	if (m_fRateOfProgress >= 1)
	{
		this->_stop();
	}
}

void e2d::ETransitionMove::_init()
{
	if (m_Direct == ETransitionMove::UP)
	{
		m_Vec = EVector2(0, -EWindow::getHeight());
		m_NextPos = EPoint(0, EWindow::getHeight());
	}
	else if (m_Direct == ETransitionMove::DOWN)
	{
		m_Vec = EVector2(0, EWindow::getHeight());
		m_NextPos = EPoint(0, -EWindow::getHeight());
	}
	else if (m_Direct == ETransitionMove::LEFT)
	{
		m_Vec = EVector2(-EWindow::getWidth(), 0);
		m_NextPos = EPoint(EWindow::getWidth(), 0);
	}
	else if (m_Direct == ETransitionMove::RIGHT)
	{
		m_Vec = EVector2(EWindow::getWidth(), 0);
		m_NextPos = EPoint(-EWindow::getWidth(), 0);
	}

	if (m_pPrevScene) m_pPrevScene->getRoot()->setPos(0, 0);
	m_pNextScene->getRoot()->setPos(m_NextPos);
}

void e2d::ETransitionMove::_reset()
{
	if (m_pPrevScene) m_pPrevScene->getRoot()->setPos(0, 0);
	m_pNextScene->getRoot()->setPos(0, 0);
}

