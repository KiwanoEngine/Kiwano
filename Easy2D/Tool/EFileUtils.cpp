#include "..\etools.h"
#include "..\Win\winbase.h"
#include <sstream>
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

void e2d::EFileUtils::saveInt(LPCTSTR key, int value)
{
	std::wstringstream ss;
	ss << value;
	::WritePrivateProfileString(L"Default", key, ss.str().c_str(), getDefaultSavePath().c_str());
}

void e2d::EFileUtils::saveDouble(LPCTSTR key, double value)
{
	std::wstringstream ss;
	ss << value;
	::WritePrivateProfileString(L"Default", key, ss.str().c_str(), getDefaultSavePath().c_str());
}

void e2d::EFileUtils::saveString(LPCTSTR key, EString value)
{
	::WritePrivateProfileString(L"Default", key, value.c_str(), getDefaultSavePath().c_str());
}

int e2d::EFileUtils::getInt(LPCTSTR key, int default)
{
	return ::GetPrivateProfileInt(L"Default", key, default, getDefaultSavePath().c_str());
}

double e2d::EFileUtils::getDouble(LPCTSTR key, double default)
{
	// 将 default 参数转化为字符串
	std::wstringstream ss;
	ss << default;
	// 读取数据
	TCHAR temp[128] = { 0 };
	::GetPrivateProfileString(L"Default", key, ss.str().c_str(), temp, 128, getDefaultSavePath().c_str());
	// 转换为字符串流
	ss.str(L"");
	ss << temp;
	// 将字符串转化为 double
	return _wtof(ss.str().c_str());
}

e2d::EString e2d::EFileUtils::geTString(LPCTSTR key, EString default)
{
	TCHAR temp[128] = { 0 };
	::GetPrivateProfileString(L"Default", key, default.c_str(), temp, 128, getDefaultSavePath().c_str());
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

e2d::EString e2d::EFileUtils::getSaveFilePath(LPCTSTR title, LPCTSTR defExt)
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
	ofn.lpstrTitle = title;								// 使用系统默认标题留空即可
	ofn.lpstrDefExt = defExt;							// 默认追加的扩展名

	if (GetSaveFileName(&ofn))
	{
		return strFilename;
	}
	return L"";
}
