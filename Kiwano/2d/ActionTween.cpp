// Copyright (c) 2016-2018 Kiwano - Nomango
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
#include "include-forwards.h"
#include "Node.h"

namespace kiwano
{
	//-------------------------------------------------------
	// Ease Functions
	//-------------------------------------------------------

	inline EaseFunc MakeEaseIn(float rate) { return std::bind(math::EaseIn, std::placeholders::_1, rate); }
	inline EaseFunc MakeEaseOut(float rate) { return std::bind(math::EaseOut, std::placeholders::_1, rate); }
	inline EaseFunc MakeEaseInOut(float rate) { return std::bind(math::EaseInOut, std::placeholders::_1, rate); }
	inline EaseFunc MakeEaseElasticIn(float period) { return std::bind(math::EaseElasticIn, std::placeholders::_1, period); }
	inline EaseFunc MakeEaseElasticOut(float period) { return std::bind(math::EaseElasticOut, std::placeholders::_1, period); }
	inline EaseFunc MakeEaseElasticInOut(float period) { return std::bind(math::EaseElasticInOut, std::placeholders::_1, period); }

	EaseFunc Ease::Linear = math::Linear;
	EaseFunc Ease::EaseIn = MakeEaseIn(2.f);
	EaseFunc Ease::EaseOut = MakeEaseOut(2.f);
	EaseFunc Ease::EaseInOut = MakeEaseInOut(2.f);
	EaseFunc Ease::ExpoIn = math::EaseExponentialIn;
	EaseFunc Ease::ExpoOut = math::EaseExponentialOut;
	EaseFunc Ease::ExpoInOut = math::EaseExponentialInOut;
	EaseFunc Ease::BounceIn = math::EaseBounceIn;
	EaseFunc Ease::BounceOut = math::EaseBounceOut;
	EaseFunc Ease::BounceInOut = math::EaseBounceInOut;
	EaseFunc Ease::ElasticIn = MakeEaseElasticIn(0.3f);
	EaseFunc Ease::ElasticOut = MakeEaseElasticOut(0.3f);
	EaseFunc Ease::ElasticInOut = MakeEaseElasticInOut(0.3f);
	EaseFunc Ease::SineIn = math::EaseSineIn;
	EaseFunc Ease::SineOut = math::EaseSineOut;
	EaseFunc Ease::SineInOut = math::EaseSineInOut;
	EaseFunc Ease::BackIn = math::EaseBackIn;
	EaseFunc Ease::BackOut = math::EaseBackOut;
	EaseFunc Ease::BackInOut = math::EaseBackInOut;
	EaseFunc Ease::QuadIn = math::EaseQuadIn;
	EaseFunc Ease::QuadOut = math::EaseQuadOut;
	EaseFunc Ease::QuadInOut = math::EaseQuadInOut;
	EaseFunc Ease::CubicIn = math::EaseCubicIn;
	EaseFunc Ease::CubicOut = math::EaseCubicOut;
	EaseFunc Ease::CubicInOut = math::EaseCubicInOut;
	EaseFunc Ease::QuartIn = math::EaseQuartIn;
	EaseFunc Ease::QuartOut = math::EaseQuartOut;
	EaseFunc Ease::QuartInOut = math::EaseQuartInOut;
	EaseFunc Ease::QuintIn = math::EaseQuintIn;
	EaseFunc Ease::QuintOut = math::EaseQuintOut;
	EaseFunc Ease::QuintInOut = math::EaseQuintInOut;

	//-------------------------------------------------------
	// ActionTween
	//-------------------------------------------------------

	ActionTween::ActionTween()
		: dur_()
		, ease_func_(nullptr)
	{
	}

	ActionTween::ActionTween(Duration duration, EaseFunc func)
	{
		SetDuration(duration);
		SetEaseFunc(func);
	}

	void ActionTween::SetEaseFunc(EaseFunc const& func)
	{
		ease_func_ = func;
	}

	EaseFunc const & ActionTween::GetEaseFunc() const
	{
		return ease_func_;
	}

	Duration ActionTween::GetDuration() const
	{
		return dur_;
	}

	void ActionTween::Update(NodePtr const& target, Duration dt)
	{
		float percent;

		if (dur_.IsZero())
		{
			percent = 1.f;
			Complete(target);
		}
		else
		{
			Duration elapsed = elapsed_ - delay_;
			float loops_done = elapsed / dur_;

			while (loops_done_ < static_cast<int>(loops_done))
			{
				Complete(target);	// loops_done_++
			}

			percent = (status_ == Status::Done) ? 1.f : (loops_done - static_cast<float>(loops_done_));
		}

		if (ease_func_)
			percent = ease_func_(percent);

		UpdateTween(target, percent);
	}

	void ActionTween::SetDuration(Duration duration)
	{
		dur_ = duration;
	}


	//-------------------------------------------------------
	// Move Action
	//-------------------------------------------------------

	ActionMoveBy::ActionMoveBy(Duration duration, Point const& vector, EaseFunc func)
		: ActionTween(duration, func)
	{
		delta_pos_ = vector;
	}

