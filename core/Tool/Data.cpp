#include "..\e2dtool.h"


void e2d::Data::saveInt(const String& key, int value, const String& field)
{
	::WritePrivateProfileString((LPCWSTR)field, (LPCWSTR)key, (LPCWSTR)String::parse(value), (LPCWSTR)Path::getDataSavePath());
}

void e2d::Data::saveDouble(const String& key, double value, const String& field)
{
	::WritePrivateProfileString((LPCWSTR)field, (LPCWSTR)key, (LPCWSTR)String::parse(value), (LPCWSTR)Path::getDataSavePath());
}

void e2d::Data::saveBool(const String& key, bool value, const String& field)
{
	::WritePrivateProfileString((LPCWSTR)field, (LPCWSTR)key, (value ? L"1" : L"0"), (LPCWSTR)Path::getDataSavePath());
}

void e2d::Data::saveString(const String& key, const String& value, const String& field)
{
	::WritePrivateProfileString((LPCWSTR)field, (LPCWSTR)key, (LPCWSTR)value, (LPCWSTR)Path::getDataSavePath());
}

int e2d::Data::getInt(const String& key, int defaultValue, const String& field)
{
	return ::GetPrivateProfileInt((LPCWSTR)field, (LPCWSTR)key, defaultValue, (LPCWSTR)Path::getDataSavePath());
}

double e2d::Data::getDouble(const String& key, double defaultValue, const String& field)
{
	wchar_t temp[32] = { 0 };
	::GetPrivateProfileString((LPCWSTR)field, (LPCWSTR)key, (LPCWSTR)String::parse(defaultValue), temp, 31, (LPCWSTR)Path::getDataSavePath());
	return std::stof(temp);
}

bool e2d::Data::getBool(const String& key, bool defaultValue, const String& field)
{
	int nDefaultValue = defaultValue ? 1 : 0;
	int nValue = ::GetPrivateProfileInt((LPCWSTR)field, (LPCWSTR)key, nDefaultValue, (LPCWSTR)Path::getDataSavePath());
	return nValue != 0;
}

e2d::String e2d::Data::getString(const String& key, const String& defaultValue, const String& field)
{
	wchar_t temp[256] = { 0 };
	::GetPrivateProfileString((LPCWSTR)field, (LPCWSTR)key, (LPCWSTR)defaultValue, temp, 255, (LPCWSTR)Path::getDataSavePath());
	return temp;
}