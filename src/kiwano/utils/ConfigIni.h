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
    /// @brief 键值字典
    typedef UnorderedMap<String, String> ValueMap;

    /// \~chinese
    /// @brief Section字典
    typedef UnorderedMap<String, ValueMap> SectionMap;

    ConfigIni();

    /// \~chinese
    /// @brief 加载 ini 文件
    /// @param file_path 文件路径
    ConfigIni(StringView file_path);

    /// \~chinese
    /// @brief 加载 ini 文件
    /// @param file_path 文件路径
    bool Load(StringView file_path);

    /// \~chinese
    /// @brief 加载 ini 文件
    /// @param is 输入流
    bool Load(std::istream& is);

    /// \~chinese
    /// @brief 加载 ini 格式字符串
    /// @param content 输入内容
    bool LoadFromString(StringView content);

    /// \~chinese
    /// @brief 保存至 ini 文件
    /// @param file_path 文件路径
    bool Save(StringView file_path);

    /// \~chinese
    /// @brief 保存至 ini 文件
    /// @param os 输出流
    bool Save(std::ostream& os);

    /// \~chinese
    /// @brief 保存至字符串
    /// @param content 输出内容
    bool SaveToString(String& content);

    /// \~chinese
    /// @brief 是否存在section
    /// @param section section的名称
    bool HasSection(StringView section) const;

    /// \~chinese
    /// @brief 是否存在值
    /// @param section section的名称
    /// @param key key的名称
    bool HasKey(StringView section, StringView key) const;

    /// \~chinese
    /// @brief 获取所有section
    SectionMap GetSectionMap() const;

    /// \~chinese
    /// @brief 获取section
    /// @param section section的名称
    ValueMap GetSection(StringView section) const;

    /// \~chinese
    /// @brief 获取值
    /// @param section section的名称
    /// @param key key的名称
    String GetString(StringView section, StringView key, StringView default_value = String()) const;

    /// \~chinese
    /// @brief 获取值
    /// @param section section的名称
    /// @param key key的名称
    /// @param default_value 不存在时的默认值
    float GetFloat(StringView section, StringView key, float default_value = 0.0f) const;

    /// \~chinese
    /// @brief 获取值
    /// @param section section的名称
    /// @param key key的名称
    /// @param default_value 不存在时的默认值
    double GetDouble(StringView section, StringView key, double default_value = 0.0) const;

    /// \~chinese
    /// @brief 获取值
    /// @param section section的名称
    /// @param key key的名称
    /// @param default_value 不存在时的默认值
    int GetInt(StringView section, StringView key, int default_value = 0) const;

    /// \~chinese
    /// @brief 获取值
    /// @param section section的名称
    /// @param key key的名称
    /// @param default_value 不存在时的默认值
    bool GetBool(StringView section, StringView key, bool default_value = false) const;

    /// \~chinese
    /// @brief 设置所有section
    /// @param sections section字典
    void SetSectionMap(const SectionMap& sections);

    /// \~chinese
    /// @brief 设置section
    /// @param section section的名称
    /// @param values 键值字典
    void SetSection(StringView section, const ValueMap& values);

    /// \~chinese
    /// @brief 设置值
    /// @param section section的名称
    /// @param key key的名称
    /// @param value 值
    void SetString(StringView section, StringView key, StringView value);

    /// \~chinese
    /// @brief 设置值
    /// @param section section的名称
    /// @param key key的名称
    /// @param value 值
    void SetFloat(StringView section, StringView key, float value);

    /// \~chinese
    /// @brief 设置值
    /// @param section section的名称
    /// @param key key的名称
    /// @param value 值
    void SetDouble(StringView section, StringView key, double value);

    /// \~chinese
    /// @brief 设置值
    /// @param section section的名称
    /// @param key key的名称
    /// @param value 值
    void SetInt(StringView section, StringView key, int value);

    /// \~chinese
    /// @brief 设置值
    /// @param section section的名称
    /// @param key key的名称
    /// @param value 值
    void SetBool(StringView section, StringView key, bool value);

    /// \~chinese
    /// @brief 删除section
    /// @param section section的名称
    void DeleteSection(StringView section);

    /// \~chinese
    /// @brief 删除值
    /// @param section section的名称
    /// @param key key的名称
    void DeleteKey(StringView section, StringView key);

    ValueMap& operator[](StringView section);

    const ValueMap& operator[](StringView section) const;

private:
    void ParseLine(StringView line, String* section);

private:
    SectionMap sections_;
};

}  // namespace kiwano
