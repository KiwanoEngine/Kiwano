// Copyright (c) 2016-2018 Easy2D - Nomango
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

namespace easy2d
{
	namespace network
	{
		typedef std::function<void(HttpRequestPtr, HttpResponsePtr)> ResponseCallback;

		class HttpRequest
			: public Object
		{
		public:
			enum class Type
			{
				Unknown,
				Get,
				Post,
				Put,
				Delete
			};

			inline HttpRequest()
				: type_(Type::Unknown)
			{

			}

			inline HttpRequest(Type type)
				: type_(type)
			{

			}

			inline void SetUrl(String const& url)
			{
				url_ = url.to_string();
			}

			inline String const& GetUrl() const
			{
				return url_;
			}

			inline void SetType(Type type)
			{
				type_ = type;
			}

			inline Type GetType() const
			{
				return type_;
			}

			inline void SetData(String const& data)
			{
				data_ = data.to_string();
			}

			inline String const& GetData() const
			{
				return data_;
			}

			inline void SetHeaders(Array<String> const& headers)
			{
				headers_ = headers;
			}

			inline Array<String> const& GetHeaders() const
			{
				return headers_;
			}

			inline void SetResponseCallback(ResponseCallback const& callback)
			{
				response_cb_ = callback;
			}

			inline ResponseCallback const& GetResponseCallback() const
			{
				return response_cb_;
			}

		protected:
			Type type_;
			String url_;
			String data_;
			Array<String> headers_;
			ResponseCallback response_cb_;
		};
	}
}
