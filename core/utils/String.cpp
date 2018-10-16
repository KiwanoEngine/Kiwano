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

#include "..\e2dutil.h"
#include <iomanip>
#include <cwctype>
#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")


easy2d::String::String()
	: string_(L"")
{
}

easy2d::String::String(const wchar_t *str)
	: string_(str)
{
}

easy2d::String::String(const char *cstr)
	: string_(static_cast<wchar_t*>(_bstr_t(cstr)))
{
}

easy2d::String::String(easy2d::String && str)
{
	string_ = std::move(str.string_);
}

easy2d::String::String(const easy2d::String &str)
	: string_(str.string_)
{
}

easy2d::String::~String()
{
	string_.clear();
}

easy2d::String &easy2d::String::operator=(const wchar_t *str)
{
	string_ = str;
	return (*this);
}

easy2d::String & easy2d::String::operator=(const char *cstr)
{
	string_ = static_cast<wchar_t*>(_bstr_t(cstr));
	return (*this);
}

easy2d::String easy2d::String::Parse(int value)
{
	String tmp;
	tmp.string_ = std::to_wstring(value);
	return std::move(tmp);
}

easy2d::String easy2d::String::Parse(unsigned int value)
{
	String tmp;
	tmp.string_ = std::to_wstring(value);
	return std::move(tmp);
}

easy2d::String easy2d::String::Parse(float value)
{
	String tmp;
	tmp.string_ = std::to_wstring(value);
	return std::move(tmp);
}

easy2d::String easy2d::String::Parse(double value)
{
	String tmp;
	tmp.string_ = std::to_wstring(value);
	return std::move(tmp);
}

easy2d::String easy2d::String::Format(const char * format, ...)
{
	std::string tmp;

	va_list marker;
	va_start(marker, format);

	size_t nu_of_chars = _vscprintf(format, marker);

	if (nu_of_chars > tmp.capacity())
	{
		tmp.resize(nu_of_chars + 1);
	}

	vsprintf_s(const_cast<LPSTR>(tmp.data()), tmp.capacity(), format, marker);

	va_end(marker);

	String str = tmp.c_str();
	return std::move(str);
}

easy2d::String easy2d::String::Format(const wchar_t * format, ...)
{
	std::wstring tmp;

	va_list marker;
	va_start(marker, format);

	size_t nu_of_chars = _vscwprintf(format, marker);

	if (nu_of_chars > tmp.capacity())
	{
		tmp.resize(nu_of_chars + 1);
	}

	vswprintf_s(const_cast<LPWSTR>(tmp.data()), tmp.capacity(), format, marker);

	va_end(marker);

	String str = tmp.c_str();
	return std::move(str);
}

easy2d::String & easy2d::String::operator=(const String &str)
{
	string_ = str.string_;
	return (*this);
}

bool easy2d::String::operator==(const wchar_t *str) const
{
	if (str)
	{
		return (string_.compare(str) == 0);
	}
	else
	{
		return false;
	}
}

bool easy2d::String::operator==(const char *str) const
{
	if (str)
	{
		String temp(str);
		return (string_ == temp.string_);
	}
	else
	{
		return false;
	}
}

bool easy2d::String::operator ==(const easy2d::String &str) const
{
	return string_ == str.string_;
}

bool easy2d::String::operator!=(const wchar_t *str) const
{
	if (str)
	{
		return (string_.compare(str) != 0);
	}
	else
	{
		return true;
	}
}

bool easy2d::String::operator!=(const char *str) const
{
	if (str)
	{
		String temp(str);
		return (string_ != temp.string_);
	}
	else
	{
		return true;
	}
}

bool easy2d::String::operator!=(const easy2d::String &str) const
{
	return string_ != str.string_;
}

wchar_t &easy2d::String::operator[](size_t index)
{
	return string_[index];
}

easy2d::String easy2d::String::operator+(const wchar_t *str) const
{
	String temp;
	temp.string_ = string_ + str;
	return std::move(temp);
}

easy2d::String easy2d::String::operator+(const char *str) const
{
	String temp;
	temp.string_ = string_ + static_cast<wchar_t*>(_bstr_t(str));
	return std::move(temp);
}

easy2d::String easy2d::String::operator+(const easy2d::String &str) const
{
	String temp;
	temp.string_ = string_ + str.string_;
	return std::move(temp);
}

easy2d::String easy2d::operator+(const wchar_t *str1, const easy2d::String &str2)
{
	String temp;
	temp.string_ = str1 + str2.string_;
	return std::move(temp);
}

easy2d::String easy2d::operator+(const char *str1, const String &str2)
{
	String temp;
	temp.string_ = static_cast<wchar_t*>(_bstr_t(str1)) + str2.string_;
	return std::move(temp);
}

easy2d::String & easy2d::String::operator+=(const wchar_t *str)
{
	string_ += str;
	return (*this);
}

easy2d::String & easy2d::String::operator+=(const char *str)
{
	string_ += static_cast<wchar_t*>(_bstr_t(str));
	return (*this);
}

easy2d::String & easy2d::String::operator+=(const String &str)
{
	string_ += str.string_;
	return (*this);
}

bool easy2d::String::operator>(const String &str) const
{
	return string_ > str.string_;
}

bool easy2d::String::operator>=(const String &str) const
{
	return string_ >= str.string_;
}

