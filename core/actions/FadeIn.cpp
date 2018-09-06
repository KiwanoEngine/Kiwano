#include "..\e2daction.h"

e2d::FadeIn::FadeIn(float duration)
	: OpacityTo(duration, 1)
{
}
