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

namespace easy2d
{
	namespace logs
	{
		namespace
		{
			inline void OutputDebugStringExW(LPCWSTR pszOutput, ...)
			{
				va_list args = NULL;
				va_start(args, pszOutput);

				size_t nLen = ::_vscwprintf(pszOutput, args) + 1;
				wchar_t* psBuffer = new wchar_t[nLen];
				::_vsnwprintf_s(psBuffer, nLen, nLen, pszOutput, args);

				va_end(args);

				::OutputDebugStringW(psBuffer);
				delete[] psBuffer;
			}
		}

		inline void Trace(LPCWSTR output)
		{
			OutputDebugStringExW(L"[easy2d] Error: %s\r\n", output);
		}

		inline void Trace(LPCWSTR output, HRESULT hr)
		{
			OutputDebugStringExW(L"[easy2d] Failure with HRESULT of %08X: %s\r\n", output, static_cast<unsigned int>(hr));
		}
	}
}
