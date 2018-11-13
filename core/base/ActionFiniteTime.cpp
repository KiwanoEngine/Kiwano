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
#include "base.h"
#include "Node.h"
#include <algorithm>

namespace easy2d
{
	//-------------------------------------------------------
	// FiniteTimeAction
	//-------------------------------------------------------

	FiniteTimeAction::FiniteTimeAction(float duration)
		: delta_(0)
		, duration_(std::max(duration, 0.f))
	{
	}

	void FiniteTimeAction::Reset()
	{
		Action::Reset();
		delta_ = 0;
	}

	void FiniteTimeAction::Init(Node* target)
	{
		Action::Init(target);
	}

	void FiniteTimeAction::Update(Node* target)
	{
		Action::Update(target);

		if (duration_ == 0)
		{
			delta_ = 1;
			this->Stop();
		}
		else
		{
			delta_ = std::min((time::Now() - started_).Seconds() / duration_, 1.f);

			if (delta_ >= 1)
			{
				this->Stop();
			}
		}
	}

	void FiniteTimeAction::ResetTime()
	{
		Action::ResetTime();
		started_ = time::Now() - time::Second * (delta_ * duration_);
	}


	//-------------------------------------------------------
	// Move Action
	//-------------------------------------------------------

	MoveBy::MoveBy(float duration, Point vector)
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

	void MoveBy::Update(Node* target)
	{
		FiniteTimeAction::Update(target);

		if (target)
		{
			Point currentPos = target->GetPosition();
			Point diff = currentPos - prev_pos_;
			start_pos_ = start_pos_ + diff;

			Point newPos = start_pos_ + (delta_pos_ * delta_);
			target->SetPosition(newPos);

			prev_pos_ = newPos;
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

	MoveTo::MoveTo(float duration, Point pos)
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

	JumpBy::JumpBy(float duration, const Point & vec, float height, int jumps)
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

	void JumpBy::Update(Node* target)
	{
		FiniteTimeAction::Update(target);

		if (target)
		{
			float frac = fmod(delta_ * jumps_, 1.f);
			float x = delta_pos_.x * delta_;
			float y = height_ * 4 * frac * (1 - frac);
			y += delta_pos_.y * delta_;

			Point currentPos = target->GetPosition();

			Point diff = currentPos - prev_pos_;
			start_pos_ = diff + start_pos_;

			Point newPos = start_pos_ + Point(x, y);
			target->SetPosition(newPos);

			prev_pos_ = newPos;
		}
	}

	JumpTo::JumpTo(float duration, const Point & pos, float height, int jumps)
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

	ScaleBy::ScaleBy(float duration, float scale)
		: FiniteTimeAction(duration)
	{
		delta_x_ = scale;
		delta_y_ = scale;
	}

	ScaleBy::ScaleBy(float duration, float scale_x, float scale_y)
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

	void ScaleBy::Update(Node* target)
	{
		FiniteTimeAction::Update(target);

		if (target)
		{
			target->SetScale(start_scale_x_ + delta_x_ * delta_, start_scale_y_ + delta_y_ * delta_);
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

	ScaleTo::ScaleTo(float duration, float scale)
		: ScaleBy(duration, 0, 0)
	{
		end_scale_x_ = scale;
		end_scale_y_ = scale;
	}

	ScaleTo::ScaleTo(float duration, float scale_x, float scale_y)
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

	OpacityBy::OpacityBy(float duration, float opacity)
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

	void OpacityBy::Update(Node* target)
	{
		FiniteTimeAction::Update(target);

		if (target)
		{
			target->SetOpacity(start_val_ + delta_val_ * delta_);
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

	OpacityTo::OpacityTo(float duration, float opacity)
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

	FadeIn::FadeIn(float duration)
		: OpacityTo(duration, 1)
	{
	}

	FadeOut::FadeOut(float duration)
		: OpacityTo(duration, 0)
	{
	}


	//-------------------------------------------------------
	// Rotate Action
	//-------------------------------------------------------

	RotateBy::RotateBy(float duration, float rotation)
		: FiniteTimeAction(duration)
	{
		delta_val_ = rotation;
	}

	void RotateBy::Init(Node* target)
	{
		FiniteTimeAction::Init(target);

		if (target)
		{
			start_val_ = target->GetRotation();
		}
	}

	void RotateBy::Update(Node* target)
	{
		FiniteTimeAction::Update(target);

		if (target)
		{
			target->SetRotation(start_val_ + delta_val_ * delta_);
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

	RotateTo::RotateTo(float duration, float rotation)
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

	Delay::Delay(float duration)
		: delta_(0)
		, delay_(std::max(duration, 0.f))
	{
	}

	void Delay::Reset()
	{
		Action::Reset();
		delta_ = 0;
	}

	void Delay::Init(Node* target)
	{
		Action::Init(target);
	}

	void Delay::Update(Node* target)
	{
		Action::Update(target);

		delta_ = (time::Now() - started_).Seconds();

		if (delta_ >= delay_)
		{
			this->Stop();
		}
	}

	void Delay::ResetTime()
	{
		Action::ResetTime();
		started_ = time::Now() - time::Second * delta_;
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
