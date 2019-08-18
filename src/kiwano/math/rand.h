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
#include <random>

namespace kiwano
{
	namespace math
	{
		//
		// 随机数
		// 
		// 获取指定范围内的一个随机数, 如:
		// Int32 n = math::Rand(1, 5);  // 获取 1~5 内的随机整数, 包含 1 和 5
		// 产生的随机数类型取决于参数的类型, 如获取随机浮点数:
		// Float64 d = math::Rand(1.2, 1.5);
		//

		Int32 Rand(Int32 min, Int32 max);

		UInt32 Rand(UInt32 min, UInt32 max);

		long Rand(long min, long max);

		unsigned long Rand(unsigned long min, unsigned long max);

		char Rand(char min, char max);

		UChar Rand(UChar min, UChar max);

		Float32 Rand(Float32 min, Float32 max);

		Float64 Rand(Float64 min, Float64 max);


		//
		// Details of math::Rand
		//

		namespace __rand_detail
		{
			inline std::default_random_engine& GetRandomEngine()
			{
				static std::random_device device;
				static std::default_random_engine engine(device());
				return engine;
			}

			template<typename T>
			inline T RandomInt(T min, T max)
			{
				std::uniform_int_distribution<T> dist(min, max);
				return dist(GetRandomEngine());
			}

			template<typename T>
			inline T RandomReal(T min, T max)
			{
				std::uniform_real_distribution<T> dist(min, max);
				return dist(GetRandomEngine());
			}
		}

		inline Int32 Rand(Int32 min, Int32 max)
		{
			return __rand_detail::RandomInt(min, max);
		}

		inline UInt32 Rand(UInt32 min, UInt32 max)
		{
			return __rand_detail::RandomInt(min, max);
		}

		inline long Rand(long min, long max)
		{
			return __rand_detail::RandomInt(min, max);
		}

		inline unsigned long Rand(unsigned long min, unsigned long max)
		{
			return __rand_detail::RandomInt(min, max);
		}

		inline char Rand(char min, char max)
		{
			return static_cast<char>(
				__rand_detail::RandomInt(static_cast<Int32>(min), static_cast<Int32>(max))
			);
		}

		inline UChar Rand(UChar min, UChar max)
		{
			return static_cast<UChar>(
				__rand_detail::RandomInt(static_cast<UInt32>(min), static_cast<UInt32>(max))
			);
		}

		inline Float32 Rand(Float32 min, Float32 max)
		{
			return __rand_detail::RandomReal(min, max);
		}

		inline Float64 Rand(Float64 min, Float64 max)
		{
			return __rand_detail::RandomReal(min, max);
		}
	}
}