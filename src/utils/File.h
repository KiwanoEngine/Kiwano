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
#include "../core/helper.hpp"
#include "../core/Resource.h"

namespace easy2d
{
    // 文件
	class E2D_API  File
	{
	public:
		File();

		File(
			String const& file_name
		);

		virtual ~File();

		// 打开文件
		bool Open(
			String const& file_name
		);

		// 文件是否存在
		bool Exists() const;

		// 删除文件
		bool Delete();

		// 获取文件路径
		String const& GetPath() const;

		// 获取文件扩展名
		String GetExtension() const;

		// 释放二进制资源到临时文件目录
		static File Extract(
			Resource const& res,			/* 资源 */
			String const& dest_file_name	/* 目标文件名 */
		);

	protected:
		String file_path_;
	};
}
