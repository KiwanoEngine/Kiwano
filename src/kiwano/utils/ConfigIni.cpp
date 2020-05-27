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

#include <kiwano/utils/ConfigIni.h>
#include <kiwano/core/Exception.h>
#include <fstream>  // std::ifstream, std::ofstream
#include <algorithm>  // std::sort, std::for_each
#include <cctype>  // std::isspace

#define KGE_DEFAULT_INI_SECTION_NAME "default"

namespace kiwano
{

StringView Trim(StringView str)
{
    if (!str.IsEmpty())
    {
        std::size_t start = 0, end = str.GetLength();
        while (std::isspace(str[start]))
            ++start;
        while (std::isspace(str[end - 1]))
            --end;

        if (end - start)
            return StringView(str.Data() + start, end - start);
    }
    return StringView();
}

class IniParser
{
    StringView line_;
public:
    IniParser(StringView line)
        : line_(line)
    {
    }

    bool ClearComment()
    {
        auto pos = line_.Find(';');
        if (pos != String::npos)
        {
            if (pos == 0)
                return true;

            if (std::isspace(line_[pos - 1]))
            {
                line_ = Trim(line_.SubStr(0, pos - 1));
                return line_.IsEmpty();
            }
        }
        return false;
    }

    bool IsSection() const
    {
        return line_[0] == '[' && line_.GetLength() > 2 && line_[line_.GetLength() - 1] == ']';
    }

    StringView GetSectionName() const
    {
        return Trim(line_.SubStr(1, line_.GetLength() - 2));
    }

    bool GetKeyValue(StringView* key, StringView* value)
    {
        auto pos = line_.Find('=');
        if (pos == String::npos)
            return false;

        *key = Trim(line_.SubStr(0, pos));
        *value = Trim(line_.SubStr(pos + 1));

        return !(*key).IsEmpty() && !(*value).IsEmpty();
    }
};

ConfigIniPtr ConfigIni::Create(const String& file_path)
{
    ConfigIniPtr ptr = memory::New<ConfigIni>();
    if (ptr)
    {
        if (!ptr->Load(file_path))
        {
            return nullptr;
        }
    }
    return ptr;
}

bool ConfigIni::Load(const String& file_path)
{
    std::ifstream ifs(file_path);

    if (ifs.is_open())
    {
        return Load(ifs);
    }
    return false;
}

bool ConfigIni::Load(std::istream& istream)
{
    try
    {
        String section = KGE_DEFAULT_INI_SECTION_NAME;
        for (String line; std::getline(istream, line);)
        {
            ParseLine(line, &section);
        }
        return true;
    }
    catch (Exception)
    {
        return false;
    }
    return false;
}

bool ConfigIni::Save(const String& file_path)
{
    std::ofstream ofs(file_path);

    if (ofs.is_open())
    {
        return Save(ofs);
    }
    return false;
}

bool ConfigIni::Save(std::ostream& os)
{
    // Get all keys
    Vector<String> keys;
    keys.reserve(sections_.size());
    std::for_each(sections_.begin(), sections_.end(), [&](SectionMap::value_type& pair) { keys.push_back(pair.first); });

    // Sort for keys
    std::sort(keys.begin(), keys.end());

    // Output to ini
    for (const auto& key : keys)
    {
        os << '[' << key << ']' << std::endl;
        for (const auto& pair : sections_[key])
        {
            os << pair.first << " = " << pair.second << std::endl;
        }
        os << std::endl;
    }
    return false;
}

ConfigIni::SectionMap ConfigIni::GetSectionMap() const
{
    return sections_;
}

ConfigIni::ValueMap ConfigIni::GetSection(const String& section) const
{
    auto iter = sections_.find(section);
    if (iter != sections_.end())
        return iter->second;
    return ValueMap();
}

String ConfigIni::GetString(const String& section_name, const String& key) const
{
    if (HasSection(section_name))
    {
        const auto& section = sections_.at(section_name);

        auto iter_key = section.find(key);
        if (iter_key != section.end())
            return iter_key->second;
    }
    return String();
}

float ConfigIni::GetFloat(const String& section, const String& key, float default_value) const
{
    String str = GetString(section, key);
    if (str.empty())
        return default_value;

    try
    {
        std::size_t pos   = 0;
        float       value = std::stof(str, &pos);
        if (pos == str.size())
            return value;
    }
    catch (std::invalid_argument)
    {
        return default_value;
    }
    return default_value;
}

double ConfigIni::GetDouble(const String& section, const String& key, double default_value) const
{
    String str = GetString(section, key);
    if (str.empty())
        return default_value;

    try
    {
        std::size_t pos   = 0;
        double      value = std::stod(str, &pos);
        if (pos == str.size())
            return value;
    }
    catch (std::invalid_argument)
    {
        return default_value;
    }
    return default_value;
}

int ConfigIni::GetInt(const String& section, const String& key, int default_value) const
{
    String str = GetString(section, key);
    if (str.empty())
        return default_value;

    try
    {
        std::size_t pos   = 0;
        int         value = std::stoi(str, &pos);
        if (pos == str.size())
            return value;
    }
    catch (std::invalid_argument)
    {
        return default_value;
    }
    return default_value;
}

bool ConfigIni::GetBool(const String& section, const String& key, bool default_value) const
{
    String str = GetString(section, key);
    if (!str.empty())
    {
        if (str == "true")
            return true;
        else if (str == "false")
            return false;
    }
    return default_value;
}

bool ConfigIni::HasSection(const String& section) const
{
    return !!sections_.count(section);
}

bool ConfigIni::HasValue(const String& section, const String& key) const
{
    if (HasSection(section))
    {
        return !!sections_.at(section).count(section);
    }
    return false;
}

void ConfigIni::SetSectionMap(const SectionMap& sections)
{
    sections_ = sections;
}

void ConfigIni::SetSection(const String& section, const ValueMap& values)
{
    sections_.insert(std::make_pair(section, values));
}

void ConfigIni::SetString(const String& section, const String& key, const String& value)
{
    if (HasSection(section))
        sections_[section].insert(std::make_pair(key, value));
    else
        SetSection(section, ValueMap{ { key, value } });
}

void ConfigIni::SetFloat(const String& section, const String& key, float value)
{
    String str = std::to_string(value);
    SetString(section, key, str);
}

void ConfigIni::SetDouble(const String& section, const String& key, double value)
{
    String str = std::to_string(value);
    SetString(section, key, str);
}

void ConfigIni::SetInt(const String& section, const String& key, int value)
{
    String str = std::to_string(value);
    SetString(section, key, str);
}

void ConfigIni::SetBool(const String& section, const String& key, bool value)
{
    SetString(section, key, value ? "true" : "false");
}

void ConfigIni::ParseLine(StringView line, String* section)
{
    line = Trim(line);
    if (line.IsEmpty())
        return;

    IniParser parser(line);
    if (parser.ClearComment())
        return;

    if (parser.IsSection())
    {
        auto name = parser.GetSectionName();
        if (name.IsEmpty())
            throw Exception("Empty section name");
        *section = name;
        return;
    }

    StringView key, value;
    if (!parser.GetKeyValue(&key, &value))
    {
        throw Exception("Parse key-value failed");
    }
    SetString(*section, key, value);
}

}  // namespace kiwano
