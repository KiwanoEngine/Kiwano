#include "..\e2daction.h"
#include "..\e2dnode.h"

e2d::Animate::Animate() 
	: _frameIndex(0)
	, _animation(nullptr)
{
}

e2d::Animate::Animate(Animation * animation)
	: _frameIndex(0)
	, _animation(nullptr)
{
	this->animation(animation);
}

e2d::Animate::~Animate()
{
	GC::instance()->safeRelease(_animation);
}

e2d::Animation * e2d::Animate::animation() const
{
	return _animation;
}

void e2d::Animate::animation(Animation * animation)
{
	if (animation && animation != _animation && !animation->frames().empty())
	{
		if (_animation) _animation->release();
		_animation = animation;
		_animation->retain();
	}
}

void e2d::Animate::_init()
{
	Action::_init();

	auto target = dynamic_cast<Sprite*>(_target);
	if (target && _animation)
	{
		target->open(_animation->frames()[_frameIndex]);
		++_frameIndex;
	}
}

void e2d::Animate::_update()
{
	Action::_update();

	if (!_animation)
	{
		this->stop();
		return;
	}

	while ((Time::now() - _started).seconds() >= _animation->interval())
	{
		auto& frames = _animation->frames();
		auto target = dynamic_cast<Sprite*>(_target);

		if (target)
		{
			target->open(frames[_frameIndex]);
		}

		_started += Duration(_animation->interval());
		++_frameIndex;

		if (_frameIndex == frames.size())
		{
			this->stop();
			break;
		}
	}
}

void e2d::Animate::_resetTime()
{
	Action::_resetTime();
}

void e2d::Animate::reset()
{
	Action::reset();
	_frameIndex = 0;
}

e2d::Animate * e2d::Animate::clone() const
{
	if (_animation)
	{
		return new (e2d::autorelease) Animate(_animation);
	}
	return nullptr;
}

e2d::Animate * e2d::Animate::reverse() const
{
	if (_animation)
	{
		auto animation = _animation->reverse();
		if (animation)
		{
			return new (e2d::autorelease) Animate(animation);
		}
	}
	return nullptr;
}
