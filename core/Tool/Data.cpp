#include "..\etools.h"

static e2d::String s_sDefaultFileName = L"DefaultData.ini";

void e2d::Data::saveInt(String& key, int value, String field)
{
	::WritePrivateProfileString(field, key, String::toString(value), Data::getDataFilePath());
}

void e2d::Data::saveDouble(String& key, double value, String field)
{
	::WritePrivateProfileString(field, key, String::toString(value), Data::getDataFilePath());
}

void e2d::Data::saveBool(String& key, bool value, String field)
{
	const wchar_t* sValue = value ? L"1" : L"0";
	::WritePrivateProfileString(field, key, sValue, Data::getDataFilePath());
}

void e2d::Data::saveString(String& key, String& value, String field)
{
	::WritePrivateProfileString(field, key, value, Data::getDataFilePath());
}

int e2d::Data::getInt(String& key, int defaultValue, String field)
{
	return ::GetPrivateProfileInt(field, key, defaultValue, Data::getDataFilePath());
}

double e2d::Data::getDouble(String& key, double defaultValue, String field)
{
	wchar_t temp[32] = { 0 };
	::GetPrivateProfileString(field, key, String::toString(defaultValue), temp, 31, Data::getDataFilePath());
	return std::stof(temp);
}

bool e2d::Data::getBool(String& key, bool defaultValue, String field)
{
	int nDefaultValue = defaultValue ? 1 : 0;
	int nValue = ::GetPrivateProfileInt(field, key, nDefaultValue, Data::getDataFilePath());
	return nValue != 0;
}

e2d::String e2d::Data::getString(String& key, String& defaultValue, String field)
{
	wchar_t temp[256] = { 0 };
	::GetPrivateProfileString(field, key, defaultValue, temp, 255, Data::getDataFilePath());
	return temp;
}

void e2d::Data::setDataFileName(String& fileName)
{
	if (!fileName.isEmpty())
	{
		s_sDefaultFileName.clear();
		s_sDefaultFileName << fileName << L".ini";
	}
}

e2d::String e2d::Data::getDataFilePath()
{
	return Path::getDefaultSavePath() + s_sDefaultFileName;
}
