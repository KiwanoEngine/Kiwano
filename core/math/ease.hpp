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

#pragma once
#include "scalar.hpp"

namespace easy2d
{
	namespace math
	{
		inline float Linear(float step)
		{
			return step;
		}

		inline float EaseIn(float step, float rate)
		{
			return math::Pow(step, rate);
		}

		inline float EaseOut(float step, float rate)
		{
			return math::Pow(step, 1.f / rate);
		}

		inline float EaseInOut(float step, float rate)
		{
			if (step < .5f)
				return .5f * math::Pow(2 * step, rate);
			return 1.f - .5f * math::Pow(2.f - 2 * step, rate);
		}

		inline float EaseExponentialIn(float step)
		{
			return math::Pow(2.f, 10 * (step - 1));
		}

		inline float EaseExponentialOut(float step)
		{
			return 1.f - math::Pow(2.f, -10 * step);
		}

		inline float EaseExponentialInOut(float step)
		{
			if (step < .5f)
				return .5f * math::Pow(2.f, 10 * (2 * step - 1));
			return 0.5f * (2 - math::Pow(2, -10 * (step * 2 - 1)));
		}

		inline float EaseBounceOut(float step)
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

		inline float EaseBounceIn(float step)
		{
			return 1 - EaseBounceOut(1 - step);
		}

		inline float EaseBounceInOut(float step)
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

		inline float EaseElasticIn(float step, float period)
		{
			if (step == 0 || step == 1)
				return step;

			step = step - 1;
			return -math::Pow(2, 10 * step) * math::Sin((step - period / 4) * 360.f / period);
		}

		inline float EaseElasticOut(float step, float period)
		{
			if (step == 0 || step == 1)
				return step;
			
			return math::Pow(2, -10 * step) * math::Sin((step - period / 4) * 360.f / period) + 1;
		}

		inline float EaseElasticInOut(float step, float period)
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

		inline float EaseSineIn(float step)
		{
			return 1.f - math::Cos(step * 90);
		}

		inline float EaseSineOut(float step)
		{
			return math::Sin(step * 90);
		}

		inline float EaseSineInOut(float step)
		{
			return -0.5f * (math::Cos(step * 180) - 1);
		}
	}
}
