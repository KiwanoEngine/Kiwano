#include "..\e2dtransition.h"
#include "..\e2dobject.h"

e2d::FadeTransition::FadeTransition(float duration)
	: Transition(duration)
{
}

void e2d::FadeTransition::Init(Scene * prev, Scene * next)
{
	Transition::Init(prev, next);
	
	out_layer_param_.opacity = 1;
	in_layer_param_.opacity = 0;
}

void e2d::FadeTransition::Update()
{
	Transition::Update();

	if (process_ < 0.5)
	{
		out_layer_param_.opacity = 1 - process_ * 2;
		in_layer_param_.opacity = 0;
	}
	else
	{
		out_layer_param_.opacity = 0;
		in_layer_param_.opacity = (process_ - 0.5f) * 2;
	}
}
