#include "..\e2dtool.h"
#include <algorithm>
#include <commdlg.h>

#define DEFINE_KNOWN_FOLDER(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	EXTERN_C const GUID DECLSPEC_SELECTANY name \
	= { l, w1, w2,{ b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

DEFINE_KNOWN_FOLDER(FOLDERID_LocalAppData, 0xF1B32785, 0x6FBA, 0x4FCF, 0x9D, 0x55, 0x7B, 0x8E, 0x7F, 0x15, 0x70, 0x91);


static e2d::String s_sLocalAppDataPath;
static e2d::String s_sTempPath;
static e2d::String s_sDefaultSavePath;

bool e2d::Path::__init()
{
	// 获取 AppData\Local 文件夹的路径
	typedef HRESULT(WINAPI* pFunSHGetKnownFolderPath)(const GUID& rfid, DWORD dwFlags, HANDLE hToken, PWSTR *ppszPath);

	PWSTR pszPath = nullptr;
	HMODULE hModule = LoadLibrary(L"shell32.dll");
	pFunSHGetKnownFolderPath SHGetKnownFolderPath = (pFunSHGetKnownFolderPath)GetProcAddress(hModule, "SHGetKnownFolderPath");
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &pszPath);

	if (SUCCEEDED(hr))
	{
		s_sLocalAppDataPath = pszPath;
		CoTaskMemFree(pszPath);
	}
	else
	{
		WARN("Cannot get local AppData path!");
	}

	// 获取游戏名称
	String sGameName = Game::getName();
	
	// 获取默认保存路径
	bool bInitSavePath = false;
	do
	{
		String localAppPath = s_sLocalAppDataPath;
		if (localAppPath.isEmpty())
		{

			break;
		}
		else
		{
			s_sDefaultSavePath = localAppPath;
		}

		localAppPath << L"\\Easy2DGameData";
		if (Path::createFolder(localAppPath))
		{
			s_sDefaultSavePath = localAppPath;
		}
		else
		{
			break;
		}

		if (!sGameName.isEmpty())
		{
			localAppPath << L"\\" << sGameName;
			// 创建文件夹
			if (Path::createFolder(localAppPath))
			{
				s_sDefaultSavePath = localAppPath;
			}
		}

		s_sDefaultSavePath << L"\\";
		bInitSavePath = true;
	} while (0);

	// 获取临时文件目录
	bool bInitTempPath = false;
	do
	{
		wchar_t path[_MAX_PATH];
		if (0 == ::GetTempPath(_MAX_PATH, path))
		{
			break;
		}
		else
		{
			s_sTempPath = path;
		}

		// 创建临时文件目录
		String tempPath;
		tempPath << s_sTempPath << L"\\Easy2DGameTemp";
		// 创建文件夹
		if (Path::createFolder(tempPath))
		{
			s_sTempPath = path;
		}
		else
		{
			break;
		}

		if (!sGameName.isEmpty())
		{
			tempPath << L"\\" << sGameName;
			// 创建文件夹
			if (Path::createFolder(tempPath))
			{
				s_sTempPath = tempPath;
			}
		}

		s_sTempPath << L"\\";
		bInitTempPath = true;
	} while (0);

	return SUCCEEDED(hr) && bInitSavePath && bInitTempPath;
}

e2d::String e2d::Path::getTempPath()
{
	return s_sTempPath;
}

e2d::String e2d::Path::getDefaultSavePath()
{
	return s_sDefaultSavePath;
}

e2d::String e2d::Path::getFileExtension(const String& filePath)
{
	String fileExtension;
	// 找到文件名中的最后一个 '.' 的位置
	size_t pos = filePath.getWString().find_last_of(L'.');
	// 判断 pos 是否是有效位置
	if (pos != std::wstring::npos)
	{
		// 截取扩展名
		fileExtension = filePath.subtract(static_cast<int>(pos));
		// 转换为小写字母
		fileExtension = fileExtension.toLower();
	}

	return fileExtension;
}

e2d::String e2d::Path::getSaveFilePath(const String& title, const String& defExt)
{
	// 弹出保存对话框
	OPENFILENAME ofn = { 0 };
	wchar_t strFilename[MAX_PATH] = { 0 };				// 用于接收文件名
	ofn.lStructSize = sizeof(OPENFILENAME);				// 结构体大小
	ofn.hwndOwner = Window::getHWnd();					// 窗口句柄
	ofn.lpstrFilter = L"所有文件\0*.*\0\0";				// 设置过滤
	ofn.nFilterIndex = 1;								// 过滤器索引
	ofn.lpstrFile = strFilename;						// 接收返回的文件路径和文件名
	ofn.nMaxFile = sizeof(strFilename);					// 缓冲区长度
	ofn.lpstrInitialDir = nullptr;						// 初始目录为默认
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrTitle = title;								// 标题
	ofn.lpstrDefExt = defExt;							// 默认追加的扩展名

	if (GetSaveFileName(&ofn))
	{
		return strFilename;
	}
	return L"";
}

bool e2d::Path::createFolder(const String& dirPath)
{
	if (dirPath.isEmpty())
	{
		WARN("Path::createFolder Failed: Invalid directory path!");
		return false;
	}

	if (-1 == ::_waccess(dirPath, 0))
	{
		if (0 != ::_wmkdir(dirPath))
		{
			return false;
		}
	}
	return true;
}
