#include "..\e2dtransition.h"
#include "..\e2dnode.h"

e2d::MoveTransition::MoveTransition(Scene* scene, float duration, Direction direction)
	: Transition(scene, duration)
	, direction_(direction)
{
}

bool e2d::MoveTransition::Init(Game * game, Scene * prev)
{
	if (Transition::Init(game, prev))
	{
		auto size = Window::GetInstance()->GetSize();
		if (direction_ == Direction::Up)
		{
			pos_delta_ = Point(0, -size.height);
			start_pos_ = Point(0, size.height);
		}
		else if (direction_ == Direction::Down)
		{
			pos_delta_ = Point(0, size.height);
			start_pos_ = Point(0, -size.height);
		}
		else if (direction_ == Direction::Left)
		{
			pos_delta_ = Point(-size.width, 0);
			start_pos_ = Point(size.width, 0);
		}
		else if (direction_ == Direction::Right)
		{
			pos_delta_ = Point(size.width, 0);
			start_pos_ = Point(-size.width, 0);
		}

		if (out_scene_) out_scene_->SetPos(0, 0);
		in_scene_->SetPos(start_pos_);
		return true;
	}
	return false;
}

void e2d::MoveTransition::Update()
{
	Transition::Update();

	if (out_scene_)
	{
		out_scene_->SetPos(pos_delta_ * delta_);
	}
	if (in_scene_)
	{
		in_scene_->SetPos(start_pos_ + pos_delta_ * delta_);
	}

	if (delta_ >= 1)
	{
		this->Stop();
	}
}

void e2d::MoveTransition::Reset()
{
	if (out_scene_) out_scene_->SetPos(0, 0);
	in_scene_->SetPos(0, 0);
}
