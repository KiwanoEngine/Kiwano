#include "..\etools.h"
#include "..\Win\winbase.h"
#include <algorithm>
#include <commdlg.h>

#define DEFINE_KNOWN_FOLDER(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	EXTERN_C const GUID DECLSPEC_SELECTANY name \
	= { l, w1, w2,{ b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

DEFINE_KNOWN_FOLDER(FOLDERID_LocalAppData, 0xF1B32785, 0x6FBA, 0x4FCF, 0x9D, 0x55, 0x7B, 0x8E, 0x7F, 0x15, 0x70, 0x91);

typedef HRESULT(WINAPI* pFunSHGetKnownFolderPath)(
	const GUID& rfid,
	DWORD dwFlags,
	HANDLE hToken,
	PWSTR *ppszPath);

e2d::EString e2d::EFileUtils::getLocalAppDataPath()
{
	// 获取 AppData\Local 文件夹的路径
	PWSTR pszPath = NULL;
	HMODULE hModule = LoadLibrary(L"shell32.dll");
	pFunSHGetKnownFolderPath SHGetKnownFolderPath = (pFunSHGetKnownFolderPath)GetProcAddress(hModule, "SHGetKnownFolderPath");
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &pszPath);

	if (SUCCEEDED(hr))
	{
		EString path = pszPath;
		CoTaskMemFree(pszPath);
		return path;
	}
	return L"";
}

e2d::EString e2d::EFileUtils::getTempPath()
{
	// 获取临时文件目录
	wchar_t path[_MAX_PATH];
	::GetTempPath(_MAX_PATH, path);

	// 创建临时文件目录
	e2d::EString tempFilePath = path + e2d::EApp::getAppName();
	if (_waccess(tempFilePath, 0) == -1)
	{
		_wmkdir(tempFilePath);
	}
	return tempFilePath;
}

e2d::EString e2d::EFileUtils::getDefaultSavePath()
{
	EString path = EFileUtils::getLocalAppDataPath();
	WARN_IF(path.isEmpty(), "Cannot get local AppData path!");

	path += L"\\" + EApp::getAppName();

	if (_waccess(path, 0) == -1)
	{
		_wmkdir(path);
	}

	path += L"\\DefaultData.ini";

	return path;
}

void e2d::EFileUtils::saveInt(const EString & key, int value)
{
	::WritePrivateProfileString(L"Default", key, EString::parse(value), getDefaultSavePath());
}

void e2d::EFileUtils::saveFloat(const EString & key, float value)
{
	::WritePrivateProfileString(L"Default", key, EString::parse(value), getDefaultSavePath());
}

void e2d::EFileUtils::saveString(const EString & key, const EString & value)
{
	::WritePrivateProfileString(L"Default", key, value, getDefaultSavePath());
}

int e2d::EFileUtils::getInt(const EString & key, int default)
{
	return ::GetPrivateProfileInt(L"Default", key, default, getDefaultSavePath());
}

float e2d::EFileUtils::getFloat(const EString & key, float default)
{
	wchar_t temp[32] = { 0 };
	::GetPrivateProfileString(L"Default", key, EString::parse(default), temp, 31, getDefaultSavePath());
	return std::stof(temp);
}

e2d::EString e2d::EFileUtils::getString(const EString & key, const EString & default)
{
	wchar_t temp[256] = { 0 };
	::GetPrivateProfileString(L"Default", key, default, temp, 255, getDefaultSavePath());
	return temp;
}

e2d::EString e2d::EFileUtils::getFileExtension(const EString & filePath)
{
	EString fileExtension;
	// 找到文件名中的最后一个 '.' 的位置
	size_t pos = filePath.findLastOf(L'.');
	// 判断 pos 是否是个有效位置
	if (pos != -1)
	{
		// 截取扩展名
		fileExtension = filePath.sub(pos, filePath.length());
		// 转换为小写字母
		fileExtension = fileExtension.lower();
	}

	return fileExtension;
}

e2d::EString e2d::EFileUtils::getSaveFilePath(const EString & title, const EString & defExt)
{
	// 弹出保存对话框
	OPENFILENAME ofn = { 0 };
	TCHAR strFilename[MAX_PATH] = { 0 };				// 用于接收文件名
	ofn.lStructSize = sizeof(OPENFILENAME);				// 结构体大小
	ofn.hwndOwner = GetHWnd();							// 窗口句柄
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
