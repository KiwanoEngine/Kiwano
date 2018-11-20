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

namespace easy2d
{
	namespace
	{
		inline float EaseExponentialIn(float percent)
		{
			return math::Pow(2.f, 10 * (percent - 1));
		}

		inline float EaseExponentialOut(float percent)
		{
			return 1.f - math::Pow(2.f, -10 * percent);
		}

		inline float EaseExponentialInOut(float percent)
		{
			if (percent < .5f)
				return .5f * math::Pow(2.f, 10 * (2 * percent - 1));
			return 0.5f * (2 - math::Pow(2, -10 * (percent * 2 - 1)));
		}

		inline float EaseSineIn(float percent)
		{
			return 1.f - math::Cos(percent * 90);
		}

		inline float EaseSineOut(float percent)
		{
			return math::Sin(percent * 90);
		}

		inline float EaseSineInOut(float percent)
		{
			return -0.5f * (math::Cos(percent * 180) - 1);
		}
	}

	//-------------------------------------------------------
	// TweenAction
	//-------------------------------------------------------

	TweenAction::TweenAction()
	{
	}

	TweenAction::TweenAction(spIntervalAction const & action, TweenType type)
		: inner_(action)
		, type_(type)
	{
		if (inner_)
			this->SetDuration(inner_->GetDuration());

		switch (type_)
		{
		case TweenType::EaseExponentialIn:
			ease_func_ = EaseExponentialIn;
			break;
		case TweenType::EaseExponentialOut:
			ease_func_ = EaseExponentialOut;
			break;
		case TweenType::EaseExponentialInOut:
			ease_func_ = EaseExponentialInOut;
			break;
		case TweenType::EaseSineIn:
			ease_func_ = EaseSineIn;
			break;
		case TweenType::EaseSineOut:
			ease_func_ = EaseSineOut;
			break;
		case TweenType::EaseSineInOut:
			ease_func_ = EaseSineInOut;
			break;
		default:
			break;
		}
	}

	TweenAction::TweenAction(spIntervalAction const & action, EaseFunc const & func)
	{
	}

	spAction TweenAction::Clone() const
	{
		auto clone = inner_->Clone();
		return new TweenAction(dynamic_cast<IntervalAction*>(clone.Get()), ease_func_);
	}

	spAction TweenAction::Reverse() const
	{
		TweenType reverse_type;
		switch (type_)
		{
		case TweenType::EaseExponentialIn:
			reverse_type = TweenType::EaseExponentialOut;
			break;
		case TweenType::EaseExponentialOut:
			reverse_type = TweenType::EaseExponentialIn;
			break;
		case TweenType::EaseExponentialInOut:
			reverse_type = TweenType::EaseExponentialInOut;
			break;
		case TweenType::EaseSineIn:
			reverse_type = TweenType::EaseSineOut;
			break;
		case TweenType::EaseSineOut:
			reverse_type = TweenType::EaseSineIn;
			break;
		case TweenType::EaseSineInOut:
			reverse_type = TweenType::EaseSineInOut;
			break;
		default:
			break;
		}
		auto reverse = inner_->Reverse();
		return new TweenAction(dynamic_cast<IntervalAction*>(reverse.Get()), reverse_type);
	}

	void TweenAction::Init(Node * target)
	{
		IntervalAction::Init(target);
		if (inner_)
			inner_->Init(target);
	}

	void TweenAction::Update(Node * target, float percent)
	{
		percent = ease_func_(percent);

		if (inner_)
		{
			inner_->Update(target, percent);

			if (this->IsDone())
				inner_->Stop();
		}
	}


	//-------------------------------------------------------
	// EaseAction
	//-------------------------------------------------------

	namespace
	{
		inline float EaseIn(float percent, float rate)
		{
			return math::Pow(percent, rate);
		}

		inline float EaseOut(float percent, float rate)
		{
			return math::Pow(percent, 1.f / rate);
		}

		inline float EaseInOut(float percent, float rate)
		{
			if (percent < .5f)
				return .5f * math::Pow(2 * percent, rate);
			return 1.f - .5f * math::Pow(2.f - 2 * percent, rate);
		}
	}

	EaseAction::EaseAction(spIntervalAction const & action, EaseType type, float rate)
		: inner_(action)
		, rate_(rate)
	{
		if (inner_)
			this->SetDuration(inner_->GetDuration());

		switch (type)
		{
		case EaseType::EaseIn:
			ease_func_ = EaseIn;
			break;
		case EaseType::EaseOut:
			ease_func_ = EaseOut;
			break;
		case EaseType::EaseInOut:
			ease_func_ = EaseInOut;
			break;
		default:
			break;
		}
	}

	EaseAction::EaseAction(spIntervalAction const & action, EaseFunc const & func, float rate)
		: inner_(action)
		, rate_(rate)
		, ease_func_(func)
	{
	}

	spAction EaseAction::Clone() const
	{
		auto clone = inner_->Clone();
		return new EaseAction(dynamic_cast<IntervalAction*>(clone.Get()), ease_func_, rate_);
	}

	spAction EaseAction::Reverse() const
	{
		auto reverse = inner_->Reverse();
		return new EaseAction(dynamic_cast<IntervalAction*>(reverse.Get()), ease_func_, 1.f / rate_);
	}

	void EaseAction::Update(Node * target, float percent)
	{
		percent = ease_func_(percent, rate_);

		if (inner_)
		{
			inner_->Update(target, percent);

			if (this->IsDone())
				inner_->Stop();
		}
	}

}
