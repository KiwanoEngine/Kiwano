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
		using ConsoleColor = std::wostream&(*)(std::wostream&);

		namespace color
		{
			const wchar_t _reset[] = L"\x1b[0m";

			const wchar_t _black[] = L"\x1b[30m";
			const wchar_t _red[] = L"\x1b[31m";
			const wchar_t _green[] = L"\x1b[32m";
			const wchar_t _yellow[] = L"\x1b[33m";
			const wchar_t _blue[] = L"\x1b[34m";
			const wchar_t _white[] = L"\x1b[37m";

			const wchar_t _black_bg[] = L"\x1b[40m";
			const wchar_t _red_bg[] = L"\x1b[41m";
			const wchar_t _green_bg[] = L"\x1b[42m";
			const wchar_t _yellow_bg[] = L"\x1b[43m";
			const wchar_t _blue_bg[] = L"\x1b[44m";
			const wchar_t _white_bg[] = L"\x1b[47m";

#define DECLARE_COLOR(COLOR) \
	inline std::wostream& (COLOR)(std::wostream& _out)\
		{ _out.write(_##COLOR, 5); return _out; }\
	inline std::wostream& (COLOR##_bg)(std::wostream& _out)\
		{ _out.write(_##COLOR##_bg, 5); return _out; }

			inline std::wostream& (reset)(std::wostream& _out) { _out.write(_reset, 4); return _out; }

			DECLARE_COLOR(red);
			DECLARE_COLOR(green);
			DECLARE_COLOR(yellow);
			DECLARE_COLOR(blue);
			DECLARE_COLOR(white);

#undef DECLARE_COLOR
		}

		namespace
		{
#ifdef E2D_DEBUG
			bool enabled = true;
#else
			bool enabled = false;
#endif

			std::wostream& OutPrefix(std::wostream& out)
			{
				std::time_t unix = std::time(nullptr);
				std::tm tmbuf;
				localtime_s(&tmbuf, &unix);
				out << std::put_time(&tmbuf, L"[easy2d] %H:%M:%S ");
				return out;
			}

			void Output(std::wostream& os, ConsoleColor color, bool endl, const wchar_t* prompt, const wchar_t* format, va_list args)
			{
				if (!enabled)
					return;

				size_t len = ::_vscwprintf(format, args) + 1;
				wchar_t* buffer = new wchar_t[len];
				::_vsnwprintf_s(buffer, len, len, format, args);

				std::wstringstream ss;
				if (prompt)
					ss << OutPrefix << prompt << buffer;
				else
					ss << OutPrefix << buffer;

				if (endl)
					ss << L"\r\n";

				std::wstring output = ss.str();
				os << color << output << color::reset;
				::OutputDebugStringW(output.c_str());

				delete[] buffer;
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

			Output(std::wcout, color::white, false, nullptr, format, args);

			va_end(args);
		}

		void Println(const wchar_t* format, ...)
		{
			va_list args = nullptr;
			va_start(args, format);

			Output(std::wcout, color::white, true, nullptr, format, args);

			va_end(args);
		}

		void Message(const wchar_t * format, ...)
		{
			va_list args = nullptr;
			va_start(args, format);

			Output(std::wcout, color::blue, false, nullptr, format, args);

			va_end(args);
		}

		void Messageln(const wchar_t * format, ...)
		{
			va_list args = nullptr;
			va_start(args, format);

			Output(std::wcout, color::blue, true, nullptr, format, args);

			va_end(args);
		}

		void Warning(const wchar_t* format, ...)
		{
			va_list args = nullptr;
			va_start(args, format);

			Output(std::wcerr, color::yellow_bg, false, L"Warning: ", format, args);

			va_end(args);
		}

		void Warningln(const wchar_t* format, ...)
		{
			va_list args = nullptr;
			va_start(args, format);

			Output(std::wcerr, color::yellow_bg, true, L"Warning: ", format, args);

			va_end(args);
		}

		void Error(const wchar_t* format, ...)
		{
			va_list args = nullptr;
			va_start(args, format);

			Output(std::wcerr, color::red_bg, false, L"Error: ", format, args);

			va_end(args);
		}

		void Errorln(const wchar_t* format, ...)
		{
			va_list args = nullptr;
			va_start(args, format);

			Output(std::wcerr, color::red_bg, true, L"Error: ", format, args);

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
