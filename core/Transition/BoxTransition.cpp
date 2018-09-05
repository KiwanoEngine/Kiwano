#include "..\e2dtransition.h"
#include "..\e2dnode.h"
#include "..\e2dmodule.h"

e2d::BoxTransition::BoxTransition(Scene* scene, float duration)
	: Transition(scene, duration)
{
}

bool e2d::BoxTransition::Init(Game * game, Scene * prev)
{
	if (Transition::Init(game, prev))
	{
		in_layer_param_.opacity = 0;
		return true;
	}
	return false;
}

void e2d::BoxTransition::Update()
{
	Transition::Update();

	auto size = Window::GetInstance()->GetSize();
	if (delta_ <= 0.5)
	{
		out_layer_param_.contentBounds = D2D1::RectF(
			size.width * delta_,
			size.height * delta_,
			size.width * (1 - delta_),
			size.height * (1 - delta_)
		);
	}
	else
	{
		out_layer_param_.opacity = 0;
		in_layer_param_.opacity = 1;
		in_layer_param_.contentBounds = D2D1::RectF(
			size.width * (1 - delta_),
			size.height * (1 - delta_),
			size.width * delta_,
			size.height * delta_
		);
		if (delta_ >= 1)
		{
			this->Stop();
		}
	}
}
