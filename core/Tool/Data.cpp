#include "..\etools.h"

static e2d::String s_sDefaultFileName = L"DefaultData.ini";

void e2d::Data::saveInt(const String & key, int value, const String & fileName)
{
	::WritePrivateProfileString(L"Default", key, String::toString(value), Data::getDataFilePath(fileName));
}

void e2d::Data::saveDouble(const String & key, double value, const String & fileName)
{
	::WritePrivateProfileString(L"Default", key, String::toString(value), Data::getDataFilePath(fileName));
}

void e2d::Data::saveBool(const String & key, bool value, const String & fileName)
{
	const wchar_t* sValue = value ? L"1" : L"0";
	::WritePrivateProfileString(L"Default", key, sValue, Data::getDataFilePath(fileName));
}

void e2d::Data::saveString(const String & key, const String & value, const String & fileName)
{
	::WritePrivateProfileString(L"Default", key, value, Data::getDataFilePath(fileName));
}

int e2d::Data::getInt(const String & key, int defaultValue, const String & fileName)
{
	return ::GetPrivateProfileInt(L"Default", key, defaultValue, Data::getDataFilePath(fileName));
}

double e2d::Data::getDouble(const String & key, double defaultValue, const String & fileName)
{
	wchar_t temp[32] = { 0 };
	::GetPrivateProfileString(L"Default", key, String::toString(defaultValue), temp, 31, Data::getDataFilePath(fileName));
	return std::stof(temp);
}

bool e2d::Data::getBool(const String & key, bool defaultValue, const String & fileName)
{
	int nDefaultValue = defaultValue ? 1 : 0;
	int nValue = ::GetPrivateProfileInt(L"Default", key, nDefaultValue, Data::getDataFilePath(fileName));
	return nValue != 0;
}

e2d::String e2d::Data::getString(const String & key, const String & defaultValue, const String & fileName)
{
	wchar_t temp[256] = { 0 };
	::GetPrivateProfileString(L"Default", key, defaultValue, temp, 255, Data::getDataFilePath(fileName));
	return temp;
}

void e2d::Data::setDataFilePath(const String & fileName)
{
	if (!fileName.isEmpty())
	{
		s_sDefaultFileName.clear();
		s_sDefaultFileName << fileName << L".ini";
	}
}

e2d::String e2d::Data::getDataFilePath(const String & fileName)
{
	if (fileName.isEmpty())
	{
		return Path::getDefaultSavePath() + s_sDefaultFileName;
	}
	return Path::getDefaultSavePath() + fileName + L".ini";
}
