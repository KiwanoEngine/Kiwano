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

	void FiniteTimeAction::Initialize()
	{
		Action::Initialize();
	}

	void FiniteTimeAction::Update()
	{
		Action::Update();

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

	void MoveBy::Initialize()
	{
		FiniteTimeAction::Initialize();

		if (target_)
		{
			prev_pos_ = start_pos_ = target_->GetPosition();
		}
	}

	void MoveBy::Update()
	{
		FiniteTimeAction::Update();

		if (target_)
		{
			Point currentPos = target_->GetPosition();
			Point diff = currentPos - prev_pos_;
			start_pos_ = start_pos_ + diff;

			Point newPos = start_pos_ + (delta_pos_ * delta_);
			target_->SetPosition(newPos);

			prev_pos_ = newPos;
		}
	}

	MoveBy * MoveBy::Clone() const
	{
		return new MoveBy(duration_, delta_pos_);
	}

	MoveBy * MoveBy::Reverse() const
	{
		return new MoveBy(duration_, -delta_pos_);
	}

	MoveTo::MoveTo(float duration, Point pos)
		: MoveBy(duration, Point())
	{
		end_pos_ = pos;
	}

	MoveTo * MoveTo::Clone() const
	{
		return new MoveTo(duration_, end_pos_);
	}

	void MoveTo::Initialize()
	{
		MoveBy::Initialize();
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

	JumpBy * JumpBy::Clone() const
	{
		return new JumpBy(duration_, delta_pos_, height_, jumps_);
	}

	JumpBy * JumpBy::Reverse() const
	{
		return new JumpBy(duration_, -delta_pos_, height_, jumps_);
	}

	void JumpBy::Initialize()
	{
		FiniteTimeAction::Initialize();

		if (target_)
		{
			prev_pos_ = start_pos_ = target_->GetPosition();
		}
	}

	void JumpBy::Update()
	{
		FiniteTimeAction::Update();

		if (target_)
		{
			float frac = fmod(delta_ * jumps_, 1.f);
			float x = delta_pos_.x * delta_;
			float y = height_ * 4 * frac * (1 - frac);
			y += delta_pos_.y * delta_;

			Point currentPos = target_->GetPosition();

			Point diff = currentPos - prev_pos_;
			start_pos_ = diff + start_pos_;

			Point newPos = start_pos_ + Point(x, y);
			target_->SetPosition(newPos);

			prev_pos_ = newPos;
		}
	}

	JumpTo::JumpTo(float duration, const Point & pos, float height, int jumps)
		: JumpBy(duration, Point(), height, jumps)
		, end_pos_(pos)
	{
	}

	JumpTo * JumpTo::Clone() const
	{
		return new JumpTo(duration_, end_pos_, height_, jumps_);
	}

	void JumpTo::Initialize()
	{
		JumpBy::Initialize();
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

	void ScaleBy::Initialize()
	{
		FiniteTimeAction::Initialize();

		if (target_)
		{
			start_scale_x_ = target_->GetScaleX();
			start_scale_y_ = target_->GetScaleY();
		}
	}

	void ScaleBy::Update()
	{
		FiniteTimeAction::Update();

		if (target_)
		{
			target_->SetScale(start_scale_x_ + delta_x_ * delta_, start_scale_y_ + delta_y_ * delta_);
		}
	}

	ScaleBy * ScaleBy::Clone() const
	{
		return new ScaleBy(duration_, delta_x_, delta_y_);
	}

	ScaleBy * ScaleBy::Reverse() const
	{
		return new ScaleBy(duration_, -delta_x_, -delta_y_);
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

	ScaleTo * ScaleTo::Clone() const
	{
		return new ScaleTo(duration_, end_scale_x_, end_scale_y_);
	}

	void ScaleTo::Initialize()
	{
		ScaleBy::Initialize();
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

	void OpacityBy::Initialize()
	{
		FiniteTimeAction::Initialize();

		if (target_)
		{
			start_val_ = target_->GetOpacity();
		}
	}

	void OpacityBy::Update()
	{
		FiniteTimeAction::Update();

		if (target_)
		{
			target_->SetOpacity(start_val_ + delta_val_ * delta_);
		}
	}

	OpacityBy * OpacityBy::Clone() const
	{
		return new OpacityBy(duration_, delta_val_);
	}

	OpacityBy * OpacityBy::Reverse() const
	{
		return new OpacityBy(duration_, -delta_val_);
	}

	OpacityTo::OpacityTo(float duration, float opacity)
		: OpacityBy(duration, 0)
	{
		end_val_ = opacity;
	}

	OpacityTo * OpacityTo::Clone() const
	{
		return new OpacityTo(duration_, end_val_);
	}

	void OpacityTo::Initialize()
	{
		OpacityBy::Initialize();
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

	void RotateBy::Initialize()
	{
		FiniteTimeAction::Initialize();

		if (target_)
		{
			start_val_ = target_->GetRotation();
		}
	}

	void RotateBy::Update()
	{
		FiniteTimeAction::Update();

		if (target_)
		{
			target_->SetRotation(start_val_ + delta_val_ * delta_);
		}
	}

	RotateBy * RotateBy::Clone() const
	{
		return new RotateBy(duration_, delta_val_);
	}

	RotateBy * RotateBy::Reverse() const
	{
		return new RotateBy(duration_, -delta_val_);
	}

	RotateTo::RotateTo(float duration, float rotation)
		: RotateBy(duration, 0)
	{
		end_val_ = rotation;
	}

	RotateTo * RotateTo::Clone() const
	{
		return new RotateTo(duration_, end_val_);
	}

	void RotateTo::Initialize()
	{
		RotateBy::Initialize();
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

	void Delay::Initialize()
	{
		Action::Initialize();
	}

	void Delay::Update()
	{
		Action::Update();

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

	Delay * Delay::Clone() const
	{
		return new Delay(delay_);
	}

	Delay * Delay::Reverse() const
	{
		return new Delay(delay_);
	}
}
