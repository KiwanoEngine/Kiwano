#include "..\ecommon.h"
#include <iomanip>
using namespace e2d;


EString::EString()
{
	_size = 0;
	_string = new wchar_t[1];
	_string[0] = 0;
}

e2d::EString::EString(const wchar_t ch)
{
	_size = 1;
	_string = new wchar_t[2];
	_string[0] = ch;
	_string[1] = 0;
}

EString::EString(const wchar_t *str)
{
	if (str)
	{
		_size = wcslen(str);
		_string = new wchar_t[_size + 1];
		wcscpy_s(_string, _size + 1, str);
	}
	else
	{
		_string = nullptr;
		_size = 0;
	}
}

EString::EString(EString && str)
{
	_size = str._size;
	_string = str._string;
	str._string = nullptr;
}

EString::EString(const EString &str)
{
	if (str._size)
	{
		_size = str._size;
		_string = new wchar_t[_size + 1];
		wcscpy_s(_string, _size + 1, str._string);
	}
	else
	{
		_string = nullptr;
		_size = 0;
	}
}

e2d::EString::EString(const std::wstring &str)
{
	if (!str.empty())
	{
		_size = static_cast<int>(str.length());
		_string = new wchar_t[_size + 1];
		wcscpy_s(_string, _size + 1, str.c_str());
	}
	else
	{
		_string = nullptr;
		_size = 0;
	}
}

EString::~EString()
{
	delete[] _string;
}

EString &EString::operator=(const wchar_t *str)
{
	if (_string == str)
		return *this;

	if (str)
	{
		delete[] _string;
		_size = wcslen(str);
		_string = new wchar_t[_size + 1];
		wcscpy_s(_string, _size + 1, str);
	}
	else
	{
		_string = nullptr;
		_size = 0;
	}
	return *this;
}

EString &EString::operator=(const EString &str)
{
	if (_string == str._string)
		return *this;

	if (str._size)
	{
		delete[] _string;
		_size = str._size;
		_string = new wchar_t[_size + 1];
		wcscpy_s(_string, _size + 1, str._string);
	}
	else
	{
		_string = nullptr;
		_size = 0;
	}
	return *this;
}

EString & e2d::EString::operator=(const std::wstring &str)
{
	if (!str.empty())
	{
		delete[] _string;
		_size = static_cast<int>(str.length());
		_string = new wchar_t[_size + 1];
		wcscpy_s(_string, _size + 1, str.c_str());
	}
	else
	{
		_string = nullptr;
		_size = 0;
	}
	return *this;
}

bool EString::operator==(const wchar_t *str)
{
	return (wcscmp(str, _string) == 0);
}

bool EString::operator ==(const EString &str)
{
	return (wcscmp(str._string, _string) == 0);
}

bool e2d::EString::operator==(const std::wstring &str)
{
	return (str.compare(_string) == 0);
}

bool e2d::EString::operator!=(const wchar_t *str)
{
	return (wcscmp(str, _string) != 0);
}

bool e2d::EString::operator!=(const EString &str)
{
	return (wcscmp(str._string, _string) != 0);
}

bool e2d::EString::operator!=(const std::wstring &str)
{
	return (str.compare(_string) != 0);
}

wchar_t &EString::operator[](int index)
{
	ASSERT(index >= 0 && index < _size, "EString subscript out of range");
	return _string[index];
}

EString EString::operator+(const wchar_t *str)
{
	EString str_temp(*this);

	str_temp += str;
	return std::move(str_temp);
}

EString EString::operator+(const wchar_t x)
{
	EString str_temp(*this);

	str_temp += x;
	return std::move(str_temp);
}

EString EString::operator+(const EString &str)
{
	EString str_temp(*this);

	str_temp += str;
	return std::move(str_temp);
}

EString e2d::EString::operator+(const std::wstring &str)
{
	EString str_temp(*this);

	str_temp += str;
	return std::move(str_temp);
}

EString &EString::operator+=(const wchar_t x)
{
	wchar_t *str_temp = new wchar_t[_size + 2];
	if (_string) wcscpy_s(str_temp, _size + 2, _string);
	str_temp[_size] = x;
	str_temp[_size + 1] = 0;

	delete[] _string;
	_string = str_temp;
	_size++;
	return *this;
}

EString &EString::operator+=(const wchar_t *str)
{
	if (!str) return *this;

	int d_size = wcslen(str);
	if (d_size == 0) return *this;

	wchar_t *str_temp = new wchar_t[_size + d_size + 1];
	if (_string) wcscpy_s(str_temp, _size + d_size + 1, _string);
	wcscpy_s(str_temp + _size, d_size + 1, str);

	delete[] _string;
	_string = str_temp;
	_size += d_size;
	return *this;
}

