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
 * @brief ����״̬
 */
struct ObjectStatus
{
    int    code = 0;  ///< ״̬�룬���� 0 ʱΪ�ɹ�״̬������Ϊʧ��״̬
    String msg;       ///< ״̬��Ϣ

    ObjectStatus() = default;

    ObjectStatus(int code, StringView msg)
        : code(code)
        , msg(msg)
    {
    }

    /// \~chinese
    /// @brief ����״̬�Ƿ�ɹ�
    inline bool Success() const
    {
        return this->code == 0;
    }

    /// \~chinese
    /// @brief ����ʧ��״̬
    static const int fail = -1;
};

/**
 * \~chinese
 * @brief ����ʧ��״̬�쳣
 */
class ObjectFailException : public Exception
{
public:
    ObjectFailException(ObjectBase* obj, const ObjectStatus& status);

    /// \~chinese
    /// @brief ��ȡʧ�ܵĶ���ָ��
    inline ObjectBase* GetObj() const
    {
        return obj_;
    }

    /// \~chinese
    /// @brief ��ȡ����״̬
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
 * @brief ��������Է���
 */
typedef Function<void(ObjectBase*, const ObjectStatus&)> ObjectPolicyFunc;

/**
 * \~chinese
 * @brief ���������
 */
struct ObjectPolicy
{
    /// \~chinese
    /// @brief ���Զ���ʧ��״̬
    static inline ObjectPolicyFunc Ignore()
    {
        return nullptr;
    }

    /// \~chinese
    /// @brief �ڶ���״̬��Ϊʧ��ʱ��ӡ������־
    /// @param threshold ������ֵ
    /// @return ��������Է���
    static ObjectPolicyFunc WarnLog(int threshold = ObjectStatus::fail);

    /// \~chinese
    /// @brief �ڶ���״̬��Ϊʧ��ʱ��ӡ������־��Ĭ�ϲ��ԣ�
    /// @param threshold ������ֵ
    /// @return ��������Է���
    static ObjectPolicyFunc ErrorLog(int threshold = ObjectStatus::fail);

    /// \~chinese
    /// @brief �ڶ���״̬��Ϊʧ��ʱ�׳� ObjectFailException
    /// @param threshold ������ֵ
    /// @return ��������Է���
    static ObjectPolicyFunc Exception(int threshold = ObjectStatus::fail);
};

/**
 * \~chinese
 * @brief ��������
 */
class KGE_API ObjectBase
    : public RefObject
    , public Serializable
{
public:
    /// \~chinese
    /// @brief �����������
    ObjectBase();

    virtual ~ObjectBase();

    /// \~chinese
    /// @brief ���ö�����
    void SetName(StringView name);

    /// \~chinese
    /// @brief ��ȡ������
    StringView GetName() const;

    /// \~chinese
    /// @brief �ж϶���������Ƿ���ͬ
    /// @param name ��Ҫ�жϵ�����
    bool IsName(StringView name) const;

    /// \~chinese
    /// @brief ��ȡ�û�����
    void* GetUserData() const;

    /// \~chinese
    /// @brief �����û�����
    /// @param data ����ָ��
    void SetUserData(void* data);

    /// \~chinese
    /// @brief ����һ�����󲢹�������������
    /// @param other ����ָ��
    void Hold(ObjectBasePtr other);

    /// \~chinese
    /// @brief �������еĶ���
    /// @param other ����ָ��
    void Unhold(ObjectBasePtr other);

    /// \~chinese
    /// @brief ��ȡ����ID
    uint64_t GetObjectID() const;

    /// \~chinese
    /// @brief ���л�
    void DoSerialize(Serializer* serializer) const override;

    /// \~chinese
    /// @brief �����л�
    void DoDeserialize(Deserializer* deserializer) override;

    /// \~chinese
    /// @brief �ж϶����Ƿ���Ч
    virtual bool IsValid() const;

    /// \~chinese
    /// @brief ��ȡ����״̬
    ObjectStatus* GetStatus() const;

    /// \~chinese
    /// @brief ���ö���״̬
    void SetStatus(const ObjectStatus& status);

    /// \~chinese
    /// @brief ��������Ϊʧ��״̬
    void Fail(StringView msg, int code = ObjectStatus::fail);

    /// \~chinese
    /// @brief �������״̬
    void ClearStatus();

    /// \~chinese
    /// @brief ���ö��������
    static void SetObjectPolicy(const ObjectPolicyFunc& policy);

public:
    /// \~chinese
    /// @brief �Ƿ��������ڴ�й©׷��
    static bool IsTracingLeaks();

    /// \~chinese
    /// @brief ��ʼ׷���ڴ�й©
    static void StartTracingLeaks();

    /// \~chinese
    /// @brief ֹͣ׷���ڴ�й©
    static void StopTracingLeaks();

    /// \~chinese
    /// @brief ��ӡ����׷���еĶ�����Ϣ
    static void DumpTracingObjects();

    /// \~chinese
    /// @brief ��ȡ����׷���еĶ���
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
