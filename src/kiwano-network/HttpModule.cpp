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

#include <chrono>
#include <thread>
#include <kiwano/utils/Logger.h>
#include <kiwano/platform/Application.h>
#include <kiwano-network/HttpRequest.h>
#include <kiwano-network/HttpResponse.hpp>
#include <kiwano-network/HttpModule.h>
#include <curl/curl.h>  // CURL

namespace
{
using namespace kiwano;
using namespace kiwano::network;

uint32_t write_data(void* buffer, uint32_t size, uint32_t nmemb, void* userp)
{
    String*  recv_buffer = (String*)userp;
    uint32_t total       = size * nmemb;

    // add data to the end of recv_buffer
    // write data maybe called more than once in a single request
    recv_buffer->append((char*)buffer, total);

    return total;
}

class Curl
{
public:
    Curl()
        : curl_(curl_easy_init())
        , curl_headers_(nullptr)
    {
    }

    ~Curl()
    {
        if (curl_)
        {
            curl_easy_cleanup(curl_);
            curl_ = nullptr;
        }

        if (curl_headers_)
        {
            curl_slist_free_all(curl_headers_);
            curl_headers_ = nullptr;
        }
    }

    bool Init(HttpModule* client, const Vector<String>& headers, const String& url, String* response_data,
              String* response_header, char* error_buffer)
    {
        if (!SetOption(CURLOPT_ERRORBUFFER, error_buffer))
            return false;
        if (!SetOption(CURLOPT_TIMEOUT, client->GetTimeoutForRead()))
            return false;
        if (!SetOption(CURLOPT_CONNECTTIMEOUT, client->GetTimeoutForConnect()))
            return false;

        const String& ssl_ca_file = client->GetSSLVerification();
        if (ssl_ca_file.empty())
        {
            if (!SetOption(CURLOPT_SSL_VERIFYPEER, 0L))
                return false;
            if (!SetOption(CURLOPT_SSL_VERIFYHOST, 0L))
                return false;
        }
        else
        {
            if (!SetOption(CURLOPT_SSL_VERIFYPEER, 1L))
                return false;
            if (!SetOption(CURLOPT_SSL_VERIFYHOST, 2L))
                return false;
            if (!SetOption(CURLOPT_CAINFO, ssl_ca_file.c_str()))
                return false;
        }

        if (!SetOption(CURLOPT_NOSIGNAL, 1L))
            return false;
        if (!SetOption(CURLOPT_ACCEPT_ENCODING, ""))
            return false;

        // set request headers
        if (!headers.empty())
        {
            for (const auto& header : headers)
            {
                curl_headers_ = curl_slist_append(curl_headers_, header.c_str());
            }
            if (!SetOption(CURLOPT_HTTPHEADER, curl_headers_))
                return false;
        }

        return SetOption(CURLOPT_URL, url.c_str()) && SetOption(CURLOPT_WRITEFUNCTION, write_data)
               && SetOption(CURLOPT_WRITEDATA, response_data) && SetOption(CURLOPT_HEADERFUNCTION, write_data)
               && SetOption(CURLOPT_HEADERDATA, response_header);
    }

    bool Perform(long* response_code)
    {
        if (CURLE_OK != curl_easy_perform(curl_))
            return false;

        CURLcode code = curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, response_code);
        return code == CURLE_OK && (*response_code >= 200 && *response_code < 300);
    }

    template <typename... _Args>
    bool SetOption(CURLoption option, _Args&&... args)
    {
        return CURLE_OK == curl_easy_setopt(curl_, option, std::forward<_Args>(args)...);
    }

public:
    static inline bool GetRequest(HttpModule* client, const Vector<String>& headers, const String& url,
                                  long* response_code, String* response_data, String* response_header,
                                  char* error_buffer)
    {
        Curl curl;
        return curl.Init(client, headers, url, response_data, response_header, error_buffer)
               && curl.SetOption(CURLOPT_FOLLOWLOCATION, true) && curl.Perform(response_code);
    }

    static inline bool PostRequest(HttpModule* client, const Vector<String>& headers, const String& url,
                                   const String& request_data, long* response_code, String* response_data,
                                   String* response_header, char* error_buffer)
    {
        Curl curl;
        return curl.Init(client, headers, url, response_data, response_header, error_buffer)
               && curl.SetOption(CURLOPT_POST, 1) && curl.SetOption(CURLOPT_POSTFIELDS, request_data.c_str())
               && curl.SetOption(CURLOPT_POSTFIELDSIZE, request_data.size()) && curl.Perform(response_code);
    }

    static inline bool PutRequest(HttpModule* client, const Vector<String>& headers, const String& url,
                                  const String& request_data, long* response_code, String* response_data,
                                  String* response_header, char* error_buffer)
    {
        Curl curl;
        return curl.Init(client, headers, url, response_data, response_header, error_buffer)
               && curl.SetOption(CURLOPT_CUSTOMREQUEST, "PUT")
               && curl.SetOption(CURLOPT_POSTFIELDS, request_data.c_str())
               && curl.SetOption(CURLOPT_POSTFIELDSIZE, request_data.size()) && curl.Perform(response_code);
    }

