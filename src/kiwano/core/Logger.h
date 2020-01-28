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
#include <iomanip>
#include <kiwano/core/Common.h>
#include <kiwano/macros.h>
#include <sstream>

#ifndef KGE_SYS_LOG
#ifdef KGE_DEBUG
#define KGE_SYS_LOG(FORMAT, ...) \
    ::kiwano::Logger::Instance().Printf(::kiwano::Logger::Level::System, FORMAT, __VA_ARGS__)
#else
#define KGE_SYS_LOG __noop
#endif
#endif

#ifndef KGE_WARN
#define KGE_WARN(FORMAT, ...) ::kiwano::Logger::Instance().Printf(::kiwano::Logger::Level::Warning, FORMAT, __VA_ARGS__)
#endif

#ifndef KGE_ERROR
#define KGE_ERROR(FORMAT, ...) ::kiwano::Logger::Instance().Printf(::kiwano::Logger::Level::Error, FORMAT, __VA_ARGS__)
#endif

#ifndef KGE_LOG
#define KGE_LOG(...) ::kiwano::Logger::Instance().Println(::kiwano::Logger::Level::Info, __VA_ARGS__)
#endif

#ifndef KGE_LOGF
#define KGE_LOGF(FORMAT, ...) ::kiwano::Logger::Instance().Printf(::kiwano::Logger::Level::Info, FORMAT, __VA_ARGS__)
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
    /// @brief �����
    using OutputStream = std::wostream;

    /// \~chinese
    /// @brief ����̨��ɫ
    using ConsoleColor = Function<OutputStream&(OutputStream&)>;

    /// \~chinese
    /// @brief ��ӡ��־
    /// @param level ��־����
    /// @param format ��ʽ�ַ���
    void Printf(Level level, const wchar_t* format, ...);

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
    /// @brief ��ʾ��رտ���̨
    /// @note �˲��������������
    void ShowConsole(bool show);

    /// \~chinese
    /// @brief ������־
    void Enable();

    /// \~chinese
    /// @brief ������־
    void Disable();

    /// \~chinese
    /// @brief ��ȡ�����
    std::wostream& GetOutputStream();

    /// \~chinese
    /// @brief ��ȡ������
    std::wostream& GetErrorStream();

    /// \~chinese
    /// @brief �ض��������
    std::wstreambuf* RedirectOutputStreamBuffer(std::wstreambuf* buf);

    /// \~chinese
    /// @brief �ض��������
    std::wstreambuf* RedirectErrorStreamBuffer(std::wstreambuf* buf);

    /// \~chinese
    /// @brief ���������
    void ResetOutputStream();

private:
    Logger();

    ~Logger();

    void Prepare(Level level, StringStream& sstream);

    void Output(Level level, StringStream& sstream);

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

    sstream << L"\r\n";

    Output(level, sstream);
}

inline void Logger::ResetConsoleColor() const
{
    ::SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), default_stdout_color_);
    ::SetConsoleTextAttribute(::GetStdHandle(STD_ERROR_HANDLE), default_stderr_color_);
}

inline Logger::OutputStream& Logger::DefaultOutputColor(OutputStream& out)
{
    ::SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), default_stdout_color_);
    return out;
}

inline std::wostream& Logger::GetOutputStream()
{
    return output_stream_;
}

inline std::wostream& Logger::GetErrorStream()
{
    return error_stream_;
}
}  // namespace kiwano
