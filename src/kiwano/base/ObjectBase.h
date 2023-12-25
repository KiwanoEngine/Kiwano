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
#include <kiwano/macros.h>
#include <kiwano/core/Common.h>
#include <kiwano/core/Exception.h>
#include <kiwano/core/Serializable.h>
#include <kiwano/base/RefObject.h>
#include <kiwano/base/RefPtr.h>

namespace kiwano
{
KGE_DECLARE_SMART_PTR(ObjectBase);

/**
 * \~chinese
 * @brief 对象状态
 */
struct ObjectStatus
{
    int    code = 0;  ///< 状态码，等于 0 时为成功状态，否则为失败状态
    String msg;       ///< 状态信息

    ObjectStatus() = default;

    ObjectStatus(int code, StringView msg)
        : code(code)
        , msg(msg)
    {
    }

    /// \~chinese
    /// @brief 对象状态是否成功
    inline bool Success() const
    {
        return this->code == 0;
    }

    /// \~chinese
    /// @brief 对象失败状态
    static const int fail = -1;
};

/**
 * \~chinese
 * @brief 对象失败状态异常
 */
class ObjectFailException : public Exception
{
public:
    ObjectFailException(ObjectBase* obj, const ObjectStatus& status);

    /// \~chinese
    /// @brief 获取失败的对象指针
    inline ObjectBase* GetObj() const
    {
        return obj_;
    }

    /// \~chinese
    /// @brief 获取对象状态
    inline ObjectStatus GetStatus() const
    {
        return status_;
    }

    virtual char const* what() const override;

private:
    ObjectBase*  obj_;
    ObjectStatus status_;
};

/**
 * \~chinese
 * @brief 对象处理策略方法
 */
typedef Function<void(ObjectBase*, const ObjectStatus&)> ObjectPolicyFunc;

/**
 * \~chinese
 * @brief 对象处理策略
 */
struct ObjectPolicy
{
    /// \~chinese
    /// @brief 忽略对象失败状态
    static inline ObjectPolicyFunc Ignore()
    {
        return nullptr;
    }

    /// \~chinese
    /// @brief 在对象状态变为失败时打印警告日志
    /// @param threshold 触发阈值
    /// @return 对象处理策略方法
    static ObjectPolicyFunc WarnLog(int threshold = ObjectStatus::fail);

    /// \~chinese
    /// @brief 在对象状态变为失败时打印错误日志（默认策略）
    /// @param threshold 触发阈值
    /// @return 对象处理策略方法
    static ObjectPolicyFunc ErrorLog(int threshold = ObjectStatus::fail);

    /// \~chinese
    /// @brief 在对象状态变为失败时抛出 ObjectFailException
    /// @param threshold 触发阈值
    /// @return 对象处理策略方法
    static ObjectPolicyFunc Exception(int threshold = ObjectStatus::fail);
};

/**
 * \~chinese
 * @brief 基础对象
 */
class KGE_API ObjectBase
    : public RefObject
    , public Serializable
{
public:
    /// \~chinese
    /// @brief 构造基础对象
    ObjectBase();

    virtual ~ObjectBase();

    /// \~chinese
    /// @brief 设置对象名
    void SetName(StringView name);

    /// \~chinese
    /// @brief 获取对象名
    StringView GetName() const;

    /// \~chinese
    /// @brief 判断对象的名称是否相同
    /// @param name 需要判断的名称
    bool IsName(StringView name) const;

    /// \~chinese
    /// @brief 获取用户数据
    void* GetUserData() const;

    /// \~chinese
    /// @brief 设置用户数据
    /// @param data 数据指针
    void SetUserData(void* data);

    /// \~chinese
    /// @brief 持有一个对象并管理其生命周期
    /// @param other 对象指针
    void Hold(ObjectBasePtr other);

    /// \~chinese
    /// @brief 放弃持有的对象
    /// @param other 对象指针
    void Unhold(ObjectBasePtr other);

    /// \~chinese
    /// @brief 获取对象ID
    uint64_t GetObjectID() const;

    /// \~chinese
    /// @brief 序列化
    void DoSerialize(Serializer* serializer) const override;

    /// \~chinese
    /// @brief 反序列化
    void DoDeserialize(Deserializer* deserializer) override;

    /// \~chinese
    /// @brief 判断对象是否有效
    virtual bool IsValid() const;

    /// \~chinese
    /// @brief 获取对象状态
    ObjectStatus* GetStatus() const;

    /// \~chinese
    /// @brief 设置对象状态
    void SetStatus(const ObjectStatus& status);

    /// \~chinese
    /// @brief 将对象标记为失败状态
    void Fail(StringView msg, int code = ObjectStatus::fail);

    /// \~chinese
    /// @brief 清除对象状态
    void ClearStatus();

    /// \~chinese
    /// @brief 设置对象处理策略
    static void SetObjectPolicy(const ObjectPolicyFunc& policy);

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
    const uint64_t id_;

    bool    tracing_leak_;
    String* name_;
    void*   user_data_;

    ObjectStatus*       status_;
    Set<ObjectBasePtr>* holdings_;
};

inline StringView ObjectBase::GetName() const
{
    if (name_)
        return *name_;
    return StringView();
}

inline bool ObjectBase::IsName(StringView name) const
{
    return name_ ? (*name_ == name) : name.empty();
}

inline uint64_t ObjectBase::GetObjectID() const
{
    return id_;
}
}  // namespace kiwano
