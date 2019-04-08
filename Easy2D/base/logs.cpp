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

#include "logs.h"
#include <iostream>

namespace easy2d
{
	namespace __console_colors
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

#define DECLARE_HANDLE_COLOR(NAME, HANDLE_NAME, COLOR)\
	inline std::wostream& (NAME)(std::wostream& _out)\
		{ ::SetConsoleTextAttribute(::GetStdHandle(HANDLE_NAME), _##COLOR); return _out; }

#define DECLARE_COLOR(COLOR) \
	DECLARE_HANDLE_COLOR(stdout_##COLOR, STD_OUTPUT_HANDLE, COLOR)\
	DECLARE_HANDLE_COLOR(stderr_##COLOR, STD_ERROR_HANDLE, COLOR)

#define DECLARE_BG_COLOR(COLOR) \
	DECLARE_HANDLE_COLOR(stdout_##COLOR##_bg, STD_OUTPUT_HANDLE, COLOR##_bg)\
	DECLARE_HANDLE_COLOR(stderr_##COLOR##_bg, STD_ERROR_HANDLE, COLOR##_bg)

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
#undef DECLARE_BG_COLOR
	}

	Logger::Logger()
		: enabled_(true)
		, has_console_(false)
		, default_stdout_color_(0)
		, default_stderr_color_(0)
		, output_stream_(std::wcout.rdbuf())
		, error_stream_(std::wcerr.rdbuf())
	{
		ResetOutputStream();
	}

	void Logger::ResetOutputStream()
	{
		has_console_ = ::GetConsoleWindow() != nullptr;
		if (has_console_)
		{
			default_stdout_color_ = default_stderr_color_ = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;

			CONSOLE_SCREEN_BUFFER_INFO stdout_info;
			if (::GetConsoleScreenBufferInfo(::GetStdHandle(STD_OUTPUT_HANDLE), &stdout_info))
			{
				default_stdout_color_ = stdout_info.wAttributes;
			}

			CONSOLE_SCREEN_BUFFER_INFO stderr_info;
			if (::GetConsoleScreenBufferInfo(::GetStdHandle(STD_ERROR_HANDLE), &stderr_info))
			{
				default_stderr_color_ = stderr_info.wAttributes;
			}

			RedirectOutputStreamBuffer(std::wcout.rdbuf());
			RedirectErrorStreamBuffer(std::wcerr.rdbuf());
		}
	}

	std::wstreambuf* Logger::RedirectOutputStreamBuffer(std::wstreambuf* buf)
	{
		return output_stream_.rdbuf(buf);
	}

	std::wstreambuf* Logger::RedirectErrorStreamBuffer(std::wstreambuf* buf)
	{
		return error_stream_.rdbuf(buf);
	}

	void Logger::Printf(const wchar_t* format, ...)
	{
		va_list args = nullptr;
		va_start(args, format);

		Outputf(output_stream_, Logger::DefaultOutputColor, nullptr, format, args);

		va_end(args);
	}

	void Logger::Messagef(const wchar_t* format, ...)
	{
		using namespace __console_colors;

		va_list args = nullptr;
		va_start(args, format);

		Outputf(output_stream_, stdout_blue, nullptr, format, args);

		va_end(args);
	}

	void Logger::Warningf(const wchar_t* format, ...)
	{
		using namespace __console_colors;

		va_list args = nullptr;
		va_start(args, format);

		Outputf(output_stream_, stdout_yellow_bg, L" Warning:", format, args);

		va_end(args);
	}

	void Logger::Errorf(const wchar_t* format, ...)
	{
		using namespace __console_colors;

		va_list args = nullptr;
		va_start(args, format);

		Outputf(error_stream_, stderr_red_bg, L" Error:", format, args);

		va_end(args);
	}

	void Logger::Outputf(std::wostream& os, std::wostream& (*color)(std::wostream&), const wchar_t* prompt, const wchar_t* format, va_list args) const
	{
		if (enabled_ && has_console_)
		{
			std::wstring output = MakeOutputStringf(prompt, format, args);

			os << color << output << std::flush;
			::OutputDebugStringW(output.c_str());

			ResetConsoleColor();
		}
	}

	std::wstring Logger::MakeOutputStringf(const wchar_t* prompt, const wchar_t* format, va_list args) const
	{
		static wchar_t temp_buffer[1024 * 3 + 1];

		std::wstringstream ss;
		ss << Logger::OutPrefix;

		if (prompt)
			ss << prompt;

		if (format)
		{
			const auto len = ::_vscwprintf(format, args) + 1;
			::_vsnwprintf_s(temp_buffer, len, len, format, args);

			ss << ' ' << temp_buffer;
		}
		return ss.str();
	}

	std::wostream& Logger::OutPrefix(std::wostream& out)
	{
		std::time_t unix = std::time(nullptr);
		std::tm tmbuf;
		localtime_s(&tmbuf, &unix);
		out << std::put_time(&tmbuf, L"[easy2d] %H:%M:%S");
		return out;
	}

}
