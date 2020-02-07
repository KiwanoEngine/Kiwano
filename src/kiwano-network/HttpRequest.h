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
#include <kiwano/core/ObjectBase.h>
#include <kiwano/core/SmartPtr.hpp>

namespace kiwano
{
namespace network
{
class HttpResponse;

KGE_DECLARE_SMART_PTR(HttpRequest);

/**
 * \addtogroup Network
 * @{
 */

/// \~chinese
/// @brief HTTP��������
enum class HttpType
{
    Unknown,  ///< δ֪
    Get,      ///< HTTP GET����
    Post,     ///< HTTP POST����
    Put,      ///< HTTP PUT����
    Delete    ///< HTTP DELETE����
};

/**
 * \~chinese
 * @brief HTTP����
 */
class KGE_API HttpRequest : public virtual ObjectBase
{
public:
    /// \~chinese
    /// @brief ��Ӧ�ص�����
    using ResponseCallback = Function<void(HttpRequest* /* request */, HttpResponse* /* response */)>;

    /// \~chinese
    /// @brief ����HTTP����
    /// @param url �����ַ
    /// @param type ��������
    /// @param callback ��Ӧ�ص�����
    static HttpRequestPtr Create(String const& url, HttpType type, ResponseCallback const& callback);

    /// \~chinese
    /// @brief ����HTTP����
    /// @param url �����ַ
    /// @param type ��������
    /// @param data ��������
    /// @param callback ��Ӧ�ص�����
    static HttpRequestPtr Create(String const& url, HttpType type, String const& data, ResponseCallback const& callback);

    /// \~chinese
    /// @brief ����HTTP����
    /// @param url �����ַ
    /// @param type ��������
    /// @param json �����JSON����
    /// @param callback ��Ӧ�ص�����
    static HttpRequestPtr Create(String const& url, HttpType type, Json const& json, ResponseCallback const& callback);

    HttpRequest();

    HttpRequest(HttpType type);

    /// \~chinese
    /// @brief ���������ַ
    void SetUrl(String const& url);

    /// \~chinese
    /// @brief ������������
    void SetType(HttpType type);

    /// \~chinese
    /// @brief ������������
    void SetData(String const& data);

    /// \~chinese
    /// @brief ���������JSON����
    void SetJsonData(Json const& json);

    /// \~chinese
    /// @brief ����HTTPͷ
    void SetHeaders(Map<String, String> const& headers);

    /// \~chinese
    /// @brief ����HTTPͷ
    void SetHeader(String const& field, String const& content);

    /// \~chinese
    /// @brief ������Ӧ�ص�����
    void SetResponseCallback(ResponseCallback const& callback);

    /// \~chinese
    /// @brief ��ȡ�����ַ
    String const& GetUrl() const;

    /// \~chinese
    /// @brief ��ȡ��������
    HttpType GetType() const;

    /// \~chinese
    /// @brief ��ȡ��������
    String const& GetData() const;

    /// \~chinese
    /// @brief ��ȡHTTPͷ
    Map<String, String>& GetHeaders();

    /// \~chinese
    /// @brief ��ȡHTTPͷ
    String const& GetHeader(String const& header) const;

    /// \~chinese
    /// @brief ��ȡ��Ӧ�ص�����
    ResponseCallback const& GetResponseCallback() const;

private:
    HttpType                type_;
    String              url_;
    String              data_;
    Map<String, String> headers_;
    ResponseCallback    response_cb_;
};

/** @} */

inline HttpRequest::HttpRequest()
    : type_(HttpType::Unknown)
{
}

inline HttpRequest::HttpRequest(HttpType type)
    : type_(type)
{
}

inline void HttpRequest::SetUrl(String const& url)
{
    url_ = url;
}

inline String const& HttpRequest::GetUrl() const
{
    return url_;
}

inline void HttpRequest::SetType(HttpType type)
{
    type_ = type;
}

inline HttpType HttpRequest::GetType() const
{
    return type_;
}

inline void HttpRequest::SetData(String const& data)
{
    data_ = data;
}

inline String const& HttpRequest::GetData() const
{
    return data_;
}

inline void HttpRequest::SetHeaders(Map<String, String> const& headers)
{
    headers_ = headers;
}

inline void HttpRequest::SetHeader(String const& field, String const& content)
{
    headers_[field] = content;
}

inline Map<String, String>& HttpRequest::GetHeaders()
{
    return headers_;
}

inline String const& HttpRequest::GetHeader(String const& header) const
{
    return headers_.at(header);
}

inline void HttpRequest::SetResponseCallback(ResponseCallback const& callback)
{
    response_cb_ = callback;
}

inline HttpRequest::ResponseCallback const& HttpRequest::GetResponseCallback() const
{
    return response_cb_;
}
}  // namespace network
}  // namespace kiwano
