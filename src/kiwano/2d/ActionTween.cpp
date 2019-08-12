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
#include "Actor.h"

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

	KGE_API EaseFunc Ease::Linear = math::Linear;
	KGE_API EaseFunc Ease::EaseIn = MakeEaseIn(2.f);
	KGE_API EaseFunc Ease::EaseOut = MakeEaseOut(2.f);
	KGE_API EaseFunc Ease::EaseInOut = MakeEaseInOut(2.f);
	KGE_API EaseFunc Ease::ExpoIn = math::EaseExponentialIn;
	KGE_API EaseFunc Ease::ExpoOut = math::EaseExponentialOut;
	KGE_API EaseFunc Ease::ExpoInOut = math::EaseExponentialInOut;
	KGE_API EaseFunc Ease::BounceIn = math::EaseBounceIn;
	KGE_API EaseFunc Ease::BounceOut = math::EaseBounceOut;
	KGE_API EaseFunc Ease::BounceInOut = math::EaseBounceInOut;
	KGE_API EaseFunc Ease::ElasticIn = MakeEaseElasticIn(0.3f);
	KGE_API EaseFunc Ease::ElasticOut = MakeEaseElasticOut(0.3f);
	KGE_API EaseFunc Ease::ElasticInOut = MakeEaseElasticInOut(0.3f);
	KGE_API EaseFunc Ease::SineIn = math::EaseSineIn;
	KGE_API EaseFunc Ease::SineOut = math::EaseSineOut;
	KGE_API EaseFunc Ease::SineInOut = math::EaseSineInOut;
	KGE_API EaseFunc Ease::BackIn = math::EaseBackIn;
	KGE_API EaseFunc Ease::BackOut = math::EaseBackOut;
	KGE_API EaseFunc Ease::BackInOut = math::EaseBackInOut;
	KGE_API EaseFunc Ease::QuadIn = math::EaseQuadIn;
	KGE_API EaseFunc Ease::QuadOut = math::EaseQuadOut;
	KGE_API EaseFunc Ease::QuadInOut = math::EaseQuadInOut;
	KGE_API EaseFunc Ease::CubicIn = math::EaseCubicIn;
	KGE_API EaseFunc Ease::CubicOut = math::EaseCubicOut;
	KGE_API EaseFunc Ease::CubicInOut = math::EaseCubicInOut;
	KGE_API EaseFunc Ease::QuartIn = math::EaseQuartIn;
	KGE_API EaseFunc Ease::QuartOut = math::EaseQuartOut;
	KGE_API EaseFunc Ease::QuartInOut = math::EaseQuartInOut;
	KGE_API EaseFunc Ease::QuintIn = math::EaseQuintIn;
	KGE_API EaseFunc Ease::QuintOut = math::EaseQuintOut;
	KGE_API EaseFunc Ease::QuintInOut = math::EaseQuintInOut;

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

	void ActionTween::Update(ActorPtr target, Duration dt)
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

	void ActionMoveBy::Init(ActorPtr target)
	{
		if (target)
		{
			prev_pos_ = start_pos_ = target->GetPosition();
		}
	}

	void ActionMoveBy::UpdateTween(ActorPtr target, float percent)
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

	void ActionMoveTo::Init(ActorPtr target)
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

	void ActionJumpBy::Init(ActorPtr target)
	{
		if (target)
		{
			prev_pos_ = start_pos_ = target->GetPosition();
		}
	}

	void ActionJumpBy::UpdateTween(ActorPtr target, float percent)
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

	void ActionJumpTo::Init(ActorPtr target)
	{
		ActionJumpBy::Init(target);
		delta_pos_ = end_pos_ - start_pos_;
	}


	//-------------------------------------------------------
	// Scale Action
	//-------------------------------------------------------

	ActionScaleBy::ActionScaleBy(Duration duration, float scale, EaseFunc func)
		: ActionScaleBy(duration, scale, scale, func)
	{
	}

	ActionScaleBy::ActionScaleBy(Duration duration, float scale_x, float scale_y, EaseFunc func)
		: ActionTween(duration, func)
		, delta_x_(scale_x)
		, delta_y_(scale_y)
		, start_scale_x_(0.f)
		, start_scale_y_(0.f)
	{
	}

	void ActionScaleBy::Init(ActorPtr target)
	{
		if (target)
		{
			start_scale_x_ = target->GetScaleX();
			start_scale_y_ = target->GetScaleY();
		}
	}

	void ActionScaleBy::UpdateTween(ActorPtr target, float percent)
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

	void ActionScaleTo::Init(ActorPtr target)
	{
		ActionScaleBy::Init(target);
		delta_x_ = end_scale_x_ - start_scale_x_;
		delta_y_ = end_scale_y_ - start_scale_y_;
	}


	//-------------------------------------------------------
	// Opacity Action
	//-------------------------------------------------------

	ActionFadeTo::ActionFadeTo(Duration duration, float opacity, EaseFunc func)
		: ActionTween(duration, func)
		, delta_val_(0.f)
		, start_val_(0.f)
		, end_val_(opacity)
	{
	}

	void ActionFadeTo::Init(ActorPtr target)
	{
		if (target)
		{
			start_val_ = target->GetOpacity();
			delta_val_ = end_val_ - start_val_;
		}
	}

	void ActionFadeTo::UpdateTween(ActorPtr target, float percent)
	{
		target->SetOpacity(start_val_ + delta_val_ * percent);
	}

	ActionPtr ActionFadeTo::Clone() const
	{
		return new (std::nothrow) ActionFadeTo(dur_, end_val_, ease_func_);
	}

	ActionFadeIn::ActionFadeIn(Duration duration, EaseFunc func)
		: ActionFadeTo(duration, 1, func)
	{
	}

	ActionFadeOut::ActionFadeOut(Duration duration, EaseFunc func)
		: ActionFadeTo(duration, 0, func)
	{
	}


	//-------------------------------------------------------
	// Rotate Action
	//-------------------------------------------------------

	ActionRotateBy::ActionRotateBy(Duration duration, float rotation, EaseFunc func)
		: ActionTween(duration, func)
		, start_val_()
		, delta_val_(rotation)
	{
	}

	void ActionRotateBy::Init(ActorPtr target)
	{
		if (target)
		{
			start_val_ = target->GetRotation();
		}
	}

	void ActionRotateBy::UpdateTween(ActorPtr target, float percent)
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

	void ActionRotateTo::Init(ActorPtr target)
	{
		ActionRotateBy::Init(target);
		delta_val_ = end_val_ - start_val_;
	}


	//-------------------------------------------------------
	// ActionPath
	//-------------------------------------------------------

	ActionPath::ActionPath(Duration duration, bool rotating, float start, float end, EaseFunc func)
		: ActionTween(duration, func)
		, start_(start)
		, end_(end)
		, rotating_(rotating)
		, path_beginning_(false)
		, geo_(nullptr)
		, geo_sink_(nullptr)
	{
	}

	ActionPtr ActionPath::Clone() const
	{
		ActionPathPtr clone = new ActionPath(dur_, rotating_, start_, end_, ease_func_);
		if (clone)
		{
			clone->SetGeometry(geo_);
		}
		return clone;
	}

	ActionPtr ActionPath::Reverse() const
	{
		ActionPathPtr reverse = new ActionPath(dur_, rotating_, end_, start_, ease_func_);
		if (reverse)
		{
			reverse->SetGeometry(geo_);
		}
		return reverse;
	}

	float ActionPath::GetPathLength() const
	{
		float length = 0.f;
		if (geo_)
		{
			// no matter it failed or not
			geo_->ComputeLength(D2D1::Matrix3x2F::Identity(), &length);
		}
		return length;
	}

	bool ActionPath::ComputePointAtLength(float length, Point* point, Vec2* tangent) const
	{
		if (geo_)
		{
			HRESULT hr = geo_->ComputePointAtLength(
				length,
				D2D1::Matrix3x2F::Identity(),
				DX::ConvertToPoint2F(point),
				DX::ConvertToPoint2F(tangent)
			);
			return SUCCEEDED(hr);
		}
		return false;
	}

	void ActionPath::Init(ActorPtr target)
	{
		start_pos_ = target->GetPosition();

		if (path_beginning_)
		{
			EndPath();
		}

		if (!geo_)
		{
			Complete(target);
		}
	}

	void ActionPath::UpdateTween(ActorPtr target, float percent)
	{
		float length = GetPathLength() * std::min(std::max((end_ - start_) * percent + start_, 0.f), 1.f);

		Point point, tangent;
		if (ComputePointAtLength(length, &point, &tangent))
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

	void ActionPath::BeginPath()
	{
		if (path_beginning_) return;
		path_beginning_ = true;

		geo_ = nullptr;
		geo_sink_ = nullptr;

		auto factory = Renderer::Instance()->GetD2DDeviceResources()->GetFactory();

		ThrowIfFailed(
			factory->CreatePathGeometry(&geo_)
		);

		ThrowIfFailed(
			geo_->Open(&geo_sink_)
		);

		geo_sink_->BeginFigure(DX::ConvertToPoint2F(Point{ 0, 0 }), D2D1_FIGURE_BEGIN_FILLED);
	}

	void ActionPath::EndPath(bool closed)
	{
		if (!path_beginning_) return;
		path_beginning_ = false;

		if (geo_sink_)
		{
			geo_sink_->EndFigure(closed ? D2D1_FIGURE_END_CLOSED : D2D1_FIGURE_END_OPEN);
			ThrowIfFailed(
				geo_sink_->Close()
			);

			// Clear geometry sink
			geo_sink_ = nullptr;
		}
	}

	void ActionPath::AddLine(Point const& point)
	{
		if (!path_beginning_)
		{
			BeginPath();
		}

		if (geo_sink_)
		{
			geo_sink_->AddLine(DX::ConvertToPoint2F(point));
		}
	}

	void ActionPath::AddLines(Array<Point> const& points)
	{
		if (!path_beginning_)
		{
			BeginPath();
		}

		if (geo_sink_ && !points.empty())
		{
			geo_sink_->AddLines(
				reinterpret_cast<const D2D_POINT_2F*>(&points[0]),
				static_cast<UINT32>(points.size())
			);
		}
	}

	void ActionPath::AddBezier(Point const& point1, Point const& point2, Point const& point3)
	{
		if (!path_beginning_)
		{
			BeginPath();
		}

		if (geo_sink_)
		{
			geo_sink_->AddBezier(
				D2D1::BezierSegment(
					DX::ConvertToPoint2F(point1),
					DX::ConvertToPoint2F(point2),
					DX::ConvertToPoint2F(point3)
				)
			);
		}
	}

	void ActionPath::AddArc(Point const& point, Size const& radius, float rotation, bool clockwise, bool is_small)
	{
		if (!path_beginning_)
		{
			BeginPath();
		}

		if (geo_sink_)
		{
			geo_sink_->AddArc(
				D2D1::ArcSegment(
					DX::ConvertToPoint2F(point),
					DX::ConvertToSizeF(radius),
					rotation,
					clockwise ? D2D1_SWEEP_DIRECTION_CLOCKWISE : D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE,
					is_small ? D2D1_ARC_SIZE_SMALL : D2D1_ARC_SIZE_LARGE
				)
			);
		}
	}

	void ActionPath::ClearPath()
	{
		if (path_beginning_)
		{
			EndPath();
		}

		geo_sink_ = nullptr;
		geo_ = nullptr;
	}

	//-------------------------------------------------------
	// ActionCustom
	//-------------------------------------------------------

	ActionCustom::ActionCustom(Duration duration, TweenFunc tween_func, EaseFunc func)
		: ActionTween(duration, func)
		, tween_func_(tween_func)
	{
	}

	ActionPtr ActionCustom::Clone() const
	{
		return new ActionCustom(dur_, tween_func_);
	}

	void ActionCustom::Init(ActorPtr target)
	{
		if (!tween_func_)
			this->Done();
	}

	void ActionCustom::UpdateTween(ActorPtr target, float percent)
	{
		if (tween_func_)
			tween_func_(target, percent);
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
