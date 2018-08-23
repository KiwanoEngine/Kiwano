#include "..\e2daction.h"

e2d::Animation::Animation()
	: _interval(1)
{
}

e2d::Animation::Animation(const std::vector<Image*>& frames)
	: _interval(1)
{
	this->add(frames);
}

e2d::Animation::Animation(float interval)
	: _interval(interval)
{
}

e2d::Animation::Animation(float interval, const std::vector<Image*>& frames)
	: _interval(interval)
{
	this->add(frames);
}

e2d::Animation::~Animation()
{
	for (const auto& frame : _frames)
	{
		GC::instance()->safeRelease(frame);
	}
}

e2d::Animation& e2d::Animation::interval(float interval)
{
	_interval = std::max(interval, 0.f);
	return *this;
}

e2d::Animation& e2d::Animation::add(Image * frame)
{
	WARN_IF(frame == nullptr, "Animation::add failed, frame is nullptr.");
	if (frame)
	{
		_frames.push_back(frame);
		frame->retain();
	}
	return *this;
}

e2d::Animation& e2d::Animation::add(const std::vector<Image*>& frames)
{
	for (const auto &image : frames)
	{
		this->add(image);
	}
	return *this;
}

float e2d::Animation::interval() const
{
	return _interval;
}

const std::vector<e2d::Image*>& e2d::Animation::frames() const
{
	return _frames;
}

e2d::Animation * e2d::Animation::clone() const
{
	auto animation = new (e2d::autorelease) Animation(_interval);
	if (animation)
	{
		for (const auto& frame : _frames)
		{
			animation->add(frame);
		}
	}
	return animation;
}

e2d::Animation * e2d::Animation::reverse() const
{
	auto& oldFrames = this->frames();
	std::vector<Image*> frames(oldFrames.size());

	if (!oldFrames.empty())
	{
		for (auto iter = oldFrames.crbegin(),
			iterCrend = oldFrames.crend();
			iter != iterCrend;
			++iter)
		{
			Image* frame = *iter;
			if (frame)
			{
				frames.push_back(frame);
			}
		}
	}

	return new (e2d::autorelease) Animation(this->interval(), frames);
}
