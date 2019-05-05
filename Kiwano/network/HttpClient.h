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
#include <condition_variable>

namespace kiwano
{
	namespace network
	{
		class KGE_API HttpClient
			: public Singleton<HttpClient>
			, public Component
		{
			KGE_DECLARE_SINGLETON(HttpClient);

		public:
			void Send(
				HttpRequestPtr request
			);

			inline void SetTimeoutForConnect(Duration timeout)
			{
				timeout_for_connect_ = timeout;
			}

			inline Duration GetTimeoutForConnect() const
			{
				return timeout_for_connect_;
			}

			inline void SetTimeoutForRead(Duration timeout)
			{
				timeout_for_read_ = timeout;
			}

			inline Duration GetTimeoutForRead() const
			{
				return timeout_for_read_;
			}

			inline void SetSSLVerification(String const& root_certificate_path)
			{
				ssl_verification_ = root_certificate_path;
			}

			inline String const& GetSSLVerification() const
			{
				return ssl_verification_;
			}

		public:
			virtual void SetupComponent(Application* app) override;

			virtual void DestroyComponent() override;

		private:
			HttpClient();

			void NetworkThread();

			void Perform(
				HttpRequestPtr request,
				HttpResponsePtr response
			);

			void DispatchResponseCallback();

		private:
			Duration timeout_for_connect_;
			Duration timeout_for_read_;

			String ssl_verification_;

			std::mutex request_mutex_;
			Queue<HttpRequestPtr> request_queue_;

			std::mutex response_mutex_;
			Queue<HttpResponsePtr> response_queue_;

			std::condition_variable_any sleep_condition_;
		};
	}
}
