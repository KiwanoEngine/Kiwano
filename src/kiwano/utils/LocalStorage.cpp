// Copyright (c) 2016-2018 Kiwano - Nomango
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "LocalStorage.h"

namespace kiwano
{
	LocalStorage::LocalStorage(String const & file_path, String const & field)
	{
		SetFilePath(file_path);
		SetFieldName(field);
	}

	void LocalStorage::SetFilePath(String const & file_path)
	{
		file_path_ = file_path;
	}

	void LocalStorage::SetFieldName(String const & field_name)
	{
		field_name_ = field_name;
	}

	bool LocalStorage::Exists(String const& key) const
	{
		wchar_t temp[256] = { 0 };
		::GetPrivateProfileStringW(
			field_name_.c_str(),
			key.c_str(),
			L"",
			temp,
			255,
			file_path_.c_str()
		);
		return temp[0] == L'\0';
	}

	bool LocalStorage::SaveInt(String const& key, int val) const
	{
		BOOL ret = ::WritePrivateProfileStringW(
			field_name_.c_str(),
			key.c_str(),
			std::to_wstring(val).c_str(),
			file_path_.c_str()
		);
		return ret == TRUE;
	}

	bool LocalStorage::SaveFloat(String const& key, float val) const
	{
		BOOL ret = ::WritePrivateProfileStringW(
			field_name_.c_str(),
			key.c_str(),
			std::to_wstring(val).c_str(),
			file_path_.c_str()
		);
		return ret == TRUE;
	}

	bool LocalStorage::SaveDouble(String const& key, double val) const
	{
		BOOL ret = ::WritePrivateProfileStringW(
			field_name_.c_str(),
			key.c_str(),
			std::to_wstring(val).c_str(),
			file_path_.c_str()
		);
		return ret == TRUE;
	}

	bool LocalStorage::SaveBool(String const& key, bool val) const
	{
		BOOL ret = ::WritePrivateProfileStringW(
			field_name_.c_str(),
			key.c_str(),
			(val ? L"1" : L"0"),
			file_path_.c_str()
		);
		return ret == TRUE;
	}

	bool LocalStorage::SaveString(String const& key, String const& val) const
	{
		BOOL ret = ::WritePrivateProfileStringW(
			field_name_.c_str(),
			key.c_str(),
			val.c_str(),
			file_path_.c_str()
		);
		return ret == TRUE;
	}

	int LocalStorage::GetInt(String const & key, int default_value) const
	{
		return ::GetPrivateProfileIntW(
			field_name_.c_str(),
			key.c_str(),
			default_value,
			file_path_.c_str()
		);
	}

	float LocalStorage::GetFloat(String const & key, float default_value) const
	{
		wchar_t temp[32] = { 0 };
		String default_str = String::parse(default_value);
		::GetPrivateProfileStringW(field_name_.c_str(), key.c_str(), default_str.c_str(), temp, 31, file_path_.c_str());
		return std::stof(temp);
	}

	double LocalStorage::GetDouble(String const & key, double default_value) const
	{
		wchar_t temp[32] = { 0 };
		String default_str = String::parse(default_value);
		::GetPrivateProfileStringW(field_name_.c_str(), key.c_str(), default_str.c_str(), temp, 31, file_path_.c_str());
		return std::stod(temp);
	}

	bool LocalStorage::GetBool(String const & key, bool default_value) const
	{
		int nValue = ::GetPrivateProfileIntW(
			field_name_.c_str(),
			key.c_str(),
			default_value ? 1 : 0,
			file_path_.c_str());
		return nValue == TRUE;
	}

	String LocalStorage::GetString(String const & key, String const & default_value) const
	{
		wchar_t temp[256] = { 0 };
		::GetPrivateProfileStringW(
			field_name_.c_str(),
			key.c_str(),
			default_value.c_str(),
			temp,
			255,
			file_path_.c_str()
		);
		return temp;
	}
}
