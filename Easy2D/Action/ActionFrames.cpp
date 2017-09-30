#include "..\easy2d.h"

ActionFrames::ActionFrames() :
	m_nFrameIndex(0)
{
	// 帧动画默认 .5s 刷新一次
	setInterval(500);
}

ActionFrames::ActionFrames(UINT frameDelay) :
	m_nFrameIndex(0)
{
	setInterval(frameDelay);
}

ActionFrames::~ActionFrames()
{
	for (auto frame : m_vFrames)
	{
		frame->release();
	}
}

void ActionFrames::_init()
{
	// 记录当前时间
	QueryPerformanceCounter(&m_nLast);
}

bool ActionFrames::_exec(LARGE_INTEGER nNow)
{
	if (m_bStop) return true;
	if (!m_bRunning) return false;

	// 判断时间间隔是否足够
	while (nNow.QuadPart - m_nLast.QuadPart > m_nAnimationInterval.QuadPart)
	{
		// 用求余的方法重新记录时间
		m_nLast.QuadPart = nNow.QuadPart - (nNow.QuadPart % m_nAnimationInterval.QuadPart);
		m_pParent->setImage(m_vFrames[m_nFrameIndex]);
		m_nFrameIndex++;
		// 判断动作是否结束
		if (m_nFrameIndex == m_vFrames.size())
		{
			return true;
		}
	}
	return false;
}

void ActionFrames::_reset()
{
	m_nFrameIndex = 0;
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
	auto a = new ActionFrames(this->m_nMilliSeconds);
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
