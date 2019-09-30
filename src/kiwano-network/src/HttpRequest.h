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
#include <kiwano/core/Function.hpp>
#include <kiwano/core/basic_json.hpp>
#include <kiwano/base/ObjectBase.h>
#include <kiwano/base/SmartPtr.hpp>

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

			inline HttpRequest()			: type_(Type::Unknown) {}

			inline HttpRequest(Type type)	: type_(type) {}

			inline void						SetUrl(String const& url)								{ url_ = url; }

			inline String const&			GetUrl() const											{ return url_; }

			inline void						SetType(Type type)										{ type_ = type; }

			inline Type						GetType() const											{ return type_; }

			inline void						SetData(String const& data)								{ data_ = data; }

			void							SetJsonData(Json const& json);

			inline String const&			GetData() const											{ return data_; }

			inline void						SetHeaders(Map<String, String> const& headers)			{ headers_ = headers; }

			void							SetHeader(String const& field, String const& content);

			inline Map<String, String>&		GetHeaders()											{ return headers_; }

			inline String const&			GetHeader(String const& header) const					{ return headers_.at(header); }

			inline void						SetResponseCallback(ResponseCallback const& callback)	{ response_cb_ = callback; }

			inline ResponseCallback const&	GetResponseCallback() const								{ return response_cb_; }

		protected:
			Type type_;
			String url_;
			String data_;
			Map<String, String> headers_;
			ResponseCallback response_cb_;
		};
	}
}
