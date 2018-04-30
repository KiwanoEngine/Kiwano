#include "..\e2dtransition.h"

e2d::TransitionFade * e2d::Transition::Fade(double duration)
{
	return new (std::nothrow) TransitionFade(duration);
}

e2d::TransitionFade * e2d::Transition::Fade(double fadeOutDuration, double fadeInDuration)
{
	return new (std::nothrow) TransitionFade(fadeOutDuration, fadeInDuration);
}

e2d::TransitionEmerge * e2d::Transition::Emerge(double duration)
{
	return new (std::nothrow) TransitionEmerge(duration);
}

e2d::TransitionMove * e2d::Transition::Move(double duration, Direct direct)
{
	return new (std::nothrow) TransitionMove(duration, direct);
}
