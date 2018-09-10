#include "..\e2dtransition.h"
#include "..\e2dobject.h"
#include "..\e2dmodule.h"

e2d::MoveTransition::MoveTransition(float duration, Direction direction)
	: Transition(duration)
	, direction_(direction)
{
}

bool e2d::MoveTransition::Init(Scene * prev, Scene * next)
{
	if (Transition::Init(prev, next))
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

		if (out_scene_ && out_scene_->GetRoot()) out_scene_->GetRoot()->SetPos(0, 0);
		if (in_scene_->GetRoot()) in_scene_->GetRoot()->SetPos(start_pos_);
		return true;
	}
	return false;
}

void e2d::MoveTransition::Update()
{
	Transition::Update();

	if (out_scene_ && out_scene_->GetRoot())
	{
		out_scene_->GetRoot()->SetPos(pos_delta_ * delta_);
	}

	if (in_scene_->GetRoot())
	{
		in_scene_->GetRoot()->SetPos(start_pos_ + pos_delta_ * delta_);
	}

	if (delta_ >= 1)
	{
		this->Stop();
	}
}

void e2d::MoveTransition::Reset()
{
	if (out_scene_ && out_scene_->GetRoot())
	{
		out_scene_->GetRoot()->SetPos(0, 0);
	}
	
	if (in_scene_->GetRoot())
	{
		in_scene_->GetRoot()->SetPos(0, 0);
	}
}
