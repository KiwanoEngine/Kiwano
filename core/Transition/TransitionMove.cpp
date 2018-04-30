#include "..\e2dtransition.h"
#include "..\e2dnode.h"

e2d::TransitionMove::TransitionMove(double duration, Direct direct)
	: TransitionBase(duration)
	, m_Direct(direct)
{
}

void e2d::TransitionMove::_init(Scene * prev, Scene * next)
{
	TransitionBase::_init(prev, next);

	double width = m_WindowSize.width;
	double height = m_WindowSize.height;
	if (m_Direct == Direct::UP)
	{
		m_Vector = Vector(0, -height);
		m_NextPos = Point(0, height);
	}
	else if (m_Direct == Direct::DOWN)
	{
		m_Vector = Vector(0, height);
		m_NextPos = Point(0, -height);
	}
	else if (m_Direct == Direct::LEFT)
	{
		m_Vector = Vector(-width, 0);
		m_NextPos = Point(width, 0);
	}
	else if (m_Direct == Direct::RIGHT)
	{
		m_Vector = Vector(width, 0);
		m_NextPos = Point(-width, 0);
	}

	if (m_pPrevScene) m_pPrevScene->getRoot()->setPos(0, 0);
	m_pNextScene->getRoot()->setPos(m_NextPos);
}

void e2d::TransitionMove::_updateCustom()
{
	if (m_pPrevScene)
	{
		m_pPrevScene->getRoot()->setPos(m_Vector * m_fRateOfProgress);
	}
	if (m_pNextScene)
	{
		m_pNextScene->getRoot()->setPos(m_NextPos + m_Vector * m_fRateOfProgress);
	}

	if (m_fRateOfProgress >= 1)
	{
		this->_stop();
	}
}

void e2d::TransitionMove::_reset()
{
	if (m_pPrevScene) m_pPrevScene->getRoot()->setPos(0, 0);
	m_pNextScene->getRoot()->setPos(0, 0);
}

