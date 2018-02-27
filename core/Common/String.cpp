#include "..\ecommon.h"
#include <iomanip>


e2d::String::String()
	: m_str(L"")
{
}

e2d::String::String(const wchar_t *str)
	: m_str(str)
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

e2d::String::String(const std::wstring &str)
	: m_str(str)
{
}

e2d::String::~String()
{
	m_str.clear();
}

e2d::String &e2d::String::operator=(const wchar_t *str)
{
	m_str = str;
	return *this;
}

e2d::String & e2d::String::operator=(const String &str)
{
	m_str = str.m_str;
	return *this;
}

e2d::String & e2d::String::operator=(const std::wstring &str)
{
	m_str = str;
	return *this;
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

bool e2d::String::operator ==(const e2d::String &str)
{
	return m_str == str.m_str;
}

bool e2d::String::operator==(const std::wstring &str)
{
	return m_str == str;
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

bool e2d::String::operator!=(const e2d::String &str)
{
	return m_str != str.m_str;
}

bool e2d::String::operator!=(const std::wstring &str)
{
	return m_str != str;
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

e2d::String e2d::String::operator+(const wchar_t x)
{
	String temp;
	temp.m_str = m_str + x;
	return std::move(temp);
}

e2d::String e2d::String::operator+(const e2d::String &str)
{
	String temp;
	temp.m_str = m_str + str.m_str;
	return std::move(temp);
}

e2d::String e2d::String::operator+(const std::wstring &str)
{
	String temp;
	temp.m_str = m_str + str;
	return std::move(temp);
}

e2d::String e2d::operator+(const wchar_t ch, const e2d::String &str)
{
	String temp;
	temp.m_str = ch + str.m_str;
	return std::move(temp);
}

e2d::String e2d::operator+(const wchar_t *str1, const e2d::String &str2)
{
	String temp;
	temp.m_str = str1 + str2.m_str;
	return std::move(temp);
}

e2d::String e2d::operator+(const std::wstring &str1, const e2d::String &str2)
{
	String temp;
	temp.m_str = str1 + str2.m_str;
	return std::move(temp);
}

e2d::String &e2d::String::operator+=(const wchar_t x)
{
	m_str += x;
	return *this;
}

e2d::String &e2d::String::operator+=(const wchar_t *str)
{
	m_str += str;
	return *this;
}

e2d::String &e2d::String::operator+=(const e2d::String &str)
{
	m_str += str.m_str;
	return *this;
}

e2d::String & e2d::String::operator+=(const std::wstring &str)
{
	m_str += str;
	return *this;
}

e2d::String::operator const wchar_t*() const
{
	return m_str.c_str();
}

e2d::String::operator bool() const
{
	return getLength() != 0;
}

bool e2d::String::isEmpty() const
{
	return m_str.empty();
}

int e2d::String::getLength() const
{
	return static_cast<int>(m_str.size());
}

unsigned int e2d::String::getHash() const
{
	unsigned int hash = 0;

	for (int i = 0; i < getLength(); i++)
	{
		hash *= 16777619;
		hash ^= (unsigned int)towupper(m_str[i]);
	}
	return hash;
}

std::wistream & e2d::operator>>(std::wistream &cin, e2d::String &str)
{
	cin >> str.m_str;
	return cin;
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

e2d::String & e2d::String::append(const wchar_t ch)
{
	return (*this) += ch;
}

e2d::String & e2d::String::append(const wchar_t * str)
{
	return (*this) += str;
}

e2d::String & e2d::String::append(const e2d::String & str)
{
	return (*this) += str;
}
