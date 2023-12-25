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
#include <kiwano/core/BinaryData.h>

namespace kiwano
{

/**
 * \~chinese
 * @brief ��Դ
 * @details
 *   ��Դ�Ǳ����� exe �еĶ��������ݣ�
 *   ���磬һ����Ƶ��Դ������Ϊ "WAVE"�����Ʊ�ʶ��Ϊ
 * IDR_WAVE_1����ô��������ָ������Դ:
 *   @code
 *     Resource(IDR_WAVE_1, "WAVE");
 *   @endcode
 *   �˽���Դ�ĸ�����Ϣ:
 * https://docs.microsoft.com/en-us/windows/desktop/menurc/resources
 */
class KGE_API Resource
{
public:
    /// \~chinese
    /// @brief ������Դ
    Resource();

    /// \~chinese
    /// @brief ������Դ
    /// @param id ��Դ ID
    /// @param type ��Դ����
    Resource(uint32_t id, StringView type);

    /// \~chinese
    /// @brief ��ȡ��Դ�Ķ���������
    /// @return ��Դ����
    BinaryData GetData() const;

    /// \~chinese
    /// @brief ��ȡ��Դ ID
    uint32_t GetId() const;

    /// \~chinese
    /// @brief ��ȡ��Դ����
    StringView GetType() const;

private:
    uint32_t id_;
    String   type_;

    mutable BinaryData data_;
};

inline uint32_t Resource::GetId() const
{
    return id_;
}

inline StringView Resource::GetType() const
{
    return type_;
}
}  // namespace kiwano
