#include "..\etools.h"


void e2d::Data::saveInt(const String & key, int value)
{
	::WritePrivateProfileString(L"Default", key, String::toString(value), File::getDefaultSavePath());
}

void e2d::Data::saveFloat(const String & key, float value)
{
	::WritePrivateProfileString(L"Default", key, String::toString(value), File::getDefaultSavePath());
}

void e2d::Data::saveString(const String & key, const String & value)
{
	::WritePrivateProfileString(L"Default", key, value, File::getDefaultSavePath());
}

int e2d::Data::getInt(const String & key, int default)
{
	return ::GetPrivateProfileInt(L"Default", key, default, File::getDefaultSavePath());
}

float e2d::Data::getFloat(const String & key, float default)
{
	wchar_t temp[32] = { 0 };
	::GetPrivateProfileString(L"Default", key, String::toString(default), temp, 31, File::getDefaultSavePath());
	return std::stof(temp);
}

e2d::String e2d::Data::getString(const String & key, const String & default)
{
	wchar_t temp[256] = { 0 };
	::GetPrivateProfileString(L"Default", key, default, temp, 255, File::getDefaultSavePath());
	return temp;
}
