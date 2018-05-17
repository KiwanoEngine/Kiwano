#include "..\e2dcommon.h"
#include <iomanip>
#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")


e2d::String::String()
	: _str(L"")
{
}

e2d::String::String(const wchar_t *str)
	: _str(str)
{
}

e2d::String::String(const char *cstr)
	: _str(static_cast<wchar_t*>(_bstr_t(cstr)))
{
}

e2d::String::String(e2d::String && str)
{
	_str = std::move(str._str);
}

e2d::String::String(const e2d::String &str)
	: _str(str._str)
{
}

e2d::String::~String()
{
	_str.clear();
}

e2d::String &e2d::String::operator=(const wchar_t *str)
{
	_str = str;
	return (*this);
}

e2d::String & e2d::String::operator=(const char *cstr)
{
	_str = static_cast<wchar_t*>(_bstr_t(cstr));
	return (*this);
}

e2d::String e2d::String::parse(int value)
{
	String tmp;
	tmp._str = std::to_wstring(value);
	return std::move(tmp);
}

e2d::String e2d::String::parse(unsigned int value)
{
	String tmp;
	tmp._str = std::to_wstring(value);
	return std::move(tmp);
}

e2d::String e2d::String::parse(float value)
{
	String tmp;
	tmp._str = std::to_wstring(value);
	return std::move(tmp);
}

e2d::String e2d::String::parse(double value)
{
	String tmp;
	tmp._str = std::to_wstring(value);
	return std::move(tmp);
}

