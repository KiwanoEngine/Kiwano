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
#include <kiwano/core/Logger.h>
#include <stdexcept>
#include <system_error>

#define KGE_THROW(MESSAGE)                               \
    do                                                   \
    {                                                    \
        KGE_ERROR("An exception occurred: %s", MESSAGE); \
        kiwano::StackTracer().Print();                   \
        throw kiwano::RuntimeError(MESSAGE);             \
    } while (0)

#define KGE_THROW_SYSTEM_ERROR(ERRCODE, MESSAGE)                                          \
    do                                                                                    \
    {                                                                                     \
        KGE_ERROR("An exception occurred (%#x): %s", ERRCODE, MESSAGE);                   \
        kiwano::StackTracer().Print();                                                    \
        throw kiwano::SystemError(std::error_code(kiwano::error_enum(ERRCODE)), MESSAGE); \
    } while (0)

namespace kiwano
{

/// \~chinese
/// @brief 异常
typedef std::exception Exception;

/// \~chinese
/// @brief 运行时异常
typedef std::runtime_error RuntimeError;

/// \~chinese
/// @brief 系统异常
typedef std::system_error SystemError;

class StackTracer
{
public:
    StackTracer();

    void Print() const;
};

#ifdef KGE_PLATFORM_WINDOWS

// Enables classifying error codes
// @note We don't bother listing all possible values
enum class error_enum
{
};

#else

typedef std::errc error_enum;

#endif

}  // namespace kiwano

#ifdef KGE_PLATFORM_WINDOWS

namespace std
{
template <>
struct is_error_code_enum<kiwano::error_enum> : true_type
{
};
}  // namespace std

namespace kiwano
{

const std::error_category& com_category() noexcept;

inline std::error_code make_error_code(kiwano::error_enum errc) noexcept
{
    return std::error_code(static_cast<int>(errc), kiwano::com_category());
}

inline std::error_condition make_error_condition(kiwano::error_enum errc) noexcept
{
    return std::error_condition(static_cast<int>(errc), kiwano::com_category());
}

#define KGE_THROW_IF_FAILED(HR, MESSAGE)     \
    if (FAILED(HR))                          \
    {                                        \
        KGE_THROW_SYSTEM_ERROR(HR, MESSAGE); \
    }

}  // namespace kiwano

#endif  // KGE_PLATFORM_WINDOWS
