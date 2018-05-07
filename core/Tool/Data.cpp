#include "..\e2dtool.h"

static e2d::String s_sDataFileName = L"DefaultData.ini";

void e2d::Data::saveInt(const String& key, int value, const String& field)
{
	::WritePrivateProfileString(field, key, String::parse(value), Data::getDataFilePath());
}

void e2d::Data::saveDouble(const String& key, double value, const String& field)
{
	::WritePrivateProfileString(field, key, String::parse(value), Data::getDataFilePath());
}

void e2d::Data::saveBool(const String& key, bool value, const String& field)
{
	const wchar_t* sValue = value ? L"1" : L"0";
	::WritePrivateProfileString(field, key, sValue, Data::getDataFilePath());
}

void e2d::Data::saveString(const String& key, const String& value, const String& field)
{
	::WritePrivateProfileString(field, key, value, Data::getDataFilePath());
}

int e2d::Data::getInt(const String& key, int defaultValue, const String& field)
{
	return ::GetPrivateProfileInt(field, key, defaultValue, Data::getDataFilePath());
}

double e2d::Data::getDouble(const String& key, double defaultValue, const String& field)
{
	wchar_t temp[32] = { 0 };
	::GetPrivateProfileString(field, key, String::parse(defaultValue), temp, 31, Data::getDataFilePath());
	return std::stof(temp);
}

bool e2d::Data::getBool(const String& key, bool defaultValue, const String& field)
{
	int nDefaultValue = defaultValue ? 1 : 0;
	int nValue = ::GetPrivateProfileInt(field, key, nDefaultValue, Data::getDataFilePath());
	return nValue != 0;
}

e2d::String e2d::Data::getString(const String& key, const String& defaultValue, const String& field)
{
	wchar_t temp[256] = { 0 };
	::GetPrivateProfileString(field, key, defaultValue, temp, 255, Data::getDataFilePath());
	return temp;
}

void e2d::Data::setDataFileName(const String& fileName)
{
	if (!fileName.isEmpty())
	{
		s_sDataFileName.clear();
		s_sDataFileName << fileName << L".ini";
	}
}

e2d::String e2d::Data::getDataFilePath()
{
	return Path::getDefaultSavePath() + s_sDataFileName;
}
