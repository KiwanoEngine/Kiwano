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
#include "constants.hpp"
#include <cmath>

namespace kiwano
{
	namespace math
	{
		inline Int32 Abs(Int32 val)							{ return ::abs(val); }

		inline Float32 Abs(Float32 val)						{ return ::fabsf(val); }

		inline Float64 Abs(Float64 val)						{ return ::fabs(val); }

		inline Float32 Sqrt(Float32 val)					{ return ::sqrtf(val); }

		inline Float64 Sqrt(Float64 val)					{ return ::sqrt(val); }

		inline Float32 Pow(Float32 base, Float32 exponent)	{ return ::powf(base, exponent); }

		inline Float64 Pow(Float64 base, Float64 exponent)	{ return ::pow(base, exponent); }

		inline Int32 Sign(Int32 val)						{ return val < 0 ? -1 : 1; }

		inline Float32 Sign(Float32 val)					{ return val < 0 ? -1.f : 1.f; }

		inline Float64 Sign(Float64 val)					{ return val < 0 ? -1.0 : 1.0; }

		inline Float32 Sin(Float32 val)						{ return ::sinf(val * constants::PI_F / 180.f); }

		inline Float64 Sin(Float64 val)						{ return ::sin(val * constants::PI_D / 180.0); }

		inline Float32 Cos(Float32 val)						{ return ::cosf(val * constants::PI_F / 180.f); }

		inline Float64 Cos(Float64 val)						{ return ::cos(val * constants::PI_D / 180.0); }

		inline Float32 Tan(Float32 val)						{ return ::tanf(val * constants::PI_F / 180.f); }

		inline Float64 Tan(Float64 val)						{ return ::tan(val * constants::PI_D / 180.0); }

		inline Float32 Asin(Float32 val)					{ return ::asinf(val) * 180.f / constants::PI_F; }

		inline Float64 Asin(Float64 val)					{ return ::asin(val) * 180.f / constants::PI_F; }

		inline Float32 Acos(Float32 val)					{ return ::acosf(val) * 180.f / constants::PI_F; }

		inline Float64 Acos(Float64 val)					{ return ::acos(val) * 180.f / constants::PI_F; }

		inline Float32 Atan(Float32 val)					{ return ::atanf(val) * 180.f / constants::PI_F; }

		inline Float64 Atan(Float64 val)					{ return ::atan(val) * 180.f / constants::PI_F; }

		inline Float32 Ceil(Float32 val)					{ return ::ceil(val); }

		inline Float64 Ceil(Float64 val)					{ return ::ceil(val); }

		inline Float32 Floor(Float32 val)					{ return ::floor(val); }

		inline Float64 Floor(Float64 val)					{ return ::floor(val); }
	}
}
