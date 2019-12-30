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

#include <kiwano/platform/FileSystem.h>

namespace kiwano
{
	namespace
	{
		inline String ConvertPathFormat(String const& path)
		{
			// C:\a\b\c.txt => C:/a/b/c.txt

			size_t len = path.length();
			String result = path;
			for (size_t i = 0; i < len; i++)
			{
				if (result[i] == L'\\')
				{
					result[i] = L'/';
				}
			}
			return result;
		}

		inline bool IsFileExists(String const& path)
		{
			DWORD dwAttrib = ::GetFileAttributesW(path.c_str());

			return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
				!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
		}
	}

	FileSystem::FileSystem()
	{
	}

	FileSystem::~FileSystem()
	{
	}

	void FileSystem::AddSearchPath(String const& path)
	{
		String search_path = ConvertPathFormat(path);
		if (!search_path.empty() && search_path[search_path.length() - 1] != L'/')
		{
			search_path += L"/";
		}

		search_paths_.push_back(search_path);
	}

	void FileSystem::SetSearchPaths(Vector<String> const& paths)
	{
		search_paths_ = paths;

		for (auto& path : search_paths_)
		{
			path = ConvertPathFormat(path);
			if (!path.empty() && path[path.length() - 1] != L'/')
			{
				path += L"/";
			}
		}
	}

	String FileSystem::GetFullPathForFile(String const& file) const
	{
		if (file.empty())
		{
			return L"";
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
				file_lookup_cache_.emplace(file, full_path);	// Save to cache
				return full_path;
			}
		}

		// File not found
		return L"";
	}

	void FileSystem::AddFileLookupRule(String const& key, String const& file_path)
	{
		file_lookup_dict_.emplace(key, ConvertPathFormat(file_path));
	}

	void FileSystem::SetFileLookupDictionary(UnorderedMap<String, String> const& dict)
	{
		file_lookup_cache_.clear();

		file_lookup_dict_ = dict;
	}

	bool FileSystem::IsFileExists(String const& file_path) const
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

	bool FileSystem::IsAbsolutePath(String const& path) const
	{
		// like "C:\some.file"
		return path.length() > 2 && ((std::isalpha(path[0]) && path[1] == L':') || (path[0] == L'/' && path[1] == L'/'));
	}

	bool FileSystem::RemoveFile(String const& file_path) const
	{
		if (::DeleteFileW(file_path.c_str()))
			return true;
		return false;
	}

	bool FileSystem::ExtractResourceToFile(Resource const& res, String const& dest_file_name) const
	{
		HANDLE file_handle = ::CreateFileW(
			dest_file_name.c_str(),
			GENERIC_WRITE,
			NULL,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (file_handle == INVALID_HANDLE_VALUE)
			return false;

		Resource::Data data = res.GetData();
		if (data)
		{
			DWORD written_bytes = 0;
			::WriteFile(file_handle, data.buffer, data.size, &written_bytes, NULL);
			::CloseHandle(file_handle);

			return true;
		}
		else
		{
			::CloseHandle(file_handle);
			::DeleteFile(dest_file_name.c_str());
		}
		return false;
	}

}
