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
			const WORD _blue = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			const WORD _green = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			const WORD _red = FOREGROUND_RED | FOREGROUND_INTENSITY;
			const WORD _yellow = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
			const WORD _white = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;

			const WORD _blue_bg = _white | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
			const WORD _green_bg = _white | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
			const WORD _red_bg = _white | BACKGROUND_RED | BACKGROUND_INTENSITY;
			const WORD _yellow_bg = BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;
			const WORD _white_bg = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;

			const WORD _reset = _white;


#define DECLARE_COLOR(COLOR) \
	inline std::wostream& (stdout_##COLOR)(std::wostream& _out)\
		{ ::SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), _##COLOR); return _out; }\
	\
	inline std::wostream& (stderr_##COLOR)(std::wostream& _out)\
		{ ::SetConsoleTextAttribute(::GetStdHandle(STD_ERROR_HANDLE), _##COLOR); return _out; }

#define DECLARE_BG_COLOR(COLOR) \
	inline std::wostream& (stdout_##COLOR##_bg)(std::wostream& _out)\
		{ ::SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), _##COLOR##_bg); return _out; }\
	\
	inline std::wostream& (stderr_##COLOR##_bg)(std::wostream& _out)\
		{ ::SetConsoleTextAttribute(::GetStdHandle(STD_ERROR_HANDLE), _##COLOR##_bg); return _out; }

			DECLARE_COLOR(red);
			DECLARE_COLOR(green);
			DECLARE_COLOR(yellow);
			DECLARE_COLOR(blue);
			DECLARE_COLOR(white);
			DECLARE_COLOR(reset);

			DECLARE_BG_COLOR(red);
			DECLARE_BG_COLOR(green);
			DECLARE_BG_COLOR(yellow);
			DECLARE_BG_COLOR(blue);
			DECLARE_BG_COLOR(white);

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

				static wchar_t temp_buffer[1024 * 3 + 1];

				size_t len = ::_vscwprintf(format, args) + 1;
				::_vsnwprintf_s(temp_buffer, len, len, format, args);

				std::wstringstream ss;
				if (prompt)
					ss << OutPrefix << prompt << temp_buffer;
				else
					ss << OutPrefix << temp_buffer;

				if (endl)
					ss << L"\r\n";

				std::wstring output = ss.str();
				os << color << output;
				::OutputDebugStringW(output.c_str());
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

			Output(std::wcout, color::stdout_white, false, nullptr, format, args);

			va_end(args);
		}

		void Println(const wchar_t* format, ...)
		{
			va_list args = nullptr;
			va_start(args, format);

			Output(std::wcout, color::stdout_white, true, nullptr, format, args);

			va_end(args);
		}

		void Message(const wchar_t * format, ...)
		{
			va_list args = nullptr;
			va_start(args, format);

			Output(std::wcout, color::stdout_blue, false, nullptr, format, args);

			va_end(args);
		}

		void Messageln(const wchar_t * format, ...)
		{
			va_list args = nullptr;
			va_start(args, format);

			Output(std::wcout, color::stdout_blue, true, nullptr, format, args);

			va_end(args);
		}

		void Warning(const wchar_t* format, ...)
		{
			va_list args = nullptr;
			va_start(args, format);

			Output(std::wcerr, color::stdout_yellow_bg, false, L"Warning: ", format, args);

			va_end(args);
		}

		void Warningln(const wchar_t* format, ...)
		{
			va_list args = nullptr;
			va_start(args, format);

			Output(std::wcerr, color::stdout_yellow_bg, true, L"Warning: ", format, args);

			va_end(args);
		}

		void Error(const wchar_t* format, ...)
		{
			va_list args = nullptr;
			va_start(args, format);

			Output(std::wcerr, color::stderr_red_bg, false, L"Error: ", format, args);

			va_end(args);
		}

		void Errorln(const wchar_t* format, ...)
		{
			va_list args = nullptr;
			va_start(args, format);

			Output(std::wcerr, color::stderr_red_bg, true, L"Error: ", format, args);

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
