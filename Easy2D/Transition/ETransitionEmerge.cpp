#include "..\etransitions.h"
#include "..\enodes.h"

e2d::ETransitionEmerge::ETransitionEmerge(float duration)
	: ETransition(duration)
{
}

void e2d::ETransitionEmerge::_update()
{
	if (_isDelayEnough())
	{
		if (m_pPrevScene) m_pPrevScene->getRoot()->setOpacity(1 - m_fRateOfProgress);
		m_pNextScene->getRoot()->setOpacity(m_fRateOfProgress);

		if (m_fDuration >= m_fTotalDuration)
		{
			this->_stop();
		}
	}
}

void e2d::ETransitionEmerge::_init()
{
	if (m_pPrevScene) m_pPrevScene->getRoot()->setOpacity(1);
	m_pNextScene->getRoot()->setOpacity(0);
}

void e2d::ETransitionEmerge::_reset()
{
	if (m_pPrevScene) m_pPrevScene->getRoot()->setOpacity(1);
	m_pNextScene->getRoot()->setOpacity(1);
}
