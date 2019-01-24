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
#include <functional>

#define CLOSURE_PARAMS_TYPE_1	typename Param1
#define CLOSURE_PARAMS_1		Param1
#define CLOSURE_PLACEHOLDERS_1	std::placeholders::_1
#define CLOSURE_PARAMS_TYPE_2	typename Param1, typename Param2
#define CLOSURE_PARAMS_2		Param1, Param2
#define CLOSURE_PLACEHOLDERS_2	std::placeholders::_1, std::placeholders::_2
#define CLOSURE_PARAMS_TYPE_3	typename Param1, typename Param2, typename Param3
#define CLOSURE_PARAMS_3		Param1, Param2, Param3
#define CLOSURE_PLACEHOLDERS_3	std::placeholders::_1, std::placeholders::_2, std::placeholders::_3
#define CLOSURE_PARAMS_TYPE_4	typename Param1, typename Param2, typename Param3, typename Param4
#define CLOSURE_PARAMS_4		Param1, Param2, Param3, Param4
#define CLOSURE_PLACEHOLDERS_4	std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4
#define CLOSURE_PARAMS_TYPE_5	typename Param1, typename Param2, typename Param3, typename Param4, typename Param5
#define CLOSURE_PARAMS_5		Param1, Param2, Param3, Param4, Param5
#define CLOSURE_PLACEHOLDERS_5	std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5
#define CLOSURE_PARAMS_TYPE_6	typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6
#define CLOSURE_PARAMS_6		Param1, Param2, Param3, Param4, Param5, Param6
#define CLOSURE_PLACEHOLDERS_6	std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6

#define CLOSURE_EXPAND(x) x

#define CLOSURE_DECLARE(TYPE, PARAMS, PHS)\
	template<typename T, typename R, CLOSURE_EXPAND(TYPE)>\
	inline std::function<R(CLOSURE_EXPAND(PARAMS))> Closure(T* ptr, R(T::*func)(CLOSURE_EXPAND(PARAMS)))\
	{\
		return std::bind(func, ptr, CLOSURE_EXPAND(PHS));\
	}

namespace easy2d
{
	template<typename T, typename R>
	inline std::function<R(void)> Closure(T* ptr, R(T::*func)(void))
	{
		return std::bind(func, ptr);
	}

	CLOSURE_DECLARE(CLOSURE_PARAMS_TYPE_1, CLOSURE_PARAMS_1, CLOSURE_PLACEHOLDERS_1);
	CLOSURE_DECLARE(CLOSURE_PARAMS_TYPE_2, CLOSURE_PARAMS_2, CLOSURE_PLACEHOLDERS_2);
	CLOSURE_DECLARE(CLOSURE_PARAMS_TYPE_3, CLOSURE_PARAMS_3, CLOSURE_PLACEHOLDERS_3);
	CLOSURE_DECLARE(CLOSURE_PARAMS_TYPE_4, CLOSURE_PARAMS_4, CLOSURE_PLACEHOLDERS_4);
	CLOSURE_DECLARE(CLOSURE_PARAMS_TYPE_5, CLOSURE_PARAMS_5, CLOSURE_PLACEHOLDERS_5);
	CLOSURE_DECLARE(CLOSURE_PARAMS_TYPE_6, CLOSURE_PARAMS_6, CLOSURE_PLACEHOLDERS_6);
}

#undef CLOSURE_PARAMS_TYPE_1
#undef CLOSURE_PARAMS_1
#undef CLOSURE_PLACEHOLDERS_1
#undef CLOSURE_PARAMS_TYPE_2
#undef CLOSURE_PARAMS_2
#undef CLOSURE_PLACEHOLDERS_2
#undef CLOSURE_PARAMS_TYPE_3
#undef CLOSURE_PARAMS_3
#undef CLOSURE_PLACEHOLDERS_3
#undef CLOSURE_PARAMS_TYPE_4
#undef CLOSURE_PARAMS_4
#undef CLOSURE_PLACEHOLDERS_4
#undef CLOSURE_PARAMS_TYPE_5
#undef CLOSURE_PARAMS_5
#undef CLOSURE_PLACEHOLDERS_5
#undef CLOSURE_PARAMS_TYPE_6
#undef CLOSURE_PARAMS_6
#undef CLOSURE_PLACEHOLDERS_6
#undef CLOSURE_EXPAND
#undef CLOSURE_DECLARE
