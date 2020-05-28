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
    /// @brief ���� ini �ļ�
    /// @param file_path �ļ�·��
    static ConfigIniPtr Create(const String& file_path);

    /// \~chinese
    /// @brief ��ֵ�ֵ�
    typedef Map<String, String> ValueMap;

    /// \~chinese
    /// @brief Section�ֵ�
    typedef UnorderedMap<String, ValueMap> SectionMap;

    /// \~chinese
    /// @brief ���� ini �ļ�
    /// @param file_path �ļ�·��
    bool Load(const String& file_path);

    /// \~chinese
    /// @brief ���� ini �ļ�
    /// @param is ������
    bool Load(std::istream& is);

    /// \~chinese
    /// @brief ������ ini �ļ�
    /// @param file_path �ļ�·��
    bool Save(const String& file_path);

    /// \~chinese
    /// @brief ������ ini �ļ�
    /// @param os �����
    bool Save(std::ostream& os);

    /// \~chinese
    /// @brief ��ȡ����section
    SectionMap GetSectionMap() const;

    /// \~chinese
    /// @brief ��ȡsection
    /// @param section section������
    ValueMap GetSection(const String& section) const;

    /// \~chinese
    /// @brief ��ȡֵ
    /// @param section section������
    /// @param key key������
    String GetString(const String& section, const String& key) const;

    /// \~chinese
    /// @brief ��ȡֵ
    /// @param section section������
    /// @param key key������
    /// @param default_value ������ʱ��Ĭ��ֵ
    float GetFloat(const String& section, const String& key, float default_value = 0.0f) const;

    /// \~chinese
    /// @brief ��ȡֵ
    /// @param section section������
    /// @param key key������
    /// @param default_value ������ʱ��Ĭ��ֵ
    double GetDouble(const String& section, const String& key, double default_value = 0.0) const;

    /// \~chinese
    /// @brief ��ȡֵ
    /// @param section section������
    /// @param key key������
    /// @param default_value ������ʱ��Ĭ��ֵ
    int GetInt(const String& section, const String& key, int default_value = 0) const;

    /// \~chinese
    /// @brief ��ȡֵ
    /// @param section section������
    /// @param key key������
    /// @param default_value ������ʱ��Ĭ��ֵ
    bool GetBool(const String& section, const String& key, bool default_value = false) const;

    /// \~chinese
    /// @brief �Ƿ����section
    /// @param section section������
    bool HasSection(const String& section) const;

    /// \~chinese
    /// @brief �Ƿ����ֵ
    /// @param section section������
    /// @param key key������
    bool HasValue(const String& section, const String& key) const;

    /// \~chinese
    /// @brief ��������section
    /// @param sections section�ֵ�
    void SetSectionMap(const SectionMap& sections);

    /// \~chinese
    /// @brief ����section
    /// @param section section������
    /// @param values ��ֵ�ֵ�
    void SetSection(const String& section, const ValueMap& values);

    /// \~chinese
    /// @brief ����ֵ
    /// @param section section������
    /// @param key key������
    /// @param value ֵ
    void SetString(const String& section, const String& key, const String& value);

    /// \~chinese
    /// @brief ����ֵ
    /// @param section section������
    /// @param key key������
    /// @param value ֵ
    void SetFloat(const String& section, const String& key, float value);

    /// \~chinese
    /// @brief ����ֵ
    /// @param section section������
    /// @param key key������
    /// @param value ֵ
    void SetDouble(const String& section, const String& key, double value);

    /// \~chinese
    /// @brief ����ֵ
    /// @param section section������
    /// @param key key������
    /// @param value ֵ
    void SetInt(const String& section, const String& key, int value);

    /// \~chinese
    /// @brief ����ֵ
    /// @param section section������
    /// @param key key������
    /// @param value ֵ
    void SetBool(const String& section, const String& key, bool value);

    ValueMap& operator[](const String& section);

    const ValueMap& operator[](const String& section) const;

private:
    void ParseLine(StringView line, String* section);

private:
    SectionMap sections_;
};

}  // namespace kiwano
