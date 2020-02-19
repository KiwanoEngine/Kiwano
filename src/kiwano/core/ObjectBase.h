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
#include <kiwano/core/RefCounter.h>
#include <kiwano/core/SmartPtr.hpp>
#include <kiwano/macros.h>

namespace kiwano
{
KGE_DECLARE_SMART_PTR(ObjectBase);

/**
 * \~chinese
 * @brief 基础对象
 */
class KGE_API ObjectBase : public RefCounter
{
public:
    /// \~chinese
    /// @brief 构造基础对象
    ObjectBase();

    virtual ~ObjectBase();

    /// \~chinese
    /// @brief 设置对象名
    void SetName(const String& name);

    /// \~chinese
    /// @brief 获取对象名
    String GetName() const;

    /// \~chinese
    /// @brief 判断对象的名称是否相同
    /// @param name 需要判断的名称
    bool IsName(const String& name) const;

    /// \~chinese
    /// @brief 获取用户数据
    const Any& GetUserData() const;

    /// \~chinese
    /// @brief 设置用户数据
    void SetUserData(const Any& data);

    /// \~chinese
    /// @brief 获取对象ID
    uint32_t GetObjectID() const;

    /// \~chinese
    /// @brief 序列化对象
    String DumpObject();

public:
    /// \~chinese
    /// @brief 是否启用了内存泄漏追踪
    static bool IsTracingLeaks();

    /// \~chinese
    /// @brief 开始追踪内存泄漏
    static void StartTracingLeaks();

    /// \~chinese
    /// @brief 停止追踪内存泄漏
    static void StopTracingLeaks();

    /// \~chinese
    /// @brief 打印所有追踪中的对象信息
    static void DumpTracingObjects();

    /// \~chinese
    /// @brief 获取所有追踪中的对象
    static Vector<ObjectBase*>& GetTracingObjects();

private:
    static void AddObjectToTracingList(ObjectBase*);

    static void RemoveObjectFromTracingList(ObjectBase*);

private:
    bool    tracing_leak_;
    Any     user_data_;
    String* name_;

    const uint32_t id_;
};

inline String ObjectBase::GetName() const
{
    if (name_)
        return *name_;
    return String();
}

inline bool ObjectBase::IsName(const String& name) const
{
    return name_ ? (*name_ == name) : name.empty();
}

inline uint32_t ObjectBase::GetObjectID() const
{
    return id_;
}
}  // namespace kiwano
