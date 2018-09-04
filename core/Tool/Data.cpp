#include "..\e2dtool.h"


e2d::Data::Data(const String & key, const String & field)
	: key_(key)
	, field_(field)
	, data_path_(Path::GetDataPath())
{
}

void e2d::Data::SaveInt(int value)
{
	::WritePrivateProfileString(
		(LPCWSTR)field_,
		(LPCWSTR)key_,
		(LPCWSTR)String::Parse(value),
		(LPCWSTR)data_path_
	);
}

void e2d::Data::SaveDouble(float value)
{
	::WritePrivateProfileString(
		(LPCWSTR)field_,
		(LPCWSTR)key_,
		(LPCWSTR)String::Parse(value),
		(LPCWSTR)data_path_
	);
}

void e2d::Data::SaveBool(bool value)
{
	::WritePrivateProfileString(
		(LPCWSTR)field_,
		(LPCWSTR)key_,
		(value ? L"1" : L"0"),
		(LPCWSTR)data_path_
	);
}

void e2d::Data::SaveString(const String& value)
{
	::WritePrivateProfileString(
		(LPCWSTR)field_,
		(LPCWSTR)key_,
		(LPCWSTR)value,
		(LPCWSTR)data_path_
	);
}

int e2d::Data::GetInt(int default_value)
{
	return ::GetPrivateProfileInt(
		(LPCWSTR)field_,
		(LPCWSTR)key_,
		default_value,
		(LPCWSTR)data_path_
	);
}

float e2d::Data::GetDouble(float default_value)
{
	wchar_t temp[32] = { 0 };
	::GetPrivateProfileString((LPCWSTR)field_, (LPCWSTR)key_, (LPCWSTR)String::Parse(default_value), temp, 31, (LPCWSTR)data_path_);
	return std::stof(temp);
}

bool e2d::Data::GetBool(bool default_value)
{
	int nValue = ::GetPrivateProfileInt(
		(LPCWSTR)field_,
		(LPCWSTR)key_,
		default_value ? 1 : 0,
		(LPCWSTR)data_path_);
	return nValue != 0;
}

e2d::String e2d::Data::GetString(const String& default_value)
{
	wchar_t temp[256] = { 0 };
	::GetPrivateProfileString(
		(LPCWSTR)field_,
		(LPCWSTR)key_,
		(LPCWSTR)default_value,
		temp,
		255,
		(LPCWSTR)data_path_
	);
	return temp;
}