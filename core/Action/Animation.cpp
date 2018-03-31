#include "..\eactions.h"

e2d::Animation::Animation() 
	: m_nFrameIndex(0)
	, m_fInterval(1)
{
}

e2d::Animation::Animation(double interval)
	: m_nFrameIndex(0)
	, m_fInterval(interval)
{
}

e2d::Animation::Animation(std::initializer_list<Image*>& vImages)
{
	this->add(vImages);
}

e2d::Animation::~Animation()
{
	for (auto frame : m_vFrames)
	{
		SafeRelease(&frame);
	}
}

void e2d::Animation::setInterval(double interval)
{
	m_fInterval = max(interval, 0);
}

void e2d::Animation::_init()
{
	Action::_init();
}

void e2d::Animation::_update()
{
	Action::_update();

	if (m_pTarget == nullptr)
	{
		this->stop();
		return;
	}

	// 判断时间间隔是否足够
	while ((Time::getTotalTime() - m_fLast) >= m_fInterval)
	{
		// 重新记录时间
		m_fLast += m_fInterval;
		// 加载关键帧
		static_cast<Sprite*>(m_pTarget)->open(m_vFrames[m_nFrameIndex]);
		m_nFrameIndex++;
		// 判断动作是否结束
		if (m_nFrameIndex == m_vFrames.size())
		{
			this->stop();
			break;
		}
	}
}

void e2d::Animation::reset()
{
	Action::reset();
	m_nFrameIndex = 0;
}

void e2d::Animation::add(Image * frame)
{
	if (frame)
	{
		m_vFrames.push_back(frame);
		frame->retain();
	}
}

void e2d::Animation::add(std::initializer_list<Image*>& vImages)
{
	for (const auto &image : vImages)
	{
		this->add(image);
	}
}

e2d::Animation * e2d::Animation::clone() const
{
	auto a = new Animation(m_fInterval);
	for (auto frame : m_vFrames)
	{
		a->add(frame);
	}
	return a;
}

e2d::Animation * e2d::Animation::reverse() const
{
	auto a = this->clone();
	a->m_vFrames.reserve(m_vFrames.size());
	return a;
}