	void ActionMoveBy::Init(NodePtr const& target)
	{
		if (target)
		{
			prev_pos_ = start_pos_ = target->GetPosition();
		}
	}

	void ActionMoveBy::UpdateTween(NodePtr const& target, float percent)
	{
		Point diff = target->GetPosition() - prev_pos_;
		start_pos_ = start_pos_ + diff;

		Point new_pos = start_pos_ + (delta_pos_ * percent);
		target->SetPosition(new_pos);

		prev_pos_ = new_pos;
	}

	ActionPtr ActionMoveBy::Clone() const
	{
		return new (std::nothrow) ActionMoveBy(dur_, delta_pos_, ease_func_);
	}

	ActionPtr ActionMoveBy::Reverse() const
	{
		return new (std::nothrow) ActionMoveBy(dur_, -delta_pos_, ease_func_);
	}

	ActionMoveTo::ActionMoveTo(Duration duration, Point const& pos, EaseFunc func)
		: ActionMoveBy(duration, Point(), func)
	{
		end_pos_ = pos;
	}

	ActionPtr ActionMoveTo::Clone() const
	{
		return new (std::nothrow) ActionMoveTo(dur_, end_pos_, ease_func_);
	}

	void ActionMoveTo::Init(NodePtr const& target)
	{
		ActionMoveBy::Init(target);
		delta_pos_ = end_pos_ - start_pos_;
	}


	//-------------------------------------------------------
	// Jump Action
	//-------------------------------------------------------

	ActionJumpBy::ActionJumpBy(Duration duration, Point const& vec, float height, int jumps, EaseFunc func)
		: ActionTween(duration, func)
		, delta_pos_(vec)
		, height_(height)
		, jumps_(jumps)
	{
	}

	ActionPtr ActionJumpBy::Clone() const
	{
		return new (std::nothrow) ActionJumpBy(dur_, delta_pos_, height_, jumps_, ease_func_);
	}

	ActionPtr ActionJumpBy::Reverse() const
	{
		return new (std::nothrow) ActionJumpBy(dur_, -delta_pos_, height_, jumps_, ease_func_);
	}

	void ActionJumpBy::Init(NodePtr const& target)
	{
		if (target)
		{
			prev_pos_ = start_pos_ = target->GetPosition();
		}
	}

	void ActionJumpBy::UpdateTween(NodePtr const& target, float percent)
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

	ActionJumpTo::ActionJumpTo(Duration duration, Point const& pos, float height, int jumps, EaseFunc func)
		: ActionJumpBy(duration, Point(), height, jumps, func)
		, end_pos_(pos)
	{
	}

	ActionPtr ActionJumpTo::Clone() const
	{
		return new (std::nothrow) ActionJumpTo(dur_, end_pos_, height_, jumps_, ease_func_);
	}

	void ActionJumpTo::Init(NodePtr const& target)
	{
		ActionJumpBy::Init(target);
		delta_pos_ = end_pos_ - start_pos_;
	}


	//-------------------------------------------------------
	// Scale Action
	//-------------------------------------------------------

	ActionScaleBy::ActionScaleBy(Duration duration, float scale, EaseFunc func)
		: ActionTween(duration, func)
	{
		delta_x_ = scale;
		delta_y_ = scale;
	}

	ActionScaleBy::ActionScaleBy(Duration duration, float scale_x, float scale_y, EaseFunc func)
		: ActionTween(duration, func)
	{
		delta_x_ = scale_x;
		delta_y_ = scale_y;
	}

	void ActionScaleBy::Init(NodePtr const& target)
	{
		if (target)
		{
			start_scale_x_ = target->GetScaleX();
			start_scale_y_ = target->GetScaleY();
		}
	}

	void ActionScaleBy::UpdateTween(NodePtr const& target, float percent)
	{
		target->SetScale(start_scale_x_ + delta_x_ * percent, start_scale_y_ + delta_y_ * percent);
	}

	ActionPtr ActionScaleBy::Clone() const
	{
		return new (std::nothrow) ActionScaleBy(dur_, delta_x_, delta_y_, ease_func_);
	}

	ActionPtr ActionScaleBy::Reverse() const
	{
		return new (std::nothrow) ActionScaleBy(dur_, -delta_x_, -delta_y_, ease_func_);
	}

	ActionScaleTo::ActionScaleTo(Duration duration, float scale, EaseFunc func)
		: ActionScaleBy(duration, 0, 0, func)
	{
		end_scale_x_ = scale;
		end_scale_y_ = scale;
	}

	ActionScaleTo::ActionScaleTo(Duration duration, float scale_x, float scale_y, EaseFunc func)
		: ActionScaleBy(duration, 0, 0, func)
	{
		end_scale_x_ = scale_x;
		end_scale_y_ = scale_y;
	}

	ActionPtr ActionScaleTo::Clone() const
	{
		return new (std::nothrow) ActionScaleTo(dur_, end_scale_x_, end_scale_y_, ease_func_);
	}

