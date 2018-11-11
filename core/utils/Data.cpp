// Copyright (c) 2016-2018 Easy2D - Nomango
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

#include "Data.h"
#include "Path.h"

namespace easy2d
{
	Data::Data(const String & key, const String & field)
		: key_(key)
		, field_(field)
		, data_path_(Path::GetDataPath())
	{
	}

	bool Data::Exists() const
	{
		wchar_t temp[256] = { 0 };
		::GetPrivateProfileStringW(
			field_.c_str(),
			key_.c_str(),
			L"",
			temp,
			255,
			data_path_.c_str()
		);
		return temp[0] == L'\0';
	}

	bool Data::SaveInt(int val)
	{
		BOOL ret = ::WritePrivateProfileStringW(
			field_.c_str(),
			key_.c_str(),
			std::to_wstring(val).c_str(),
			data_path_.c_str()
		);
		return ret == TRUE;
	}

	bool Data::SaveFloat(float val)
	{
		BOOL ret = ::WritePrivateProfileStringW(
			field_.c_str(),
			key_.c_str(),
			std::to_wstring(val).c_str(),
			data_path_.c_str()
		);
		return ret == TRUE;
	}

	bool Data::SaveDouble(double val)
	{
		BOOL ret = ::WritePrivateProfileStringW(
			field_.c_str(),
			key_.c_str(),
			std::to_wstring(val).c_str(),
			data_path_.c_str()
		);
		return ret == TRUE;
	}

	bool Data::SaveBool(bool val)
	{
		BOOL ret = ::WritePrivateProfileStringW(
			field_.c_str(),
			key_.c_str(),
			(val ? L"1" : L"0"),
			data_path_.c_str()
		);
		return ret == TRUE;
	}

	bool Data::SaveString(const String& val)
	{
		BOOL ret = ::WritePrivateProfileStringW(
			field_.c_str(),
			key_.c_str(),
			val.c_str(),
			data_path_.c_str()
		);
		return ret == TRUE;
	}

	int Data::GetInt() const
	{
		return ::GetPrivateProfileIntW(
			field_.c_str(),
			key_.c_str(),
			0,
			data_path_.c_str()
		);
	}

	float Data::GetFloat() const
	{
		wchar_t temp[32] = { 0 };
		::GetPrivateProfileStringW(field_.c_str(), key_.c_str(), L"0.0", temp, 31, data_path_.c_str());
		return std::stof(temp);
	}

	double Data::GetDouble() const
	{
		wchar_t temp[32] = { 0 };
		::GetPrivateProfileStringW(field_.c_str(), key_.c_str(), L"0.0", temp, 31, data_path_.c_str());
		return std::stod(temp);
	}

	bool Data::GetBool() const
	{
		int nValue = ::GetPrivateProfileIntW(
			field_.c_str(),
			key_.c_str(),
			0,
			data_path_.c_str());
		return nValue == TRUE;
	}

	String Data::GetString()
	{
		wchar_t temp[256] = { 0 };
		::GetPrivateProfileStringW(
			field_.c_str(),
			key_.c_str(),
			L"",
			temp,
			255,
			data_path_.c_str()
		);
		return temp;
	}
}