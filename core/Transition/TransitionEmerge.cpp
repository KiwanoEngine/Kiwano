#include "..\etransition.h"
#include "..\enode.h"

e2d::TransitionEmerge::TransitionEmerge(double duration)
	: Transition(duration)
{
}

void e2d::TransitionEmerge::_updateCustom()
{
	m_sPrevLayerParam.opacity = float(1 - m_fRateOfProgress);
	m_sNextLayerParam.opacity = float(m_fRateOfProgress);

	if (m_fRateOfProgress >= 1)
	{
		this->_stop();
	}
}

void e2d::TransitionEmerge::_init(Scene * prev, Scene * next)
{
	Transition::_init(prev, next);
	m_sPrevLayerParam.opacity = 1;
	m_sNextLayerParam.opacity = 0;
}

void e2d::TransitionEmerge::_reset()
{
}
