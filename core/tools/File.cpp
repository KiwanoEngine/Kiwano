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
#include <shobjidl.h> 

std::list<easy2d::String>	easy2d::File::search_paths_;

easy2d::File::File()
	: file_path_()
{
}

easy2d::File::File(const String & file_name)
	: file_path_(file_name)
{
	this->Open(file_name);
}

easy2d::File::~File()
{
}

bool easy2d::File::Open(const String & file_name)
{
	if (file_name.IsEmpty())
		return false;

	auto FindFile = [](const String & path) -> bool
	{
		if (::PathFileExists((const wchar_t*)path))
			return true;
		return false;
	};

	if (FindFile(file_name))
	{
		file_path_ = file_name;
		return true;
	}
	
	for (const auto& path : search_paths_)
	{
		if (FindFile(path + file_name))
		{
			file_path_ = path + file_name;
			return true;
		}
	}
	return false;
}

bool easy2d::File::Exists() const
{
	if (::PathFileExists(static_cast<LPCWSTR>(file_path_)))
		return true;
	return false;
}

const easy2d::String& easy2d::File::GetPath() const
{
	return file_path_;
}

easy2d::String easy2d::File::GetExtension() const
{
	String file_ext;
	// 找到文件名中的最后一个 '.' 的位置
	size_t pos = file_path_.operator std::wstring().find_last_of(L'.');
	// 判断 pos 是否是有效位置
	if (pos != std::wstring::npos)
	{
		// 截取扩展名
		file_ext = file_path_.Subtract(static_cast<int>(pos));
		// 转换为小写字母
		file_ext = file_ext.ToLower();
	}
	return std::move(file_ext);
}

bool easy2d::File::Delete()
{
	if (::DeleteFile(static_cast<LPCWSTR>(file_path_)))
		return true;
	return false;
}

easy2d::File easy2d::File::Extract(const Resource& res, const String& dest_file_name)
{
	File file;
	HANDLE file_handle = ::CreateFile(
		static_cast<LPCWSTR>(dest_file_name),
		GENERIC_WRITE,
		NULL,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_TEMPORARY,
		NULL
	);

	if (file_handle == INVALID_HANDLE_VALUE)
		return std::move(file);

	// 查找资源文件中、加载资源到内存、得到资源大小
	HRSRC res_info = ::FindResource(NULL, MAKEINTRESOURCE(res.id), static_cast<LPCWSTR>(res.type));
	HGLOBAL res_data = ::LoadResource(NULL, res_info);
	DWORD res_size = ::SizeofResource(NULL, res_info);

	if (res_info && res_data && res_size)
	{
		// 写入文件
		DWORD written_bytes = 0;
		::WriteFile(file_handle, res_data, res_size, &written_bytes, NULL);
		::CloseHandle(file_handle);

		file.Open(dest_file_name);
	}
	else
	{
		::CloseHandle(file_handle);
		::DeleteFile(static_cast<LPCWSTR>(dest_file_name));
	}

	::FreeResource(res_data);
	return std::move(file);
}

void easy2d::File::AddSearchPath(const String & path)
{
	String tmp = path;
	tmp.Replace(L"/", L"\\");
	if (tmp.At(tmp.Length() - 1) != L'\\')
	{
		tmp << L"\\";
	}
	auto iter = std::find(search_paths_.cbegin(), search_paths_.cend(), tmp);
	if (iter == search_paths_.cend())
	{
		search_paths_.push_front(path);
	}
}

easy2d::File easy2d::File::ShowOpenDialog(const String & title, const String & filter)
{
	String file_path;
	HRESULT hr = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	if (SUCCEEDED(hr))
	{
		IFileOpenDialog *file_open;

		hr = ::CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&file_open));

		if (SUCCEEDED(hr))
		{
			if (!title.IsEmpty())
			{
				file_open->SetTitle(LPCWSTR(title));
			}

			if (!filter.IsEmpty())
			{
				COMDLG_FILTERSPEC spec[] =
				{
					{ L"", LPCWSTR(filter) }
				};
				file_open->SetFileTypes(1, spec);
			}
			else
			{
				COMDLG_FILTERSPEC spec[] =
				{
					{ L"所有文件", L"*.*" }
				};
				file_open->SetFileTypes(1, spec);
			}

			hr = file_open->Show(nullptr);

			if (SUCCEEDED(hr))
			{
				IShellItem *item;
				hr = file_open->GetResult(&item);
				if (SUCCEEDED(hr))
				{
					PWSTR str_file_path;
					hr = item->GetDisplayName(SIGDN_FILESYSPATH, &str_file_path);

					if (SUCCEEDED(hr))
					{
						file_path = str_file_path;
						::CoTaskMemFree(str_file_path);
					}
					item->Release();
				}
			}
			file_open->Release();
		}
		::CoUninitialize();
	}
	return std::move(File(file_path));
}

easy2d::File easy2d::File::ShowSaveDialog(const String & title, const String& def_file, const String & def_ext)
{
	String file_path;
	HRESULT hr = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	
	if (SUCCEEDED(hr))
	{
		IFileSaveDialog *file_save;

		hr = ::CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
			IID_IFileSaveDialog, reinterpret_cast<void**>(&file_save));

		if (SUCCEEDED(hr))
		{
			if (!title.IsEmpty())
			{
				file_save->SetTitle(LPCWSTR(title));
			}

			if (!def_file.IsEmpty())
			{
				file_save->SetFileName(LPCWSTR(def_file));
			}

			if (!def_ext.IsEmpty())
			{
				file_save->SetDefaultExtension(LPCWSTR(def_ext));

				String ext = L"*." + def_ext;
				COMDLG_FILTERSPEC spec[] =
				{
					{ L"", LPCWSTR(ext) }
				};
				file_save->SetFileTypes(1, spec);
			}
			else
			{
				COMDLG_FILTERSPEC spec[] =
				{
					{ L"所有文件", L"*.*" }
				};
				file_save->SetFileTypes(1, spec);
			}

			hr = file_save->Show(nullptr);

			if (SUCCEEDED(hr))
			{
				IShellItem *item;
				hr = file_save->GetResult(&item);
				if (SUCCEEDED(hr))
				{
					PWSTR str_file_path;
					hr = item->GetDisplayName(SIGDN_FILESYSPATH, &str_file_path);

					if (SUCCEEDED(hr))
					{
						file_path = str_file_path;
						::CoTaskMemFree(str_file_path);
					}
					item->Release();
				}
			}
			file_save->Release();
		}
		::CoUninitialize();
	}
	return std::move(File(file_path));
}
