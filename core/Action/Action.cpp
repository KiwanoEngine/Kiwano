#include "..\eactions.h"
#include "..\emanagers.h"

e2d::Action::Action() 
	: m_bRunning(false)
	, m_bEnding(false)
	, m_bInit(false)
	, m_pTarget(nullptr)
	, m_pParentScene(nullptr)
	, m_fLast(0)
{
	ActionManager::__add(this);
}

e2d::Action::~Action()
{
	ActionManager::__remove(this);
}

bool e2d::Action::isRunning()
{
	return m_bRunning;
}

bool e2d::Action::_isEnding()
{
	return m_bEnding;
}

void e2d::Action::setTarget(Node* pTarget)
{
	if (pTarget)
	{
		m_bRunning = true;
		m_pTarget = pTarget;
		this->reset();
	}
}

void e2d::Action::resume()
{
	m_bRunning = true;
	m_fLast = Time::getTotalTime();
}

void e2d::Action::pause()
{
	m_bRunning = false;
}

void e2d::Action::stop()
{
	m_bEnding = true;
}

e2d::String e2d::Action::getName() const
{
	return m_sName;
}

void e2d::Action::setName(String& name)
{
	m_sName = name;
}

e2d::Action * e2d::Action::reverse() const
{
	ASSERT(false, "Action cannot be reversed!");
	return nullptr;
}

e2d::Node * e2d::Action::getTarget()
{
	return m_pTarget;
}

void e2d::Action::_init()
{
	m_bInit = true;
	// 记录当前时间
	m_fLast = Time::getTotalTime();
}

void e2d::Action::_update()
{
	if (!m_bInit)
	{
		_init();
	}
}

void e2d::Action::reset()
{
	m_bInit = false;
	m_bEnding = false;
	m_fLast = Time::getTotalTime();
}

void e2d::Action::_resetTime()
{
	m_fLast = Time::getTotalTime();
}

e2d::ActionMoveBy * e2d::action::MoveBy(double duration, Vector vector)
{
	return new (std::nothrow) ActionMoveBy(duration, vector);
}

e2d::ActionMoveTo * e2d::action::MoveTo(double duration, Point pos)
{
	return new (std::nothrow) ActionMoveTo(duration, pos);
}

e2d::ActionScaleBy * e2d::action::ScaleBy(double duration, double scale)
{
	return new (std::nothrow) ActionScaleBy(duration, scale);
}

e2d::ActionScaleBy * e2d::action::ScaleBy(double duration, double scaleX, double scaleY)
{
	return new (std::nothrow) ActionScaleBy(duration, scaleX, scaleY);
}

e2d::ActionScaleTo * e2d::action::ScaleTo(double duration, double scale)
{
	return new (std::nothrow) ActionScaleTo(duration, scale);
}

e2d::ActionScaleTo * e2d::action::ScaleTo(double duration, double scaleX, double scaleY)
{
	return new (std::nothrow) ActionScaleTo(duration, scaleX, scaleY);
}

e2d::ActionOpacityBy * e2d::action::OpacityBy(double duration, double opacity)
{
	return new (std::nothrow) ActionOpacityBy(duration, opacity);
}

e2d::ActionOpacityTo * e2d::action::OpacityTo(double duration, double opacity)
{
	return new (std::nothrow) ActionOpacityTo(duration, opacity);
}

e2d::ActionFadeIn * e2d::action::FadeIn(double duration)
{
	return new (std::nothrow) ActionFadeIn(duration);
}

e2d::ActionFadeOut * e2d::action::FadeOut(double duration)
{
	return new (std::nothrow) ActionFadeOut(duration);
}

e2d::ActionRotateBy * e2d::action::RotateBy(double duration, double rotation)
{
	return new (std::nothrow) ActionRotateBy(duration, rotation);
}

e2d::ActionRotateTo * e2d::action::RotateTo(double duration, double rotation)
{
	return new (std::nothrow) ActionRotateTo(duration, rotation);
}

e2d::ActionTwo * e2d::action::Two(Action * pActionFirst, Action * pActionSecond, bool bAtSameTime)
{
	return new (std::nothrow) ActionTwo(pActionFirst, pActionSecond, bAtSameTime);
}

e2d::ActionSequence * e2d::action::Sequence(int number, Action * action1, ...)
{
	auto action = new (std::nothrow) ActionSequence();
	if (action)
	{
		Action ** ppAction = &action1;

		while (number > 0)
		{
			ASSERT((*ppAction) != nullptr, "ActionSequence NULL pointer exception!");
			action->add(*ppAction);
			ppAction++;
			number--;
		}
	}
	return action;
}

e2d::ActionDelay * e2d::action::Delay(double duration)
{
	return new (std::nothrow) ActionDelay(duration);
}

e2d::ActionLoop * e2d::action::Loop(Action * action, int times)
{
	return new (std::nothrow) ActionLoop(action, times);
}

e2d::Animation * e2d::action::Animate(double interval, int number, Image * frame, ...)
{
	auto animation = new (std::nothrow) Animation(interval);
	if (animation)
	{
		Image ** ppImage = &frame;

		while (number > 0)
		{
			ASSERT((*ppImage) != nullptr, "Animation NULL pointer exception!");
			animation->addKeyframe(*ppImage);
			ppImage++;
			number--;
		}
	}
	return animation;
}

e2d::ActionFunc * e2d::action::Func(Function func)
{
	return new (std::nothrow) ActionFunc(func);
}
