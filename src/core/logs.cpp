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
#include "logs.h"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace easy2d
{
	namespace logs
	{
		namespace
		{
			bool enabled = true;

			void Out(std::wostream& stream, const wchar_t* output)
			{
				stream << output;
				::OutputDebugStringW(output);
			}

			void OutPrefix(std::wstringstream& ss)
			{
				std::time_t unix = ::time(NULL);
				struct tm tmbuf;
				localtime_s(&tmbuf, &unix);
				ss << std::put_time(&tmbuf, L"[easy2d] %H:%M:%S ");
			}

			void Output(std::wostream& stream, const wchar_t* prompt, const wchar_t* format, va_list args)
			{
				if (!enabled)
					return;

				size_t len = ::_vscwprintf(format, args) + 1;
				wchar_t* buffer = new wchar_t[len];
				::_vsnwprintf_s(buffer, len, len, format, args);

				std::wstringstream ss;
				OutPrefix(ss);
				ss << prompt << buffer;
				Out(stream, ss.str().c_str());

				delete[] buffer;
			}

			void OutputLine(std::wostream& stream, const wchar_t* prompt, const wchar_t* format, va_list args)
			{
				if (!enabled)
					return;

				Output(stream, prompt, format, args);
				Out(stream, L"\r\n");
			}
		}

		void easy2d::logs::Enable()
		{
			enabled = true;
		}

		void easy2d::logs::Disable()
		{
			enabled = false;
		}

		void Print(const wchar_t* format, ...)
		{
			va_list args = nullptr;
			va_start(args, format);

			Output(std::wcout, L"", format, args);

			va_end(args);
		}

		void Println(const wchar_t* format, ...)
		{
			va_list args = nullptr;
			va_start(args, format);

			OutputLine(std::wcout, L"", format, args);

			va_end(args);
		}

		void Warning(const wchar_t* format, ...)
		{
			va_list args = nullptr;
			va_start(args, format);

			Output(std::wcerr, L"Warning: ", format, args);

			va_end(args);
		}

		void Warningln(const wchar_t* format, ...)
		{
			va_list args = nullptr;
			va_start(args, format);

			OutputLine(std::wcerr, L"Warning: ", format, args);

			va_end(args);
		}

		void Error(const wchar_t* format, ...)
		{
			va_list args = nullptr;
			va_start(args, format);

			Output(std::wcerr, L"Error: ", format, args);

			va_end(args);
		}

		void Errorln(const wchar_t* format, ...)
		{
			va_list args = nullptr;
			va_start(args, format);

			OutputLine(std::wcerr, L"Error: ", format, args);

			va_end(args);
		}

		void Errorln(HRESULT hr)
		{
			Errorln(L"failure with HRESULT of %08X", hr);
		}

		void Errorln(HRESULT hr, const wchar_t* output)
		{
			Errorln(L"failure with HRESULT of %08X: %s", hr, output);
		}
	}
}
