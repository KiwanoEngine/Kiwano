#include "..\e2dtransition.h"
#include "..\e2dobject.h"

e2d::EmergeTransition::EmergeTransition(Scene* scene, float duration)
	: Transition(scene, duration)
{
}

bool e2d::EmergeTransition::Init(Game * game, Scene * prev)
{
	if (Transition::Init(game, prev))
	{
		out_layer_param_.opacity = 1;
		in_layer_param_.opacity = 0;
		return true;
	}
	return false;
}

void e2d::EmergeTransition::Update()
{
	Transition::Update();

	out_layer_param_.opacity = 1 - delta_;
	in_layer_param_.opacity = delta_;

	if (delta_ >= 1)
	{
		this->Stop();
	}
}
