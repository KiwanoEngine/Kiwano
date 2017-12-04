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
	TCHAR path[_MAX_PATH];
	::GetTempPath(_MAX_PATH, path);

	// 创建临时文件目录
	e2d::EString tempFilePath = path + e2d::EApp::getAppName();
	if (_waccess(tempFilePath.c_str(), 0) == -1)
	{
		_wmkdir(tempFilePath.c_str());
	}
	return tempFilePath;
}

e2d::EString e2d::EFileUtils::getDefaultSavePath()
{
	EString path = EFileUtils::getLocalAppDataPath();
	WARN_IF(path.empty(), "Cannot get local AppData path!");

	path.append(L"\\");
	path.append(EApp::getAppName());

	if (_waccess(path.c_str(), 0) == -1)
	{
		_wmkdir(path.c_str());
	}

	path.append(L"\\DefaultData.ini");

	return path;
}

void e2d::EFileUtils::saveInt(const EString & key, int value)
{
	::WritePrivateProfileString(L"Default", key.c_str(), std::to_wstring(value).c_str(), getDefaultSavePath().c_str());
}

void e2d::EFileUtils::saveFloat(const EString & key, float value)
{
	::WritePrivateProfileString(L"Default", key.c_str(), std::to_wstring(value).c_str(), getDefaultSavePath().c_str());
}

void e2d::EFileUtils::saveString(const EString & key, const EString & value)
{
	::WritePrivateProfileString(L"Default", key.c_str(), value.c_str(), getDefaultSavePath().c_str());
}

int e2d::EFileUtils::getInt(const EString & key, int default)
{
	return ::GetPrivateProfileInt(L"Default", key.c_str(), default, getDefaultSavePath().c_str());
}

float e2d::EFileUtils::getFloat(const EString & key, float default)
{
	TCHAR temp[32] = { 0 };
	::GetPrivateProfileString(L"Default", key.c_str(), std::to_wstring(default).c_str(), temp, 31, getDefaultSavePath().c_str());
	return std::stof(temp);
}

e2d::EString e2d::EFileUtils::getString(const EString & key, const EString & default)
{
	TCHAR temp[256] = { 0 };
	::GetPrivateProfileString(L"Default", key.c_str(), default.c_str(), temp, 255, getDefaultSavePath().c_str());
	return EString(temp);
}

e2d::EString e2d::EFileUtils::getFileExtension(const EString & filePath)
{
	EString fileExtension;
	// 找到文件名中的最后一个 '.' 的位置
	size_t pos = filePath.find_last_of('.');
	// 判断 pos 是否是个有效位置
	if (pos != EString::npos)
	{
		// 截取扩展名
		fileExtension = filePath.substr(pos, filePath.length());
		// 转换为小写字母
		std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);
	}

	return fileExtension;
}

e2d::EString e2d::EFileUtils::getSaveFilePath(const EString & title, const EString & defExt)
{
	// 弹出保存对话框
	OPENFILENAME ofn = { 0 };
	TCHAR strFilename[MAX_PATH] = { 0 };				// 用于接收文件名
	ofn.lStructSize = sizeof(OPENFILENAME);				// 结构体大小
	ofn.hwndOwner = GetHWnd();							// 拥有着窗口句柄，NULL 表示对话框是非模态的
	ofn.lpstrFilter = L"所有文件\0*.*\0\0";			// 设置过滤
	ofn.nFilterIndex = 1;								// 过滤器索引
	ofn.lpstrFile = strFilename;						// 接收返回的文件路径和文件名
	ofn.nMaxFile = sizeof(strFilename);					// 缓冲区长度
	ofn.lpstrInitialDir = NULL;							// 初始目录为默认
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;// 目录必须存在，覆盖文件前发出警告
	ofn.lpstrTitle = title.c_str();						// 使用系统默认标题留空即可
	ofn.lpstrDefExt = defExt.c_str();					// 默认追加的扩展名

	if (GetSaveFileName(&ofn))
	{
		return strFilename;
	}
	return L"";
}
