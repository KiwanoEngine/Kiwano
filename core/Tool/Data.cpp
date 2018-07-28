#include "..\e2dtool.h"


e2d::Data::Data(const String & key, const String & field)
	: _key(key)
	, _field(field)
	, _dataPath(Path::getDataPath())
{
}

void e2d::Data::saveInt(int value)
{
	::WritePrivateProfileString(
		(LPCWSTR)_field,
		(LPCWSTR)_key,
		(LPCWSTR)String::parse(value),
		(LPCWSTR)_dataPath
	);
}

void e2d::Data::saveDouble(float value)
{
	::WritePrivateProfileString(
		(LPCWSTR)_field,
		(LPCWSTR)_key,
		(LPCWSTR)String::parse(value),
		(LPCWSTR)_dataPath
	);
}

void e2d::Data::saveBool(bool value)
{
	::WritePrivateProfileString(
		(LPCWSTR)_field,
		(LPCWSTR)_key,
		(value ? L"1" : L"0"),
		(LPCWSTR)_dataPath
	);
}

void e2d::Data::saveString(const String& value)
{
	::WritePrivateProfileString(
		(LPCWSTR)_field,
		(LPCWSTR)_key,
		(LPCWSTR)value,
		(LPCWSTR)_dataPath
	);
}

int e2d::Data::getInt(int defaultValue)
{
	return ::GetPrivateProfileInt(
		(LPCWSTR)_field,
		(LPCWSTR)_key,
		defaultValue,
		(LPCWSTR)_dataPath
	);
}

float e2d::Data::getDouble(float defaultValue)
{
	wchar_t temp[32] = { 0 };
	::GetPrivateProfileString((LPCWSTR)_field, (LPCWSTR)_key, (LPCWSTR)String::parse(defaultValue), temp, 31, (LPCWSTR)_dataPath);
	return std::stof(temp);
}

bool e2d::Data::getBool(bool defaultValue)
{
	int nValue = ::GetPrivateProfileInt(
		(LPCWSTR)_field,
		(LPCWSTR)_key,
		defaultValue ? 1 : 0,
		(LPCWSTR)_dataPath);
	return nValue != 0;
}

e2d::String e2d::Data::getString(const String& defaultValue)
{
	wchar_t temp[256] = { 0 };
	::GetPrivateProfileString(
		(LPCWSTR)_field,
		(LPCWSTR)_key,
		(LPCWSTR)defaultValue,
		temp,
		255,
		(LPCWSTR)_dataPath
	);
	return temp;
}