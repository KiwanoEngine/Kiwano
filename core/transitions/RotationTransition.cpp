#include "..\e2dtransition.h"
#include "..\e2dobject.h"

e2d::RotationTransition::RotationTransition(float duration, float rotation)
	: Transition(duration)
	, rotation_(rotation)
{
}

void e2d::RotationTransition::Init(Scene * prev, Scene * next)
{
	Transition::Init(prev, next);

	if (out_scene_)
	{
		out_scene_->SetTransform(D2D1::Matrix3x2F::Identity());
	}

	if (in_scene_)
	{
		in_scene_->SetTransform(D2D1::Matrix3x2F::Identity());
	}

	in_layer_param_.opacity = 0;
}

void e2d::RotationTransition::Update()
{
	Transition::Update();

	auto center_pos = D2D1::Point2F(
		window_size_.width / 2,
		window_size_.height / 2
	);

	if (process_ < .5f)
	{
		if (out_scene_)
		{
			out_scene_->SetTransform(
				D2D1::Matrix3x2F::Scale(
					(.5f - process_) * 2,
					(.5f - process_) * 2,
					center_pos
				) * D2D1::Matrix3x2F::Rotation(
					rotation_ * (.5f - process_) * 2,
					center_pos
				)
			);
		}
	}
	else
	{
		if (in_scene_)
		{
			out_layer_param_.opacity = 0;
			in_layer_param_.opacity = 1;

			in_scene_->SetTransform(
				D2D1::Matrix3x2F::Scale(
					(process_ - .5f) * 2,
					(process_ - .5f) * 2,
					center_pos
				) * D2D1::Matrix3x2F::Rotation(
					rotation_ * (process_ - .5f) * 2,
					center_pos
				)
			);
		}
	}
}

void e2d::RotationTransition::Reset()
{
	if (out_scene_)
	{
		out_scene_->SetTransform(D2D1::Matrix3x2F::Identity());
	}
	
	if (in_scene_)
	{
		in_scene_->SetTransform(D2D1::Matrix3x2F::Identity());
	}
}
