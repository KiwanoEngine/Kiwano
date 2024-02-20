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
#include <kiwano/core/Resource.h>
#include <kiwano/base/ObjectBase.h>

namespace kiwano
{

/// \~chinese
/// @brief ��Դ����
class KGE_API ResourceCache final : public ObjectBase
{
public:
    ResourceCache();

    virtual ~ResourceCache();

    /// \~chinese
    /// @brief �� JSON �ļ�������Դ��Ϣ
    /// @param file_path JSON�ļ�·��
    bool LoadFromJsonFile(StringView file_path);

    /// \~chinese
    /// @brief �� XML �ļ�������Դ��Ϣ
    /// @param file_path XML�ļ�·��
    bool LoadFromXmlFile(StringView file_path);

    /// \~chinese
    /// @brief ��ȡ��Դ
    /// @param id ����ID
    RefPtr<ObjectBase> Get(StringView id) const;

    /// \~chinese
    /// @brief ��ȡ��Դ
    /// @tparam _Ty ��������
    /// @param id ����ID
    /// @return ָ���������͵�ָ��
    template <typename _Ty>
    RefPtr<_Ty> Get(StringView id) const
    {
        return dynamic_cast<_Ty*>(Get(id).Get());
    }

    /// \~chinese
    /// @brief ��������뻺��
    /// @param id ����ID
    /// @param obj ����
    void AddObject(StringView id, RefPtr<ObjectBase> obj);

    /// \~chinese
    /// @brief ɾ��ָ����Դ
    /// @param id ����ID
    void Remove(StringView id);

    /// \~chinese
    /// @brief ���������Դ
    void Clear();

private:
    UnorderedMap<String, RefPtr<ObjectBase>> object_cache_;
};

}  // namespace kiwano
