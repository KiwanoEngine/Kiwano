#include "..\eactions.h"
#include "..\Win\winbase.h"

e2d::ActionFrames::ActionFrames() :
	m_nFrameIndex(0)
{
	// 帧动画默认 .5s 刷新一次
	setInterval(500);
}

e2d::ActionFrames::ActionFrames(LONGLONG frameDelay) :
	m_nFrameIndex(0)
{
	setInterval(frameDelay);
}

e2d::ActionFrames::~ActionFrames()
{
	for (auto frame : m_vFrames)
	{
		frame->autoRelease();
		frame->release();
	}
}

void e2d::ActionFrames::_init()
{
	EAction::_init();
	// 记录当前时间
	m_nLast = GetNow();
}

void e2d::ActionFrames::_exec()
{
	// 判断时间间隔是否足够
	while (GetInterval(m_nLast) > m_nAnimationInterval)
	{
		// 重新记录时间
		m_nLast += milliseconds(m_nAnimationInterval);
		m_pTarget->setImage(m_vFrames[m_nFrameIndex]);
		m_nFrameIndex++;
		// 判断动作是否结束
		if (m_nFrameIndex == m_vFrames.size())
		{
			this->stop();
			break;
		}
	}
}

void e2d::ActionFrames::_reset()
{
	EAction::_reset();
	m_nFrameIndex = 0;
	// 记录当前时间
	m_nLast = steady_clock::now();
}

void e2d::ActionFrames::addFrame(Image * frame)
{
	if (frame)
	{
		m_vFrames.push_back(frame);
		frame->retain();
	}
}

e2d::ActionFrames * e2d::ActionFrames::copy() const
{
	auto a = new ActionFrames(this->m_nAnimationInterval);
	for (auto f : m_vFrames)
	{
		a->addFrame(f);
	}
	return a;
}

e2d::ActionFrames * e2d::ActionFrames::reverse() const
{
	auto a = this->copy();
	a->m_vFrames.reserve(m_vFrames.size());
	return a;
}
