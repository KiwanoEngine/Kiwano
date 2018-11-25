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

#include "ActionTween.h"
#include "Geometry.h"
#include "include-forwards.h"
#include "Node.h"
#include <cfloat>

namespace easy2d
{
	//-------------------------------------------------------
	// Tween
	//-------------------------------------------------------

	Tween::Tween()
		: elapsed_()
		, duration_()
		, ease_func_(math::Linear)
		, ease_type_(EaseFunc::Linear)
	{
	}

	Tween::Tween(Duration const& duration, EaseFunc func)
		: elapsed_()
		, ease_func_(math::Linear)
		, ease_type_(EaseFunc::Linear)
	{
		SetDuration(duration);
		SetEaseFunction(func);
	}

	void Tween::SetEaseFunction(EaseFunc func)
	{
		ease_type_ = func;
		switch (func)
		{
		case EaseFunc::Linear:
			ease_func_ = math::Linear;
			break;
		case EaseFunc::EaseIn:
			ease_func_ = MakeEaseIn(2.f);
			break;
		case EaseFunc::EaseOut:
			ease_func_ = MakeEaseOut(2.f);
			break;
		case EaseFunc::EaseInOut:
			ease_func_ = MakeEaseInOut(2.f);
			break;
		case EaseFunc::ExpoIn:
			ease_func_ = math::EaseExponentialIn;
			break;
		case EaseFunc::ExpoOut:
			ease_func_ = math::EaseExponentialOut;
			break;
		case EaseFunc::ExpoInOut:
			ease_func_ = math::EaseExponentialInOut;
			break;
		case EaseFunc::BounceIn:
			ease_func_ = math::EaseBounceIn;
			break;
		case EaseFunc::BounceOut:
			ease_func_ = math::EaseBounceOut;
			break;
		case EaseFunc::BounceInOut:
			ease_func_ = math::EaseBounceInOut;
			break;
		case EaseFunc::ElasticIn:
			ease_func_ = MakeEaseElasticIn(0.3f);
			break;
		case EaseFunc::ElasticOut:
			ease_func_ = MakeEaseElasticOut(0.3f);
			break;
		case EaseFunc::ElasticInOut:
			ease_func_ = MakeEaseElasticInOut(0.3f);
			break;
		case EaseFunc::SineIn:
			ease_func_ = math::EaseSineIn;
			break;
		case EaseFunc::SineOut:
			ease_func_ = math::EaseSineOut;
			break;
		case EaseFunc::SineInOut:
			ease_func_ = math::EaseSineInOut;
			break;
		case EaseFunc::BackIn:
			ease_func_ = math::EaseBackIn;
			break;
		case EaseFunc::BackOut:
			ease_func_ = math::EaseBackOut;
			break;
		case EaseFunc::BackInOut:
			ease_func_ = math::EaseBackInOut;
			break;
		case EaseFunc::QuadIn:
			ease_func_ = math::EaseQuadIn;
			break;
		case EaseFunc::QuadOut:
			ease_func_ = math::EaseQuadOut;
			break;
		case EaseFunc::QuadInOut:
			ease_func_ = math::EaseQuadInOut;
			break;
		case EaseFunc::CubicIn:
			ease_func_ = math::EaseCubicIn;
			break;
		case EaseFunc::CubicOut:
			ease_func_ = math::EaseCubicOut;
			break;
		case EaseFunc::CubicInOut:
			ease_func_ = math::EaseCubicInOut;
			break;
		case EaseFunc::QuartIn:
			ease_func_ = math::EaseQuartIn;
			break;
		case EaseFunc::QuartOut:
			ease_func_ = math::EaseQuartOut;
			break;
		case EaseFunc::QuartInOut:
			ease_func_ = math::EaseQuartInOut;
			break;
		case EaseFunc::QuintIn:
			ease_func_ = math::EaseQuintIn;
			break;
		case EaseFunc::QuintOut:
			ease_func_ = math::EaseQuintOut;
			break;
		case EaseFunc::QuintInOut:
			ease_func_ = math::EaseQuintInOut;
			break;
		default:
			break;
		}
	}

	void Tween::SetEaseFunction(EaseFunction func)
	{
		ease_func_ = func;
		ease_type_ = EaseFunc(-1);
	}

	void Tween::Reset()
	{
		Action::Reset();
		elapsed_ = Duration{};
	}

	Duration const & Tween::GetDuration() const
	{
		return duration_;
	}

	void Tween::Init(Node* target)
	{
		Action::Init(target);
	}

	void Tween::Update(Node* target, Duration const& dt)
	{
		Action::Update(target, dt);

		float step;
		if (duration_.IsZero())
		{
			step = 1.f;
		}
		else
		{
			elapsed_ += dt;
			step = std::min(elapsed_ / duration_, 1.f);
		}

		if ((1.f - step) <= FLT_EPSILON)
		{
			this->Stop();
		}

		UpdateStep(target, ease_func_(step));
	}

