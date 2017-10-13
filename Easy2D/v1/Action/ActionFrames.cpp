#include "..\easy2d.h"
#include "..\Win\winbase.h"

ActionFrames::ActionFrames() :
	m_nFrameIndex(0)
{
	// 帧动画默认 .5s 刷新一次
	setInterval(500);
}

ActionFrames::ActionFrames(LONGLONG frameDelay) :
	m_nFrameIndex(0)
{
	setInterval(frameDelay);
}

ActionFrames::~ActionFrames()
{
	for (auto frame : m_vFrames)
	{
		frame->autoRelease();
		frame->release();
	}
}

void ActionFrames::_init()
{
	Action::_init();
	// 记录当前时间
	m_nLast = steady_clock::now();
}

void ActionFrames::_exec(steady_clock::time_point nNow)
{
	// 判断时间间隔是否足够
	while (duration_cast<milliseconds>(nNow - m_nLast).count() > m_nAnimationInterval)
	{
		// 重新记录时间
		m_nLast += milliseconds(m_nAnimationInterval);
		m_pTargetSprite->setImage(m_vFrames[m_nFrameIndex]);
		m_nFrameIndex++;
		// 判断动作是否结束
		if (m_nFrameIndex == m_vFrames.size())
		{
			this->stop();
			break;
		}
	}
}

void ActionFrames::_reset()
{
	Action::_reset();
	m_nFrameIndex = 0;
	// 记录当前时间
	m_nLast = steady_clock::now();
}

void ActionFrames::addFrame(Image * frame)
{
	if (frame)
	{
		m_vFrames.push_back(frame);
		frame->retain();
	}
}

ActionFrames * ActionFrames::copy() const
{
	auto a = new ActionFrames(this->m_nAnimationInterval);
	for (auto f : m_vFrames)
	{
		a->addFrame(f);
	}
	return a;
}

ActionFrames * ActionFrames::reverse() const
{
	auto a = this->copy();
	a->m_vFrames.reserve(m_vFrames.size());
	return a;
}
