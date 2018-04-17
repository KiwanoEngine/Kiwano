#include "..\ecommon.h"
#include <iomanip>
#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")


e2d::String::String()
	: m_str(L"")
{
}

e2d::String::String(const wchar_t *str)
	: m_str(str)
{
}

e2d::String::String(const char *cstr)
	: m_str(static_cast<wchar_t*>(_bstr_t(cstr)))
{
}

e2d::String::String(e2d::String && str)
{
	m_str = std::move(str.m_str);
}

e2d::String::String(const e2d::String &str)
	: m_str(str.m_str)
{
}

e2d::String::~String()
{
	m_str.clear();
}

e2d::String &e2d::String::operator=(const wchar_t *str)
{
	m_str = str;
	return (*this);
}

e2d::String & e2d::String::operator=(const char *cstr)
{
	m_str = static_cast<wchar_t*>(_bstr_t(cstr));
	return (*this);
}

e2d::String e2d::String::parse(int value)
{
	String tmp;
#if HIGHER_THAN_VS2010
	tmp.m_str = std::to_wstring(value);
#else
	tmp.m_str = std::to_wstring(static_cast<long long>(value));
#endif
	return std::move(tmp);
}

e2d::String e2d::String::parse(unsigned int value)
{
	String tmp;
#if HIGHER_THAN_VS2010
	tmp.m_str = std::to_wstring(value);
#else
	tmp.m_str = std::to_wstring(static_cast<long long>(value));
#endif
	return std::move(tmp);
}

e2d::String e2d::String::parse(float value)
{
	String tmp;
#if HIGHER_THAN_VS2010
	tmp.m_str = std::to_wstring(value);
#else
	tmp.m_str = std::to_wstring(static_cast<long double>(value));
#endif
	return std::move(tmp);
}

e2d::String e2d::String::parse(double value)
{
	String tmp;
#if HIGHER_THAN_VS2010
	tmp.m_str = std::to_wstring(value);
#else
	tmp.m_str = std::to_wstring(static_cast<long double>(value));
#endif
	return std::move(tmp);
}

e2d::String e2d::String::format(const char * format, ...)
{
	std::string tmp;

	va_list marker = NULL;
	va_start(marker, format);

	size_t num_of_chars = _vscprintf(format, marker);

	if (num_of_chars > tmp.capacity()) 
	{
		tmp.resize(num_of_chars + 1);
	}

	vsprintf_s(const_cast<LPSTR>(tmp.data()), tmp.capacity(), format, marker);

	va_end(marker);

	String str = tmp.c_str();
	return std::move(str);
}

e2d::String e2d::String::format(const wchar_t * format, ...)
{
	std::wstring tmp;

	va_list marker = NULL;
	va_start(marker, format);

	size_t num_of_chars = _vscwprintf(format, marker);

	if (num_of_chars > tmp.capacity())
	{
		tmp.resize(num_of_chars + 1);
	}

	vswprintf_s(const_cast<LPWSTR>(tmp.data()), tmp.capacity(), format, marker);

	va_end(marker);

	String str = tmp.c_str();
	return std::move(str);
}

e2d::String & e2d::String::operator=(const String &str)
{
	m_str = str.m_str;
	return (*this);
}

bool e2d::String::operator==(const wchar_t *str)
{
	if (str)
	{
		return (m_str.compare(str) == 0);
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
		return (m_str == temp.m_str);
	}
	else
	{
		return false;
	}
}

bool e2d::String::operator ==(const e2d::String &str)
{
	return m_str == str.m_str;
}

bool e2d::String::operator!=(const wchar_t *str)
{
	if (str)
	{
		return (m_str.compare(str) != 0);
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
		return (m_str != temp.m_str);
	}
	else
	{
		return true;
	}
}

bool e2d::String::operator!=(const e2d::String &str)
{
	return m_str != str.m_str;
}

wchar_t &e2d::String::operator[](int index)
{
	return m_str[static_cast<size_t>(index)];
}

e2d::String e2d::String::operator+(const wchar_t *str)
{
	String temp;
	temp.m_str = m_str + str;
	return std::move(temp);
}

e2d::String e2d::String::operator+(const char *str)
{
	String temp;
	temp.m_str = m_str + static_cast<wchar_t*>(_bstr_t(str));
	return std::move(temp);
}

e2d::String e2d::String::operator+(const e2d::String &str)
{
	String temp;
	temp.m_str = m_str + str.m_str;
	return std::move(temp);
}

e2d::String e2d::operator+(const wchar_t *str1, const e2d::String &str2)
{
	String temp;
	temp.m_str = str1 + str2.m_str;
	return std::move(temp);
}