	void Tween::SetDuration(Duration const & duration)
	{
		duration_ = duration;
	}


	//-------------------------------------------------------
	// Move Action
	//-------------------------------------------------------

	MoveBy::MoveBy(Duration const& duration, Point const& vector, EaseFunc func)
		: Tween(duration, func)
	{
		delta_pos_ = vector;
	}

	void MoveBy::Init(Node* target)
	{
		Tween::Init(target);

		if (target)
		{
			prev_pos_ = start_pos_ = target->GetPosition();
		}
	}

	void MoveBy::UpdateStep(Node* target, float step)
	{
		if (target)
		{
			Point diff = target->GetPosition() - prev_pos_;
			start_pos_ = start_pos_ + diff;

			Point new_pos = start_pos_ + (delta_pos_ * step);
			target->SetPosition(new_pos);

			prev_pos_ = new_pos;
		}
	}

	spAction MoveBy::Clone() const
	{
		return new (std::nothrow) MoveBy(duration_, delta_pos_, ease_type_);
	}

	spAction MoveBy::Reverse() const
	{
		return new (std::nothrow) MoveBy(duration_, -delta_pos_, ease_type_);
	}

	MoveTo::MoveTo(Duration const& duration, Point const& pos, EaseFunc func)
		: MoveBy(duration, Point(), func)
	{
		end_pos_ = pos;
	}

	spAction MoveTo::Clone() const
	{
		return new (std::nothrow) MoveTo(duration_, end_pos_, ease_type_);
	}

	void MoveTo::Init(Node* target)
	{
		MoveBy::Init(target);
		delta_pos_ = end_pos_ - start_pos_;
	}


	//-------------------------------------------------------
	// Jump Action
	//-------------------------------------------------------

	JumpBy::JumpBy(Duration const& duration, Point const& vec, float height, int jumps, EaseFunc func)
		: Tween(duration, func)
		, delta_pos_(vec)
		, height_(height)
		, jumps_(jumps)
	{
	}

	spAction JumpBy::Clone() const
	{
		return new (std::nothrow) JumpBy(duration_, delta_pos_, height_, jumps_, ease_type_);
	}

	spAction JumpBy::Reverse() const
	{
		return new (std::nothrow) JumpBy(duration_, -delta_pos_, height_, jumps_, ease_type_);
	}

	void JumpBy::Init(Node* target)
	{
		Tween::Init(target);

		if (target)
		{
			prev_pos_ = start_pos_ = target->GetPosition();
		}
	}

	void JumpBy::UpdateStep(Node* target, float step)
	{
		if (target)
		{
			float frac = fmod(step * jumps_, 1.f);
			float x = delta_pos_.x * step;
			float y = height_ * 4 * frac * (1 - frac);
			y += delta_pos_.y * step;

			Point diff = target->GetPosition() - prev_pos_;
			start_pos_ = diff + start_pos_;

			Point new_pos = start_pos_ + Point(x, y);
			target->SetPosition(new_pos);

			prev_pos_ = new_pos;
		}
	}

	JumpTo::JumpTo(Duration const& duration, Point const& pos, float height, int jumps, EaseFunc func)
		: JumpBy(duration, Point(), height, jumps, func)
		, end_pos_(pos)
	{
	}

	spAction JumpTo::Clone() const
	{
		return new (std::nothrow) JumpTo(duration_, end_pos_, height_, jumps_, ease_type_);
	}

	void JumpTo::Init(Node* target)
	{
		JumpBy::Init(target);
		delta_pos_ = end_pos_ - start_pos_;
	}


	//-------------------------------------------------------
	// Scale Action
	//-------------------------------------------------------

	ScaleBy::ScaleBy(Duration const& duration, float scale, EaseFunc func)
		: Tween(duration, func)
	{
		delta_x_ = scale;
		delta_y_ = scale;
	}

	ScaleBy::ScaleBy(Duration const& duration, float scale_x, float scale_y, EaseFunc func)
		: Tween(duration, func)
	{
		delta_x_ = scale_x;
		delta_y_ = scale_y;
	}

	void ScaleBy::Init(Node* target)
	{
		Tween::Init(target);

		if (target)
		{
			start_scale_x_ = target->GetScaleX();
			start_scale_y_ = target->GetScaleY();
		}
	}

	void ScaleBy::UpdateStep(Node* target, float step)
	{
		if (target)
		{
			target->SetScale(start_scale_x_ + delta_x_ * step, start_scale_y_ + delta_y_ * step);
		}
	}

	spAction ScaleBy::Clone() const
	{
		return new (std::nothrow) ScaleBy(duration_, delta_x_, delta_y_, ease_type_);
	}

	spAction ScaleBy::Reverse() const
	{
		return new (std::nothrow) ScaleBy(duration_, -delta_x_, -delta_y_, ease_type_);
	}

