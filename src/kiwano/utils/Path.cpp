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

#include "Path.h"
#include "../platform/modules.h"

#pragma warning (push)
#pragma warning (disable:4091)  // ignore warning 4091 (on winSDK 8.1)
#include <shlobj.h>
#pragma warning (pop)

namespace kiwano
{
	namespace
	{
		// 创建指定文件夹
		bool CreateFolder(String const& dir_path)
		{
			if (dir_path.empty() || dir_path.size() >= MAX_PATH)
				return false;

			wchar_t tmp_dir_path[MAX_PATH] = { 0 };
			std::size_t length = dir_path.length();

			for (std::size_t i = 0; i < length; ++i)
			{
				tmp_dir_path[i] = dir_path.at(i);
				if (tmp_dir_path[i] == L'\\' || tmp_dir_path[i] == L'/' || i == (length - 1))
				{
					if (::_waccess(tmp_dir_path, 0) != 0)
					{
						if (::_wmkdir(tmp_dir_path) != 0)
						{
							return false;
						}
					}
				}
			}
			return true;
		}
	}

	String const& Path::GetTemporaryPath()
	{
		static String temp_path;
		if (temp_path.empty())
		{
			// 设置临时文件保存路径
			wchar_t path[_MAX_PATH];

			if (0 != ::GetTempPath(_MAX_PATH, path))
			{
				temp_path.append(path).append(L"\\KiwanoGameTemp\\");

				if (!modules::Shlwapi::Get().PathFileExistsW(temp_path.c_str()) && !CreateFolder(temp_path))
				{
					temp_path = L"";
				}
			}
		}
		return temp_path;
	}

	String const& Path::GetLocalAppDataPath()
	{
		static String local_app_data_path;
		if (local_app_data_path.empty())
		{
			// 获取 AppData/Local 文件夹的路径
			wchar_t path[MAX_PATH] = { 0 };
			::SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, path);
			local_app_data_path = path;
		}

		return local_app_data_path;
	}

	String const& Path::GetExeFilePath()
	{
		static String exe_file_path;
		if (exe_file_path.empty())
		{
			TCHAR path[_MAX_PATH] = { 0 };
			if (::GetModuleFileName(nullptr, path, _MAX_PATH) != 0)
			{
				exe_file_path = path;
			}
		}
		return exe_file_path;
	}
}
