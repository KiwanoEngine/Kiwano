#include "..\etools.h"

e2d::EMouseClickListener::EMouseClickListener()
	: EMouseListener()
{
}

e2d::EMouseClickListener::EMouseClickListener(EString name)
	: EMouseListener(name)
{
}

e2d::EMouseClickListener::EMouseClickListener(const MOUSE_LISTENER_CALLBACK & callback)
	: EMouseListener(callback)
{
}

e2d::EMouseClickListener::EMouseClickListener(EString name, const MOUSE_LISTENER_CALLBACK & callback)
	: EMouseListener(name, callback)
{
}

void e2d::EMouseClickListener::runCallback()
{
	if (EMouseMsg::getMsg() == EMouseMsg::LBUTTON_DOWN || 
		EMouseMsg::getMsg() == EMouseMsg::LBUTTON_DBLCLK)
	{
		EMouseListener::runCallback();
	}
}
