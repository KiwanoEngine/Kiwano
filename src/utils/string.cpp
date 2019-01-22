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

#include "string.h"
#include "../core/macros.h"
#include "../core/logs.h"

namespace easy2d
{
	std::wstring StringMultiByteToWideChar(const std::string& str)
	{
		std::wstring ret;
		if (!str.empty())
		{
			int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
			if (len)
			{
				WCHAR* wstr_tmp = new WCHAR[len + 1];
				wstr_tmp[0] = 0;

				len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wstr_tmp, len + 1);

				ret = wstr_tmp;
				delete[] wstr_tmp;
			}
			else
			{
				logs::Errorln(HRESULT_FROM_WIN32(GetLastError()), "Wrong convert to WideChar code");
			}
		}
		return ret;
	}

	std::string StringWideCharToMultiByte(const std::wstring& wstr)
	{
		std::string ret;
		if (!wstr.empty())
		{
			int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, FALSE);
			if (len)
			{
				char* str_tmp = new char[len + 1];
				str_tmp[0] = 0;

				len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, str_tmp, len + 1, nullptr, FALSE);

				ret = str_tmp;
				delete[] str_tmp;
			}
			else
			{
				logs::Errorln(HRESULT_FROM_WIN32(GetLastError()), ("Wrong convert to MultiByte code"));
			}
		}

		return ret;
	}
}
