// Copyright (c) 2016-2018 Easy2D - Nomango
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "ActionFiniteTime.h"
#include "base.hpp"
#include "Node.h"
#include <algorithm>

namespace easy2d
{
	//-------------------------------------------------------
	// FiniteTimeAction
	//-------------------------------------------------------

	FiniteTimeAction::FiniteTimeAction(Duration const& duration)
		: process_(0)
		, duration_(duration)
	{
	}

	void FiniteTimeAction::Reset()
	{
		Action::Reset();
		process_ = 0;
	}

	void FiniteTimeAction::Init(Node* target)
	{
		Action::Init(target);
	}

	void FiniteTimeAction::Update(Node* target, Duration const& dt)
	{
		Action::Update(target, dt);

		if (duration_.IsZero())
		{
			process_ = 1.f;
			this->Stop();
		}
		else
		{
			process_ += dt / duration_;
			process_ = std::min(process_, 1.f);

			if (process_ >= 1)
			{
				this->Stop();
			}
		}
	}


	//-------------------------------------------------------
	// Move Action
	//-------------------------------------------------------

	MoveBy::MoveBy(Duration const& duration, Point const& vector)
		: FiniteTimeAction(duration)
	{
		delta_pos_ = vector;
	}

	void MoveBy::Init(Node* target)
	{
		FiniteTimeAction::Init(target);

		if (target)
		{
			prev_pos_ = start_pos_ = target->GetPosition();
		}
	}

	void MoveBy::Update(Node* target, Duration const& dt)
	{
		FiniteTimeAction::Update(target, dt);

		if (target)
		{
			Point diff = target->GetPosition() - prev_pos_;
			start_pos_ = start_pos_ + diff;

			Point new_pos = start_pos_ + (delta_pos_ * process_);
			target->SetPosition(new_pos);

			prev_pos_ = new_pos;
		}
	}

	spAction MoveBy::Clone() const
	{
		return new (std::nothrow) MoveBy(duration_, delta_pos_);
	}

	spAction MoveBy::Reverse() const
	{
		return new (std::nothrow) MoveBy(duration_, -delta_pos_);
	}

	MoveTo::MoveTo(Duration const& duration, Point const& pos)
		: MoveBy(duration, Point())
	{
		end_pos_ = pos;
	}

	spAction MoveTo::Clone() const
	{
		return new (std::nothrow) MoveTo(duration_, end_pos_);
	}

	void MoveTo::Init(Node* target)
	{
		MoveBy::Init(target);
		delta_pos_ = end_pos_ - start_pos_;
	}


	//-------------------------------------------------------
	// Jump Action
	//-------------------------------------------------------

	JumpBy::JumpBy(Duration const& duration, Point const& vec, float height, int jumps)
		: FiniteTimeAction(duration)
		, delta_pos_(vec)
		, height_(height)
		, jumps_(jumps)
	{
	}

	spAction JumpBy::Clone() const
	{
		return new (std::nothrow) JumpBy(duration_, delta_pos_, height_, jumps_);
	}

	spAction JumpBy::Reverse() const
	{
		return new (std::nothrow) JumpBy(duration_, -delta_pos_, height_, jumps_);
	}

	void JumpBy::Init(Node* target)
	{
		FiniteTimeAction::Init(target);

		if (target)
		{
			prev_pos_ = start_pos_ = target->GetPosition();
		}
	}

	void JumpBy::Update(Node* target, Duration const& dt)
	{
		FiniteTimeAction::Update(target, dt);

		if (target)
		{
			float frac = fmod(process_ * jumps_, 1.f);
			float x = delta_pos_.x * process_;
			float y = height_ * 4 * frac * (1 - frac);
			y += delta_pos_.y * process_;

			Point diff = target->GetPosition() - prev_pos_;
			start_pos_ = diff + start_pos_;

			Point new_pos = start_pos_ + Point(x, y);
			target->SetPosition(new_pos);

			prev_pos_ = new_pos;
		}
	}

	JumpTo::JumpTo(Duration const& duration, Point const& pos, float height, int jumps)
		: JumpBy(duration, Point(), height, jumps)
		, end_pos_(pos)
	{
	}

	spAction JumpTo::Clone() const
	{
		return new (std::nothrow) JumpTo(duration_, end_pos_, height_, jumps_);
	}

	void JumpTo::Init(Node* target)
	{
		JumpBy::Init(target);
		delta_pos_ = end_pos_ - start_pos_;
	}


	//-------------------------------------------------------
	// Scale Action
	//-------------------------------------------------------

	ScaleBy::ScaleBy(Duration const& duration, float scale)
		: FiniteTimeAction(duration)
	{
		delta_x_ = scale;
		delta_y_ = scale;
	}

	ScaleBy::ScaleBy(Duration const& duration, float scale_x, float scale_y)
		: FiniteTimeAction(duration)
	{
		delta_x_ = scale_x;
		delta_y_ = scale_y;
	}

	void ScaleBy::Init(Node* target)
	{
		FiniteTimeAction::Init(target);

		if (target)
		{
			start_scale_x_ = target->GetScaleX();
			start_scale_y_ = target->GetScaleY();
		}
	}

