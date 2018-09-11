#include "..\e2dtransition.h"
#include "..\e2dobject.h"

e2d::BoxTransition::BoxTransition(float duration)
	: Transition(duration)
{
}

void e2d::BoxTransition::Init(Scene * prev, Scene * next)
{
	Transition::Init(prev, next);

	in_layer_param_.opacity = 0;
}

void e2d::BoxTransition::Update()
{
	Transition::Update();

	if (process_ < .5f)
	{
		out_layer_param_.contentBounds = D2D1::RectF(
			window_size_.width * process_,
			window_size_.height * process_,
			window_size_.width * (1 - process_),
			window_size_.height * (1 - process_)
		);
	}
	else
	{
		out_layer_param_.opacity = 0;
		in_layer_param_.opacity = 1;
		in_layer_param_.contentBounds = D2D1::RectF(
			window_size_.width * (1 - process_),
			window_size_.height * (1 - process_),
			window_size_.width * process_,
			window_size_.height * process_
		);
	}
}
