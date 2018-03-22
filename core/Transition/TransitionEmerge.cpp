#include "..\etransitions.h"
#include "..\enodes.h"

e2d::TransitionEmerge::TransitionEmerge(double duration)
	: Transition(duration)
{
}

void e2d::TransitionEmerge::_update()
{
	this->_calcRateOfProgress();

	if (m_pPrevScene) m_pPrevScene->getRoot()->setOpacity(1 - m_fRateOfProgress);
	m_pNextScene->getRoot()->setOpacity(m_fRateOfProgress);

	if (m_fRateOfProgress >= 1)
	{
		this->_stop();
	}
}

void e2d::TransitionEmerge::_init()
{
	if (m_pPrevScene) m_pPrevScene->getRoot()->setOpacity(1);
	m_pNextScene->getRoot()->setOpacity(0);
}

void e2d::TransitionEmerge::_reset()
{
	if (m_pPrevScene) m_pPrevScene->getRoot()->setOpacity(1);
	m_pNextScene->getRoot()->setOpacity(1);
}
