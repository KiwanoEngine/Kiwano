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
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <kiwano/core/Common.h>
#include <kiwano/base/Module.h>

namespace kiwano
{
namespace network
{
/**
 * \~chinese
 * \defgroup Network 网络模块
 */

/**
 * \addtogroup Network
 * @{
 */

/**
 * \~chinese
 * @brief HTTP模块
 */
class KGE_API HttpModule
    : public Singleton<HttpModule>
    , public Module
{
    friend Singleton<HttpModule>;

public:
    /// \~chinese
    /// @brief 发送HTTP请求
    /// @param[in] request HTTP请求
    /// @details 发送请求后，无论结束或失败都将调用请求的响应回调函数
    void Send(HttpRequestPtr request);

    /// \~chinese
    /// @brief 设置连接超时时长
    void SetTimeoutForConnect(Duration timeout);

    /// \~chinese
    /// @brief 获取连接超时时长
    Duration GetTimeoutForConnect() const;

    /// \~chinese
    /// @brief 设置读取超时时长
    void SetTimeoutForRead(Duration timeout);

    /// \~chinese
    /// @brief 获取读取超时时长
    Duration GetTimeoutForRead() const;

    /// \~chinese
    /// @brief 设置SSL证书地址
    void SetSSLVerification(const String& root_certificate_path);

    /// \~chinese
    /// @brief 获取SSL证书地址
    const String& GetSSLVerification() const;

public:
    virtual void SetupModule() override;

    virtual void DestroyModule() override;

private:
    HttpModule();

    void NetworkThread();

    void Perform(HttpRequestPtr request, HttpResponsePtr response);

    void DispatchResponseCallback();

private:
    Duration timeout_for_connect_;
    Duration timeout_for_read_;

    String ssl_verification_;

    std::mutex            request_mutex_;
    Queue<HttpRequestPtr> request_queue_;

    std::mutex             response_mutex_;
    Queue<HttpResponsePtr> response_queue_;

    std::condition_variable sleep_cond_;

    std::atomic<bool>       quit_flag_;
    std::mutex              quit_mutex_;
    std::condition_variable quit_cond_;
};

/** @} */

inline void HttpModule::SetTimeoutForConnect(Duration timeout)
{
    timeout_for_connect_ = timeout;
}

inline Duration HttpModule::GetTimeoutForConnect() const
{
    return timeout_for_connect_;
}

inline void HttpModule::SetTimeoutForRead(Duration timeout)
{
    timeout_for_read_ = timeout;
}

inline Duration HttpModule::GetTimeoutForRead() const
{
    return timeout_for_read_;
}

inline void HttpModule::SetSSLVerification(const String& root_certificate_path)
{
    ssl_verification_ = root_certificate_path;
}

inline const String& HttpModule::GetSSLVerification() const
{
    return ssl_verification_;
}

}  // namespace network
}  // namespace kiwano
