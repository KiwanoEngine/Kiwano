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
#include "macros.h"

#ifndef E2D_LOG
#	ifdef E2D_DEBUG
#		define E2D_LOG(format, ...) easy2d::logs::Println(format, ##__VA_ARGS__)
#	else
#		define E2D_LOG __noop
#	endif
#endif

namespace easy2d
{
	namespace logs
	{
		void Enable();

		void Disable();

		void Print(const wchar_t* format, ...);

		void Println(const wchar_t* format, ...);

		void Warning(const wchar_t* format, ...);

		void Warningln(const wchar_t* format, ...);

		void Error(const wchar_t* format, ...);

		void Errorln(const wchar_t* format, ...);

		void Errorln(HRESULT hr);

		void Errorln(HRESULT hr, const wchar_t* output);
	}

	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			logs::Errorln(hr);
			throw std::runtime_error("Fatal error");
		}
	}
}
