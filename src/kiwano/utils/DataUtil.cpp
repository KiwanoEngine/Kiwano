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

#include "DataUtil.h"

namespace kiwano
{
	DataUtil::DataUtil(String const & file_path, String const & field)
	{
		SetFilePath(file_path);
		SetFieldName(field);
	}

	void DataUtil::SetFilePath(String const & file_path)
	{
		file_path_ = file_path;
	}

	void DataUtil::SetFieldName(String const & field_name)
	{
		field_name_ = field_name;
	}

	bool DataUtil::Exists(String const& key) const
	{
		WChar temp[256] = { 0 };
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

	bool DataUtil::SaveInt(String const& key, Int32 val) const
	{
		BOOL ret = ::WritePrivateProfileStringW(
			field_name_.c_str(),
			key.c_str(),
			std::to_wstring(val).c_str(),
			file_path_.c_str()
		);
		return ret == TRUE;
	}

	bool DataUtil::SaveFloat(String const& key, Float32 val) const
	{
		BOOL ret = ::WritePrivateProfileStringW(
			field_name_.c_str(),
			key.c_str(),
			std::to_wstring(val).c_str(),
			file_path_.c_str()
		);
		return ret == TRUE;
	}

	bool DataUtil::SaveDouble(String const& key, Float64 val) const
	{
		BOOL ret = ::WritePrivateProfileStringW(
			field_name_.c_str(),
			key.c_str(),
			std::to_wstring(val).c_str(),
			file_path_.c_str()
		);
		return ret == TRUE;
	}

	bool DataUtil::SaveBool(String const& key, bool val) const
	{
		BOOL ret = ::WritePrivateProfileStringW(
			field_name_.c_str(),
			key.c_str(),
			(val ? L"1" : L"0"),
			file_path_.c_str()
		);
		return ret == TRUE;
	}

	bool DataUtil::SaveString(String const& key, String const& val) const
	{
		BOOL ret = ::WritePrivateProfileStringW(
			field_name_.c_str(),
			key.c_str(),
			val.c_str(),
			file_path_.c_str()
		);
		return ret == TRUE;
	}

	Int32 DataUtil::GetInt(String const & key, Int32 default_value) const
	{
		return ::GetPrivateProfileIntW(
			field_name_.c_str(),
			key.c_str(),
			default_value,
			file_path_.c_str()
		);
	}

	Float32 DataUtil::GetFloat(String const & key, Float32 default_value) const
	{
		WChar temp[32] = { 0 };
		String default_str = String::parse(default_value);
		::GetPrivateProfileStringW(field_name_.c_str(), key.c_str(), default_str.c_str(), temp, 31, file_path_.c_str());
		return std::stof(temp);
	}

	Float64 DataUtil::GetDouble(String const & key, Float64 default_value) const
	{
		WChar temp[32] = { 0 };
		String default_str = String::parse(default_value);
		::GetPrivateProfileStringW(field_name_.c_str(), key.c_str(), default_str.c_str(), temp, 31, file_path_.c_str());
		return std::stod(temp);
	}

	bool DataUtil::GetBool(String const & key, bool default_value) const
	{
		Int32 nValue = ::GetPrivateProfileIntW(
			field_name_.c_str(),
			key.c_str(),
			default_value ? 1 : 0,
			file_path_.c_str());
		return nValue == TRUE;
	}

	String DataUtil::GetString(String const & key, String const & default_value) const
	{
		WChar temp[256] = { 0 };
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