	void ActionScaleTo::Init(NodePtr const& target)
	{
		ActionScaleBy::Init(target);
		delta_x_ = end_scale_x_ - start_scale_x_;
		delta_y_ = end_scale_y_ - start_scale_y_;
	}


	//-------------------------------------------------------
	// Opacity Action
	//-------------------------------------------------------

	ActionOpacityBy::ActionOpacityBy(Duration duration, float opacity, EaseFunc func)
		: ActionTween(duration, func)
	{
		delta_val_ = opacity;
	}

	void ActionOpacityBy::Init(NodePtr const& target)
	{
		if (target)
		{
			start_val_ = target->GetOpacity();
		}
	}

	void ActionOpacityBy::UpdateTween(NodePtr const& target, float percent)
	{
		target->SetOpacity(start_val_ + delta_val_ * percent);
	}

	ActionPtr ActionOpacityBy::Clone() const
	{
		return new (std::nothrow) ActionOpacityBy(dur_, delta_val_, ease_func_);
	}

	ActionPtr ActionOpacityBy::Reverse() const
	{
		return new (std::nothrow) ActionOpacityBy(dur_, -delta_val_, ease_func_);
	}

	ActionOpacityTo::ActionOpacityTo(Duration duration, float opacity, EaseFunc func)
		: ActionOpacityBy(duration, 0, func)
	{
		end_val_ = opacity;
	}

	ActionPtr ActionOpacityTo::Clone() const
	{
		return new (std::nothrow) ActionOpacityTo(dur_, end_val_, ease_func_);
	}

	void ActionOpacityTo::Init(NodePtr const& target)
	{
		ActionOpacityBy::Init(target);
		delta_val_ = end_val_ - start_val_;
	}

	ActionFadeIn::ActionFadeIn(Duration duration, EaseFunc func)
		: ActionOpacityTo(duration, 1, func)
	{
	}

	ActionFadeOut::ActionFadeOut(Duration duration, EaseFunc func)
		: ActionOpacityTo(duration, 0, func)
	{
	}


	//-------------------------------------------------------
	// Rotate Action
	//-------------------------------------------------------

	ActionRotateBy::ActionRotateBy(Duration duration, float rotation, EaseFunc func)
		: ActionTween(duration, func)
		, delta_val_(rotation)
	{
	}

	void ActionRotateBy::Init(NodePtr const& target)
	{
		if (target)
		{
			start_val_ = target->GetRotation();
		}
	}

	void ActionRotateBy::UpdateTween(NodePtr const& target, float percent)
	{
		float rotation = start_val_ + delta_val_ * percent;
		if (rotation > 360.f)
			rotation -= 360.f;

		target->SetRotation(rotation);
	}

	ActionPtr ActionRotateBy::Clone() const
	{
		return new (std::nothrow) ActionRotateBy(dur_, delta_val_, ease_func_);
	}

	ActionPtr ActionRotateBy::Reverse() const
	{
		return new (std::nothrow) ActionRotateBy(dur_, -delta_val_, ease_func_);
	}

	ActionRotateTo::ActionRotateTo(Duration duration, float rotation, EaseFunc func)
		: ActionRotateBy(duration, 0, func)
	{
		end_val_ = rotation;
	}

	ActionPtr ActionRotateTo::Clone() const
	{
		return new (std::nothrow) ActionRotateTo(dur_, end_val_, ease_func_);
	}

	void ActionRotateTo::Init(NodePtr const& target)
	{
		ActionRotateBy::Init(target);
		delta_val_ = end_val_ - start_val_;
	}


	//-------------------------------------------------------
	// ActionPath
	//-------------------------------------------------------

	ActionPath::ActionPath(Duration duration, GeometryPtr const& geo, bool rotating, float start, float end, EaseFunc func)
		: ActionTween(duration, func)
		, start_(start)
		, end_(end)
		, geo_(geo)
		, rotating_(rotating)
	{
	}

	ActionPtr ActionPath::Clone() const
	{
		return new ActionPath(dur_, geo_, rotating_, start_, end_, ease_func_);
	}

	ActionPtr ActionPath::Reverse() const
	{
		return new ActionPath(dur_, geo_, rotating_, end_, start_, ease_func_);
	}

	void ActionPath::Init(NodePtr const& target)
	{
		start_pos_ = target->GetPosition();
	}

	void ActionPath::UpdateTween(NodePtr const& target, float percent)
	{
		float length = geo_->GetLength() * std::min(std::max((end_ - start_) * percent + start_, 0.f), 1.f);

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


	//-------------------------------------------------------
	// ActionDelay
	//-------------------------------------------------------

	ActionDelay::ActionDelay(Duration delay)
	{
		SetDelay(delay);
	}

	ActionPtr ActionDelay::Clone() const
	{
		return new ActionDelay(delay_);
	}

	ActionPtr ActionDelay::Reverse() const
	{
		return new ActionDelay(delay_);
	}

}
