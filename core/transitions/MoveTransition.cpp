#include "..\e2dtransition.h"
#include "..\e2dobject.h"
#include "..\e2dmodule.h"

e2d::MoveTransition::MoveTransition(float duration, Direction direction)
	: Transition(duration)
	, direction_(direction)
{
}

void e2d::MoveTransition::Init(Scene * prev, Scene * next)
{
	Transition::Init(prev, next);
	
	if (direction_ == Direction::Up)
	{
		pos_delta_ = Point(0, -window_size_.height);
		start_pos_ = Point(0, window_size_.height);
	}
	else if (direction_ == Direction::Down)
	{
		pos_delta_ = Point(0, window_size_.height);
		start_pos_ = Point(0, -window_size_.height);
	}
	else if (direction_ == Direction::Left)
	{
		pos_delta_ = Point(-window_size_.width, 0);
		start_pos_ = Point(window_size_.width, 0);
	}
	else if (direction_ == Direction::Right)
	{
		pos_delta_ = Point(window_size_.width, 0);
		start_pos_ = Point(-window_size_.width, 0);
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
		auto translation = pos_delta_ * delta_;
		out_scene_->SetTransform(
			D2D1::Matrix3x2F::Translation(
				translation.x,
				translation.y
			)
		);
	}

	if (in_scene_)
	{
		auto translation = start_pos_ + pos_delta_ * delta_;
		in_scene_->SetTransform(
			D2D1::Matrix3x2F::Translation(
				translation.x,
				translation.y
			)
		);
	}

	if (delta_ >= 1)
	{
		this->Stop();
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
