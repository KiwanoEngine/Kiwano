#include "..\easy2d.h"

Animation::Animation(float duration)
{
	m_nDuration = 0;
	m_nTotalDuration = UINT(duration * 1000);
}

Animation::~Animation()
{
}

void Animation::_init()
{
	// 记录当前时间
	QueryPerformanceCounter(&m_nLast);
}

bool Animation::_exec(LARGE_INTEGER nNow)
{
	// 判断时间间隔是否足够
	if (nNow.QuadPart - m_nLast.QuadPart > m_nAnimationInterval.QuadPart)
	{
		// 用求余的方法重新记录时间
		m_nLast.QuadPart = nNow.QuadPart - (nNow.QuadPart % m_nAnimationInterval.QuadPart);
		m_nDuration += m_nMilliSeconds;
		return true;
	}
	return false;
}

void Animation::_reset()
{
	m_nDuration = 0;
	// 重新记录当前时间
	QueryPerformanceCounter(&m_nLast);
}
