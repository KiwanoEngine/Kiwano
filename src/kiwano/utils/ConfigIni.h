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
/// @brief ini��ʽ�ļ�
class KGE_API ConfigIni : public ObjectBase
{
public:
    /// \~chinese
    /// @brief ��ֵ�ֵ�
    typedef UnorderedMap<String, String> ValueMap;

    /// \~chinese
    /// @brief Section�ֵ�
    typedef UnorderedMap<String, ValueMap> SectionMap;

    ConfigIni();

    /// \~chinese
    /// @brief ���� ini �ļ�
    /// @param file_path �ļ�·��
    ConfigIni(StringView file_path);

    /// \~chinese
    /// @brief ���� ini �ļ�
    /// @param file_path �ļ�·��
    bool Load(StringView file_path);

    /// \~chinese
    /// @brief ���� ini �ļ�
    /// @param is ������
    bool Load(std::istream& is);

    /// \~chinese
    /// @brief ���� ini ��ʽ�ַ���
    /// @param content ��������
    bool LoadFromString(StringView content);

    /// \~chinese
    /// @brief ������ ini �ļ�
    /// @param file_path �ļ�·��
    bool Save(StringView file_path);

    /// \~chinese
    /// @brief ������ ini �ļ�
    /// @param os �����
    bool Save(std::ostream& os);

    /// \~chinese
    /// @brief �������ַ���
    /// @param content �������
    bool SaveToString(String& content);

    /// \~chinese
    /// @brief �Ƿ����section
    /// @param section section������
    bool HasSection(StringView section) const;

    /// \~chinese
    /// @brief �Ƿ����ֵ
    /// @param section section������
    /// @param key key������
    bool HasKey(StringView section, StringView key) const;

    /// \~chinese
    /// @brief ��ȡ����section
    SectionMap GetSectionMap() const;

    /// \~chinese
    /// @brief ��ȡsection
    /// @param section section������
    ValueMap GetSection(StringView section) const;

    /// \~chinese
    /// @brief ��ȡֵ
    /// @param section section������
    /// @param key key������
    String GetString(StringView section, StringView key, StringView default_value = String()) const;

    /// \~chinese
    /// @brief ��ȡֵ
    /// @param section section������
    /// @param key key������
    /// @param default_value ������ʱ��Ĭ��ֵ
    float GetFloat(StringView section, StringView key, float default_value = 0.0f) const;

    /// \~chinese
    /// @brief ��ȡֵ
    /// @param section section������
    /// @param key key������
    /// @param default_value ������ʱ��Ĭ��ֵ
    double GetDouble(StringView section, StringView key, double default_value = 0.0) const;

    /// \~chinese
    /// @brief ��ȡֵ
    /// @param section section������
    /// @param key key������
    /// @param default_value ������ʱ��Ĭ��ֵ
    int GetInt(StringView section, StringView key, int default_value = 0) const;

    /// \~chinese
    /// @brief ��ȡֵ
    /// @param section section������
    /// @param key key������
    /// @param default_value ������ʱ��Ĭ��ֵ
    bool GetBool(StringView section, StringView key, bool default_value = false) const;

    /// \~chinese
    /// @brief ��������section
    /// @param sections section�ֵ�
    void SetSectionMap(const SectionMap& sections);

    /// \~chinese
    /// @brief ����section
    /// @param section section������
    /// @param values ��ֵ�ֵ�
    void SetSection(StringView section, const ValueMap& values);

    /// \~chinese
    /// @brief ����ֵ
    /// @param section section������
    /// @param key key������
    /// @param value ֵ
    void SetString(StringView section, StringView key, StringView value);

    /// \~chinese
    /// @brief ����ֵ
    /// @param section section������
    /// @param key key������
    /// @param value ֵ
    void SetFloat(StringView section, StringView key, float value);

    /// \~chinese
    /// @brief ����ֵ
    /// @param section section������
    /// @param key key������
    /// @param value ֵ
    void SetDouble(StringView section, StringView key, double value);

    /// \~chinese
    /// @brief ����ֵ
    /// @param section section������
    /// @param key key������
    /// @param value ֵ
    void SetInt(StringView section, StringView key, int value);

    /// \~chinese
    /// @brief ����ֵ
    /// @param section section������
    /// @param key key������
    /// @param value ֵ
    void SetBool(StringView section, StringView key, bool value);

    /// \~chinese
    /// @brief ɾ��section
    /// @param section section������
    void DeleteSection(StringView section);

    /// \~chinese
    /// @brief ɾ��ֵ
    /// @param section section������
    /// @param key key������
    void DeleteKey(StringView section, StringView key);

    ValueMap& operator[](StringView section);

    const ValueMap& operator[](StringView section) const;

private:
    void ParseLine(StringView line, String* section);

private:
    SectionMap sections_;
};

}  // namespace kiwano
