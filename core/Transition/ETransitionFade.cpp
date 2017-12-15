#include "..\etransitions.h"
#include "..\enodes.h"

e2d::ETransitionFade::ETransitionFade(float fadeOutDuration, float fadeInDuration)
	: ETransition(0)
	, m_fFadeOutDuration(fadeOutDuration * 1000)
	, m_fFadeInDuration(fadeInDuration * 1000)
	, m_bFadeOutTransioning(true)
{
	m_fTotalDuration = m_fFadeOutDuration;
}

void e2d::ETransitionFade::_update()
{
	if (_isDelayEnough())
	{
		if (m_bFadeOutTransioning)
		{
			m_pPrevScene->getRoot()->setOpacity(1 - m_fRateOfProgress);
			if (m_fDuration >= m_fTotalDuration)
			{
				m_bFadeOutTransioning = false;
				m_fTotalDuration = m_fFadeInDuration;
				m_fDuration = 0;
			}
		}
		else
		{
			m_pNextScene->getRoot()->setOpacity(m_fRateOfProgress);
			if (m_fDuration >= m_fTotalDuration)
			{
				this->_stop();
			}
		}
	}
}

void e2d::ETransitionFade::_init()
{
	if (m_pPrevScene)
	{
		m_bFadeOutTransioning = true;
		m_pPrevScene->getRoot()->setOpacity(1);
	}
	else
	{
		m_bFadeOutTransioning = false;
		m_fTotalDuration = m_fFadeInDuration;
	}
	m_pNextScene->getRoot()->setOpacity(0);
}

void e2d::ETransitionFade::_reset()
{
	if (m_pPrevScene) m_pPrevScene->getRoot()->setOpacity(1);
	m_pNextScene->getRoot()->setOpacity(1);
}
