#include "..\e2daction.h"

e2d::ActionMoveBy * e2d::Action::MoveBy(double duration, Vector vector)
{
	return new (std::nothrow) ActionMoveBy(duration, vector);
}

e2d::ActionMoveTo * e2d::Action::MoveTo(double duration, Point pos)
{
	return new (std::nothrow) ActionMoveTo(duration, pos);
}

e2d::ActionScaleBy * e2d::Action::ScaleBy(double duration, double scale)
{
	return new (std::nothrow) ActionScaleBy(duration, scale);
}

e2d::ActionScaleBy * e2d::Action::ScaleBy(double duration, double scaleX, double scaleY)
{
	return new (std::nothrow) ActionScaleBy(duration, scaleX, scaleY);
}

e2d::ActionScaleTo * e2d::Action::ScaleTo(double duration, double scale)
{
	return new (std::nothrow) ActionScaleTo(duration, scale);
}

e2d::ActionScaleTo * e2d::Action::ScaleTo(double duration, double scaleX, double scaleY)
{
	return new (std::nothrow) ActionScaleTo(duration, scaleX, scaleY);
}

e2d::ActionOpacityBy * e2d::Action::OpacityBy(double duration, double opacity)
{
	return new (std::nothrow) ActionOpacityBy(duration, opacity);
}

e2d::ActionOpacityTo * e2d::Action::OpacityTo(double duration, double opacity)
{
	return new (std::nothrow) ActionOpacityTo(duration, opacity);
}

e2d::ActionFadeIn * e2d::Action::FadeIn(double duration)
{
	return new (std::nothrow) ActionFadeIn(duration);
}

e2d::ActionFadeOut * e2d::Action::FadeOut(double duration)
{
	return new (std::nothrow) ActionFadeOut(duration);
}

e2d::ActionRotateBy * e2d::Action::RotateBy(double duration, double rotation)
{
	return new (std::nothrow) ActionRotateBy(duration, rotation);
}

e2d::ActionRotateTo * e2d::Action::RotateTo(double duration, double rotation)
{
	return new (std::nothrow) ActionRotateTo(duration, rotation);
}

e2d::ActionTwo * e2d::Action::Two(ActionBase * pActionFirst, ActionBase * pActionSecond, bool bAtSameTime)
{
	return new (std::nothrow) ActionTwo(pActionFirst, pActionSecond, bAtSameTime);
}

e2d::ActionDelay * e2d::Action::Delay(double duration)
{
	return new (std::nothrow) ActionDelay(duration);
}

e2d::ActionLoop * e2d::Action::Loop(ActionBase * action, int times)
{
	return new (std::nothrow) ActionLoop(action, times);
}

e2d::ActionFunc * e2d::Action::Func(Function func)
{
	return new (std::nothrow) ActionFunc(func);
}

#ifdef HIGHER_THAN_VS2012

e2d::ActionSequence * e2d::Action::Sequence(const InitList<ActionBase*>& vActions)
{
	return new (std::nothrow) ActionSequence(vActions);
}

e2d::Animation * e2d::Action::Animation(double interval, const InitList<Image*>& vFrames)
{
	return new (std::nothrow) e2d::Animation(interval, vFrames);
}

#else

e2d::ActionSequence * e2d::Action::Sequence(int number, ActionBase * action1, ...)
{
	auto action = new (std::nothrow) ActionSequence();
	if (action)
	{
		ActionBase ** ppAction = &action1;

		while (number > 0)
		{
			WARN_IF((*ppAction) == nullptr, "ActionSequence NULL pointer exception!");
			action->add(*ppAction);
			ppAction++;
			number--;
		}
	}
	return action;
}

e2d::Animation * e2d::Action::Animation(double interval, int number, Image * frame, ...)
{
	auto animation = new (std::nothrow) e2d::Animation(interval);
	if (animation)
	{
		Image ** ppImage = &frame;

		while (number > 0)
		{
			WARN_IF((*ppImage) == nullptr, "Animation NULL pointer exception!");
			animation->add(*ppImage);
			ppImage++;
			number--;
		}
	}
	return animation;
}

#endif