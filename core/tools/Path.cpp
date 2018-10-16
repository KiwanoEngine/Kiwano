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

#include "..\e2dtool.h"
#include "..\e2dmodule.h"
#include <shlobj.h>

const easy2d::String& easy2d::Path::GetDataPath()
{
	static String data_path;
	if (data_path.IsEmpty())
	{
		// 设置数据的保存路径
		String local_app_data_path = Path::GetLocalAppDataPath();
		String title = Game::GetInstance()->GetTitle();
		String folder_name = String::Parse(title.GetHash());

		if (!local_app_data_path.IsEmpty())
		{
			data_path = local_app_data_path + L"\\Easy2DGameData\\" << folder_name << L"\\";

			File file(data_path);
			if (!file.Exists() && !File::CreateFolder(data_path))
			{
				data_path = L"";
			}
		}
		data_path << L"Data.ini";
	}
	return data_path;
}

const easy2d::String& easy2d::Path::GetTemporaryPath()
{
	static String temp_path;
	if (temp_path.IsEmpty())
	{
		// 设置临时文件保存路径
		wchar_t path[_MAX_PATH];
		String title = Game::GetInstance()->GetTitle();
		String folder_name = String::Parse(title.GetHash());

		if (0 != ::GetTempPath(_MAX_PATH, path))
		{
			temp_path << path << L"\\Easy2DGameTemp\\" << folder_name << L"\\";

			File file(temp_path);
			if (!file.Exists() && !File::CreateFolder(temp_path))
			{
				temp_path = L"";
			}
		}
	}
	return temp_path;
}

const easy2d::String& easy2d::Path::GetLocalAppDataPath()
{
	static String local_app_data_path;
	if (local_app_data_path.IsEmpty())
	{
		// 获取 AppData/Local 文件夹的路径
		wchar_t path[MAX_PATH] = { 0 };
		::SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, path);
		local_app_data_path = path;
	}

	return local_app_data_path;
}

const easy2d::String& easy2d::Path::GetExeFilePath()
{
	static String exe_file_path;
	if (exe_file_path.IsEmpty())
	{
		TCHAR path[_MAX_PATH] = { 0 };
		if (::GetModuleFileName(nullptr, path, _MAX_PATH) != 0)
		{
			exe_file_path = path;
		}
	}
	return exe_file_path;
}
