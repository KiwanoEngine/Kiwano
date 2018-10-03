#include "..\e2dtransition.h"
#include "..\e2dobject.h"

e2d::MoveTransition::MoveTransition(float duration, Direction direction)
	: Transition(duration)
	, direction_(direction)
{
}

void e2d::MoveTransition::Init(Scene * prev, Scene * next, Game * game)
{
	Transition::Init(prev, next, game);
	
	switch (direction_)
	{
	case Direction::Up:
		pos_delta_ = Point(0, -window_size_.height);
		start_pos_ = Point(0, window_size_.height);
		break;
	case Direction::Down:
		pos_delta_ = Point(0, window_size_.height);
		start_pos_ = Point(0, -window_size_.height);
		break;
	case Direction::Left:
		pos_delta_ = Point(-window_size_.width, 0);
		start_pos_ = Point(window_size_.width, 0);
		break;
	case Direction::Right:
		pos_delta_ = Point(window_size_.width, 0);
		start_pos_ = Point(-window_size_.width, 0);
		break;
	}

	if (out_scene_)
	{
		out_scene_->SetTransform(D2D1::Matrix3x2F::Identity());
	}
	
	if (in_scene_)
	{
		in_scene_->SetTransform(
			D2D1::Matrix3x2F::Translation(
				start_pos_.x,
				start_pos_.y
			)
		);
	}
}

void e2d::MoveTransition::Update()
{
	Transition::Update();

	if (out_scene_)
	{
		auto translation = pos_delta_ * process_;
		out_scene_->SetTransform(
			D2D1::Matrix3x2F::Translation(
				translation.x,
				translation.y
			)
		);
	}

	if (in_scene_)
	{
		auto translation = start_pos_ + pos_delta_ * process_;
		in_scene_->SetTransform(
			D2D1::Matrix3x2F::Translation(
				translation.x,
				translation.y
			)
		);
	}
}

void e2d::MoveTransition::Reset()
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
