#include "..\etool.h"
#include <algorithm>
#include <commdlg.h>

#define DEFINE_KNOWN_FOLDER(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	EXTERN_C const GUID DECLSPEC_SELECTANY name \
	= { l, w1, w2,{ b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

DEFINE_KNOWN_FOLDER(FOLDERID_LocalAppData, 0xF1B32785, 0x6FBA, 0x4FCF, 0x9D, 0x55, 0x7B, 0x8E, 0x7F, 0x15, 0x70, 0x91);


e2d::String e2d::Path::getLocalAppDataPath()
{
	typedef HRESULT(WINAPI* pFunSHGetKnownFolderPath)(const GUID& rfid, DWORD dwFlags, HANDLE hToken, PWSTR *ppszPath);

	// 获取 AppData\Local 文件夹的路径
	PWSTR pszPath = NULL;
	HMODULE hModule = LoadLibrary(L"shell32.dll");
	pFunSHGetKnownFolderPath SHGetKnownFolderPath = (pFunSHGetKnownFolderPath)GetProcAddress(hModule, "SHGetKnownFolderPath");
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &pszPath);

	if (SUCCEEDED(hr))
	{
		String path = pszPath;
		CoTaskMemFree(pszPath);
		return path;
	}
	return L"";
}

e2d::String e2d::Path::getTempPath()
{
	// 获取临时文件目录
	wchar_t path[_MAX_PATH];
	if (0 == ::GetTempPath(_MAX_PATH, path))
	{
		return L"";
	}

	// 创建临时文件目录
	e2d::String tempFilePath;
	tempFilePath << path << L"Easy2DGameTemp\\";
	// 创建文件夹
	if (!Path::createFolder(tempFilePath))
	{
		return path;
	}

	// 获取 AppName
	String sAppName = Game::getAppName();
	if (!sAppName.isEmpty())
	{
		// 创建文件夹
		if (!Path::createFolder(tempFilePath + sAppName + L"\\"))
		{
			return std::move(tempFilePath);
		}
		tempFilePath << sAppName << L"\\";
	}
	return std::move(tempFilePath);
}

e2d::String e2d::Path::getDefaultSavePath()
{
	// 获取 AppData 路径
	String path = Path::getLocalAppDataPath();

	if (path.isEmpty())
	{
		WARN_IF(true, "Cannot get local AppData path!");
		return std::move(path);
	}
	
	// 创建文件夹
	if (!Path::createFolder(path + L"\\Easy2DGameData"))
	{
		return std::move(path);
	}
	path << L"\\Easy2DGameData";

	// 获取 AppName
	String sAppName = Game::getAppName();
	if (!sAppName.isEmpty())
	{
		// 创建文件夹
		if (!Path::createFolder(path + L"\\" + sAppName))
		{
			return std::move(path);
		}
		path << L"\\" << sAppName;
	}
	path << L"\\";
	
	return std::move(path);
}

e2d::String e2d::Path::getFileExtension(String filePath)
{
	String fileExtension;
	// 找到文件名中的最后一个 '.' 的位置
	int pos = filePath.findLastOf(L'.');
	// 判断 pos 是否是个有效位置
	if (pos != -1)
	{
		// 截取扩展名
		fileExtension = filePath.subtract(pos);
		// 转换为小写字母
		fileExtension = fileExtension.toLower();
	}

	return fileExtension;
}

e2d::String e2d::Path::getSaveFilePath(const String title, const String defExt)
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
	ofn.lpstrInitialDir = NULL;							// 初始目录为默认
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrTitle = title;								// 标题
	ofn.lpstrDefExt = defExt;							// 默认追加的扩展名

	if (GetSaveFileName(&ofn))
	{
		return strFilename;
	}
	return L"";
}

bool e2d::Path::createFolder(String strDirPath)
{
	if (strDirPath.isEmpty())
	{
		WARN_IF(true, "Path::createFolder Failed: Invalid directory path!");
		return false;
	}

	if (-1 == ::_waccess(strDirPath, 0))
	{
		if (0 != ::_wmkdir(strDirPath))
		{
			return false;
		}
	}
	return true;
}
