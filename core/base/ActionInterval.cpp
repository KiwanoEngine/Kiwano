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

#include "ActionInterval.h"
#include "Geometry.h"
#include "base.hpp"
#include "Node.h"
#include <algorithm>
#include <cfloat>

namespace easy2d
{
	//-------------------------------------------------------
	// IntervalAction
	//-------------------------------------------------------

	IntervalAction::IntervalAction()
		: elapsed_()
		, duration_()
	{
	}

	IntervalAction::IntervalAction(Duration const& duration)
		: elapsed_()
	{
		SetDuration(duration);
	}

	void IntervalAction::Reset()
	{
		Action::Reset();
		elapsed_ = Duration{};
	}

	Duration const & IntervalAction::GetDuration() const
	{
		return duration_;
	}

	void IntervalAction::Init(Node* target)
	{
		Action::Init(target);
	}

	void IntervalAction::Update(Node* target, Duration const& dt)
	{
		Action::Update(target, dt);

		float percent;
		if (duration_.IsZero())
		{
			percent = 1.f;
		}
		else
		{
			elapsed_ += dt;
			percent = std::min(elapsed_ / duration_, 1.f);
		}

		if ((1.f - percent) <= FLT_EPSILON)
		{
			this->Stop();
		}

		this->Update(target, percent);
	}

	void IntervalAction::SetDuration(Duration const & duration)
	{
		duration_ = duration;
	}


	//-------------------------------------------------------
	// Move Action
	//-------------------------------------------------------

	MoveBy::MoveBy(Duration const& duration, Point const& vector)
		: IntervalAction(duration)
	{
		delta_pos_ = vector;
	}

	void MoveBy::Init(Node* target)
	{
		IntervalAction::Init(target);

		if (target)
		{
			prev_pos_ = start_pos_ = target->GetPosition();
		}
	}

	void MoveBy::Update(Node* target, float percent)
	{
		if (target)
		{
			Point diff = target->GetPosition() - prev_pos_;
			start_pos_ = start_pos_ + diff;

			Point new_pos = start_pos_ + (delta_pos_ * percent);
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
		: IntervalAction(duration)
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
		IntervalAction::Init(target);

		if (target)
		{
			prev_pos_ = start_pos_ = target->GetPosition();
		}
	}

	void JumpBy::Update(Node* target, float percent)
	{
		if (target)
		{
			float frac = fmod(percent * jumps_, 1.f);
			float x = delta_pos_.x * percent;
			float y = height_ * 4 * frac * (1 - frac);
			y += delta_pos_.y * percent;

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
		: IntervalAction(duration)
	{
		delta_x_ = scale;
		delta_y_ = scale;
	}

	ScaleBy::ScaleBy(Duration const& duration, float scale_x, float scale_y)
		: IntervalAction(duration)
	{
		delta_x_ = scale_x;
		delta_y_ = scale_y;
	}

	void ScaleBy::Init(Node* target)
	{
		IntervalAction::Init(target);

		if (target)
		{
			start_scale_x_ = target->GetScaleX();
			start_scale_y_ = target->GetScaleY();
		}
	}

	void ScaleBy::Update(Node* target, float percent)
	{
		if (target)
		{
			target->SetScale(start_scale_x_ + delta_x_ * percent, start_scale_y_ + delta_y_ * percent);
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
		: IntervalAction(duration)
	{
		delta_val_ = opacity;
	}

	void OpacityBy::Init(Node* target)
	{
		IntervalAction::Init(target);

		if (target)
		{
			start_val_ = target->GetOpacity();
		}
	}

	void OpacityBy::Update(Node* target, float percent)
	{
		if (target)
		{
			target->SetOpacity(start_val_ + delta_val_ * percent);
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
		: IntervalAction(duration)
		, delta_val_(rotation)
	{
	}

	void RotateBy::Init(Node* target)
	{
		IntervalAction::Init(target);

		if (target)
		{
			start_val_ = target->GetRotation();
		}
	}

	void RotateBy::Update(Node* target, float percent)
	{
		if (target)
		{
			target->SetRotation(start_val_ + delta_val_ * percent);
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
	// PathAction
	//-------------------------------------------------------

	PathAction::PathAction(Duration const & duration, spGeometry const& geo, bool rotating, float start, float end)
		: IntervalAction(duration)
		, start_(start)
		, end_(end)
		, geo_(geo)
		, rotating_(rotating)
	{
	}

	spAction PathAction::Clone() const
	{
		return new PathAction(duration_, geo_, rotating_, start_, end_);
	}

	spAction PathAction::Reverse() const
	{
		return new PathAction(duration_, geo_, rotating_, end_, start_);
	}

	void PathAction::Update(Node* target, float percent)
	{
		if (target)
		{
			float length = geo_->GetLength() * std::min(std::max((end_ - start_) * percent + start_, 0.f), 1.f);

			Point point, tangent;
			if (geo_->ComputePointAt(length, &point, &tangent))
			{
				target->SetPosition(point);

				if (rotating_)
				{
					float ac = math::Acos(tangent.x);
					float rotation = (tangent.y < 0.f) ? 360.f - ac : ac;
					target->SetRotation(rotation);
				}
			}
		}
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