e2d::String e2d::String::format(const char * format, ...)
{
	std::string tmp;

	va_list marker = nullptr;
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

e2d::String e2d::String::format(const wchar_t * format, ...)
{
	std::wstring tmp;

	va_list marker = nullptr;
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

void e2d::String::swap(String & str1, String & str2)
{
	str1._str.swap(str2._str);
}

e2d::String & e2d::String::operator=(const String &str)
{
	_str = str._str;
	return (*this);
}

bool e2d::String::operator==(const wchar_t *str)
{
	if (str)
	{
		return (_str.compare(str) == 0);
	}
	else
	{
		return false;
	}
}

bool e2d::String::operator==(const char *str)
{
	if (str)
	{
		String temp(str);
		return (_str == temp._str);
	}
	else
	{
		return false;
	}
}

bool e2d::String::operator ==(const e2d::String &str)
{
	return _str == str._str;
}

bool e2d::String::operator!=(const wchar_t *str)
{
	if (str)
	{
		return (_str.compare(str) != 0);
	}
	else
	{
		return true;
	}
}

bool e2d::String::operator!=(const char *str)
{
	if (str)
	{
		String temp(str);
		return (_str != temp._str);
	}
	else
	{
		return true;
	}
}

bool e2d::String::operator!=(const e2d::String &str)
{
	return _str != str._str;
}

wchar_t &e2d::String::operator[](int index)
{
	return _str[size_t(index)];
}

e2d::String e2d::String::operator+(const wchar_t *str)
{
	String temp;
	temp._str = _str + str;
	return std::move(temp);
}

e2d::String e2d::String::operator+(const char *str)
{
	String temp;
	temp._str = _str + static_cast<wchar_t*>(_bstr_t(str));
	return std::move(temp);
}

e2d::String e2d::String::operator+(const e2d::String &str)
{
	String temp;
	temp._str = _str + str._str;
	return std::move(temp);
}

e2d::String e2d::operator+(const wchar_t *str1, const e2d::String &str2)
{
	String temp;
	temp._str = str1 + str2._str;
	return std::move(temp);
}

e2d::String e2d::operator+(const char *str1, const String &str2)
{
	String temp;
	temp._str = static_cast<wchar_t*>(_bstr_t(str1)) + str2._str;
	return std::move(temp);
}

e2d::String & e2d::String::operator+=(const wchar_t *str)
{
	_str += str;
	return (*this);
}

e2d::String & e2d::String::operator+=(const char *str)
{
	_str += static_cast<wchar_t*>(_bstr_t(str));
	return (*this);
}

e2d::String & e2d::String::operator+=(const String &str)
{
	_str += str._str;
	return (*this);
}

bool e2d::String::operator>(const String &str) const
{
	return _str > str._str;
}

bool e2d::String::operator>=(const String &str) const
{
	return _str >= str._str;
}

bool e2d::String::operator<(const String &str) const
{
	return _str < str._str;
}

bool e2d::String::operator<=(const String &str) const
{
	return _str <= str._str;
}

e2d::String & e2d::String::operator<<(const String &str)
{
	_str += str._str;
	return (*this);
}

e2d::String & e2d::String::operator<<(const wchar_t *str)
{
	_str += str;
	return (*this);
}

e2d::String & e2d::String::operator<<(wchar_t *str)
{
	_str += str;
	return (*this);
}

e2d::String & e2d::String::operator<<(const char * cstr)
{
	_str += static_cast<wchar_t*>(_bstr_t(cstr));
	return (*this);
}

e2d::String & e2d::String::operator<<(char * cstr)
{
	_str += static_cast<wchar_t*>(_bstr_t(cstr));
	return (*this);
}

e2d::String & e2d::String::operator<<(int value)
{
	(*this) += String::parse(value);
	return (*this);
}

e2d::String & e2d::String::operator<<(unsigned int value)
{
	(*this) += String::parse(value);
	return (*this);
}

e2d::String & e2d::String::operator<<(float value)
{
	(*this) += String::parse(value);
	return (*this);
}

e2d::String & e2d::String::operator<<(double value)
{
	(*this) += String::parse(value);
	return (*this);
}

e2d::String::operator const wchar_t*() const
{
	return _str.c_str();
}

e2d::String::operator wchar_t*() const
{
	return const_cast<wchar_t*>(_str.c_str());
}

bool e2d::String::isEmpty() const
{
	return _str.empty();
}

int e2d::String::getLength() const
{
	return static_cast<int>(_str.size());
}

unsigned int e2d::String::getHashCode() const
{
	unsigned int hash = 0;

	for (size_t i = 0, length = _str.size(); i < length; ++i)
	{
		hash *= 16777619;
		hash ^= (unsigned int)towupper(_str[i]);
	}
	return hash;
}

std::wstring e2d::String::getWString() const
{
	return _str;
}

std::string e2d::String::getCString() const
{
	std::string str = static_cast<const char *>(_bstr_t(_str.c_str()));
	return std::move(str);
}

int e2d::String::compare(const String & str) const
{
	return _str.compare(str._str);
}

e2d::String e2d::String::toUpper() const
{
	String str(*this);

	for (size_t i = 0, length = _str.size(); i < length; ++i)
		str[i] = towupper(str[i]);

	return std::move(str);
}

e2d::String e2d::String::toLower() const
{
	e2d::String str(*this);

	for (size_t i = 0, length = _str.size(); i < length; ++i)
		str[i] = towlower(str[i]);

	return std::move(str);
}

int e2d::String::toInt() const
{
	if (_str.empty())
	{
		return 0;
	}
	return std::stoi(_str, 0, 10);
}

double e2d::String::toDouble() const
{
	if (_str.empty())
	{
		return 0.0;
	}
	return std::stod(_str, 0);
}

bool e2d::String::toBool() const
{
	if (_str.empty())
	{
		return false;
	}

	if (_str.compare(L"0") == 0 || _str.compare(L"false") == 0)
	{
		return false;
	}
	return true;
}

e2d::String e2d::String::subtract(int offset, int count) const
{
	String tmp;
	int length = static_cast<int>(_str.size());

	if (length == 0 || offset >= length)
		return std::move(tmp);

	offset = offset >= 0 ? offset : 0;

	if (count < 0 || (offset + count) > length)
		count = length - offset;

	tmp._str = _str.substr(offset, count);
	return std::move(tmp);
}

void e2d::String::insert(const String & str, int pos)
{
	_str.insert(size_t(pos), str._str);
}

void e2d::String::replace(const String & from, const String & to)
{
	if (from._str.empty())
		return;

	size_t start_pos = 0;
	while ((start_pos = _str.find(from, start_pos)) != std::string::npos) 
	{
		_str.replace(start_pos, from._str.length(), to);
		start_pos += to._str.length();
	}
}

void e2d::String::erase(int offset, int count)
{
	_str.erase(size_t(offset), size_t(count));
}

int e2d::String::find(const String & str, int offset) const
{
	size_t index;
	if ((index = _str.find(str._str, size_t(offset))) == std::wstring::npos)
	{
		return -1;
	}
	else
	{
		return static_cast<int>(index);
	}
}

void e2d::String::clear()
{
	_str.clear();
}

std::wostream & e2d::operator<<(std::wostream &cout, const String &str)
{
	cout << str._str;
	return cout;
}

std::wistream & e2d::operator>>(std::wistream &cin, String &str)
{
	cin >> str._str;
	return cin;
}

std::ostream & e2d::operator<<(std::ostream &cout, const String &str)
{
	std::string cstr = static_cast<char*>(_bstr_t(str._str.c_str()));
	cout << cstr;
	return cout;
}

std::istream & e2d::operator>>(std::istream &cin, String &str)
{
	std::string temp;
	cin >> temp;
	str._str = static_cast<wchar_t*>(_bstr_t(temp.c_str()));
	return cin;
}