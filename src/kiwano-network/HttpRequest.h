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
#include <kiwano/common/common.h>
#include <kiwano/core/ObjectBase.h>
#include <kiwano/core/SmartPtr.hpp>

namespace kiwano
{
	namespace network
	{
		class HttpResponse;

		KGE_DECLARE_SMART_PTR(HttpRequest);

		class KGE_API HttpRequest
			: public ObjectBase
		{
		public:
			using ResponseCallback = Function<void(HttpRequest*, HttpResponse*)>;

			enum class Type
			{
				Unknown,
				Get,
				Post,
				Put,
				Delete
			};

			HttpRequest();

			HttpRequest(Type type);

			// 请求地址
			void SetUrl(String const& url);
			String const& GetUrl() const;

			// 请求类型
			void SetType(Type type);
			Type GetType() const;

			// 请求数据
			void SetData(String const& data);
			void SetJsonData(Json const& json);
			String const& GetData() const;

			// 请求头
			void SetHeaders(Map<String, String> const& headers);
			void SetHeader(String const& field, String const& content);
			Map<String, String>& GetHeaders();
			String const& GetHeader(String const& header) const;

			// 响应回调
			void SetResponseCallback(ResponseCallback const& callback);
			ResponseCallback const& GetResponseCallback() const;

		protected:
			Type type_;
			String url_;
			String data_;
			Map<String, String> headers_;
			ResponseCallback response_cb_;
		};

		inline HttpRequest::HttpRequest()														: type_(Type::Unknown) {}

		inline HttpRequest::HttpRequest(Type type)												: type_(type) {}

		inline void HttpRequest::SetUrl(String const& url)										{ url_ = url; }

		inline String const& HttpRequest::GetUrl() const										{ return url_; }

		inline void HttpRequest::SetType(Type type)												{ type_ = type; }

		inline HttpRequest::Type HttpRequest::GetType() const									{ return type_; }

		inline void HttpRequest::SetData(String const& data)									{ data_ = data; }

		inline String const& HttpRequest::GetData() const										{ return data_; }

		inline void HttpRequest::SetHeaders(Map<String, String> const& headers)					{ headers_ = headers; }

		inline void HttpRequest::SetHeader(String const& field, String const& content)			{ headers_[field] = content; }

		inline Map<String, String>& HttpRequest::GetHeaders()									{ return headers_; }

		inline String const& HttpRequest::GetHeader(String const& header) const					{ return headers_.at(header); }

		inline void HttpRequest::SetResponseCallback(ResponseCallback const& callback)			{ response_cb_ = callback; }

		inline HttpRequest::ResponseCallback const&	HttpRequest::GetResponseCallback() const	{ return response_cb_; }
	}
}
