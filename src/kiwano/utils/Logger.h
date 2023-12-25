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
#include <mutex>
#include <iomanip>
#include <streambuf>
#include <fstream>
#include <kiwano/core/Common.h>
#include <kiwano/core/Time.h>
#include <kiwano/base/ObjectBase.h>

#ifndef KGE_DEBUG_LOG
#ifdef KGE_DEBUG
#define KGE_DEBUG_LOG(...) ::kiwano::Logger::GetInstance().Log(::kiwano::LogLevel::Debug, __VA_ARGS__)
#else
#define KGE_DEBUG_LOG __noop
#endif
#endif

#ifndef KGE_LOG
#define KGE_LOG(...) ::kiwano::Logger::GetInstance().Log(::kiwano::LogLevel::Info, __VA_ARGS__)
#endif

#ifndef KGE_NOTICE
#define KGE_NOTICE(...) ::kiwano::Logger::GetInstance().Log(::kiwano::LogLevel::Notice, __VA_ARGS__)
#endif

#ifndef KGE_WARN
#define KGE_WARN(...) ::kiwano::Logger::GetInstance().Log(::kiwano::LogLevel::Warning, __VA_ARGS__)
#endif

#ifndef KGE_ERROR
#define KGE_ERROR(...) ::kiwano::Logger::GetInstance().Log(::kiwano::LogLevel::Error, __VA_ARGS__)
#endif

#ifndef KGE_DEBUG_LOGF
#ifdef KGE_DEBUG
#define KGE_DEBUG_LOGF(FORMAT, ...) ::kiwano::Logger::GetInstance().Logf(::kiwano::LogLevel::Debug, FORMAT, __VA_ARGS__)
#else
#define KGE_DEBUG_LOGF __noop
#endif
#endif

#ifndef KGE_LOGF
#define KGE_LOGF(FORMAT, ...) ::kiwano::Logger::GetInstance().Logf(::kiwano::LogLevel::Info, FORMAT, __VA_ARGS__)
#endif

#ifndef KGE_NOTICEF
#define KGE_NOTICEF(FORMAT, ...) ::kiwano::Logger::GetInstance().Logf(::kiwano::LogLevel::Notice, FORMAT, __VA_ARGS__)
#endif

#ifndef KGE_WARNF
#define KGE_WARNF(FORMAT, ...) ::kiwano::Logger::GetInstance().Logf(::kiwano::LogLevel::Warning, FORMAT, __VA_ARGS__)
#endif

#ifndef KGE_ERRORF
#define KGE_ERRORF(FORMAT, ...) ::kiwano::Logger::GetInstance().Logf(::kiwano::LogLevel::Error, FORMAT, __VA_ARGS__)
#endif

#ifndef KGE_THROW
#define KGE_THROW(MESSAGE)                                \
    do                                                    \
    {                                                     \
        KGE_ERRORF("An exception occurred: %s", MESSAGE); \
        kiwano::StackTracer().Print();                    \
        throw kiwano::RuntimeError(MESSAGE);              \
    } while (0)
#endif

#ifndef KGE_THROW_IF
#define KGE_THROW_IF(EXPRESSION, MESSAGE) \
    if (EXPRESSION)                       \
    {                                     \
        KGE_THROW(MESSAGE);               \
    }
#endif

#ifndef KGE_THROW_SYSTEM_ERROR
#define KGE_THROW_SYSTEM_ERROR(ERRCODE, MESSAGE)                                          \
    do                                                                                    \
    {                                                                                     \
        KGE_ERRORF("An exception occurred (%#x): %s", ERRCODE, MESSAGE);                  \
        kiwano::StackTracer().Print();                                                    \
        throw kiwano::SystemError(std::error_code(kiwano::error_enum(ERRCODE)), MESSAGE); \
    } while (0)
#endif

#ifdef KGE_PLATFORM_WINDOWS
#ifndef KGE_THROW_IF_FAILED
#define KGE_THROW_IF_FAILED(HR, MESSAGE)     \
    if (FAILED(HR))                          \
    {                                        \
        KGE_THROW_SYSTEM_ERROR(HR, MESSAGE); \
    }
#endif
#endif  // KGE_PLATFORM_WINDOWS


namespace kiwano
{

KGE_DECLARE_SMART_PTR(LogFormater);
KGE_DECLARE_SMART_PTR(LogProvider);

/**
 * \~chinese
 * @brief 日志等级
 */
enum class LogLevel
{
    Debug,    ///< 调试
    Info,     ///< 信息
    Notice,   ///< 注意
    Warning,  ///< 警告
    Error,    ///< 错误
};

/**
 * \~chinese
 * @brief 日志格式化
 */
class KGE_API LogFormater : public ObjectBase
{
public:
    virtual void FormatHeader(std::ostream& out, LogLevel level, ClockTime time) = 0;

    virtual void FormatFooter(std::ostream& out, LogLevel level) = 0;

