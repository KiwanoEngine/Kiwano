#include "..\e2daction.h"

e2d::Animation::Animation() 
	: _nFrameIndex(0)
	, _fInterval(1)
{
}

e2d::Animation::Animation(double interval)
	: _nFrameIndex(0)
	, _fInterval(interval)
{
}

#ifdef HIGHER_THAN_VS2012

e2d::Animation::Animation(const std::initializer_list<Image*>& vImages)
	: _nFrameIndex(0)
	, _fInterval(1)
{
	this->add(vImages);
}

e2d::Animation::Animation(double interval, const std::initializer_list<Image*>& vImages)
	: _nFrameIndex(0)
	, _fInterval(interval)
{
	this->add(vImages);
}

#else

e2d::Animation::Animation(int number, Image * frame, ...)
	: _nFrameIndex(0)
	, _fInterval(1)
{
	Image ** ppImage = &frame;

	while (number > 0)
	{
		WARN_IF((*ppImage) == nullptr, "Animation NULL pointer exception!");
		this->add(*ppImage);
		ppImage++;
		number--;
	}
}

e2d::Animation::Animation(double interval, int number, Image * frame, ...)
	: _nFrameIndex(0)
	, _fInterval(interval)
{
	Image ** ppImage = &frame;

	while (number > 0)
	{
		WARN_IF((*ppImage) == nullptr, "Animation NULL pointer exception!");
		this->add(*ppImage);
		ppImage++;
		number--;
	}
}

#endif

e2d::Animation::~Animation()
{
}

void e2d::Animation::setInterval(double interval)
{
	_fInterval = max(interval, 0);
}

void e2d::Animation::_init()
{
	ActionBase::_init();
}

void e2d::Animation::_update()
{
	ActionBase::_update();

	if (_pTarget == nullptr)
	{
		this->stop();
		return;
	}

	// 判断时间间隔是否足够
	while ((Time::getTotalTime() - _fLast) >= _fInterval)
	{
		// 重新记录时间
		_fLast += _fInterval;
		// 加载关键帧
		static_cast<Sprite*>(_pTarget)->open(_vFrames[_nFrameIndex]);
		_nFrameIndex++;
		// 判断动作是否结束
		if (_nFrameIndex == _vFrames.size())
		{
			this->stop();
			break;
		}
	}
}

void e2d::Animation::reset()
{
	ActionBase::reset();
	_nFrameIndex = 0;
}

void e2d::Animation::destroy()
{
	ActionBase::destroy();
	for (auto frame : _vFrames)
	{
		SafeRelease(&frame);
	}
}

void e2d::Animation::add(Image * frame)
{
	if (frame)
	{
		_vFrames.push_back(frame);
		frame->retain();
	}
}

#ifdef HIGHER_THAN_VS2012
void e2d::Animation::add(const std::initializer_list<Image*>& vImages)
{
	for (const auto &image : vImages)
	{
		this->add(image);
	}
}
#else
void e2d::Animation::add(int number, Image * frame, ...)
{
	Image ** ppImage = &frame;

	while (number > 0)
	{
		WARN_IF((*ppImage) == nullptr, "Animation NULL pointer exception!");
		this->add(*ppImage);
		ppImage++;
		number--;
	}
}
#endif

e2d::Animation * e2d::Animation::clone() const
{
	auto a = new Animation(_fInterval);
	for (auto frame : _vFrames)
	{
		a->add(frame);
	}
	return a;
}

e2d::Animation * e2d::Animation::reverse() const
{
	auto a = this->clone();
	a->_vFrames.reserve(_vFrames.size());
	return a;
}
