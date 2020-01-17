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
#include <ctime>

#include <kiwano/core/Logger.h>

namespace
{
	std::streambuf* cin_buffer, * cout_buffer, * cerr_buffer;
	std::fstream console_input, console_output, console_error;

	std::wstreambuf* wcin_buffer, * wcout_buffer, * wcerr_buffer;
	std::wfstream wconsole_input, wconsole_output, wconsole_error;

	void RedirectStdIO()
	{
		cin_buffer = std::cin.rdbuf();
		cout_buffer = std::cout.rdbuf();
		cerr_buffer = std::cerr.rdbuf();
		wcin_buffer = std::wcin.rdbuf();
		wcout_buffer = std::wcout.rdbuf();
		wcerr_buffer = std::wcerr.rdbuf();

		console_input.open("CONIN$", std::ios::in);
		console_output.open("CONOUT$", std::ios::out);
		console_error.open("CONOUT$", std::ios::out);
		wconsole_input.open("CONIN$", std::ios::in);
		wconsole_output.open("CONOUT$", std::ios::out);
		wconsole_error.open("CONOUT$", std::ios::out);

		FILE* dummy;
		freopen_s(&dummy, "CONOUT$", "w+t", stdout);
		freopen_s(&dummy, "CONIN$", "r+t", stdin);
		freopen_s(&dummy, "CONOUT$", "w+t", stderr);
		(void)dummy;

		std::cin.rdbuf(console_input.rdbuf());
		std::cout.rdbuf(console_output.rdbuf());
		std::cerr.rdbuf(console_error.rdbuf());
		std::wcin.rdbuf(wconsole_input.rdbuf());
		std::wcout.rdbuf(wconsole_output.rdbuf());
		std::wcerr.rdbuf(wconsole_error.rdbuf());
	}

	void ResetStdIO()
	{
		console_input.close();
		console_output.close();
		console_error.close();
		wconsole_input.close();
		wconsole_output.close();
		wconsole_error.close();

		std::cin.rdbuf(cin_buffer);
		std::cout.rdbuf(cout_buffer);
		std::cerr.rdbuf(cerr_buffer);
		std::wcin.rdbuf(wcin_buffer);
		std::wcout.rdbuf(wcout_buffer);
		std::wcerr.rdbuf(wcerr_buffer);

		fclose(stdout);
		fclose(stdin);
		fclose(stderr);

		cin_buffer = nullptr;
		cout_buffer = nullptr;
		cerr_buffer = nullptr;
		wcin_buffer = nullptr;
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
	namespace console_colors
	{
		const WORD blue			= FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		const WORD green		= FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		const WORD red			= FOREGROUND_RED | FOREGROUND_INTENSITY;
		const WORD yellow		= FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
		const WORD white		= FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;

		const WORD blue_bg		= white | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
		const WORD green_bg		= white | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
		const WORD red_bg		= white | BACKGROUND_RED | BACKGROUND_INTENSITY;
		const WORD yellow_bg	= BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;
		const WORD white_bg		= BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;

		const WORD reset		= white;
	}

#define DECLARE_HANDLE_COLOR(NAME, HANDLE_NAME, COLOR)\
	inline std::wostream& (NAME)(std::wostream& _out)\
	{\
		::SetConsoleTextAttribute(::GetStdHandle(HANDLE_NAME), console_colors::##COLOR);\
		return _out;\
	}

#define DECLARE_COLOR(COLOR) \
	DECLARE_HANDLE_COLOR(stdout_##COLOR, STD_OUTPUT_HANDLE, COLOR)\
	DECLARE_HANDLE_COLOR(stderr_##COLOR, STD_ERROR_HANDLE, COLOR)

#define DECLARE_BG_COLOR(COLOR) \
	DECLARE_HANDLE_COLOR(stdout_##COLOR##_bg, STD_OUTPUT_HANDLE, COLOR##_bg)\
	DECLARE_HANDLE_COLOR(stderr_##COLOR##_bg, STD_ERROR_HANDLE, COLOR##_bg)

	namespace console_colors
	{
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

	void Logger::Printf(Level level, const wchar_t* format, ...)
	{
		if (!enabled_)
			return;

		StringStream sstream;
		Prepare(level, sstream);

		// Format message
		if (format)
		{
			va_list args = nullptr;
			va_start(args, format);

			static wchar_t temp_buffer[1024 * 3 + 1];
			const auto len = ::_vscwprintf(format, args) + 1;
			::_vsnwprintf_s(temp_buffer, len, len, format, args);

			sstream << ' ' << temp_buffer << L"\r\n";

			va_end(args);
		}

		Output(level, sstream);
	}

	void Logger::Prepare(Level level, StringStream& sstream)
	{
		String prefix;

		switch (level)
		{
		case Level::Info:
			prefix = L"[INFO] ";
			break;
		case Level::System:
			prefix = L"[SYSTEM] ";
			break;
		case Level::Warning:
			prefix = L"[WARN] ";
			break;
		case Level::Error:
			prefix = L"[ERROR] ";
			break;
		}

		// Timestamp
		time_t unix = std::time(nullptr);
		std::tm tmbuf;
		localtime_s(&tmbuf, &unix);
		sstream << prefix << std::put_time(&tmbuf, L"%H:%M:%S ");
	}

	void Logger::Output(Level level, StringStream& sstream)
	{
		OutputStream* ostream = nullptr;
		ConsoleColor color = nullptr;

		switch (level)
		{
		case Level::Info:
			ostream = &output_stream_;
			color = Closure(this, &Logger::DefaultOutputColor);
			break;
		case Level::System:
			ostream = &output_stream_;
			color = console_colors::stdout_blue;
			break;
		case Level::Warning:
			ostream = &output_stream_;
			color = console_colors::stdout_yellow_bg;
			break;
		case Level::Error:
			ostream = &error_stream_;
			color = console_colors::stderr_red_bg;
			break;
		}

		// Printing
		if (ostream)
		{
			auto output = sstream.str();
			color(*ostream) << output << std::flush;
			::OutputDebugStringW(output.c_str());

			ResetConsoleColor();
		}
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
