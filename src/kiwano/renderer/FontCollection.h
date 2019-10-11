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
#include <kiwano/base/Resource.h>
#include <kiwano/base/win32/ComPtr.hpp>
#include <dwrite.h>

namespace kiwano
{
	// 字体集
	class FontCollection
	{
	public:
		FontCollection();

		FontCollection(String const& file);

		FontCollection(Vector<String> const& files);

		FontCollection(Resource const& res);

		FontCollection(Vector<Resource> const& res_arr);

		// 从本地文件加载字体
		bool Load(String const& file);

		// 从多个本地文件加载字体
		bool Load(Vector<String> const& files);

		// 从资源加载字体
		bool Load(Resource const& res);

		// 从多个资源加载字体
		bool Load(Vector<Resource> const& res_arr);

		bool IsValid() const;

	public:
		inline ComPtr<IDWriteFontCollection> GetFontCollection() const			{ return collection_; }

		inline void SetFontCollection(ComPtr<IDWriteFontCollection> collection)	{ collection_ = collection; }

	protected:
		ComPtr<IDWriteFontCollection> collection_;
	};
}