bool easy2d::String::operator<(const String &str) const
{
	return string_ < str.string_;
}

bool easy2d::String::operator<=(const String &str) const
{
	return string_ <= str.string_;
}

easy2d::String & easy2d::String::operator<<(const String &str)
{
	string_ += str.string_;
	return (*this);
}

easy2d::String & easy2d::String::operator<<(const wchar_t *str)
{
	string_ += str;
	return (*this);
}

easy2d::String & easy2d::String::operator<<(wchar_t *str)
{
	string_ += str;
	return (*this);
}

easy2d::String & easy2d::String::operator<<(const char * cstr)
{
	string_ += static_cast<wchar_t*>(_bstr_t(cstr));
	return (*this);
}

easy2d::String & easy2d::String::operator<<(char * cstr)
{
	string_ += static_cast<wchar_t*>(_bstr_t(cstr));
	return (*this);
}

easy2d::String & easy2d::String::operator<<(int value)
{
	(*this) += String::Parse(value);
	return (*this);
}

easy2d::String & easy2d::String::operator<<(unsigned int value)
{
	(*this) += String::Parse(value);
	return (*this);
}

easy2d::String & easy2d::String::operator<<(float value)
{
	(*this) += String::Parse(value);
	return (*this);
}

easy2d::String & easy2d::String::operator<<(double value)
{
	(*this) += String::Parse(value);
	return (*this);
}

easy2d::String::operator const wchar_t*() const
{
	return string_.c_str();
}

easy2d::String::operator wchar_t*() const
{
	return const_cast<wchar_t*>(string_.c_str());
}

easy2d::String::operator std::wstring() const
{
	return string_;
}

easy2d::String::operator std::string() const
{
	std::string str = static_cast<const char *>(_bstr_t(string_.c_str()));
	return std::move(str);
}

bool easy2d::String::IsEmpty() const
{
	return string_.empty();
}

int easy2d::String::Length() const
{
	return static_cast<int>(string_.size());
}

size_t easy2d::String::GetHash() const
{
	std::hash<std::wstring> hash;
	return hash(string_);
}

const wchar_t& easy2d::String::At(size_t index) const
{
	return string_.at(index);
}

int easy2d::String::Compare(const String & str) const
{
	return string_.compare(str.string_);
}

easy2d::String easy2d::String::ToUpper() const
{
	String str(*this);
	std::transform(str.string_.begin(), str.string_.end(), str.string_.begin(), std::towupper);
	return std::move(str);
}

easy2d::String easy2d::String::ToLower() const
{
	easy2d::String str(*this);
	std::transform(str.string_.begin(), str.string_.end(), str.string_.begin(), std::towlower);
	return std::move(str);
}

int easy2d::String::ToInt() const
{
	if (string_.empty())
	{
		return 0;
	}
	return std::stoi(string_, 0, 10);
}

float easy2d::String::ToFloat() const
{
	if (string_.empty())
	{
		return 0.f;
	}
	return std::stof(string_, 0);
}

double easy2d::String::ToDouble() const
{
	if (string_.empty())
	{
		return 0.0;
	}
	return std::stod(string_, 0);
}

bool easy2d::String::ToBool() const
{
	if (string_.empty())
	{
		return false;
	}

	if (string_.compare(L"0") == 0 || string_.compare(L"false") == 0)
	{
		return false;
	}
	return true;
}

easy2d::String easy2d::String::Subtract(int offset, int count) const
{
	String tmp;
	int length = static_cast<int>(string_.size());

	if (length == 0 || offset >= length)
		return std::move(tmp);

	offset = offset >= 0 ? offset : 0;

	if (count < 0 || (offset + count) > length)
		count = length - offset;

	tmp.string_ = string_.substr(offset, count);
	return std::move(tmp);
}

void easy2d::String::Insert(const String & str, int pos)
{
	string_.insert(size_t(pos), str.string_);
}

void easy2d::String::Replace(const String & from, const String & to)
{
	if (from.string_.empty())
		return;

	size_t start_pos = 0;
	while ((start_pos = string_.find((const wchar_t *)from, start_pos)) != std::wstring::npos) 
	{
		string_.replace(start_pos, from.string_.length(), (const wchar_t *)to);
		start_pos += to.string_.length();
	}
}

void easy2d::String::Erase(int offset, int count)
{
	string_.erase(size_t(offset), size_t(count));
}

int easy2d::String::Find(const String & str, int offset) const
{
	size_t index;
	if ((index = string_.find(str.string_, size_t(offset))) == std::wstring::npos)
	{
		return -1;
	}
	else
	{
		return static_cast<int>(index);
	}
}

void easy2d::String::Clear()
{
	string_.clear();
}

std::wostream & easy2d::operator<<(std::wostream &cout, const String &str)
{
	cout << str.string_;
	return cout;
}

std::wistream & easy2d::operator>>(std::wistream &cin, String &str)
{
	cin >> str.string_;
	return cin;
}

std::ostream & easy2d::operator<<(std::ostream &cout, const String &str)
{
	std::string cstr = static_cast<char*>(_bstr_t(str.string_.c_str()));
	cout << cstr;
	return cout;
}

std::istream & easy2d::operator>>(std::istream &cin, String &str)
{
	std::string temp;
	cin >> temp;
	str.string_ = static_cast<wchar_t*>(_bstr_t(temp.c_str()));
	return cin;
}