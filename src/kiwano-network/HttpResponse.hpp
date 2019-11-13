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

		class KGE_API HttpResponse
			: public ObjectBase
		{
		public:
			HttpResponse(HttpRequestPtr request);

			// 获取请求
			HttpRequestPtr GetRequest() const;

			// 响应状态
			bool IsSucceed() const;
			void SetSucceed(bool succeed);

			// 响应状态码
			long GetResponseCode() const;
			void SetResponseCode(long response_code);

			// 响应头
			String GetHeader() const;
			void SetHeader(String const& response_header);

			// 响应数据
			String const& GetData() const;
			void SetData(String const& response_data);

			// 错误信息
			String const& GetError() const;
			void SetError(String const& error_buffer);

		protected:
			bool succeed_;
			long response_code_;
			HttpRequestPtr request_;

			String response_header_;
			String response_data_;
			String error_buffer_;
		};

		inline HttpResponse::HttpResponse(HttpRequestPtr request)						: request_(request), succeed_(false), response_code_(0) {}

		inline HttpRequestPtr	HttpResponse::GetRequest() const						{ return request_; }

		inline void				HttpResponse::SetSucceed(bool succeed)					{ succeed_ = succeed; }

		inline bool				HttpResponse::IsSucceed() const							{ return succeed_; }

		inline void				HttpResponse::SetResponseCode(long response_code)		{ response_code_ = response_code; }

		inline long				HttpResponse::GetResponseCode() const					{ return response_code_; }

		inline void				HttpResponse::SetHeader(String const& response_header)	{ response_header_ = response_header; }

		inline String			HttpResponse::GetHeader() const							{ return response_header_; }

		inline void				HttpResponse::SetData(String const& response_data)		{ response_data_ = response_data; }

		inline String const&	HttpResponse::GetData() const							{ return response_data_; }

		inline void				HttpResponse::SetError(String const& error_buffer)		{ error_buffer_ = error_buffer; }

		inline String const&	HttpResponse::GetError() const							{ return error_buffer_; }
	}
}
