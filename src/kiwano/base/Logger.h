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

#pragma once
#include "../macros.h"
#include "../core/core.h"
#include <ctime>
#include <iomanip>
#include <sstream>

#ifndef KGE_LOG
#	ifdef KGE_DEBUG
#		define KGE_LOG(FORMAT, ...) ::kiwano::Logger::GetInstance()->Messagef((FORMAT ## "\n"), __VA_ARGS__)
#	else
#		define KGE_LOG __noop
#	endif
#endif

#ifndef KGE_WARNING_LOG
#	define KGE_WARNING_LOG(FORMAT, ...) ::kiwano::Logger::GetInstance()->Warningf((FORMAT ## "\n"), __VA_ARGS__)
#endif

#ifndef KGE_ERROR_LOG
#	define KGE_ERROR_LOG(FORMAT, ...) ::kiwano::Logger::GetInstance()->Errorf((FORMAT ## "\n"), __VA_ARGS__)
#endif

#ifndef KGE_PRINT
#	define KGE_PRINT(...) ::kiwano::Logger::GetInstance()->Println(__VA_ARGS__)
#endif

#ifndef KGE_PRINTF
#	define KGE_PRINTF(FORMAT, ...) ::kiwano::Logger::GetInstance()->Printf((FORMAT), __VA_ARGS__)
#endif

namespace kiwano
{
	class KGE_API Logger
		: public Singleton<Logger>
	{
		KGE_DECLARE_SINGLETON(Logger);

	public:
		// 显示或关闭控制台
		void ShowConsole(bool show);

		// 启用 Logger
		void Enable();

		// 禁用 Logger
		void Disable();

		void Printf(const wchar_t* format, ...);

		void Messagef(const wchar_t * format, ...);

		void Warningf(const wchar_t* format, ...);

		void Errorf(const wchar_t* format, ...);

		template <typename ..._Args>
		void Print(_Args&& ... args);

		template <typename ..._Args>
		void Println(_Args&& ... args);

		template <typename ..._Args>
		void Message(_Args&& ... args);

		template <typename ..._Args>
		void Messageln(_Args&& ... args);

		template <typename ..._Args>
		void Warning(_Args&& ... args);

		template <typename ..._Args>
		void Warningln(_Args&& ... args);

		template <typename ..._Args>
		void Error(_Args&& ... args);

		template <typename ..._Args>
		void Errorln(_Args&& ... args);

		std::wstreambuf* RedirectOutputStreamBuffer(std::wstreambuf* buf);

		std::wstreambuf* RedirectErrorStreamBuffer(std::wstreambuf* buf);

		void ResetOutputStream();

	private:
		Logger();

		~Logger();

		void Outputf(std::wostream& os, std::wostream&(*color)(std::wostream&), const wchar_t* prompt, const wchar_t* format, va_list args) const;

		std::wstring MakeOutputStringf(const wchar_t* prompt, const wchar_t* format, va_list args) const;

		template <typename ..._Args>
		void OutputLine(std::wostream& os, std::wostream& (*color)(std::wostream&), const wchar_t* prompt, _Args&& ... args) const;

		template <typename ..._Args>
		void Output(std::wostream& os, std::wostream& (*color)(std::wostream&), const wchar_t* prompt, _Args&& ... args) const;

		template <typename ..._Args>
		std::wstring MakeOutputString(const wchar_t* prompt, _Args&& ... args) const;

		void ResetConsoleColor() const;

		static std::wostream& DefaultOutputColor(std::wostream& out);

		static std::wostream& OutPrefix(std::wostream& out);

	private:
		bool enabled_;
		WORD default_stdout_color_;
		WORD default_stderr_color_;

		std::wostream output_stream_;
		std::wostream error_stream_;
	};


	//
	// details of Logger
	//

