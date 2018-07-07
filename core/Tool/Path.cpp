#include "..\e2dtool.h"
#include <algorithm>


extern "C" const GUID DECLSPEC_SELECTANY FOLDERID_LocalAppData = { 
	0xF1B32785, 0x6FBA, 0x4FCF, { 0x9D, 0x55, 0x7B, 0x8E, 0x7F, 0x15, 0x70, 0x91 }
};


e2d::String e2d::Path::getDataPath()
{
	static String dataPath;
	if (dataPath.isEmpty())
	{
		// 设置数据的保存路径
		String localAppDataPath = Path::getLocalAppDataPath();
		String gameName = Game::getInstance()->getConfig()->getGameName();
		if (!localAppDataPath.isEmpty() && !gameName.isEmpty())
		{
			dataPath = localAppDataPath + L"\\Easy2DGameData\\" << gameName << L"\\";

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
		String gameName = Game::getInstance()->getConfig()->getGameName();

		if (0 != ::GetTempPath(_MAX_PATH, path) && !gameName.isEmpty())
		{
			tempPath << path << L"\\Easy2DGameTemp\\" << gameName << L"\\";

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
		typedef HRESULT(WINAPI* pFunSHGetKnownFolderPath)(const GUID& rfid, DWORD dwFlags, HANDLE hToken, PWSTR *ppszPath);

		PWSTR pszPath = nullptr;
		HMODULE hModule = LoadLibrary(L"shell32.dll");
		pFunSHGetKnownFolderPath SHGetKnownFolderPath = (pFunSHGetKnownFolderPath)GetProcAddress(hModule, "SHGetKnownFolderPath");
		HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &pszPath);

		if (SUCCEEDED(hr))
		{
			localAppDataPath = pszPath;
			CoTaskMemFree(pszPath);
		}
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
