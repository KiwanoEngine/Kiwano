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

#pragma once
#include <kiwano/core/Common.h>
#include <kiwano/base/ObjectBase.h>

namespace kiwano
{

KGE_DECLARE_SMART_PTR(ConfigIni);

/// \~chinese
/// @brief ini格式文件
class KGE_API ConfigIni : public ObjectBase
{
public:
    /// \~chinese
    /// @brief 加载 ini 文件
    /// @param file_path 文件路径
    static ConfigIniPtr Create(const String& file_path);

    /// \~chinese
    /// @brief 键值字典
    typedef Map<String, String> ValueMap;

    /// \~chinese
    /// @brief Section字典
    typedef UnorderedMap<String, ValueMap> SectionMap;

    /// \~chinese
    /// @brief 加载 ini 文件
    /// @param file_path 文件路径
    bool Load(const String& file_path);

    /// \~chinese
    /// @brief 加载 ini 文件
    /// @param is 输入流
    bool Load(std::istream& is);

    /// \~chinese
    /// @brief 保存至 ini 文件
    /// @param file_path 文件路径
    bool Save(const String& file_path);

    /// \~chinese
    /// @brief 保存至 ini 文件
    /// @param os 输出流
    bool Save(std::ostream& os);

    /// \~chinese
    /// @brief 获取所有section
    SectionMap GetSectionMap() const;

    /// \~chinese
    /// @brief 获取section
    /// @param section section的名称
    ValueMap GetSection(const String& section) const;

    /// \~chinese
    /// @brief 获取值
    /// @param section section的名称
    /// @param key key的名称
    String GetString(const String& section, const String& key) const;

    /// \~chinese
    /// @brief 获取值
    /// @param section section的名称
    /// @param key key的名称
    /// @param default_value 不存在时的默认值
    float GetFloat(const String& section, const String& key, float default_value = 0.0f) const;

    /// \~chinese
    /// @brief 获取值
    /// @param section section的名称
    /// @param key key的名称
    /// @param default_value 不存在时的默认值
    double GetDouble(const String& section, const String& key, double default_value = 0.0) const;

    /// \~chinese
    /// @brief 获取值
    /// @param section section的名称
    /// @param key key的名称
    /// @param default_value 不存在时的默认值
    int GetInt(const String& section, const String& key, int default_value = 0) const;

    /// \~chinese
    /// @brief 获取值
    /// @param section section的名称
    /// @param key key的名称
    /// @param default_value 不存在时的默认值
    bool GetBool(const String& section, const String& key, bool default_value = false) const;

    /// \~chinese
    /// @brief 是否存在section
    /// @param section section的名称
    bool HasSection(const String& section) const;

    /// \~chinese
    /// @brief 是否存在值
    /// @param section section的名称
    /// @param key key的名称
    bool HasValue(const String& section, const String& key) const;

    /// \~chinese
    /// @brief 设置所有section
    /// @param sections section字典
    void SetSectionMap(const SectionMap& sections);

    /// \~chinese
    /// @brief 设置section
    /// @param section section的名称
    /// @param values 键值字典
    void SetSection(const String& section, const ValueMap& values);

    /// \~chinese
    /// @brief 设置值
    /// @param section section的名称
    /// @param key key的名称
    /// @param value 值
    void SetString(const String& section, const String& key, const String& value);

    /// \~chinese
    /// @brief 设置值
    /// @param section section的名称
    /// @param key key的名称
    /// @param value 值
    void SetFloat(const String& section, const String& key, float value);

    /// \~chinese
    /// @brief 设置值
    /// @param section section的名称
    /// @param key key的名称
    /// @param value 值
    void SetDouble(const String& section, const String& key, double value);

    /// \~chinese
    /// @brief 设置值
    /// @param section section的名称
    /// @param key key的名称
    /// @param value 值
    void SetInt(const String& section, const String& key, int value);

    /// \~chinese
    /// @brief 设置值
    /// @param section section的名称
    /// @param key key的名称
    /// @param value 值
    void SetBool(const String& section, const String& key, bool value);

    ValueMap& operator[](const String& section);

    const ValueMap& operator[](const String& section) const;

private:
    void ParseLine(StringView line, String* section);

private:
    SectionMap sections_;
};

}  // namespace kiwano
