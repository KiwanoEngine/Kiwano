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
/// @brief 本地存储
/// @details LocalStorage是一个简易的持久化工具，存放（字符串-值）的键值对
/// 支持的数据类型包括 (bool | int | float | double | String)
/// 例如, 保存游戏最高分, 以便下次进行游戏时读取:
///   @code
///     LocalStorage data;                      // 创建数据对象
///     data.SaveInt(L"best-score", 20);        // 保存最高分 20
///     int best = data.GetInt(L"best-score");  // 读取之前储存的最高分
///   @endcode
class KGE_API LocalStorage : public virtual ObjectBase
{
public:
    /// \~chinese
    /// @brief 构建本地存储对象
    /// @param file_path 文件储存路径
    /// @param field 字段名
    LocalStorage(String const& file_path = L"data.ini", String const& field = L"defalut");

    /// \~chinese
    /// @brief 获取文件储存路径
    String const& GetFilePath() const;

    /// \~chinese
    /// @brief 设置文件储存路径
    void SetFilePath(String const& file_path);

    /// \~chinese
    /// @brief 获取字段名
    String const& GetFieldName() const;

    /// \~chinese
    /// @brief 设置字段名
    void SetFieldName(String const& field);

    /// \~chinese
    /// @brief 判断键对应的数据是否存在
    bool Exists(String const& key) const;

    /// \~chinese
    /// @brief 保存 int 类型的值
    /// @param key 键
    /// @param val 值
    /// @return 操作是否成功
    bool SaveInt(String const& key, int val) const;

    /// \~chinese
    /// @brief 保存 float 类型的值
    /// @param key 键
    /// @param val 值
    /// @return 操作是否成功
    bool SaveFloat(String const& key, float val) const;

    /// \~chinese
    /// @brief 保存 double 类型的值
    /// @param key 键
    /// @param val 值
    /// @return 操作是否成功
    bool SaveDouble(String const& key, double val) const;

    /// \~chinese
    /// @brief 保存 bool 类型的值
    /// @param key 键
    /// @param val 值
    /// @return 操作是否成功
    bool SaveBool(String const& key, bool val) const;

    /// \~chinese
    /// @brief 保存 String 类型的值
    /// @param key 键
    /// @param val 值
    /// @return 操作是否成功
    bool SaveString(String const& key, String const& val) const;

    /// \~chinese
    /// @brief 获取 int 类型的值
    /// @param key 键
    /// @param default_value 值不存在时返回的默认值
    /// @return 值
    int GetInt(String const& key, int default_value = 0) const;

    /// \~chinese
    /// @brief 获取 float 类型的值
    /// @param key 键
    /// @param default_value 值不存在时返回的默认值
    /// @return 值
    float GetFloat(String const& key, float default_value = 0.0f) const;

    /// \~chinese
    /// @brief 获取 double 类型的值
    /// @param key 键
    /// @param default_value 值不存在时返回的默认值
    /// @return 值
    double GetDouble(String const& key, double default_value = 0.0) const;

    /// \~chinese
    /// @brief 获取 bool 类型的值
    /// @param key 键
    /// @param default_value 值不存在时返回的默认值
    /// @return 值
    bool GetBool(String const& key, bool default_value = false) const;

    /// \~chinese
    /// @brief 获取 字符串 类型的值
    /// @param key 键
    /// @param default_value 值不存在时返回的默认值
    /// @return 值
    String GetString(String const& key, String const& default_value = String()) const;

private:
    String file_path_;
    String field_name_;
};

inline String const& LocalStorage::GetFilePath() const
{
    return file_path_;
}

inline String const& LocalStorage::GetFieldName() const
{
    return field_name_;
}

inline void LocalStorage::SetFilePath(String const& file_path)
{
    file_path_ = file_path;
}

inline void LocalStorage::SetFieldName(String const& field_name)
{
    field_name_ = field_name;
}
}  // namespace kiwano
