#include "..\e2dtool.h"
#include <shlobj.h>

e2d::String e2d::Path::getDataPath()
{
	static String dataPath;
	if (dataPath.isEmpty())
	{
		// 设置数据的保存路径
		String localAppDataPath = Path::getLocalAppDataPath();
		String title = Window::getInstance()->getTitle();
		String folderName = String::parse(title.hash());

		if (!localAppDataPath.isEmpty())
		{
			dataPath = localAppDataPath + L"\\Easy2DGameData\\" << folderName << L"\\";

			File file(dataPath);
			if (!file.exists() && !File::createFolder(dataPath))
			{
				dataPath = L"";
			}
		}
		dataPath << L"Data.ini";
	}
	return dataPath;
}

e2d::String e2d::Path::getTempPath()
{
	static String tempPath;
	if (tempPath.isEmpty())
	{
		// 设置临时文件保存路径
		wchar_t path[_MAX_PATH];
		String title = Window::getInstance()->getTitle();
		String folderName = String::parse(title.hash());

		if (0 != ::GetTempPath(_MAX_PATH, path))
		{
			tempPath << path << L"\\Easy2DGameTemp\\" << folderName << L"\\";

			File file(tempPath);
			if (!file.exists() && !File::createFolder(tempPath))
			{
				tempPath = L"";
			}
		}
	}
	return tempPath;
}

e2d::String e2d::Path::getLocalAppDataPath()
{
	static String localAppDataPath;
	if (localAppDataPath.isEmpty())
	{
		// 获取 AppData/Local 文件夹的路径
		WCHAR strPath[MAX_PATH] = { 0 };
		::SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, strPath);
		localAppDataPath = strPath;
	}

	return localAppDataPath;
}

e2d::String e2d::Path::getCurrentFilePath()
{
	static String currFilePath;
	if (currFilePath.isEmpty())
	{
		TCHAR szPath[_MAX_PATH] = { 0 };
		if (::GetModuleFileName(nullptr, szPath, _MAX_PATH) != 0)
		{
			currFilePath = szPath;
		}
	}
	return currFilePath;
}
