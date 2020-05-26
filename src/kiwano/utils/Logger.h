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
#include <kiwano/core/Common.h>
#include <iomanip>
#include <streambuf>

#ifndef KGE_SYS_LOG
#ifdef KGE_DEBUG
#define KGE_SYS_LOG(FORMAT, ...) \
    ::kiwano::Logger::GetInstance().Printf(::kiwano::Logger::Level::System, FORMAT, __VA_ARGS__)
#else
#define KGE_SYS_LOG __noop
#endif
#endif

#ifndef KGE_WARN
#define KGE_WARN(FORMAT, ...) ::kiwano::Logger::GetInstance().Printf(::kiwano::Logger::Level::Warning, FORMAT, __VA_ARGS__)
#endif

#ifndef KGE_ERROR
#define KGE_ERROR(FORMAT, ...) ::kiwano::Logger::GetInstance().Printf(::kiwano::Logger::Level::Error, FORMAT, __VA_ARGS__)
#endif

#ifndef KGE_LOG
#define KGE_LOG(...) ::kiwano::Logger::GetInstance().Println(::kiwano::Logger::Level::Info, __VA_ARGS__)
#endif

#ifndef KGE_LOGF
#define KGE_LOGF(FORMAT, ...) ::kiwano::Logger::GetInstance().Printf(::kiwano::Logger::Level::Info, FORMAT, __VA_ARGS__)
#endif

#ifndef KGE_LOG_STREAM
#define KGE_LOG_STREAM() ::kiwano::Logger::GetInstance().GetOutputStream()
#endif

#ifndef KGE_ERROR_STREAM
#define KGE_ERROR_STREAM() ::kiwano::Logger::GetInstance().GetErrorStream()
#endif

namespace kiwano
{
/**
 * \~chinese
 * @brief 日志
 */
class KGE_API Logger : public Singleton<Logger>
{
    friend Singleton<Logger>;

public:
    /// \~chinese
    /// @brief 日志级别
    enum class Level
    {
        Info,     ///< 信息
        System,   ///< 系统
        Warning,  ///< 警告
        Error     ///< 错误
    };

    /// \~chinese
    /// @brief 打印日志
    /// @param level 日志级别
    /// @param format 格式字符串
    void Printf(Level level, const char* format, ...);

    /// \~chinese
    /// @brief 打印日志
    /// @param level 日志级别
    /// @param args 参数
    template <typename... _Args>
    void Print(Level level, _Args&&... args);

    /// \~chinese
    /// @brief 打印一行日志
    /// @param level 日志级别
    /// @param args 参数
    template <typename... _Args>
    void Println(Level level, _Args&&... args);

    /// \~chinese
    /// @brief 启用日志
    void Enable();

    /// \~chinese
    /// @brief 禁用日志
    void Disable();

    /// \~chinese
    /// @brief 获取输出流
    OutputStream& GetOutputStream();

    /// \~chinese
    /// @brief 获取错误输出流
    OutputStream& GetErrorStream();

    /// \~chinese
    /// @brief 重定向输出流
    std::streambuf* RedirectOutputStream(std::streambuf* buf);

    /// \~chinese
    /// @brief 重定向错误输出流
    std::streambuf* RedirectErrorStream(std::streambuf* buf);

    /// \~chinese
    /// @brief 显示或关闭控制台
    /// @note 此操作会重定向输出流到标准输出流
    void ShowConsole(bool show);

private:
    Logger();

    ~Logger();

    void Prepare(Level level, StringStream& sstream);

    void Output(Level level, StringStream& sstream);

    void ResetStreamToStdStream();

    void ResetConsoleColor() const;

    OutputStream& DefaultOutputColor(OutputStream& out);

private:
    bool enabled_;
    WORD default_stdout_color_;
    WORD default_stderr_color_;

    OutputStream output_stream_;
    OutputStream error_stream_;
};

inline void Logger::Enable()
{
    enabled_ = true;
}

inline void Logger::Disable()
{
    enabled_ = false;
}

template <typename... _Args>
void Logger::Print(Level level, _Args&&... args)
{
    if (!enabled_)
        return;

    StringStream sstream;
    Prepare(level, sstream);

    // Format message
    (void)std::initializer_list<int>{ ((sstream << ' ' << args), 0)... };

    Output(level, sstream);
}

template <typename... _Args>
void Logger::Println(Level level, _Args&&... args)
{
    if (!enabled_)
        return;

    StringStream sstream;
    Prepare(level, sstream);

    // Format message
    (void)std::initializer_list<int>{ ((sstream << ' ' << args), 0)... };

    sstream << "\r\n";

    Output(level, sstream);
}

inline void Logger::ResetConsoleColor() const
{
    ::SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), default_stdout_color_);
    ::SetConsoleTextAttribute(::GetStdHandle(STD_ERROR_HANDLE), default_stderr_color_);
}

inline OutputStream& Logger::DefaultOutputColor(OutputStream& out)
{
    ::SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), default_stdout_color_);
    return out;
}

inline OutputStream& Logger::GetOutputStream()
{
    return output_stream_;
}

inline OutputStream& Logger::GetErrorStream()
{
    return error_stream_;
}
}  // namespace kiwano