	namespace __console_colors
	{
#define DECLARE_COLOR(COLOR)\
		extern std::wostream&(stdout_##COLOR)(std::wostream&);\
		extern std::wostream&(stderr_##COLOR)(std::wostream&);

#define DECLARE_BG_COLOR(COLOR)\
		extern std::wostream&(stdout_##COLOR##_bg)(std::wostream&);\
		extern std::wostream&(stderr_##COLOR##_bg)(std::wostream&);

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


	inline void Logger::Enable()
	{
		enabled_ = true;
	}

	inline void Logger::Disable()
	{
		enabled_ = false;
	}

	template <typename ..._Args>
	inline void Logger::Print(_Args&& ... args)
	{
		Output(output_stream_, Logger::DefaultOutputColor, nullptr, std::forward<_Args>(args)...);
	}

	template <typename ..._Args>
	inline void Logger::Println(_Args&& ... args)
	{
		OutputLine(output_stream_, Logger::DefaultOutputColor, nullptr, std::forward<_Args>(args)...);
	}

	template <typename ..._Args>
	inline void Logger::Message(_Args&& ... args)
	{
		using namespace __console_colors;
		Output(output_stream_, stdout_blue, nullptr, std::forward<_Args>(args)...);
	}

	template <typename ..._Args>
	inline void Logger::Messageln(_Args&& ... args)
	{
		using namespace __console_colors;
		OutputLine(output_stream_, stdout_blue, nullptr, std::forward<_Args>(args)...);
	}

	template <typename ..._Args>
	inline void Logger::Warning(_Args&& ... args)
	{
		using namespace __console_colors;
		Output(output_stream_, stdout_yellow_bg, L"Warning:", std::forward<_Args>(args)...);
	}

	template <typename ..._Args>
	inline void Logger::Warningln(_Args&& ... args)
	{
		using namespace __console_colors;
		OutputLine(output_stream_, stdout_yellow_bg, L"Warning:", std::forward<_Args>(args)...);
	}

	template <typename ..._Args>
	inline void Logger::Error(_Args&& ... args)
	{
		using namespace __console_colors;
		Output(error_stream_, stderr_red_bg, L"Error:", std::forward<_Args>(args)...);
	}

	template <typename ..._Args>
	inline void Logger::Errorln(_Args&& ... args)
	{
		using namespace __console_colors;
		OutputLine(error_stream_, stderr_red_bg, L"Error:", std::forward<_Args>(args)...);
	}

	template <typename ..._Args>
	void Logger::OutputLine(std::wostream& os, std::wostream& (*color)(std::wostream&), const wchar_t* prompt, _Args&& ... args) const
	{
		if (enabled_)
		{
			Output(os, color, prompt, std::forward<_Args>(args)...);

			os << std::endl;
			::OutputDebugStringW(L"\r\n");
		}
	}

	template <typename ..._Args>
	void Logger::Output(std::wostream& os, std::wostream& (*color)(std::wostream&), const wchar_t* prompt, _Args&& ... args) const
	{
		if (enabled_)
		{
			std::wstring output = MakeOutputString(prompt, std::forward<_Args>(args)...);

			os << color << output << std::flush;
			::OutputDebugStringW(output.c_str());

			ResetConsoleColor();
		}
	}

	template <typename ..._Args>
	std::wstring Logger::MakeOutputString(const wchar_t* prompt, _Args&& ... args) const
	{
		std::wstringstream ss;
		ss << Logger::OutPrefix;

		if (prompt)
			ss << prompt;

		(void)std::initializer_list<int>{((ss << ' ' << args), 0)...};

		return ss.str();
	}

	inline void Logger::ResetConsoleColor() const
	{
		::SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), default_stdout_color_);
		::SetConsoleTextAttribute(::GetStdHandle(STD_ERROR_HANDLE), default_stderr_color_);
	}

	inline std::wostream& Logger::DefaultOutputColor(std::wostream& out)
	{
		::SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), Logger::GetInstance()->default_stdout_color_);
		return out;
	}
}

//
// Display stack trace on exception
//

#include "../third-party/StackWalker/StackWalker.h"

namespace kiwano
{
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			KGE_ERROR_LOG(L"Fatal error with HRESULT of %08X", hr);

			StackWalker{}.ShowCallstack();

			static char buffer[1024 + 1];
			sprintf_s(buffer, "Fatal error with HRESULT of %08X", hr);
			throw std::runtime_error(buffer);
		}
	}
}
