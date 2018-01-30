#include "..\etools.h"


void e2d::EData::saveInt(const EString & key, int value)
{
	::WritePrivateProfileString(L"Default", key, EString::parse(value), EFile::getDefaultSavePath());
}

void e2d::EData::saveFloat(const EString & key, float value)
{
	::WritePrivateProfileString(L"Default", key, EString::parse(value), EFile::getDefaultSavePath());
}

void e2d::EData::saveString(const EString & key, const EString & value)
{
	::WritePrivateProfileString(L"Default", key, value, EFile::getDefaultSavePath());
}

int e2d::EData::getInt(const EString & key, int default)
{
	return ::GetPrivateProfileInt(L"Default", key, default, EFile::getDefaultSavePath());
}

float e2d::EData::getFloat(const EString & key, float default)
{
	wchar_t temp[32] = { 0 };
	::GetPrivateProfileString(L"Default", key, EString::parse(default), temp, 31, EFile::getDefaultSavePath());
	return std::stof(temp);
}

e2d::EString e2d::EData::getString(const EString & key, const EString & default)
{
	wchar_t temp[256] = { 0 };
	::GetPrivateProfileString(L"Default", key, default, temp, 255, EFile::getDefaultSavePath());
	return temp;
}
