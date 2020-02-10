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
#include <kiwano/macros.h>
#include <stdexcept>

namespace kiwano
{

/**
 * \~chinese
 * @brief 异常
 */
class KGE_API Exception : public std::exception
{
public:
    Exception();

    /// \~chinese
    /// @brief 构造异常
    /// @param message 描述异常的信息
    explicit Exception(String const& message);

    virtual ~Exception();

    /// \~chinese
    /// @brief 转为解释性字符串
    const String& ToString() const;

    /// \~chinese
    /// @brief 转为解释性字符串
    virtual const char* what() const override;

protected:
    String message_;
};

/**
 * \~chinese
 * @brief 系统异常
 */
class SystemException : public Exception
{
public:
#if defined(KGE_WIN32)
    /// \~chinese
    /// @brief 错误代码类型
    typedef HRESULT ErrorCodeType;
#endif

    SystemException();

    /// \~chinese
    /// @brief 构造系统异常
    /// @param code 错误代码
    /// @param message 描述异常的信息
    SystemException(ErrorCodeType code, String const& message);

    /// \~chinese
    /// @brief 获取错误代码
    ErrorCodeType GetErrorCode() const;

private:
    ErrorCodeType code_;
};

inline SystemException::ErrorCodeType SystemException::GetErrorCode() const
{
    return code_;
}

}  // namespace kiwano
