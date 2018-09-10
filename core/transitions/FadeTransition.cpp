#include "..\e2dtransition.h"
#include "..\e2dobject.h"

e2d::FadeTransition::FadeTransition(float duration)
	: Transition(duration)
{
}

bool e2d::FadeTransition::Init(Scene * prev, Scene * next)
{
	if (Transition::Init(prev, next))
	{
		out_layer_param_.opacity = 1;
		in_layer_param_.opacity = 0;
		return true;
	}
	return false;
}

void e2d::FadeTransition::Update()
{
	Transition::Update();

	if (delta_ < 0.5)
	{
		out_layer_param_.opacity = 1 - delta_ * 2;
		in_layer_param_.opacity = 0;
	}
	else
	{
		out_layer_param_.opacity = 0;
		in_layer_param_.opacity = (delta_ - 0.5f) * 2;
		if (delta_ >= 1)
		{
			this->Stop();
		}
	}
}
