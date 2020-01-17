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
#include <kiwano/core/Component.h>
#include <mutex>
#include <condition_variable>

namespace kiwano
{
	namespace network
	{
		/**
		* \~chinese
		* \defgroup Network ����ͨ��
		*/

		/**
		* \addtogroup Network
		* @{
		*/

		/**
		* \~chinese
		* @brief HTTP�ͻ���
		*/
		class KGE_API HttpClient
			: public Singleton<HttpClient>
			, public ComponentBase
		{
			friend Singleton<HttpClient>;

		public:
			/// \~chinese
			/// @brief ����HTTP����
			/// @param[in] request HTTP����
			/// @details ������������۽�����ʧ�ܶ��������������Ӧ�ص�����
			void Send(HttpRequestPtr request);

			/// \~chinese
			/// @brief �������ӳ�ʱʱ��
			void SetTimeoutForConnect(Duration timeout);

			/// \~chinese
			/// @brief ��ȡ���ӳ�ʱʱ��
			Duration GetTimeoutForConnect() const;

			/// \~chinese
			/// @brief ���ö�ȡ��ʱʱ��
			void SetTimeoutForRead(Duration timeout);

			/// \~chinese
			/// @brief ��ȡ��ȡ��ʱʱ��
			Duration GetTimeoutForRead() const;

			/// \~chinese
			/// @brief ����SSL֤���ַ
			void SetSSLVerification(String const& root_certificate_path);

			/// \~chinese
			/// @brief ��ȡSSL֤���ַ
			String const& GetSSLVerification() const;

		public:
			virtual void SetupComponent() override;

			virtual void DestroyComponent() override;

		private:
			HttpClient();

			void NetworkThread();

			void Perform(HttpRequestPtr request, HttpResponsePtr response);

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

		/** @} */


		inline void HttpClient::SetTimeoutForConnect(Duration timeout)
		{
			timeout_for_connect_ = timeout;
		}

		inline Duration HttpClient::GetTimeoutForConnect() const
		{
			return timeout_for_connect_;
		}

		inline void HttpClient::SetTimeoutForRead(Duration timeout)
		{
			timeout_for_read_ = timeout;
		}

		inline Duration HttpClient::GetTimeoutForRead() const
		{
			return timeout_for_read_;
		}

		inline void HttpClient::SetSSLVerification(String const& root_certificate_path)
		{
			ssl_verification_ = root_certificate_path;
		}

		inline String const& HttpClient::GetSSLVerification() const
		{
			return ssl_verification_;
		}

	}
}
