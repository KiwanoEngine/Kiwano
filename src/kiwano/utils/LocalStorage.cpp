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

#include <kiwano/utils/LocalStorage.h>

namespace kiwano
{

LocalStorage::LocalStorage(const String& file_path, const String& field)
{
    SetFilePath(file_path);
    SetFieldName(field);
}

bool LocalStorage::Exists(const String& key) const
{
    char temp[256] = { 0 };
    ::GetPrivateProfileStringA(field_name_.c_str(), key.c_str(), "", temp, 255, file_path_.c_str());
    return temp[0] == '\0';
}

bool LocalStorage::SaveInt(const String& key, int val) const
{
    BOOL ret =
        ::WritePrivateProfileStringA(field_name_.c_str(), key.c_str(), std::to_string(val).c_str(), file_path_.c_str());
    return ret == TRUE;
}

bool LocalStorage::SaveFloat(const String& key, float val) const
{
    BOOL ret =
        ::WritePrivateProfileStringA(field_name_.c_str(), key.c_str(), std::to_string(val).c_str(), file_path_.c_str());
    return ret == TRUE;
}

bool LocalStorage::SaveDouble(const String& key, double val) const
{
    BOOL ret =
        ::WritePrivateProfileStringA(field_name_.c_str(), key.c_str(), std::to_string(val).c_str(), file_path_.c_str());
    return ret == TRUE;
}

bool LocalStorage::SaveBool(const String& key, bool val) const
{
    BOOL ret = ::WritePrivateProfileStringA(field_name_.c_str(), key.c_str(), (val ? "1" : "0"), file_path_.c_str());
    return ret == TRUE;
}

bool LocalStorage::SaveString(const String& key, const String& val) const
{
    BOOL ret = ::WritePrivateProfileStringA(field_name_.c_str(), key.c_str(), val.c_str(), file_path_.c_str());
    return ret == TRUE;
}

int LocalStorage::GetInt(const String& key, int default_value) const
{
    return ::GetPrivateProfileIntA(field_name_.c_str(), key.c_str(), default_value, file_path_.c_str());
}

float LocalStorage::GetFloat(const String& key, float default_value) const
{
    char   temp[32]    = { 0 };
    String default_str = std::to_string(default_value);
    ::GetPrivateProfileStringA(field_name_.c_str(), key.c_str(), default_str.c_str(), temp, 31, file_path_.c_str());
    return std::stof(temp);
}

double LocalStorage::GetDouble(const String& key, double default_value) const
{
    char   temp[32]    = { 0 };
    String default_str = std::to_string(default_value);
    ::GetPrivateProfileStringA(field_name_.c_str(), key.c_str(), default_str.c_str(), temp, 31, file_path_.c_str());
    return std::stod(temp);
}

bool LocalStorage::GetBool(const String& key, bool default_value) const
{
    int nValue = ::GetPrivateProfileIntA(field_name_.c_str(), key.c_str(), default_value ? 1 : 0, file_path_.c_str());
    return nValue == TRUE;
}

String LocalStorage::GetString(const String& key, const String& default_value) const
{
    char temp[256] = { 0 };
    ::GetPrivateProfileStringA(field_name_.c_str(), key.c_str(), default_value.c_str(), temp, 255, file_path_.c_str());
    return temp;
}

}  // namespace kiwano
