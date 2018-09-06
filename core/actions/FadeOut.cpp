#include "..\e2daction.h"

e2d::FadeOut::FadeOut(float duration)
	: OpacityTo(duration, 0)
{
}