	ScaleTo::ScaleTo(Duration const& duration, float scale, EaseFunc func)
		: ScaleBy(duration, 0, 0, func)
	{
		end_scale_x_ = scale;
		end_scale_y_ = scale;
	}

	ScaleTo::ScaleTo(Duration const& duration, float scale_x, float scale_y, EaseFunc func)
		: ScaleBy(duration, 0, 0, func)
	{
		end_scale_x_ = scale_x;
		end_scale_y_ = scale_y;
	}

	spAction ScaleTo::Clone() const
	{
		return new (std::nothrow) ScaleTo(duration_, end_scale_x_, end_scale_y_, ease_type_);
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

	OpacityBy::OpacityBy(Duration const& duration, float opacity, EaseFunc func)
		: Tween(duration, func)
	{
		delta_val_ = opacity;
	}

	void OpacityBy::Init(Node* target)
	{
		Tween::Init(target);

		if (target)
		{
			start_val_ = target->GetOpacity();
		}
	}

	void OpacityBy::UpdateStep(Node* target, float step)
	{
		if (target)
		{
			target->SetOpacity(start_val_ + delta_val_ * step);
		}
	}

	spAction OpacityBy::Clone() const
	{
		return new (std::nothrow) OpacityBy(duration_, delta_val_, ease_type_);
	}

	spAction OpacityBy::Reverse() const
	{
		return new (std::nothrow) OpacityBy(duration_, -delta_val_, ease_type_);
	}

	OpacityTo::OpacityTo(Duration const& duration, float opacity, EaseFunc func)
		: OpacityBy(duration, 0, func)
	{
		end_val_ = opacity;
	}

	spAction OpacityTo::Clone() const
	{
		return new (std::nothrow) OpacityTo(duration_, end_val_, ease_type_);
	}

	void OpacityTo::Init(Node* target)
	{
		OpacityBy::Init(target);
		delta_val_ = end_val_ - start_val_;
	}

	FadeIn::FadeIn(Duration const& duration, EaseFunc func)
		: OpacityTo(duration, 1, func)
	{
	}

	FadeOut::FadeOut(Duration const& duration, EaseFunc func)
		: OpacityTo(duration, 0, func)
	{
	}


	//-------------------------------------------------------
	// Rotate Action
	//-------------------------------------------------------

	RotateBy::RotateBy(Duration const& duration, float rotation, EaseFunc func)
		: Tween(duration, func)
		, delta_val_(rotation)
	{
	}

	void RotateBy::Init(Node* target)
	{
		Tween::Init(target);

		if (target)
		{
			start_val_ = target->GetRotation();
		}
	}

	void RotateBy::UpdateStep(Node* target, float step)
	{
		if (target)
		{
			target->SetRotation(start_val_ + delta_val_ * step);
		}
	}

	spAction RotateBy::Clone() const
	{
		return new (std::nothrow) RotateBy(duration_, delta_val_, ease_type_);
	}

	spAction RotateBy::Reverse() const
	{
		return new (std::nothrow) RotateBy(duration_, -delta_val_, ease_type_);
	}

	RotateTo::RotateTo(Duration const& duration, float rotation, EaseFunc func)
		: RotateBy(duration, 0, func)
	{
		end_val_ = rotation;
	}

	spAction RotateTo::Clone() const
	{
		return new (std::nothrow) RotateTo(duration_, end_val_, ease_type_);
	}

	void RotateTo::Init(Node* target)
	{
		RotateBy::Init(target);
		delta_val_ = end_val_ - start_val_;
	}


	//-------------------------------------------------------
	// PathAction
	//-------------------------------------------------------

	PathAction::PathAction(Duration const & duration, spGeometry const& geo, bool rotating, float start, float end, EaseFunc func)
		: Tween(duration, func)
		, start_(start)
		, end_(end)
		, geo_(geo)
		, rotating_(rotating)
	{
	}

	spAction PathAction::Clone() const
	{
		return new PathAction(duration_, geo_, rotating_, start_, end_, ease_type_);
	}

	spAction PathAction::Reverse() const
	{
		return new PathAction(duration_, geo_, rotating_, end_, start_, ease_type_);
	}

	void PathAction::Init(Node * target)
	{
		Tween::Init(target);

		if (target)
		{
			start_pos_ = target->GetPosition();
		}
	}

	void PathAction::UpdateStep(Node* target, float step)
	{
		if (target)
		{
			float length = geo_->GetLength() * std::min(std::max((end_ - start_) * step + start_, 0.f), 1.f);

			Point point, tangent;
			if (geo_->ComputePointAt(length, &point, &tangent))
			{
				target->SetPosition(start_pos_ + point);

				if (rotating_)
				{
					float ac = math::Acos(tangent.x);
					float rotation = (tangent.y < 0.f) ? 360.f - ac : ac;
					target->SetRotation(rotation);
				}
			}
		}
	}

}
