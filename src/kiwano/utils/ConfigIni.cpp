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
#include <fstream>    // std::ifstream, std::ofstream
#include <algorithm>  // std::sort, std::for_each
#include <cctype>     // std::isspace

#define KGE_DEFAULT_INI_SECTION_NAME "default"

namespace kiwano
{

StringView Trim(StringView str)
{
    if (!str.empty())
    {
        std::size_t start = 0, end = str.size();
        while (start < end && std::isspace(str[start]))
            ++start;
        while (end > 0 && std ::isspace(str[end - 1]))
            --end;

        if (end > start)
            return StringView(str.data() + start, end - start);
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
        auto pos = line_.find(';');
        if (pos != String::npos)
        {
            if (pos == 0)
                return true;

            if (std::isspace(line_[pos - 1]))
            {
                line_ = Trim(line_.substr(0, pos - 1));
                return line_.empty();
            }
        }
        return false;
    }

    bool IsSection() const
    {
        return line_[0] == '[' && line_.size() > 2 && line_[line_.size() - 1] == ']';
    }

    StringView GetSectionName() const
    {
        return Trim(line_.substr(1, line_.size() - 2));
    }

    bool GetKeyValue(StringView* key, StringView* value)
    {
        auto pos = line_.find('=');
        if (pos == String::npos)
            return false;

        *key   = Trim(line_.substr(0, pos));
        *value = Trim(line_.substr(pos + 1));

        return !(*key).empty() && !(*value).empty();
    }
};

ConfigIni::ConfigIni() {}

ConfigIni::ConfigIni(StringView file_path)
{
    Load(file_path);
}

bool ConfigIni::Load(StringView file_path)
{
    std::ifstream ifs(file_path);

    if (ifs.is_open())
    {
        return Load(ifs);
    }

    Fail("ConfigIni::Load failed");
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
    }
    catch (RuntimeError& e)
    {
        Fail(String("ConfigIni::Load failed: ") + e.what());
        return false;
    }
    return true;
}

bool ConfigIni::LoadFromString(StringView content)
{
    StringStream ss;
    ss.str(content);
    return Load(ss);
}

bool ConfigIni::Save(StringView file_path)
{
    std::ofstream ofs(file_path);

    if (ofs.is_open())
    {
        return Save(ofs);
    }
    Fail("ConfigIni::Save failed");
    return false;
}

bool ConfigIni::Save(std::ostream& os)
{
    // Get all keys
    Vector<String> keys;
    keys.reserve(sections_.size());
    std::for_each(sections_.begin(), sections_.end(),
                  [&](SectionMap::value_type& pair) { keys.push_back(pair.first); });

    // Sort for keys
    std::sort(keys.begin(), keys.end());

    // Output to ini
    std::ostream::sentry ok(os);
    if (ok)
    {
        for (const auto& key : keys)
        {
            os << '[' << key << ']' << std::endl;
            for (const auto& pair : sections_[key])
            {
                os << pair.first << " = " << pair.second << std::endl;
            }
            os << std::endl;
        }
    }
    if (os.fail())
    {
        Fail("ConfigIni::Save failed");
        return false;
    }
    return true;
}

bool ConfigIni::SaveToString(String& content)
{
    StringStream ss;
    if (Save(ss))
    {
        content = ss.str();
        return true;
    }
    return false;
}

ConfigIni::SectionMap ConfigIni::GetSectionMap() const
{
    return sections_;
}

ConfigIni::ValueMap ConfigIni::GetSection(StringView section) const
{
    if (HasSection(section))
    {
        return sections_.at(section);
    }
    return ValueMap();
}

String ConfigIni::GetString(StringView section, StringView key, StringView default_value) const
{
    if (HasKey(section, key))
    {
        return sections_.at(section).at(key);
    }
    return default_value;
}

float ConfigIni::GetFloat(StringView section, StringView key, float default_value) const
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

double ConfigIni::GetDouble(StringView section, StringView key, double default_value) const
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

int ConfigIni::GetInt(StringView section, StringView key, int default_value) const
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

bool ConfigIni::GetBool(StringView section, StringView key, bool default_value) const
{
    String str = GetString(section, key);
    if (!str.empty())
    {
        if (str == "true" || str == "1")
            return true;
        else if (str == "false" || str == "0")
            return false;
    }
    return default_value;
}

bool ConfigIni::HasSection(StringView section) const
{
    return !!sections_.count(section);
}

bool ConfigIni::HasKey(StringView section, StringView key) const
{
    if (HasSection(section))
    {
        return !!sections_.at(section).count(key);
    }
    return false;
}

void ConfigIni::SetSectionMap(const SectionMap& sections)
{
    sections_ = sections;
}

void ConfigIni::SetSection(StringView section, const ValueMap& values)
{
    sections_[section] = values;
}

void ConfigIni::SetString(StringView section, StringView key, StringView value)
{
    if (HasSection(section))
        sections_[section][key] = value;
    else
        SetSection(section, ValueMap{ { key, value } });
}

void ConfigIni::SetFloat(StringView section, StringView key, float value)
{
    String str = std::to_string(value);
    SetString(section, key, str);
}

void ConfigIni::SetDouble(StringView section, StringView key, double value)
{
    String str = std::to_string(value);
    SetString(section, key, str);
}

void ConfigIni::SetInt(StringView section, StringView key, int value)
{
    String str = std::to_string(value);
    SetString(section, key, str);
}

void ConfigIni::SetBool(StringView section, StringView key, bool value)
{
    SetString(section, key, value ? "true" : "false");
}

void ConfigIni::DeleteSection(StringView section)
{
    if (HasSection(section))
    {
        sections_.erase(section);
    }
}

void ConfigIni::DeleteKey(StringView section, StringView key)
{
    if (HasKey(section, key))
    {
        sections_.at(section).erase(key);
    }
}

ConfigIni::ValueMap& ConfigIni::operator[](StringView section)
{
    return sections_[section];
}

const ConfigIni::ValueMap& ConfigIni::operator[](StringView section) const
{
    return sections_.at(section);
}

void ConfigIni::ParseLine(StringView line, String* section)
{
    line = Trim(line);
    if (line.empty())
        return;

    IniParser parser(line);
    if (parser.ClearComment())
        return;

    if (parser.IsSection())
    {
        auto name = parser.GetSectionName();
        if (name.empty())
            throw RuntimeError("Empty section name");
        *section = name;
        return;
    }

    StringView key, value;
    if (!parser.GetKeyValue(&key, &value))
    {
        throw RuntimeError("Parse key-value failed");
    }
    SetString(*section, key, value);
}

}  // namespace kiwano
