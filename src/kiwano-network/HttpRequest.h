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
#include <kiwano/base/ObjectBase.h>
#include <kiwano/utils/Json.h>

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
class KGE_API HttpRequest : public ObjectBase
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
    static HttpRequestPtr Create(const String& url, HttpType type, const ResponseCallback& callback);

    /// \~chinese
    /// @brief ����HTTP����
    /// @param url �����ַ
    /// @param type ��������
    /// @param data ��������
    /// @param callback ��Ӧ�ص�����
    static HttpRequestPtr Create(const String& url, HttpType type, const String& data, const ResponseCallback& callback);

    /// \~chinese
    /// @brief ����HTTP����
    /// @param url �����ַ
    /// @param type ��������
    /// @param json �����JSON����
    /// @param callback ��Ӧ�ص�����
    static HttpRequestPtr Create(const String& url, HttpType type, const Json& json, const ResponseCallback& callback);

    HttpRequest();

    HttpRequest(HttpType type);

    /// \~chinese
    /// @brief ���������ַ
    void SetUrl(const String& url);

    /// \~chinese
    /// @brief ������������
    void SetType(HttpType type);

    /// \~chinese
    /// @brief ������������
    void SetData(const String& data);

    /// \~chinese
    /// @brief ���������JSON����
    void SetJsonData(const Json& json);

    /// \~chinese
    /// @brief ����HTTPͷ
    void SetHeaders(const Map<String, String>& headers);

    /// \~chinese
    /// @brief ����HTTPͷ
    void SetHeader(const String& field, const String& content);

    /// \~chinese
    /// @brief ������Ӧ�ص�����
    void SetResponseCallback(const ResponseCallback& callback);

    /// \~chinese
    /// @brief ��ȡ�����ַ
    const String& GetUrl() const;

    /// \~chinese
    /// @brief ��ȡ��������
    HttpType GetType() const;

    /// \~chinese
    /// @brief ��ȡ��������
    const String& GetData() const;

    /// \~chinese
    /// @brief ��ȡHTTPͷ
    Map<String, String>& GetHeaders();

    /// \~chinese
    /// @brief ��ȡHTTPͷ
    const String& GetHeader(const String& header) const;

    /// \~chinese
    /// @brief ��ȡ��Ӧ�ص�����
    const ResponseCallback& GetResponseCallback() const;

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

inline void HttpRequest::SetUrl(const String& url)
{
    url_ = url;
}

inline const String& HttpRequest::GetUrl() const
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

inline void HttpRequest::SetData(const String& data)
{
    data_ = data;
}

inline const String& HttpRequest::GetData() const
{
    return data_;
}

inline void HttpRequest::SetHeaders(const Map<String, String>& headers)
{
    headers_ = headers;
}

inline void HttpRequest::SetHeader(const String& field, const String& content)
{
    headers_[field] = content;
}

inline Map<String, String>& HttpRequest::GetHeaders()
{
    return headers_;
}

inline const String& HttpRequest::GetHeader(const String& header) const
{
    return headers_.at(header);
}

inline void HttpRequest::SetResponseCallback(const ResponseCallback& callback)
{
    response_cb_ = callback;
}

inline const HttpRequest::ResponseCallback& HttpRequest::GetResponseCallback() const
{
    return response_cb_;
}
}  // namespace network
}  // namespace kiwano
