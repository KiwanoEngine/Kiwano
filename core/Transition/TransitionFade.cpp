#include "..\etransition.h"
#include "..\enode.h"

e2d::TransitionFade::TransitionFade(double duration)
	: Transition(0)
	, m_fFadeOutDuration(max(duration / 2, 0))
	, m_fFadeInDuration(max(duration / 2, 0))
	, m_bFadeOutTransioning(true)
{
}

e2d::TransitionFade::TransitionFade(double fadeOutDuration, double fadeInDuration)
	: Transition(0)
	, m_fFadeOutDuration(max(fadeOutDuration, 0))
	, m_fFadeInDuration(max(fadeInDuration, 0))
	, m_bFadeOutTransioning(true)
{
}

void e2d::TransitionFade::_init(Scene * prev, Scene * next)
{
	Transition::_init(prev, next);
	if (m_pPrevScene)
	{
		m_bFadeOutTransioning = true;
		m_fDuration = m_fFadeOutDuration;
	}
	else
	{
		m_bFadeOutTransioning = false;
		m_fDuration = m_fFadeInDuration;
	}
	m_sPrevLayerParam.opacity = 1;
	m_sNextLayerParam.opacity = 0;
}

void e2d::TransitionFade::_updateCustom()
{
	if (m_bFadeOutTransioning)
	{
		m_sPrevLayerParam.opacity = float(1 - m_fRateOfProgress);
		if (m_fRateOfProgress >= 1)
		{
			m_bFadeOutTransioning = false;
			m_fDuration = m_fFadeInDuration;
			m_fLast = Time::getTotalTime();
		}
	}
	else
	{
		m_sNextLayerParam.opacity = float(m_fRateOfProgress);
		if (m_fRateOfProgress >= 1)
		{
			this->_stop();
		}
	}
}

void e2d::TransitionFade::_reset()
{
}
