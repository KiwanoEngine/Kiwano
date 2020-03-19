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
#include <kiwano/core/ObjectBase.h>

namespace kiwano
{

KGE_DECLARE_SMART_PTR(LocalStorage);

/// \~chinese
/// @brief ���ش洢
/// @details LocalStorage��һ�����׵ĳ־û����ߣ���ţ��ַ���-ֵ���ļ�ֵ��
/// ֧�ֵ��������Ͱ��� (bool | int | float | double | String)
/// ����, ������Ϸ��߷�, �Ա��´ν�����Ϸʱ��ȡ:
///   @code
///     LocalStorage data;                      // �������ݶ���
///     data.SaveInt("best-score", 20);        // ������߷� 20
///     int best = data.GetInt("best-score");  // ��ȡ֮ǰ�������߷�
///   @endcode
class KGE_API LocalStorage : public ObjectBase
{
public:
    /// \~chinese
    /// @brief �������ش洢����
    /// @param file_path �ļ�����·��
    /// @param field �ֶ���
    LocalStorage(const String& file_path = "data.ini", const String& field = "defalut");

    /// \~chinese
    /// @brief ��ȡ�ļ�����·��
    const String& GetFilePath() const;

    /// \~chinese
    /// @brief �����ļ�����·��
    void SetFilePath(const String& file_path);

    /// \~chinese
    /// @brief ��ȡ�ֶ���
    const String& GetFieldName() const;

    /// \~chinese
    /// @brief �����ֶ���
    void SetFieldName(const String& field);

    /// \~chinese
    /// @brief �жϼ���Ӧ�������Ƿ����
    bool Exists(const String& key) const;

    /// \~chinese
    /// @brief ���� int ���͵�ֵ
    /// @param key ��
    /// @param val ֵ
    /// @return �����Ƿ�ɹ�
    bool SaveInt(const String& key, int val) const;

    /// \~chinese
    /// @brief ���� float ���͵�ֵ
    /// @param key ��
    /// @param val ֵ
    /// @return �����Ƿ�ɹ�
    bool SaveFloat(const String& key, float val) const;

    /// \~chinese
    /// @brief ���� double ���͵�ֵ
    /// @param key ��
    /// @param val ֵ
    /// @return �����Ƿ�ɹ�
    bool SaveDouble(const String& key, double val) const;

    /// \~chinese
    /// @brief ���� bool ���͵�ֵ
    /// @param key ��
    /// @param val ֵ
    /// @return �����Ƿ�ɹ�
    bool SaveBool(const String& key, bool val) const;

    /// \~chinese
    /// @brief ���� String ���͵�ֵ
    /// @param key ��
    /// @param val ֵ
    /// @return �����Ƿ�ɹ�
    bool SaveString(const String& key, const String& val) const;

    /// \~chinese
    /// @brief ��ȡ int ���͵�ֵ
    /// @param key ��
    /// @param default_value ֵ������ʱ���ص�Ĭ��ֵ
    /// @return ֵ
    int GetInt(const String& key, int default_value = 0) const;

    /// \~chinese
    /// @brief ��ȡ float ���͵�ֵ
    /// @param key ��
    /// @param default_value ֵ������ʱ���ص�Ĭ��ֵ
    /// @return ֵ
    float GetFloat(const String& key, float default_value = 0.0f) const;

    /// \~chinese
    /// @brief ��ȡ double ���͵�ֵ
    /// @param key ��
    /// @param default_value ֵ������ʱ���ص�Ĭ��ֵ
    /// @return ֵ
    double GetDouble(const String& key, double default_value = 0.0) const;

    /// \~chinese
    /// @brief ��ȡ bool ���͵�ֵ
    /// @param key ��
    /// @param default_value ֵ������ʱ���ص�Ĭ��ֵ
    /// @return ֵ
    bool GetBool(const String& key, bool default_value = false) const;

    /// \~chinese
    /// @brief ��ȡ �ַ��� ���͵�ֵ
    /// @param key ��
    /// @param default_value ֵ������ʱ���ص�Ĭ��ֵ
    /// @return ֵ
    String GetString(const String& key, const String& default_value = String()) const;

private:
    String file_path_;
    String field_name_;
};

inline const String& LocalStorage::GetFilePath() const
{
    return file_path_;
}

inline const String& LocalStorage::GetFieldName() const
{
    return field_name_;
}

inline void LocalStorage::SetFilePath(const String& file_path)
{
    file_path_ = file_path;
}

inline void LocalStorage::SetFieldName(const String& field_name)
{
    field_name_ = field_name;
}
}  // namespace kiwano