	void ScaleBy::Update(Node* target, Duration const& dt)
	{
		FiniteTimeAction::Update(target, dt);

		if (target)
		{
			target->SetScale(start_scale_x_ + delta_x_ * process_, start_scale_y_ + delta_y_ * process_);
		}
	}

	spAction ScaleBy::Clone() const
	{
		return new (std::nothrow) ScaleBy(duration_, delta_x_, delta_y_);
	}

	spAction ScaleBy::Reverse() const
	{
		return new (std::nothrow) ScaleBy(duration_, -delta_x_, -delta_y_);
	}

	ScaleTo::ScaleTo(Duration const& duration, float scale)
		: ScaleBy(duration, 0, 0)
	{
		end_scale_x_ = scale;
		end_scale_y_ = scale;
	}

	ScaleTo::ScaleTo(Duration const& duration, float scale_x, float scale_y)
		: ScaleBy(duration, 0, 0)
	{
		end_scale_x_ = scale_x;
		end_scale_y_ = scale_y;
	}

	spAction ScaleTo::Clone() const
	{
		return new (std::nothrow) ScaleTo(duration_, end_scale_x_, end_scale_y_);
	}

	void ScaleTo::Init(Node* target)
	{
		ScaleBy::Init(target);
		delta_x_ = end_scale_x_ - start_scale_x_;
		delta_y_ = end_scale_y_ - start_scale_y_;
	}


	//-------------------------------------------------------
	// Opacity Action
	//-------------------------------------------------------

	OpacityBy::OpacityBy(Duration const& duration, float opacity)
		: FiniteTimeAction(duration)
	{
		delta_val_ = opacity;
	}

	void OpacityBy::Init(Node* target)
	{
		FiniteTimeAction::Init(target);

		if (target)
		{
			start_val_ = target->GetOpacity();
		}
	}

	void OpacityBy::Update(Node* target, Duration const& dt)
	{
		FiniteTimeAction::Update(target, dt);

		if (target)
		{
			target->SetOpacity(start_val_ + delta_val_ * process_);
		}
	}

	spAction OpacityBy::Clone() const
	{
		return new (std::nothrow) OpacityBy(duration_, delta_val_);
	}

	spAction OpacityBy::Reverse() const
	{
		return new (std::nothrow) OpacityBy(duration_, -delta_val_);
	}

	OpacityTo::OpacityTo(Duration const& duration, float opacity)
		: OpacityBy(duration, 0)
	{
		end_val_ = opacity;
	}

	spAction OpacityTo::Clone() const
	{
		return new (std::nothrow) OpacityTo(duration_, end_val_);
	}

	void OpacityTo::Init(Node* target)
	{
		OpacityBy::Init(target);
		delta_val_ = end_val_ - start_val_;
	}

	FadeIn::FadeIn(Duration const& duration)
		: OpacityTo(duration, 1)
	{
	}

	FadeOut::FadeOut(Duration const& duration)
		: OpacityTo(duration, 0)
	{
	}


	//-------------------------------------------------------
	// Rotate Action
	//-------------------------------------------------------

	RotateBy::RotateBy(Duration const& duration, float rotation)
		: FiniteTimeAction(duration)
		, delta_val_(rotation)
	{
	}

	void RotateBy::Init(Node* target)
	{
		FiniteTimeAction::Init(target);

		if (target)
		{
			start_val_ = target->GetRotation();
		}
	}

	void RotateBy::Update(Node* target, Duration const& dt)
	{
		FiniteTimeAction::Update(target, dt);

		if (target)
		{
			target->SetRotation(start_val_ + delta_val_ * process_);
		}
	}

	spAction RotateBy::Clone() const
	{
		return new (std::nothrow) RotateBy(duration_, delta_val_);
	}

	spAction RotateBy::Reverse() const
	{
		return new (std::nothrow) RotateBy(duration_, -delta_val_);
	}

	RotateTo::RotateTo(Duration const& duration, float rotation)
		: RotateBy(duration, 0)
	{
		end_val_ = rotation;
	}

	spAction RotateTo::Clone() const
	{
		return new (std::nothrow) RotateTo(duration_, end_val_);
	}

	void RotateTo::Init(Node* target)
	{
		RotateBy::Init(target);
		delta_val_ = end_val_ - start_val_;
	}


	//-------------------------------------------------------
	// Delay
	//-------------------------------------------------------

	Delay::Delay(Duration const& duration)
		: delta_()
		, delay_(duration)
	{
	}

	void Delay::Reset()
	{
		Action::Reset();
		delta_ = Duration{};
	}

	void Delay::Init(Node* target)
	{
		Action::Init(target);
	}

	void Delay::Update(Node* target, Duration const& dt)
	{
		Action::Update(target, dt);

		delta_ += dt;

		if (delta_ >= delay_)
		{
			this->Stop();
		}
	}

	spAction Delay::Clone() const
	{
		return new (std::nothrow) Delay(delay_);
	}

	spAction Delay::Reverse() const
	{
		return new (std::nothrow) Delay(delay_);
	}
}
