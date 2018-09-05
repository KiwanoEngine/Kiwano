#include "..\e2dutil.h"
#include "..\e2dmodule.h"
#include <shobjidl.h> 

std::list<e2d::String>	e2d::File::search_paths_;

e2d::File::File()
	: file_path_()
	, attributes_(0)
{
}

e2d::File::File(const String & file_name)
	: file_path_(file_name)
	, attributes_(0)
{
	this->Open(file_name);
}

e2d::File::~File()
{
}

bool e2d::File::Open(const String & file_name)
{
	auto FindFile = [=](const String & path) -> bool
	{
		if (::_waccess((const wchar_t*)path, 0) == 0)
		{
			attributes_ = ::GetFileAttributes((LPCTSTR)path);
			return true;
		}
		return false;
	};

	if (FindFile(file_name))
	{
		file_path_ = file_name;
		return true;
	}
	else
	{
		for (const auto& resPath : search_paths_)
		{
			if (FindFile(resPath + file_name))
			{
				file_path_ = resPath + file_name;
				return true;
			}
		}
	}
	return false;
}

bool e2d::File::Exists() const
{
	return ::_waccess((const wchar_t*)file_path_, 0) == 0;
}

bool e2d::File::IsFolder() const
{
	return (attributes_ & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

const e2d::String& e2d::File::GetPath() const
{
	return file_path_;
}

e2d::String e2d::File::GetExtension() const
{
	String fileExtension;
	// 找到文件名中的最后一个 '.' 的位置
	size_t pos = std::wstring(file_path_).find_last_of(L'.');
	// 判断 pos 是否是有效位置
	if (pos != std::wstring::npos)
	{
		// 截取扩展名
		fileExtension = file_path_.Subtract(static_cast<int>(pos));
		// 转换为小写字母
		fileExtension = fileExtension.ToLower();
	}
	return std::move(fileExtension);
}

bool e2d::File::Delete()
{
	if (::DeleteFile((LPCWSTR)file_path_))
		return true;
	return false;
}

e2d::File e2d::File::Extract(int resource_name, const String & resource_type, const String& dest_file_name)
{
	// 创建文件
	HANDLE hFile = ::CreateFile(
		static_cast<LPCWSTR>(dest_file_name),
		GENERIC_WRITE,
		NULL,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_TEMPORARY,
		NULL
	);

	if (hFile == INVALID_HANDLE_VALUE)
		return std::move(File());

	// 查找资源文件中、加载资源到内存、得到资源大小
	HRSRC hRes = ::FindResource(NULL, MAKEINTRESOURCE(resource_name), (LPCWSTR)resource_type);
	HGLOBAL hMem = ::LoadResource(NULL, hRes);
	DWORD dwSize = ::SizeofResource(NULL, hRes);

	if (hRes && hMem && dwSize)
	{
		// 写入文件
		DWORD dwWrite = 0;
		::WriteFile(hFile, hMem, dwSize, &dwWrite, NULL);
		::CloseHandle(hFile);
		return File(dest_file_name);
	}
	else
	{
		::CloseHandle(hFile);
		::DeleteFile(static_cast<LPCWSTR>(dest_file_name));
		return std::move(File());
	}
}

void e2d::File::AddSearchPath(const String & path)
{
	String tmp = path;
	tmp.Replace(L"/", L"\\");
	if (tmp[tmp.GetLength() - 1] != L'\\')
	{
		tmp << L"\\";
	}
	auto iter = std::find(search_paths_.cbegin(), search_paths_.cend(), tmp);
	if (iter == search_paths_.cend())
	{
		search_paths_.push_front(path);
	}
}

bool e2d::File::CreateFolder(const String & dir_path)
{
	if (dir_path.IsEmpty() || dir_path.GetLength() >= MAX_PATH)
		return false;

	wchar_t tmpDirPath[_MAX_PATH] = { 0 };
	int length = dir_path.GetLength();

	for (int i = 0; i < length; ++i)
	{
		tmpDirPath[i] = dir_path.At(i);
		if (tmpDirPath[i] == L'\\' || tmpDirPath[i] == L'/' || i == (length - 1))
		{
			if (::_waccess(tmpDirPath, 0) != 0)
			{
				if (::_wmkdir(tmpDirPath) != 0)
				{
					return false;
				}
			}
		}
	}
	return true;
}

e2d::File e2d::File::ShowOpenDialog(const String & title, const String & filter)
{
	String file_path;
	HRESULT hr = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	if (SUCCEEDED(hr))
	{
		IFileOpenDialog *pFileOpen;

		hr = ::CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		if (SUCCEEDED(hr))
		{
			if (!title.IsEmpty())
			{
				pFileOpen->SetTitle(LPCWSTR(title));
			}

			if (!filter.IsEmpty())
			{
				COMDLG_FILTERSPEC rgSpec[] =
				{
					{ L"", LPCWSTR(filter) }
				};
				pFileOpen->SetFileTypes(1, rgSpec);
			}
			else
			{
				COMDLG_FILTERSPEC rgSpec[] =
				{
					{ L"所有文件", L"*.*" }
				};
				pFileOpen->SetFileTypes(1, rgSpec);
			}

			Game::GetInstance()->Pause();
			{
				HWND hWnd = Window::GetInstance()->GetHWnd();
				hr = pFileOpen->Show(hWnd);
			}
			Game::GetInstance()->Resume();

			if (SUCCEEDED(hr))
			{
				IShellItem *pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					if (SUCCEEDED(hr))
					{
						file_path = pszFilePath;
						::CoTaskMemFree(pszFilePath);
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		::CoUninitialize();
	}
	return std::move(File(file_path));
}

e2d::File e2d::File::ShowSaveDialog(const String & title, const String& def_file, const String & def_ext)
{
	String file_path;
	HRESULT hr = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	
	if (SUCCEEDED(hr))
	{
		IFileSaveDialog *pFileSave;

		hr = ::CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
			IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));

		if (SUCCEEDED(hr))
		{
			if (!title.IsEmpty())
			{
				pFileSave->SetTitle(LPCWSTR(title));
			}

			if (!def_file.IsEmpty())
			{
				pFileSave->SetFileName(LPCWSTR(def_file));
			}

			if (!def_ext.IsEmpty())
			{
				pFileSave->SetDefaultExtension(LPCWSTR(def_ext));

				String spec = L"*." + def_ext;
				COMDLG_FILTERSPEC rgSpec[] =
				{
					{ L"", LPCWSTR(spec) }
				};
				pFileSave->SetFileTypes(1, rgSpec);
			}
			else
			{
				COMDLG_FILTERSPEC rgSpec[] =
				{
					{ L"所有文件", L"*.*" }
				};
				pFileSave->SetFileTypes(1, rgSpec);
			}

			Game::GetInstance()->Pause();
			{
				HWND hWnd = Window::GetInstance()->GetHWnd();
				hr = pFileSave->Show(hWnd);
			}
			Game::GetInstance()->Resume();

			if (SUCCEEDED(hr))
			{
				IShellItem *pItem;
				hr = pFileSave->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					if (SUCCEEDED(hr))
					{
						file_path = pszFilePath;
						::CoTaskMemFree(pszFilePath);
					}
					pItem->Release();
				}
			}
			pFileSave->Release();
		}
		::CoUninitialize();
	}
	return std::move(File(file_path));
}
