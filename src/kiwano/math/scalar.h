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
#include <kiwano/math/constants.h>
#include <cmath>

namespace kiwano
{
	namespace math
	{
		inline int		Abs(int val)						{ return ::abs(val); }
		inline float	Abs(float val)						{ return ::fabsf(val); }
		inline double	Abs(double val)						{ return ::fabs(val); }

		inline float	Sqrt(float val)						{ return ::sqrtf(val); }
		inline double	Sqrt(double val)					{ return ::sqrt(val); }

		inline float	Pow(float core, float exponent)		{ return ::powf(core, exponent); }
		inline double	Pow(double core, double exponent)	{ return ::pow(core, exponent); }

		inline int		Sign(int val)						{ return val < 0 ? -1 : 1; }
		inline float	Sign(float val)						{ return val < 0 ? -1.f : 1.f; }
		inline double	Sign(double val)					{ return val < 0 ? -1.0 : 1.0; }

		inline float	Degree2Radian(float angle)			{ return angle * constants::PI_F / 180.f; }
		inline double	Degree2Radian(double angle)			{ return angle * constants::PI_D / 180.0; }

		inline float	Radian2Degree(float radian)			{ return radian * 180.f / math::constants::PI_F; }
		inline double	Radian2Degree(double radian)		{ return radian * 180.0 / math::constants::PI_D; }

		inline float	Sin(float val)						{ return ::sinf(Degree2Radian(val)); }
		inline double	Sin(double val)						{ return ::sin(Degree2Radian(val)); }

		inline float	Cos(float val)						{ return ::cosf(Degree2Radian(val)); }
		inline double	Cos(double val)						{ return ::cos(Degree2Radian(val)); }

		inline float	Tan(float val)						{ return ::tanf(Degree2Radian(val)); }
		inline double	Tan(double val)						{ return ::tan(Degree2Radian(val)); }

		inline float	Asin(float val)						{ return Radian2Degree(::asinf(val)); }
		inline double	Asin(double val)					{ return Radian2Degree(::asin(val)); }

		inline float	Acos(float val)						{ return Radian2Degree(::acosf(val)); }
		inline double	Acos(double val)					{ return Radian2Degree(::acos(val)); }

		inline float	Atan(float val)						{ return Radian2Degree(::atanf(val)); }
		inline double	Atan(double val)					{ return Radian2Degree(::atan(val)); }

		inline float	Ceil(float val)						{ return ::ceil(val); }
		inline double	Ceil(double val)					{ return ::ceil(val); }

		inline float	Floor(float val)					{ return ::floor(val); }
		inline double	Floor(double val)					{ return ::floor(val); }
	}
}
