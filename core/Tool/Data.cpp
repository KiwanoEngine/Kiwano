#include "..\etools.h"


void e2d::Data::saveInt(const String & key, int value)
{
	::WritePrivateProfileString(L"Default", key, String::toString(value), File::getDefaultSavePath());
}

void e2d::Data::saveDouble(const String & key, double value)
{
	::WritePrivateProfileString(L"Default", key, String::toString(value), File::getDefaultSavePath());
}

void e2d::Data::saveBool(const String & key, bool value)
{
	const wchar_t* sValue = value ? L"1" : L"0";
	::WritePrivateProfileString(L"Default", key, sValue, File::getDefaultSavePath());
}

void e2d::Data::saveString(const String & key, const String & value)
{
	::WritePrivateProfileString(L"Default", key, value, File::getDefaultSavePath());
}

int e2d::Data::getInt(const String & key, int defaultValue)
{
	return ::GetPrivateProfileInt(L"Default", key, defaultValue, File::getDefaultSavePath());
}

double e2d::Data::getDouble(const String & key, double defaultValue)
{
	wchar_t temp[32] = { 0 };
	::GetPrivateProfileString(L"Default", key, String::toString(defaultValue), temp, 31, File::getDefaultSavePath());
	return std::stof(temp);
}

bool e2d::Data::getBool(const String & key, bool defaultValue)
{
	int nDefaultValue = defaultValue ? 1 : 0;
	int nValue = ::GetPrivateProfileInt(L"Default", key, nDefaultValue, File::getDefaultSavePath());
	return nValue != 0;
}

e2d::String e2d::Data::getString(const String & key, const String & defaultValue)
{
	wchar_t temp[256] = { 0 };
	::GetPrivateProfileString(L"Default", key, defaultValue, temp, 255, File::getDefaultSavePath());
	return temp;
}
