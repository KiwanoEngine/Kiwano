#include "..\e2dtool.h"

extern "C" const GUID DECLSPEC_SELECTANY FOLDERID_LocalAppData = { 
	0xF1B32785, 0x6FBA, 0x4FCF, { 0x9D, 0x55, 0x7B, 0x8E, 0x7F, 0x15, 0x70, 0x91 }
};


const e2d::String& e2d::Path::dataPath()
{
	static String dataPath;
	if (dataPath.empty())
	{
		// 设置数据的保存路径
		String localAppDataPath = Path::localAppDataPath();
		String title = Game::instance()->window()->title();
		String folderName = String::parse(title.hash());

		if (!localAppDataPath.empty())
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

const e2d::String& e2d::Path::tempPath()
{
	static String tempPath;
	if (tempPath.empty())
	{
		// 设置临时文件保存路径
		wchar_t path[_MAX_PATH];
		String title = Game::instance()->window()->title();
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

const e2d::String& e2d::Path::localAppDataPath()
{
	static String localAppDataPath;
	if (localAppDataPath.empty())
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

const e2d::String& e2d::Path::currentFilePath()
{
	static String currFilePath;
	if (currFilePath.empty())
	{
		TCHAR szPath[_MAX_PATH] = { 0 };
		if (::GetModuleFileName(nullptr, szPath, _MAX_PATH) != 0)
		{
			currFilePath = szPath;
		}
	}
	return currFilePath;
}
