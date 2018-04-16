#include "..\etransition.h"
#include "..\enode.h"

e2d::TransitionMove::TransitionMove(double duration, int direct)
	: Transition(duration)
	, m_Direct(direct)
{
}

void e2d::TransitionMove::_updateCustom()
{
	if (m_pPrevScene)
	{
		auto root = m_pPrevScene->getRoot();
		root->setPos(m_Vector * m_fRateOfProgress);

		Point pos = root->getPos();
		m_sPrevLayerParam.contentBounds = D2D1::RectF(
			float(max(pos.x, 0)), 
			float(max(pos.y, 0)),
			float(min(pos.x + m_WindowSize.width, m_WindowSize.width)),
			float(min(pos.y + m_WindowSize.height, m_WindowSize.height))
		);
	}
	if (m_pNextScene)
	{
		auto root = m_pNextScene->getRoot();
		root->setPos(m_NextPos + m_Vector * m_fRateOfProgress);

		Point pos = root->getPos();
		m_sNextLayerParam.contentBounds = D2D1::RectF(
			float(max(pos.x, 0)),
			float(max(pos.y, 0)),
			float(min(pos.x + m_WindowSize.width, m_WindowSize.width)),
			float(min(pos.y + m_WindowSize.height, m_WindowSize.height))
		);
	}

	if (m_fRateOfProgress >= 1)
	{
		this->_stop();
	}
}

void e2d::TransitionMove::_init(Scene * prev, Scene * next)
{
	Transition::_init(prev, next);

	m_WindowSize = Window::getSize();
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

void e2d::TransitionMove::_reset()
{
	if (m_pPrevScene) m_pPrevScene->getRoot()->setPos(0, 0);
	m_pNextScene->getRoot()->setPos(0, 0);
}

