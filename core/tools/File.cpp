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
#include <cwctype>
#include <shobjidl.h> 

std::list<std::wstring>	easy2d::File::search_paths_;

easy2d::File::File()
	: file_path_()
{
}

easy2d::File::File(const std::wstring & file_name)
	: file_path_(file_name)
{
	this->Open(file_name);
}

easy2d::File::~File()
{
}

bool easy2d::File::Open(const std::wstring & file_name)
{
	if (file_name.empty())
		return false;

	auto FindFile = [](const std::wstring & path) -> bool
	{
		if (::PathFileExists(path.c_str()))
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
	if (::PathFileExists(file_path_.c_str()))
		return true;
	return false;
}

const std::wstring& easy2d::File::GetPath() const
{
	return file_path_;
}

std::wstring easy2d::File::GetExtension() const
{
	std::wstring file_ext;
	// 找到文件名中的最后一个 '.' 的位置
	size_t pos = file_path_.find_last_of(L'.');
	// 判断 pos 是否是有效位置
	if (pos != std::wstring::npos)
	{
		// 截取扩展名
		file_ext = file_path_.substr(pos);
		// 转换为小写字母
		std::transform(file_ext.begin(), file_ext.end(), file_ext.begin(), std::towlower);
	}
	return std::move(file_ext);
}

bool easy2d::File::Delete()
{
	if (::DeleteFile(file_path_.c_str()))
		return true;
	return false;
}

easy2d::File easy2d::File::Extract(Resource& res, const std::wstring& dest_file_name)
{
	File file;
	HANDLE file_handle = ::CreateFile(
		dest_file_name.c_str(),
		GENERIC_WRITE,
		NULL,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_TEMPORARY,
		NULL
	);

	if (file_handle == INVALID_HANDLE_VALUE)
		return std::move(file);

	if (res.Load())
	{
		// 写入文件
		DWORD written_bytes = 0;
		::WriteFile(file_handle, res.GetData(), res.GetDataSize(), &written_bytes, NULL);
		::CloseHandle(file_handle);

		file.Open(dest_file_name);
	}
	else
	{
		::CloseHandle(file_handle);
		::DeleteFile(dest_file_name.c_str());
	}

	return std::move(file);
}

void easy2d::File::AddSearchPath(const std::wstring & path)
{
	std::wstring tmp = path;
	size_t pos = 0;
	while ((pos = tmp.find(L"/", pos)) != std::wstring::npos)
	{
		tmp.replace(pos, 1, L"\\");
		pos++;
	}

	if (tmp.at(tmp.length() - 1) != L'\\')
	{
		tmp.append(L"\\");
	}
	auto iter = std::find(search_paths_.cbegin(), search_paths_.cend(), tmp);
	if (iter == search_paths_.cend())
	{
		search_paths_.push_front(path);
	}
}

easy2d::File easy2d::File::ShowOpenDialog(const std::wstring & title, const std::wstring & filter)
{
	std::wstring file_path;
	HRESULT hr = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	if (SUCCEEDED(hr))
	{
		IFileOpenDialog *file_open;

		hr = ::CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&file_open));

		if (SUCCEEDED(hr))
		{
			if (!title.empty())
			{
				file_open->SetTitle(title.c_str());
			}

			if (!filter.empty())
			{
				COMDLG_FILTERSPEC spec[] =
				{
					{ L"", filter.c_str() }
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

easy2d::File easy2d::File::ShowSaveDialog(const std::wstring & title, const std::wstring& def_file, const std::wstring & def_ext)
{
	std::wstring file_path;
	HRESULT hr = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	
	if (SUCCEEDED(hr))
	{
		IFileSaveDialog *file_save;

		hr = ::CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
			IID_IFileSaveDialog, reinterpret_cast<void**>(&file_save));

		if (SUCCEEDED(hr))
		{
			if (!title.empty())
			{
				file_save->SetTitle(title.c_str());
			}

			if (!def_file.empty())
			{
				file_save->SetFileName(def_file.c_str());
			}

			if (!def_ext.empty())
			{
				file_save->SetDefaultExtension(def_ext.c_str());

				std::wstring ext = L"*." + def_ext;
				COMDLG_FILTERSPEC spec[] =
				{
					{ L"", ext.c_str() }
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
