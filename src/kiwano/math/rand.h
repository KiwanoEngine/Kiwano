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
		// int n = math::Random(1, 5);  // 获取 1~5 内的随机整数, 包含 1 和 5
		// 产生的随机数类型取决于参数的类型, 如获取随机浮点数:
		// float d = math::Random(1.2f, 1.5f);
		//

		int				Random(int min, int max);

		unsigned int	Random(unsigned int min, unsigned int max);

		long			Random(long min, long max);

		unsigned long	Random(unsigned long min, unsigned long max);

		char			Random(char min, char max);

		float			Random(float min, float max);

		double			Random(double min, double max);


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

		inline int Random(int min, int max)
		{
			return __rand_detail::RandomInt(min, max);
		}

		inline unsigned int Random(unsigned int min, unsigned int max)
		{
			return __rand_detail::RandomInt(min, max);
		}

		inline long Random(long min, long max)
		{
			return __rand_detail::RandomInt(min, max);
		}

		inline unsigned long Random(unsigned long min, unsigned long max)
		{
			return __rand_detail::RandomInt(min, max);
		}

		inline char Random(char min, char max)
		{
			return static_cast<char>(
				__rand_detail::RandomInt(static_cast<int>(min), static_cast<int>(max))
			);
		}

		inline float Random(float min, float max)
		{
			return __rand_detail::RandomReal(min, max);
		}

		inline double Random(double min, double max)
		{
			return __rand_detail::RandomReal(min, max);
		}
	}
}
