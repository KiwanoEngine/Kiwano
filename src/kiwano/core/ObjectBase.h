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
#include <kiwano/core/Serializable.h>
#include <kiwano/core/RefCounter.h>
#include <kiwano/core/SmartPtr.hpp>
#include <kiwano/macros.h>

namespace kiwano
{
KGE_DECLARE_SMART_PTR(ObjectBase);

/**
 * \~chinese
 * @brief ��������
 */
class KGE_API ObjectBase
    : public RefCounter
    , public Serializable
{
public:
    /// \~chinese
    /// @brief �����������
    ObjectBase();

    virtual ~ObjectBase();

    /// \~chinese
    /// @brief ���ö�����
    void SetName(const String& name);

    /// \~chinese
    /// @brief ��ȡ������
    String GetName() const;

    /// \~chinese
    /// @brief �ж϶���������Ƿ���ͬ
    /// @param name ��Ҫ�жϵ�����
    bool IsName(const String& name) const;

    /// \~chinese
    /// @brief ��ȡ�û�����
    const Any& GetUserData() const;

    /// \~chinese
    /// @brief �����û�����
    void SetUserData(const Any& data);

    /// \~chinese
    /// @brief ��ȡ����ID
    uint32_t GetObjectID() const;

    /// \~chinese
    /// @brief ���л�
    void DoSerialize(Serializer* serializer) const override;

    /// \~chinese
    /// @brief �����л�
    void DoDeserialize(Deserializer* deserializer) override;

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
    const uint32_t id_;

    bool    tracing_leak_;
    String* name_;
    Any     user_data_;
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
