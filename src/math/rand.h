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
#include <random>

namespace easy2d
{
	namespace math
	{
		//
		// 随机数
		// 
		// 获取指定范围内的一个随机数, 如:
		// int n = math::Rand(1, 5);  // 获取 1~5 内的随机整数, 包含 1 和 5
		// 产生的随机数类型取决于参数的类型, 如获取随机浮点数:
		// double d = math::Rand(1.2, 1.5);
		//
		
		std::default_random_engine& GetRandomEngine();

		template<typename T>
		static T RandomInt(T min, T max)
		{
			std::uniform_int_distribution<T> dist(min, max);
			return dist(GetRandomEngine());
		}

		template<typename T>
		static T RandomReal(T min, T max)
		{
			std::uniform_real_distribution<T> dist(min, max);
			return dist(GetRandomEngine());
		}

		inline int Rand(int min, int max)
		{
			return RandomInt(min, max);
		}

		inline unsigned int Rand(unsigned int min, unsigned int max)
		{
			return RandomInt(min, max);
		}

		inline long Rand(long min, long max)
		{
			return RandomInt(min, max);
		}

		inline unsigned long Rand(unsigned long min, unsigned long max)
		{
			return RandomInt(min, max);
		}

		inline char Rand(char min, char max)
		{
			return static_cast<char>(
				RandomInt(static_cast<int>(min), static_cast<int>(max))
			);
		}

		inline unsigned char Rand(unsigned char min, unsigned char max)
		{
			return static_cast<unsigned char>(
				RandomInt(static_cast<unsigned int>(min), static_cast<unsigned int>(max))
			);
		}

		inline float Rand(float min, float max)
		{
			return RandomReal(min, max);
		}

		inline double Rand(double min, double max)
		{
			return RandomReal(min, max);
		}
	}
}