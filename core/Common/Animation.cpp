#include "..\e2dcommon.h"

e2d::Animation::Animation()
	: _interval(1)
{
}

e2d::Animation::Animation(double interval)
	: _interval(interval)
{
}

e2d::Animation::Animation(double interval, const std::vector<Image*>& frames)
	: _interval(interval)
{
	for (auto frame : frames)
	{
		this->add(frame);
	}
}

e2d::Animation::Animation(int number, Image * frame, ...)
	: _interval(1)
{
	va_list args;
	va_start(args, frame);

	this->add(frame);
	for (int i = 1; i < number; i++)
	{
		this->add(va_arg(args, Image*));
	}

	va_end(args);
}

e2d::Animation::Animation(double interval, int number, Image * frame, ...)
	: _interval(interval)
{
	va_list args;
	va_start(args, frame);

	this->add(frame);
	for (int i = 1; i < number; i++)
	{
		this->add(va_arg(args, Image*));
	}

	va_end(args);
}

#ifdef HIGHER_THAN_VS2012

e2d::Animation::Animation(const std::initializer_list<Image*>& frames)
	: _interval(1)
{
	this->add(frames);
}

e2d::Animation::Animation(double interval, const std::initializer_list<Image*>& frames)
	: _interval(interval)
{
	this->add(frames);
}

#endif

e2d::Animation::~Animation()
{
}

void e2d::Animation::setInterval(double interval)
{
	_interval = max(interval, 0);
}

void e2d::Animation::onDestroy()
{
	for (auto frame : _frames)
	{
		SafeRelease(&frame);
	}
}

void e2d::Animation::add(Image * frame)
{
	if (frame)
	{
		_frames.push_back(frame);
		frame->retain();
	}
}

void e2d::Animation::add(int number, Image * frame, ...)
{
	va_list args;
	va_start(args, frame);

	this->add(frame);
	--number;

	while (number > 0)
	{
		this->add(va_arg(args, Image*));
		--number;
	}

	va_end(args);
}

#ifdef HIGHER_THAN_VS2012
void e2d::Animation::add(const std::initializer_list<Image*>& frames)
{
	for (const auto &image : frames)
	{
		this->add(image);
	}
}
#endif

double e2d::Animation::getInterval() const
{
	return _interval;
}

const std::vector<e2d::Image*>& e2d::Animation::getFrames() const
{
	return _frames;
}

e2d::Animation * e2d::Animation::clone() const
{
	auto a = new Animation(_interval);
	for (auto frame : _frames)
	{
		a->add(frame);
	}
	return a;
}