    String GetLevelLabel(LogLevel level) const;
};

/**
 * \~chinese
 * @brief 日志流缓冲
 */
class KGE_API LogBuffer : public std::streambuf
{
public:
    LogBuffer(size_t buffer_size);

    void Resize(size_t size);

    void Reset();

    const char* GetRaw() const;

    LogBuffer(const LogBuffer&) = delete;

    LogBuffer& operator=(const LogBuffer&) = delete;

protected:
    int_type overflow(int_type ch) override;

    int_type underflow() override;

    pos_type seekpos(pos_type sp, std::ios_base::openmode which) override;

    pos_type seekoff(off_type off, std::ios_base::seekdir dir,
                     std::ios_base::openmode which = std::ios_base::in) override;

private:
    Vector<char_type> buf_;
    char_type*        seek_high_;
};

/**
 * \~chinese
 * @brief 日志生产者
 */
class KGE_API LogProvider : public ObjectBase
{
public:
    virtual ~LogProvider();

    virtual void Init();

    virtual void Flush();

    void Write(LogLevel level, const char* msg);

    void SetLevel(LogLevel level);

protected:
    LogProvider();

    virtual void WriteMessage(LogLevel level, const char* msg) = 0;

protected:
    LogLevel level_;
};

/**
 * \~chinese
 * @brief 控制台日志生产者
 */
class KGE_API ConsoleLogProvider : public LogProvider
{
public:
    ConsoleLogProvider();

    virtual ~ConsoleLogProvider();

    void Init() override;

    void Flush() override;

protected:
    void WriteMessage(LogLevel level, const char* msg) override;

private:
    typedef std::ostream&(*ConsoleColor)(std::ostream&);

    ConsoleColor GetColor(LogLevel level);
};

/**
 * \~chinese
 * @brief 文件日志生产者
 */
class KGE_API FileLogProvider : public LogProvider
{
public:
    FileLogProvider(StringView filepath, std::ios_base::openmode mode = std::ios_base::out);

   virtual ~FileLogProvider();

    void Init() override;

    void Flush() override;

protected:
    void WriteMessage(LogLevel level, const char* msg) override;

private:
    std::ofstream ofs_;
};

/**
 * \~chinese
 * @brief 日志记录器
 */
class KGE_API Logger final : public Singleton<Logger>
{
    friend Singleton<Logger>;

public:
    /// \~chinese
    /// @brief 打印日志
    /// @param level 日志级别
    /// @param format 格式字符串
    void Logf(LogLevel level, const char* format, ...);

    /// \~chinese
    /// @brief 打印日志
    /// @param level 日志级别
    /// @param args 参数
    template <typename... _Args>
    void Log(LogLevel level, _Args&&... args);

    /// \~chinese
    /// @brief 刷新日志缓冲
    void Flush();

    /// \~chinese
    /// @brief 启用日志
    void Enable();

    /// \~chinese
    /// @brief 禁用日志
    void Disable();

    /// \~chinese
    /// @brief 设置日志等级
    void SetLevel(LogLevel level);

    /// \~chinese
    /// @brief 添加日志生产者
    /// @param provider 日志生产者
    void AddProvider(LogProviderPtr provider);

    /// \~chinese
    /// @brief 设置日志格式
    /// @param formater 日志格式化
    void SetFormater(LogFormaterPtr formater);

    /// \~chinese
    /// @brief 获取日志格式
    /// @return 日志格式
    LogFormaterPtr GetFormater();

    /// \~chinese
    /// @brief 重设缓冲区大小
    /// @param buffer_size 缓冲区大小
    void ResizeBuffer(size_t buffer_size);

    /// \~chinese
    /// @brief 显示或关闭控制台
    void ShowConsole(bool show);

    virtual ~Logger();

private:
    Logger();

    std::iostream& GetFormatedStream(LogLevel level, LogBuffer* buffer);

    void WriteToProviders(LogLevel level, LogBuffer* buffer);

private:
    bool                   enabled_;
    LogLevel               level_;
    LogFormaterPtr         formater_;
    LogBuffer              buffer_;
    std::iostream          stream_;
    Vector<LogProviderPtr> providers_;
    std::mutex             mutex_;
};

inline void Logger::Enable()
{
    enabled_ = true;
}

inline void Logger::Disable()
{
    enabled_ = false;
}

inline void Logger::SetFormater(LogFormaterPtr formater)
{
    formater_ = formater;
}

template <typename... _Args>
inline void Logger::Log(LogLevel level, _Args&&... args)
{
    if (!enabled_)
        return;

    if (level < level_)
        return;

    std::lock_guard<std::mutex> lock(mutex_);

    // build message
    auto& stream = this->GetFormatedStream(level, &this->buffer_);
    (void)std::initializer_list<int>{ ((stream << ' ' << args), 0)... };

    // write message
    this->WriteToProviders(level, &this->buffer_);
}

}  // namespace kiwano
