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

namespace kiwano
{
/**
 * \~chinese
 * @brief 文件系统，为其他模块提供文件搜索规则等
 */
class KGE_API FileSystem : public Singleton<FileSystem>
{
    friend Singleton<FileSystem>;

public:
    /**
     * \~chinese
     * @brief 添加文件搜索路径
     * @param path 文件搜索路径
     */
    void AddSearchPath(StringView path);

    /**
     * \~chinese
     * @brief 设置文件搜索路径
     * @param paths 搜索路径数组
     */
    void SetSearchPaths(const Vector<String>& paths);

    /**
     * \~chinese
     * @brief 在搜索列表中查找文件并获取完整路径
     * @param file 文件路径
     * @return 完整的文件路径
     */
    String GetFullPathForFile(StringView file) const;

    /**
     * \~chinese
     * @brief 获取文件拓展名
     * @param file 文件路径
     * @return 文件拓展名，不包含 . 符号
     */
    String GetFileExt(StringView file) const;

    /**
     * \~chinese
     * @brief 添加文件路径查找字典规则
     * @param key 文件关键词
     * @param file_path 文件路径
     */
    void AddFileLookupRule(StringView key, StringView file_path);

    /**
     * \~chinese
     * @brief 设置文件路径查找字典
     * @param dict 文件路径查找字典
     */
    void SetFileLookupDictionary(const UnorderedMap<String, String>& dict);

    /**
     * \~chinese
     * @brief 读取文件内容
     */
    void ReadFile(StringView file_path, std::vector<uint8_t>& output);

    /**
     * \~chinese
     * @brief 文件是否存在
     * @param file_path 文件路径
     * @return 若文件存在，返回 true
     */
    bool IsFileExists(StringView file_path) const;

    /**
     * \~chinese
     * @brief 判断路径是否是绝对路径
     * @param path 文件路径
     * @return 若是绝对路径，返回 true
     */
    bool IsAbsolutePath(StringView path) const;

    /**
     * \~chinese
     * @brief 删除文件
     * @param file_path 文件路径
     * @return 删除是否成功
     */
    bool RemoveFile(StringView file_path) const;

    /**
     * \~chinese
     * @brief 释放二进制资源到临时文件目录
     * @param res 资源
     * @param dest_file_name 目标文件名
     * @return 操作是否成功
     */
    bool ExtractResourceToFile(const Resource& res, StringView dest_file_name) const;

    ~FileSystem();

private:
    FileSystem();

private:
    Vector<String>                       search_paths_;
    UnorderedMap<String, String>         file_lookup_dict_;
    mutable UnorderedMap<String, String> file_lookup_cache_;
};
}  // namespace kiwano