    static inline bool DeleteRequest(HttpModule* client, const Vector<String>& headers, const String& url,
                                     long* response_code, String* response_data, String* response_header,
                                     char* error_buffer)
    {
        Curl curl;
        return curl.Init(client, headers, url, response_data, response_header, error_buffer)
               && curl.SetOption(CURLOPT_CUSTOMREQUEST, "DELETE") && curl.SetOption(CURLOPT_FOLLOWLOCATION, true)
               && curl.Perform(response_code);
    }

private:
    CURL*       curl_;
    curl_slist* curl_headers_;
};
}  // namespace

namespace kiwano
{
namespace network
{
HttpModule::HttpModule()
    : timeout_for_connect_(30000 /* 30 seconds */)
    , timeout_for_read_(60000 /* 60 seconds */)
    , quit_flag_(false)
{
}

void HttpModule::SetupModule()
{
    ::curl_global_init(CURL_GLOBAL_ALL);

    std::thread thread(Closure(this, &HttpModule::NetworkThread));
    thread.detach();
}

void HttpModule::DestroyModule()
{
    // Set quit flag
    quit_flag_ = true;

    // Send a fake request
    {
        std::unique_lock<std::mutex> lock(request_mutex_);
        request_queue_.push(nullptr);
    }

    // Notify work thread
    sleep_cond_.notify_one();

    // Wait for work thread destroyed
    {
        std::unique_lock<std::mutex> lock(quit_mutex_);
        quit_cond_.wait_for(lock, std::chrono::seconds(1));
    }

    // Clear curl resources
    ::curl_global_cleanup();
}

void HttpModule::Send(HttpRequestPtr request)
{
    if (!request)
        return;

    {
        std::unique_lock<std::mutex> lock(request_mutex_);
        request_queue_.push(request);
    }
    sleep_cond_.notify_one();
}

void HttpModule::NetworkThread()
{
    while (true)
    {
        HttpRequestPtr request;
        {
            std::unique_lock<std::mutex> lock(request_mutex_);
            sleep_cond_.wait(lock, [&]() { return !request_queue_.empty(); });

            request = request_queue_.front();
            request_queue_.pop();
        }

        if (quit_flag_)
        {
            quit_cond_.notify_one();
            return;
        }

        HttpResponsePtr response = new (autogc) HttpResponse(request);
        Perform(request, response);

        response_mutex_.lock();
        response_queue_.push(response);
        response_mutex_.unlock();

        Application::GetInstance().PreformInMainThread(Closure(this, &HttpModule::DispatchResponseCallback));
    }
}

void HttpModule::Perform(HttpRequestPtr request, HttpResponsePtr response)
{
    bool   ok                 = false;
    long   response_code      = 0;
    char   error_message[256] = { 0 };
    String response_header;
    String response_data;
    String url  = request->GetUrl();
    String data = request->GetData();

    Vector<String> headers;
    headers.reserve(request->GetHeaders().size());
    for (const auto& pair : request->GetHeaders())
    {
        headers.push_back(pair.first + ":" + pair.second);
    }

    switch (request->GetType())
    {
    case HttpType::Get:
        ok = Curl::GetRequest(this, headers, url, &response_code, &response_data, &response_header, error_message);
        break;
    case HttpType::Post:
        ok = Curl::PostRequest(this, headers, url, data, &response_code, &response_data, &response_header,
                               error_message);
        break;
    case HttpType::Put:
        ok =
            Curl::PutRequest(this, headers, url, data, &response_code, &response_data, &response_header, error_message);
        break;
    case HttpType::Delete:
        ok = Curl::DeleteRequest(this, headers, url, &response_code, &response_data, &response_header, error_message);
        break;
    default:
        KGE_ERRORF("HttpModule: unknown request type, only GET, POST, PUT or DELETE is supported");
        return;
    }

    response->SetResponseCode(response_code);
    response->SetHeader(response_header);
    response->SetData(response_data);
    if (!ok)
    {
        response->SetSucceed(false);
        response->SetError(error_message);
    }
    else
    {
        response->SetSucceed(true);
    }
}

void HttpModule::DispatchResponseCallback()
{
    HttpResponsePtr response;

    response_mutex_.lock();
    if (!response_queue_.empty())
    {
        response = response_queue_.front();
        response_queue_.pop();
    }
    response_mutex_.unlock();

    if (response)
    {
        HttpRequestPtr request  = response->GetRequest();
        const auto&    callback = request->GetResponseCallback();

        if (callback)
        {
            callback(request.Get(), response.Get());
        }
    }
}

}  // namespace network
}  // namespace kiwano