e2d::String e2d::operator+(const char *str1, const String &str2)
{
	String temp;
	temp.m_str = static_cast<wchar_t*>(_bstr_t(str1)) + str2.m_str;
	return std::move(temp);
}

e2d::String & e2d::String::operator+=(const wchar_t *str)
{
	m_str += str;
	return (*this);
}

e2d::String & e2d::String::operator+=(const char *str)
{
	m_str += static_cast<wchar_t*>(_bstr_t(str));
	return (*this);
}

e2d::String & e2d::String::operator+=(const String &str)
{
	m_str += str.m_str;
	return (*this);
}

bool e2d::String::operator>(const String &str) const
{
	return m_str > str.m_str;
}

bool e2d::String::operator>=(const String &str) const
{
	return m_str >= str.m_str;
}

bool e2d::String::operator<(const String &str) const
{
	return m_str < str.m_str;
}

bool e2d::String::operator<=(const String &str) const
{
	return m_str <= str.m_str;
}

e2d::String & e2d::String::operator<<(const String &str)
{
	m_str += str.m_str;
	return (*this);
}

e2d::String & e2d::String::operator<<(const wchar_t *str)
{
	m_str += str;
	return (*this);
}

e2d::String & e2d::String::operator<<(wchar_t *str)
{
	m_str += str;
	return (*this);
}

e2d::String & e2d::String::operator<<(const char * cstr)
{
	m_str += static_cast<wchar_t*>(_bstr_t(cstr));
	return (*this);
}

e2d::String & e2d::String::operator<<(char * cstr)
{
	m_str += static_cast<wchar_t*>(_bstr_t(cstr));
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
	return m_str.c_str();
}

e2d::String::operator wchar_t*() const
{
	return const_cast<wchar_t*>(m_str.c_str());
}

bool e2d::String::isEmpty() const
{
	return m_str.empty();
}

int e2d::String::getLength() const
{
	return static_cast<int>(m_str.size());
}

unsigned int e2d::String::getHashCode() const
{
	unsigned int hash = 0;

	for (int i = 0; i < getLength(); i++)
	{
		hash *= 16777619;
		hash ^= (unsigned int)towupper(m_str[i]);
	}
	return hash;
}

std::wstring e2d::String::getWString() const
{
	return m_str;
}

std::string e2d::String::getCString() const
{
	std::string str = static_cast<const char *>(_bstr_t(m_str.c_str()));
	return std::move(str);
}

e2d::String e2d::String::toUpper() const
{
	String str(*this);

	for (int i = 0; i < getLength(); i++)
		str[i] = towupper(str[i]);

	return std::move(str);
}

e2d::String e2d::String::toLower() const
{
	e2d::String str(*this);

	for (int i = 0; i < getLength(); i++)
		str[i] = towlower(str[i]);

	return std::move(str);
}

int e2d::String::toInt() const
{
	if (getLength() == 0)
	{
		return 0;
	}
	return _wtoi(m_str.c_str());
}

double e2d::String::toDouble() const
{
	if (getLength() == 0)
	{
		return 0.0;
	}
	return _wtof(m_str.c_str());
}

bool e2d::String::toBool() const
{
	if (getLength() == 0)
	{
		return false;
	}

	if (m_str.compare(L"0") == 0 || m_str.compare(L"false") == 0)
	{
		return false;
	}
	return true;
}

e2d::String e2d::String::subtract(int offset, int count) const
{
	String temp;
	int length = getLength();

	if (length == 0 || offset >= length)
		return std::move(temp);

	offset = offset >= 0 ? offset : 0;

	if (count < 0 || (offset + count) > length)
		count = length - offset;

	temp.m_str = m_str.substr(offset, count);
	return std::move(temp);
}

int e2d::String::findFirstOf(const wchar_t ch) const
{
	for (int i = 0; i < getLength(); i++)
		if (m_str[i] == ch)
			return i;

	return -1;
}

int e2d::String::findLastOf(const wchar_t ch) const
{
	int index = -1;

	for (int i = 0; i < getLength(); i++)
		if (m_str[i] == ch)
			index = i;

	return index;
}

void e2d::String::clear()
{
	m_str.clear();
}

std::wostream & e2d::operator<<(std::wostream &cout, const String &str)
{
	cout << str.m_str;
	return cout;
}

std::wistream & e2d::operator>>(std::wistream &cin, String &str)
{
	cin >> str.m_str;
	return cin;
}

std::ostream & e2d::operator<<(std::ostream &cout, const String &str)
{
	std::string cstr = static_cast<char*>(_bstr_t(str.m_str.c_str()));
	cout << cstr;
	return cout;
}

std::istream & e2d::operator>>(std::istream &cin, String &str)
{
	std::string temp;
	cin >> temp;
	str.m_str = static_cast<wchar_t*>(_bstr_t(temp.c_str()));
	return cin;
}