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
#include <kiwano-network/HttpRequest.h>

namespace kiwano
{
namespace network
{
KGE_DECLARE_SMART_PTR(HttpResponse);

/**
 * \addtogroup Network
 * @{
 */

/**
 * \~chinese
 * @brief HTTP响应
 */
class KGE_API HttpResponse : public virtual ObjectBase
{
public:
    HttpResponse(HttpRequestPtr request);

    /// \~chinese
    /// @brief 获取对应的HTTP请求
    HttpRequestPtr GetRequest() const;

    /// \~chinese
    /// @brief 获取响应状态
    bool IsSucceed() const;

    /// \~chinese
    /// @brief 获取HTTP状态码
    long GetResponseCode() const;

    /// \~chinese
    /// @brief 获取响应头
    String GetHeader() const;

    /// \~chinese
    /// @brief 获取响应数据
    const String& GetData() const;

    /// \~chinese
    /// @brief 获取错误信息
    const String& GetError() const;

    /// \~chinese
    /// @brief 设置响应状态
    void SetSucceed(bool succeed);

    /// \~chinese
    /// @brief 设置HTTP状态码
    void SetResponseCode(long response_code);

    /// \~chinese
    /// @brief 设置响应头
    void SetHeader(const String& response_header);

    /// \~chinese
    /// @brief 设置响应数据
    void SetData(const String& response_data);

    /// \~chinese
    /// @brief 设置错误信息
    void SetError(const String& error_buffer);

private:
    bool           succeed_;
    long           response_code_;
    HttpRequestPtr request_;

    String response_header_;
    String response_data_;
    String error_buffer_;
};

/** @} */

inline HttpResponse::HttpResponse(HttpRequestPtr request)
    : request_(request)
    , succeed_(false)
    , response_code_(0)
{
}

inline HttpRequestPtr HttpResponse::GetRequest() const
{
    return request_;
}

inline void HttpResponse::SetSucceed(bool succeed)
{
    succeed_ = succeed;
}

inline bool HttpResponse::IsSucceed() const
{
    return succeed_;
}

inline void HttpResponse::SetResponseCode(long response_code)
{
    response_code_ = response_code;
}

inline long HttpResponse::GetResponseCode() const
{
    return response_code_;
}

inline void HttpResponse::SetHeader(const String& response_header)
{
    response_header_ = response_header;
}

inline String HttpResponse::GetHeader() const
{
    return response_header_;
}

inline void HttpResponse::SetData(const String& response_data)
{
    response_data_ = response_data;
}

inline const String& HttpResponse::GetData() const
{
    return response_data_;
}

inline void HttpResponse::SetError(const String& error_buffer)
{
    error_buffer_ = error_buffer;
}

inline const String& HttpResponse::GetError() const
{
    return error_buffer_;
}
}  // namespace network
}  // namespace kiwano
