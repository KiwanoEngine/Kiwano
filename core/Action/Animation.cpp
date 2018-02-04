#include "..\eactions.h"

e2d::EAnimation::EAnimation() 
	: m_nFrameIndex(0)
	, m_pTarget(nullptr)
{
}

e2d::EAnimation::EAnimation(float invertal)
	: m_nFrameIndex(0)
	, m_fInterval(invertal)
	, m_pTarget(nullptr)
{
}

e2d::EAnimation::~EAnimation()
{
	for (auto frame : m_vFrames)
	{
		SafeRelease(&frame);
	}
}

void e2d::EAnimation::setInterval(float interval)
{
	m_fInterval = max(interval, 0);
}

void e2d::EAnimation::_init()
{
	EAction::_init();
}

void e2d::EAnimation::_update()
{
	EAction::_update();

	if (m_pTarget == nullptr)
	{
		this->stop();
		return;
	}

	// 判断时间间隔是否足够
	while ((ETime::getTotalTime() - m_fLast) >= m_fInterval)
	{
		// 重新记录时间
		m_fLast += m_fInterval;
		// 加载关键帧
		m_pTarget->loadFrom(m_vFrames[m_nFrameIndex]);
		m_nFrameIndex++;
		// 判断动作是否结束
		if (m_nFrameIndex == m_vFrames.size())
		{
			this->stop();
			break;
		}
	}
}

void e2d::EAnimation::_reset()
{
	EAction::_reset();
	m_nFrameIndex = 0;
}

void e2d::EAnimation::_setTarget(ENode * node)
{
	m_pTarget = static_cast<ESprite*>(node);
}

void e2d::EAnimation::addKeyframe(EImage * frame)
{
	if (frame)
	{
		m_vFrames.push_back(frame);
		frame->retain();
	}
}

e2d::EAnimation * e2d::EAnimation::clone() const
{
	auto a = new EAnimation(m_fInterval);
	for (auto frame : m_vFrames)
	{
		a->addKeyframe(frame);
	}
	return a;
}

e2d::EAnimation * e2d::EAnimation::reverse() const
{
	auto a = this->clone();
	a->m_vFrames.reserve(m_vFrames.size());
	return a;
}
