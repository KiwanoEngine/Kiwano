#include "..\e2dtool.h"


e2d::Data::Data(const String & key, const String & field)
	: key_(key)
	, field_(field)
	, data_path_(Path::GetDataPath())
{
}

bool e2d::Data::Exists() const
{
	wchar_t temp[256] = { 0 };
	::GetPrivateProfileStringW(
		(LPCWSTR)field_,
		(LPCWSTR)key_,
		L"",
		temp,
		255,
		(LPCWSTR)data_path_
	);
	return temp[0] == L'\0';
}

bool e2d::Data::SaveInt(int value)
{
	BOOL ret = ::WritePrivateProfileStringW(
		(LPCWSTR)field_,
		(LPCWSTR)key_,
		(LPCWSTR)String::Parse(value),
		(LPCWSTR)data_path_
	);
	return ret != 0;
}

bool e2d::Data::SaveFloat(float value)
{
	BOOL ret = ::WritePrivateProfileStringW(
		(LPCWSTR)field_,
		(LPCWSTR)key_,
		(LPCWSTR)String::Parse(value),
		(LPCWSTR)data_path_
	);
	return ret != 0;
}

bool e2d::Data::SaveDouble(double value)
{
	BOOL ret = ::WritePrivateProfileStringW(
		(LPCWSTR)field_,
		(LPCWSTR)key_,
		(LPCWSTR)String::Parse(value),
		(LPCWSTR)data_path_
	);
	return ret != 0;
}

bool e2d::Data::SaveBool(bool value)
{
	BOOL ret = ::WritePrivateProfileStringW(
		(LPCWSTR)field_,
		(LPCWSTR)key_,
		(value ? L"1" : L"0"),
		(LPCWSTR)data_path_
	);
	return ret != 0;
}

bool e2d::Data::SaveString(const String& value)
{
	BOOL ret = ::WritePrivateProfileStringW(
		(LPCWSTR)field_,
		(LPCWSTR)key_,
		(LPCWSTR)value,
		(LPCWSTR)data_path_
	);
	return ret != 0;
}

int e2d::Data::GetInt() const
{
	return ::GetPrivateProfileIntW(
		(LPCWSTR)field_,
		(LPCWSTR)key_,
		0,
		(LPCWSTR)data_path_
	);
}

float e2d::Data::GetFloat() const
{
	wchar_t temp[32] = { 0 };
	::GetPrivateProfileStringW((LPCWSTR)field_, (LPCWSTR)key_, L"0.0", temp, 31, (LPCWSTR)data_path_);
	return std::stof(temp);
}

double e2d::Data::GetDouble() const
{
	wchar_t temp[32] = { 0 };
	::GetPrivateProfileStringW((LPCWSTR)field_, (LPCWSTR)key_, L"0.0", temp, 31, (LPCWSTR)data_path_);
	return std::stod(temp);
}

bool e2d::Data::GetBool() const
{
	int nValue = ::GetPrivateProfileIntW(
		(LPCWSTR)field_,
		(LPCWSTR)key_,
		0,
		(LPCWSTR)data_path_);
	return nValue != 0;
}

e2d::String e2d::Data::GetString()
{
	wchar_t temp[256] = { 0 };
	::GetPrivateProfileStringW(
		(LPCWSTR)field_,
		(LPCWSTR)key_,
		L"",
		temp,
		255,
		(LPCWSTR)data_path_
	);
	return temp;
}