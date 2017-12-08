#include "..\eactions.h"
#include "..\Win\winbase.h"
#include <typeinfo>

e2d::EAnimation::EAnimation() :
	m_nFrameIndex(0)
{
	// 帧动画默认 0.1s 刷新一次
	setInterval(100);
}

e2d::EAnimation::EAnimation(LONGLONG frameDelay) :
	m_nFrameIndex(0)
{
	setInterval(frameDelay);
}

e2d::EAnimation::~EAnimation()
{
	for (auto frame : m_vFrames)
	{
		SafeRelease(&frame);
	}
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
	while (GetInterval(m_tLast) > m_nAnimationInterval)
	{
		// 重新记录时间
		m_tLast += milliseconds(m_nAnimationInterval);
		// 加载精灵帧
		reinterpret_cast<ESprite*>(m_pTarget)->loadFrom(m_vFrames[m_nFrameIndex]);
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

void e2d::EAnimation::addFrame(ESpriteFrame * frame)
{
	if (frame)
	{
		m_vFrames.push_back(frame);
		frame->retain();
	}
}

e2d::EAnimation * e2d::EAnimation::clone() const
{
	auto a = new EAnimation(this->m_nAnimationInterval);
	for (auto f : m_vFrames)
	{
		a->addFrame(f);
	}
	return a;
}

e2d::EAnimation * e2d::EAnimation::reverse() const
{
	auto a = this->clone();
	a->m_vFrames.reserve(m_vFrames.size());
	return a;
}
