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
 * @brief ��־
 */
class KGE_API Logger : public Singleton<Logger>
{
    friend Singleton<Logger>;

public:
    /// \~chinese
    /// @brief ��־����
    enum class Level
    {
        Info,     ///< ��Ϣ
        System,   ///< ϵͳ
        Warning,  ///< ����
        Error     ///< ����
    };

    /// \~chinese
    /// @brief ��ӡ��־
    /// @param level ��־����
    /// @param format ��ʽ�ַ���
    void Printf(Level level, const char* format, ...);

    /// \~chinese
    /// @brief ��ӡ��־
    /// @param level ��־����
    /// @param args ����
    template <typename... _Args>
    void Print(Level level, _Args&&... args);

    /// \~chinese
    /// @brief ��ӡһ����־
    /// @param level ��־����
    /// @param args ����
    template <typename... _Args>
    void Println(Level level, _Args&&... args);

    /// \~chinese
    /// @brief ������־
    void Enable();

    /// \~chinese
    /// @brief ������־
    void Disable();

    /// \~chinese
    /// @brief ��ȡ�����
    OutputStream& GetOutputStream();

    /// \~chinese
    /// @brief ��ȡ���������
    OutputStream& GetErrorStream();

    /// \~chinese
    /// @brief �ض��������
    std::streambuf* RedirectOutputStream(std::streambuf* buf);

    /// \~chinese
    /// @brief �ض�����������
    std::streambuf* RedirectErrorStream(std::streambuf* buf);

    /// \~chinese
    /// @brief ��ʾ��رտ���̨
    /// @note �˲������ض������������׼�����
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
