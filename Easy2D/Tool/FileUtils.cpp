#include "..\easy2d.h"
#include "..\EasyX\easyx.h"
#include <Shlobj.h>
#pragma comment(lib, "shell32.lib")
#include <sstream>
#include <algorithm>

#ifndef UNICODE
	#include <io.h>
	#include <direct.h>
#endif

TString FileUtils::getLocalAppDataPath()
{
	TCHAR m_lpszDefaultDir[MAX_PATH] = { 0 };
	TCHAR szDocument[MAX_PATH] = { 0 };

	// 获取 AppData\Local 文件夹的 ID
	LPITEMIDLIST pidl = NULL;
	SHGetSpecialFolderLocation(NULL, CSIDL_LOCAL_APPDATA, &pidl);
	if (pidl && SHGetPathFromIDList(pidl, szDocument))
	{
		// 获取文件夹路径
		GetShortPathName(szDocument, m_lpszDefaultDir, _MAX_PATH);
	}

	return m_lpszDefaultDir;
}

TString FileUtils::getDefaultSavePath()
{
	TCHAR m_lpszDefaultDir[MAX_PATH] = { 0 };
	TCHAR szDocument[MAX_PATH] = { 0 };

	// 获取 AppData\Local 文件夹的 ID
	LPITEMIDLIST pidl = NULL;
	SHGetSpecialFolderLocation(NULL, CSIDL_LOCAL_APPDATA, &pidl);
	if (pidl && SHGetPathFromIDList(pidl, szDocument))
	{
		// 获取文件夹路径
		GetShortPathName(szDocument, m_lpszDefaultDir, _MAX_PATH);
	}

	TString path = m_lpszDefaultDir;
	path.append(_T("\\"));
	path.append(App::get()->getAppName());

#ifdef UNICODE
	if (_waccess(path.c_str(), 0) == -1)
	{
		_wmkdir(path.c_str());
	}
#else
	if (_access(path.c_str(), 0) == -1)
	{
		_mkdir(path.c_str());
	}
#endif

	path.append(_T("\\DefaultData.ini"));

	return path;
}

void FileUtils::saveInt(LPCTSTR key, int value)
{
#ifdef UNICODE
	std::wstringstream ss;
#else
	std::stringstream ss;
#endif
	ss << value;
	::WritePrivateProfileString(_T("Default"), key, ss.str().c_str(), getDefaultSavePath().c_str());
}

void FileUtils::saveDouble(LPCTSTR key, double value)
{
#ifdef UNICODE
	std::wstringstream ss;
#else
	std::stringstream ss;
#endif
	ss << value;
	::WritePrivateProfileString(_T("Default"), key, ss.str().c_str(), getDefaultSavePath().c_str());
}

void FileUtils::saveString(LPCTSTR key, TString value)
{
	::WritePrivateProfileString(_T("Default"), key, value.c_str(), getDefaultSavePath().c_str());
}

int FileUtils::getInt(LPCTSTR key, int default)
{
	return ::GetPrivateProfileInt(_T("Default"), key, default, getDefaultSavePath().c_str());
}
#include <iostream>
using namespace std;
double FileUtils::getDouble(LPCTSTR key, double default)
{
	// 将 default 参数转化为字符串
#ifdef UNICODE
	std::wstringstream ss;
#else
	std::stringstream ss;
#endif
	ss << default;
	// 读取数据
	TCHAR temp[128] = { 0 };
	::GetPrivateProfileString(_T("Default"), key, ss.str().c_str(), temp, 128, getDefaultSavePath().c_str());
	// 转换为字符串流
	ss.str(_T(""));
	ss << temp;
	// 将字符串转化为 double
#ifdef UNICODE
	double d = _wtof(ss.str().c_str());
#else
	double d = atof(ss.str().c_str());
#endif
	return d;
}

TString FileUtils::geTString(LPCTSTR key, TString default)
{
	TCHAR temp[128] = { 0 };
	::GetPrivateProfileString(_T("Default"), key, default.c_str(), temp, 128, getDefaultSavePath().c_str());
	return TString(temp);
}

TString FileUtils::getFileExtension(const TString & filePath)
{
	TString fileExtension;
	// 找到文件名中的最后一个 '.' 的位置
	size_t pos = filePath.find_last_of('.');
	// 判断 pos 是否是个有效位置
	if (pos != TString::npos)
	{
		// 截取扩展名
		fileExtension = filePath.substr(pos, filePath.length());
		// 转换为小写字母
		std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);
	}

	return fileExtension;
}

bool FileUtils::getSaveFilePath(TString& path, LPCTSTR title, LPCTSTR defExt)
{
	// 弹出保存对话框
	OPENFILENAME ofn = { 0 };
	TCHAR strFilename[MAX_PATH] = { 0 };				// 用于接收文件名
	ofn.lStructSize = sizeof(OPENFILENAME);				// 结构体大小
	ofn.hwndOwner = GetHWnd();							// 拥有着窗口句柄，NULL 表示对话框是非模态的
	ofn.lpstrFilter = _T("所有文件\0*.*\0\0");			// 设置过滤
	ofn.nFilterIndex = 1;								// 过滤器索引
	ofn.lpstrFile = strFilename;						// 接收返回的文件路径和文件名
	ofn.nMaxFile = sizeof(strFilename);					// 缓冲区长度
	ofn.lpstrInitialDir = NULL;							// 初始目录为默认
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;// 目录必须存在，覆盖文件前发出警告
	ofn.lpstrTitle = title;								// 使用系统默认标题留空即可
	ofn.lpstrDefExt = defExt;							// 默认追加的扩展名

	if (GetSaveFileName(&ofn))
	{
		path = strFilename;
		return true;
	}
	return false;
}
