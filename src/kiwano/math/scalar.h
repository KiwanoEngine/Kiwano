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
#include "constants.h"
#include <cmath>

namespace kiwano
{
	namespace math
	{
		inline int Abs(int val)							{ return ::abs(val); }

		inline float Abs(float val)						{ return ::fabsf(val); }

		inline double Abs(double val)					{ return ::fabs(val); }

		inline float Sqrt(float val)					{ return ::sqrtf(val); }

		inline double Sqrt(double val)					{ return ::sqrt(val); }

		inline float Pow(float base, float exponent)	{ return ::powf(base, exponent); }

		inline double Pow(double base, double exponent)	{ return ::pow(base, exponent); }

		inline int Sign(int val)						{ return val < 0 ? -1 : 1; }

		inline float Sign(float val)					{ return val < 0 ? -1.f : 1.f; }

		inline double Sign(double val)					{ return val < 0 ? -1.0 : 1.0; }

		inline float Sin(float val)						{ return ::sinf(val * constants::PI_F / 180.f); }

		inline double Sin(double val)					{ return ::sin(val * constants::PI_D / 180.0); }

		inline float Cos(float val)						{ return ::cosf(val * constants::PI_F / 180.f); }

		inline double Cos(double val)					{ return ::cos(val * constants::PI_D / 180.0); }

		inline float Tan(float val)						{ return ::tanf(val * constants::PI_F / 180.f); }

		inline double Tan(double val)					{ return ::tan(val * constants::PI_D / 180.0); }

		inline float Asin(float val)					{ return ::asinf(val) * 180.f / constants::PI_F; }

		inline double Asin(double val)					{ return ::asin(val) * 180.f / constants::PI_F; }

		inline float Acos(float val)					{ return ::acosf(val) * 180.f / constants::PI_F; }

		inline double Acos(double val)					{ return ::acos(val) * 180.f / constants::PI_F; }

		inline float Atan(float val)					{ return ::atanf(val) * 180.f / constants::PI_F; }

		inline double Atan(double val)					{ return ::atan(val) * 180.f / constants::PI_F; }

		inline float Ceil(float val)					{ return ::ceil(val); }

		inline double Ceil(double val)					{ return ::ceil(val); }

		inline float Floor(float val)					{ return ::floor(val); }

		inline double Floor(double val)					{ return ::floor(val); }
	}
}
