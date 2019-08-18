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

#pragma once
#include "scalar.hpp"

namespace kiwano
{
	namespace math
	{
		inline Float32 Linear(Float32 step)
		{
			return step;
		}


		// Ease

		inline Float32 EaseIn(Float32 step, Float32 rate)
		{
			return math::Pow(step, rate);
		}

		inline Float32 EaseOut(Float32 step, Float32 rate)
		{
			return math::Pow(step, 1.f / rate);
		}

		inline Float32 EaseInOut(Float32 step, Float32 rate)
		{
			if (step < .5f)
				return .5f * math::Pow(2 * step, rate);
			return 1.f - .5f * math::Pow(2.f - 2 * step, rate);
		}


		// Exponential Ease

		inline Float32 EaseExponentialIn(Float32 step)
		{
			return math::Pow(2.f, 10 * (step - 1));
		}

		inline Float32 EaseExponentialOut(Float32 step)
		{
			return 1.f - math::Pow(2.f, -10 * step);
		}

		inline Float32 EaseExponentialInOut(Float32 step)
		{
			if (step < .5f)
				return .5f * math::Pow(2.f, 10 * (2 * step - 1));
			return 0.5f * (2 - math::Pow(2, -10 * (step * 2 - 1)));
		}


		// Bounce Ease

		inline Float32 EaseBounceOut(Float32 step)
		{
			if (step < 1 / 2.75f)
			{
				return 7.5625f * step * step;
			}
			else if (step < 2 / 2.75f)
			{
				step -= 1.5f / 2.75f;
				return 7.5625f * step * step + 0.75f;
			}
			else if (step < 2.5f / 2.75f)
			{
				step -= 2.25f / 2.75f;
				return 7.5625f * step * step + 0.9375f;
			}

			step -= 2.625f / 2.75f;
			return 7.5625f * step * step + 0.984375f;
		}

		inline Float32 EaseBounceIn(Float32 step)
		{
			return 1 - EaseBounceOut(1 - step);
		}

		inline Float32 EaseBounceInOut(Float32 step)
		{
			if (step < 0.5f)
			{
				return EaseBounceIn(step * 2) * 0.5f;
			}
			else
			{
				return EaseBounceOut(step * 2 - 1) * 0.5f + 0.5f;
			}
		}


		// Elastic Ease

		inline Float32 EaseElasticIn(Float32 step, Float32 period)
		{
			if (step == 0 || step == 1)
				return step;

			step = step - 1;
			return -math::Pow(2, 10 * step) * math::Sin((step - period / 4) * 360.f / period);
		}

		inline Float32 EaseElasticOut(Float32 step, Float32 period)
		{
			if (step == 0 || step == 1)
				return step;
			
			return math::Pow(2, -10 * step) * math::Sin((step - period / 4) * 360.f / period) + 1;
		}

		inline Float32 EaseElasticInOut(Float32 step, Float32 period)
		{
			if (step == 0 || step == 1)
				return step;

			step = step * 2 - 1;
			if (step < 0)
			{
				return -0.5f * math::Pow(2, 10 * step) * math::Sin((step - period / 4) * 360.f / period);
			}
			return math::Pow(2, -10 * step) * math::Sin((step - period / 4) * 360.f / period) * 0.5f + 1;
		}


		// Back Ease

		inline Float32 EaseBackIn(Float32 step)
		{
			const Float32 overshoot = 1.70158f;
			return step * step * ((overshoot + 1) * step - overshoot);
		}

		inline Float32 EaseBackOut(Float32 step)
		{
			const Float32 overshoot = 1.70158f;
			step = step - 1;
			return step * step * ((overshoot + 1) * step + overshoot) + 1;
		}

		inline Float32 EaseBackInOut(Float32 step)
		{
			const Float32 overshoot = 1.70158f * 1.525f;

			step = step * 2;
			if (step < 1)
			{
				return (step * step * ((overshoot + 1) * step - overshoot)) / 2;
			}

			step = step - 2;
			return (step * step * ((overshoot + 1) * step + overshoot)) / 2 + 1;
		}


		// Sine Ease

		inline Float32 EaseSineIn(Float32 step)
		{
			return 1.f - math::Cos(step * 90);
		}

		inline Float32 EaseSineOut(Float32 step)
		{
			return math::Sin(step * 90);
		}

		inline Float32 EaseSineInOut(Float32 step)
		{
			return -0.5f * (math::Cos(step * 180) - 1);
		}


		// Quad Ease

		inline Float32 EaseQuadIn(Float32 step)
		{
			return step * step;
		}

		inline Float32 EaseQuadOut(Float32 step)
		{
			return -1 * step * (step - 2);
		}

		inline Float32 EaseQuadInOut(Float32 step)
		{
			step = step * 2;
			if (step < 1)
				return 0.5f * step * step;
			--step;
			return -0.5f * (step * (step - 2) - 1);
		}


		// Cubic Ease

		inline Float32 EaseCubicIn(Float32 step)
		{
			return step * step * step;
		}

		inline Float32 EaseCubicOut(Float32 step)
		{
			step -= 1;
			return (step * step * step + 1);
		}

		inline Float32 EaseCubicInOut(Float32 step)
		{
			step = step * 2;
			if (step < 1)
				return 0.5f * step * step * step;
			step -= 2;
			return 0.5f * (step * step * step + 2);
		}


		// Quart Ease

		inline Float32 EaseQuartIn(Float32 step)
		{
			return step * step * step * step;
		}

		inline Float32 EaseQuartOut(Float32 step)
		{
			step -= 1;
			return -(step * step * step * step - 1);
		}

		inline Float32 EaseQuartInOut(Float32 step)
		{
			step = step * 2;
			if (step < 1)
				return 0.5f * step * step * step * step;
			step -= 2;
			return -0.5f * (step * step * step * step - 2);
		}


		// Quint Ease

		inline Float32 EaseQuintIn(Float32 step)
		{
			return step * step * step * step * step;
		}

		inline Float32 EaseQuintOut(Float32 step)
		{
			step -= 1;
			return (step * step * step * step * step + 1);
		}

		inline Float32 EaseQuintInOut(Float32 step)
		{
			step = step * 2;
			if (step < 1)
				return 0.5f * step * step * step * step * step;
			step -= 2;
			return 0.5f * (step * step * step * step * step + 2);
		}
	}
}