EString &EString::operator+=(const EString &str)
{
	if (str._size == 0) return *this;

	wchar_t *str_temp = new wchar_t[_size + str._size + 1];
	if (_string) wcscpy_s(str_temp, _size + str._size + 1, _string);
	wcscpy_s(str_temp + _size, str._size + 1, str._string);

	delete[] _string;
	_string = str_temp;
	_size += str._size;
	return *this;
}

EString & e2d::EString::operator+=(const std::wstring &str)
{
	if (str.length() == 0) return *this;

	wchar_t *str_temp = new wchar_t[_size + str.length() + 1];
	if (_string) wcscpy_s(str_temp, _size + str.length() + 1, _string);
	wcscpy_s(str_temp + _size, str.length() + 1, str.c_str());

	delete[] _string;
	_string = str_temp;
	_size += static_cast<int>(str.length());
	return *this;
}

bool e2d::EString::operator<(EString const &str) const
{
	for (int i = 0; i <= _size; i++)
		if (_string[i] != str._string[i])
			return _string[i] < str[i];
	return false;
}

bool e2d::EString::operator<=(EString const &str) const
{
	for (int i = 0; i <= _size; i++)
		if (_string[i] != str._string[i])
			return _string[i] < str[i];
	return true;
}

bool e2d::EString::operator>(EString const &str) const
{
	for (int i = 0; i <= _size; i++)
		if (_string[i] != str._string[i])
			return _string[i] > str[i];
	return false;
}

bool e2d::EString::operator>=(EString const &str) const
{
	for (int i = 0; i <= _size; i++)
		if (_string[i] != str._string[i])
			return _string[i] > str[i];
	return true;
}

unsigned int e2d::EString::hash() const
{
	unsigned int hash = 0;

	for (int i = 0; i < _size; i++)
	{
		hash *= 16777619;
		hash ^= (unsigned int)towupper(_string[i]);
	}
	return (hash);
}

EString e2d::operator+(const wchar_t ch, const EString &str)
{
	return std::move((EString(ch) + str));
}

EString e2d::operator+(const wchar_t *str1, const EString &str2)
{
	return std::move((EString(str1) + str2));
}

EString e2d::operator+(const EString &str1, const EString &str2)
{
	return std::move((EString(str1) + str2));
}

EString e2d::operator+(const std::wstring &str1, const EString &str2)
{
	return std::move((EString(str1) + str2));
}

std::wistream & e2d::operator>>(std::wistream &cin, EString &str)
{
	const int limit_string_size = 4096;

	str._string = new wchar_t[limit_string_size];

	cin >> std::setw(limit_string_size) >> str._string;
	str._size = wcslen(str._string);
	return cin;
}


EString e2d::EString::upper() const
{
	EString str(*this);

	for (int i = 0; i < str._size; i++)
		if (str._string[i] >= L'a' && str._string[i] <= L'z')
			str._string[i] -= (L'a' - L'A');

	return std::move(str);
}

EString e2d::EString::lower() const
{
	EString str(*this);

	for (int i = 0; i < str._size; i++)
		str._string[i] = towlower(str._string[i]);

	return std::move(str);
}

EString e2d::EString::sub(int offset, int count) const
{
	if (_size == 0 || offset >= _size)
		return std::move(EString());

	offset = offset >= 0 ? offset : 0;

	if (count < 0 || (offset + count) > _size)
		count = _size - offset;

	EString str_temp;
	str_temp._string = new wchar_t[count + 1];

	for (int i = 0; i < count; i++)
		str_temp._string[i] = (_string + offset)[i];
	str_temp._string[count] = 0;

	return std::move(str_temp);
}

int e2d::EString::findFirstOf(const wchar_t ch) const
{
	for (int i = 0; i < _size; i++)
		if (_string[i] == ch)
			return i;

	return -1;
}

int e2d::EString::findLastOf(const wchar_t ch) const
{
	int index = -1;

	for (int i = 0; i < _size; i++)
		if (_string[i] == ch)
			index = i;

	return index;
}

EString & e2d::EString::append(const wchar_t ch)
{
	return (*this) += ch;
}

EString & e2d::EString::append(const wchar_t * str)
{
	return (*this) += str;
}

EString & e2d::EString::append(const EString & str)
{
	return (*this) += str;
}
