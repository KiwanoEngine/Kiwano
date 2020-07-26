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
#include <kiwano/utils/ResourceCache.h>
#include <kiwano/utils/Json.h>
#include <kiwano/utils/Xml.h>

namespace kiwano
{

/// \~chinese
/// @brief ��Դ������
class KGE_API ResourceLoader final : Noncopyable
{
public:
    /// \~chinese
    /// @brief �� JSON �ļ�������Դ��Ϣ
    /// @param file_path JSON�ļ�·��
    static bool LoadFromJsonFile(ResourceCachePtr cache, const String& file_path);

    /// \~chinese
    /// @brief �� JSON ������Դ��Ϣ
    /// @param json_data JSON����
    static bool LoadFromJson(ResourceCachePtr cache, const Json& json_data);

    /// \~chinese
    /// @brief �� XML �ļ�������Դ��Ϣ
    /// @param file_path XML�ļ�·��
    static bool LoadFromXmlFile(ResourceCachePtr cache, const String& file_path);

    /// \~chinese
    /// @brief �� XML �ĵ����������Դ��Ϣ
    /// @param doc XML�ĵ�����
    static bool LoadFromXml(ResourceCachePtr cache, const XmlDocument& doc);
};

}  // namespace kiwano
