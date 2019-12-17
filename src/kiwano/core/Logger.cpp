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

#include <iostream>
#include <fstream>

#include <kiwano/core/Logger.h>

namespace
{
	std::streambuf* cout_buffer, * cerr_buffer;
	std::fstream console_output, console_error;

	std::wstreambuf* wcout_buffer, * wcerr_buffer;
	std::wfstream wconsole_output, wconsole_error;

	void RedirectStdIO()
	{
		cout_buffer = std::cout.rdbuf();
		cerr_buffer = std::cerr.rdbuf();
		wcout_buffer = std::wcout.rdbuf();
		wcerr_buffer = std::wcerr.rdbuf();

		console_output.open("CONOUT$", std::ios::out);
		console_error.open("CONOUT$", std::ios::out);
		wconsole_output.open("CONOUT$", std::ios::out);
		wconsole_error.open("CONOUT$", std::ios::out);

		FILE* dummy;
		freopen_s(&dummy, "CONOUT$", "w+t", stdout);
		freopen_s(&dummy, "CONOUT$", "w+t", stderr);
		(void)dummy;

		std::cout.rdbuf(console_output.rdbuf());
		std::cerr.rdbuf(console_error.rdbuf());
		std::wcout.rdbuf(wconsole_output.rdbuf());
		std::wcerr.rdbuf(wconsole_error.rdbuf());
	}

	void ResetStdIO()
	{
		console_output.close();
		console_error.close();
		wconsole_output.close();
		wconsole_error.close();

		std::cout.rdbuf(cout_buffer);
		std::cerr.rdbuf(cerr_buffer);
		std::wcout.rdbuf(wcout_buffer);
		std::wcerr.rdbuf(wcerr_buffer);

		fclose(stdout);
		fclose(stderr);

		cout_buffer = nullptr;
		cerr_buffer = nullptr;
		wcout_buffer = nullptr;
		wcerr_buffer = nullptr;
	}

	HWND allocated_console = nullptr;

	HWND AllocateConsole()
	{
		if (::AllocConsole())
		{
			allocated_console = ::GetConsoleWindow();

			if (allocated_console)
			{
				RedirectStdIO();
			}
		}
		return allocated_console;
	}

	void FreeAllocatedConsole()
	{
		if (allocated_console)
		{
			ResetStdIO();
			::FreeConsole();
			allocated_console = nullptr;
		}
	}

	HWND GetAllocatedConsole()
	{
		return allocated_console;
	}
}

namespace kiwano
{
	namespace __console_colors
	{
		const WORD _blue	= FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		const WORD _green	= FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		const WORD _red		= FOREGROUND_RED | FOREGROUND_INTENSITY;
		const WORD _yellow	= FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
		const WORD _white	= FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;

		const WORD _blue_bg		= _white | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
		const WORD _green_bg	= _white | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
		const WORD _red_bg		= _white | BACKGROUND_RED | BACKGROUND_INTENSITY;
		const WORD _yellow_bg	= BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;
		const WORD _white_bg	= BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;

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
		, default_stdout_color_(0)
		, default_stderr_color_(0)
		, output_stream_(std::wcout.rdbuf())
		, error_stream_(std::wcerr.rdbuf())
	{
		ResetOutputStream();
	}

	Logger::~Logger()
	{
		FreeAllocatedConsole();
	}

	void Logger::ResetOutputStream()
	{
		bool has_console = ::GetConsoleWindow() != nullptr;
		if (has_console)
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

			// replace the C++ global locale with the user-preferred locale
			(void)std::locale::global(std::locale(""));
			(void)std::wcout.imbue(std::locale());
			(void)std::wcerr.imbue(std::locale());

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
		if (enabled_)
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

		StringStream ss;
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
		out << L"[KIWANO] ";

		time_t unix = std::time(nullptr);
		std::tm tmbuf;
		localtime_s(&tmbuf, &unix);
		out << std::put_time(&tmbuf, L"%H:%M:%S");
		return out;
	}

	void Logger::ShowConsole(bool show)
	{
		HWND current_console = ::GetConsoleWindow();
		if (show)
		{
			if (current_console)
			{
				::ShowWindow(current_console, SW_SHOW);
			}
			else
			{
				HWND console = ::AllocateConsole();
				if (!console)
				{
					KGE_WARN(L"AllocConsole failed");
				}
				else
				{
					// disable the close button of console
					HMENU hmenu = ::GetSystemMenu(console, FALSE);
					::RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
				}
			}
		}
		else
		{
			if (current_console)
			{
				if (current_console == GetAllocatedConsole())
				{
					FreeAllocatedConsole();
				}
				else
				{
					::ShowWindow(current_console, SW_HIDE);
				}
			}
		}

		ResetOutputStream();
	}

}
