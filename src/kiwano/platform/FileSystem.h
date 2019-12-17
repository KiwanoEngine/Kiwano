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
#include <kiwano/core/Resource.h>

namespace kiwano
{
    // 文件
	class KGE_API FileSystem
		: public Singleton<FileSystem>
	{
		OC_DECLARE_SINGLETON(FileSystem);

	public:
		// 添加文件搜索路径
		void AddSearchPath(String const& path);

		// 设置文件搜索路径
		void SetSearchPaths(Vector<String> const& paths);

		// 获取文件的完整路径
		String GetFullPathForFile(String const& file) const;

		// 添加文件路径查找字典规则
		void AddFileLookupRule(String const& key, String const& file_path);

		// 设置文件路径查找字典
		void SetFileLookupDictionary(UnorderedMap<String, String> const& dict);

		// 文件是否存在
		bool IsFileExists(String const& file_path) const;

		// 判断路径是否是绝对路径
		bool IsAbsolutePath(String const& path) const;

		// 删除文件
		bool RemoveFile(String const& file_path) const;

		// 释放二进制资源到临时文件目录
		bool ExtractResourceToFile(
			Resource const& res,			/* 资源 */
			String const& dest_file_name	/* 目标文件名 */
		) const;

	private:
		FileSystem();

		~FileSystem();

	private:
		Vector<String> search_paths_;
		UnorderedMap<String, String> file_lookup_dict_;
		mutable UnorderedMap<String, String> file_lookup_cache_;
	};
}
