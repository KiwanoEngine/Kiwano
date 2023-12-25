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

#include <cctype>
#include <kiwano/platform/FileSystem.h>

namespace kiwano
{
namespace
{
inline String ConvertPathFormat(StringView path)
{
    // C:\a\b\c.txt => C:/a/b/c.txt

    size_t len    = path.size();
    String result = path;
    for (size_t i = 0; i < len; i++)
    {
        if (result[i] == '\\')
        {
            result[i] = '/';
        }
    }
    return result;
}

inline bool IsFileExists(StringView path)
{
    DWORD dwAttrib = ::GetFileAttributesA(path.data());

    return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
}  // namespace

FileSystem::FileSystem() {}

FileSystem::~FileSystem() {}

void FileSystem::AddSearchPath(StringView path)
{
    String search_path = ConvertPathFormat(path);
    if (!search_path.empty() && search_path[search_path.length() - 1] != '/')
    {
        search_path += "/";
    }

    search_paths_.push_back(search_path);
}

void FileSystem::SetSearchPaths(const Vector<String>& paths)
{
    search_paths_ = paths;

    for (auto& path : search_paths_)
    {
        path = ConvertPathFormat(path);
        if (!path.empty() && path[path.length() - 1] != '/')
        {
            path += "/";
        }
    }
}

String FileSystem::GetFullPathForFile(StringView file) const
{
    if (file.empty())
    {
        return "";
    }

    if (IsAbsolutePath(file))
    {
        return file;
    }

    // Search file path cache
    auto cache_iter = file_lookup_cache_.find(file);
    if (cache_iter != file_lookup_cache_.end())
    {
        return cache_iter->second;
    }

    String dict_path;

    // Search file path dictionary
    auto iter = file_lookup_dict_.find(file);
    if (iter != file_lookup_dict_.end())
    {
        dict_path = iter->second;
    }
    else
    {
        dict_path = ConvertPathFormat(file);
    }

    if (kiwano::IsFileExists(dict_path))
    {
        file_lookup_cache_.emplace(file, dict_path);
        return dict_path;
    }

    for (const auto& search_path : search_paths_)
    {
        String full_path = search_path + dict_path;
        if (kiwano::IsFileExists(full_path))
        {
            file_lookup_cache_.emplace(file, full_path);  // Save to cache
            return full_path;
        }
    }

    // File not found
    return "";
}

String FileSystem::GetFileExt(StringView file) const
{
    if (file.empty())
        return "";

    auto pos = String::npos;
    for (size_t i = file.size() - 1; i > 0; i--)
    {
        if (file[i] == '.')
        {
            pos = i;
            break;
        }
    }
    if (pos == String::npos)
    {
        return "";
    }
    return file.substr(pos + 1);
}

void FileSystem::AddFileLookupRule(StringView key, StringView file_path)
{
    file_lookup_dict_.emplace(key, ConvertPathFormat(file_path));
}

void FileSystem::SetFileLookupDictionary(const UnorderedMap<String, String>& dict)
{
    file_lookup_cache_.clear();

    file_lookup_dict_ = dict;
}

bool FileSystem::IsFileExists(StringView file_path) const
{
    if (IsAbsolutePath(file_path))
    {
        return kiwano::IsFileExists(file_path);
    }
    else
    {
        String full_path = GetFullPathForFile(file_path);
        return !full_path.empty();
    }
}

bool FileSystem::IsAbsolutePath(StringView path) const
{
    // like "C:\some.file"
    return path.size() > 2 && ((std::isalpha(path[0]) && path[1] == ':') || (path[0] == '/' && path[1] == '/'));
}

bool FileSystem::RemoveFile(StringView file_path) const
{
    if (::DeleteFileA(file_path.data()))
        return true;
    return false;
}

bool FileSystem::ExtractResourceToFile(const Resource& res, StringView dest_file_name) const
{
    HANDLE file_handle =
        ::CreateFileA(dest_file_name.data(), GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (file_handle == INVALID_HANDLE_VALUE)
        return false;

    BinaryData data = res.GetData();
    if (data.IsValid())
    {
        DWORD written_bytes = 0;
        ::WriteFile(file_handle, data.buffer, data.size, &written_bytes, NULL);
        ::CloseHandle(file_handle);

        return true;
    }
    else
    {
        ::CloseHandle(file_handle);
        ::DeleteFileA(dest_file_name.data());
    }
    return false;
}

}  // namespace kiwano
