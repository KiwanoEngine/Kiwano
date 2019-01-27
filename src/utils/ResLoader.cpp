// Copyright (c) 2016-2018 Easy2D - Nomango
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

#include "ResLoader.h"
#include "../core/modules.h"
#include "../core/Image.h"
#include "../core/Frames.h"
#include "../core/Music.h"

namespace easy2d
{
	namespace
	{
		String Search(LPCWSTR file_name, List<String> const& paths)
		{
			for (const auto& path : paths)
			{
				if (modules::Shlwapi::Get().PathFileExistsW((path + file_name).c_str()))
				{
					return path + file_name;
				}
			}
			return file_name;
		}
	}

	bool ResLoader::AddImage(String const& id, Resource const& image)
	{
		ImagePtr ptr = new (std::nothrow) Image;
		if (!ptr)
			return false;

		if (ptr && ptr->Load(FindRes(image)))
		{
			res_.insert(std::make_pair(id, ptr));
			return true;
		}
		return false;
	}

	bool ResLoader::AddImage(String const & id, ImagePtr const & image)
	{
		if (image)
		{
			res_.insert(std::make_pair(id, image));
			return true;
		}
		return false;
	}

	int ResLoader::AddFrames(String const& id, Array<Resource> const& images)
	{
		if (images.empty())
			return 0;

		FramesPtr frames = new (std::nothrow) Frames;
		if (!frames)
			return 0;

		int total = 0;
		for (const auto& image : images)
		{
			ImagePtr ptr = new (std::nothrow) Image;
			if (ptr && ptr->Load(FindRes(image)))
			{
				frames->Add(ptr);
				++total;
			}
		}

		if (total)
			res_.insert(std::make_pair(id, frames));
		return total;
	}

	int ResLoader::AddFrames(String const& id, Array<ImagePtr> const& images)
	{
		if (images.empty())
			return 0;

		FramesPtr frames = new (std::nothrow) Frames;
		if (!frames)
			return 0;

		int total = 0;
		for (const auto& image : images)
		{
			if (image)
			{
				frames->Add(image);
				total++;
			}
		}

		if (total)
			res_.insert(std::make_pair(id, frames));
		return total;
	}

	int ResLoader::AddFrames(String const& id, Array<std::pair<Resource, Rect>> const& images)
	{
		if (images.empty())
			return 0;

		FramesPtr frames = new (std::nothrow) Frames;
		if (!frames)
			return 0;

		int total = 0;
		for (const auto& pair : images)
		{
			ImagePtr ptr = new (std::nothrow) Image;
			if (ptr && ptr->Load(FindRes(pair.first)))
			{
				if (!pair.second.IsEmpty())
				{
					ptr->Crop(pair.second);
				}
				frames->Add(ptr);
				++total;
			}
		}

		if (total)
			res_.insert(std::make_pair(id, frames));
		return total;
	}

	bool ResLoader::AddMusic(String const & id, Resource const & music)
	{
		MusicPtr ptr = new (std::nothrow) Music;
		if (ptr && ptr->Load(FindRes(music)))
		{
			res_.insert(std::make_pair(id, ptr));
			return true;
		}
		return false;
	}

	bool ResLoader::AddMusic(String const & id, MusicPtr const & music)
	{
		if (music)
		{
			res_.insert(std::make_pair(id, music));
			return true;
		}
		return false;
	}

	bool ResLoader::AddObj(String const& id, ObjectPtr const& obj)
	{
		if (obj)
		{
			res_.insert(std::make_pair(id, obj));
			return true;
		}
		return false;
	}

	ImagePtr ResLoader::GetImage(String const & id) const
	{
		return Get<Image*>(id);
	}

	FramesPtr ResLoader::GetFrames(String const & id) const
	{
		return Get<Frames*>(id);
	}

	MusicPtr ResLoader::GetMusic(String const & id) const
	{
		return Get<Music*>(id);
	}

	ObjectPtr ResLoader::GetObj(String const & id) const
	{
		return Get<Object*>(id);
	}

	void ResLoader::Delete(String const & id)
	{
		res_.erase(id);
	}

	void ResLoader::Destroy()
	{
		res_.clear();
	}

	void ResLoader::AddSearchPath(String const & path)
	{
		String tmp = path;
		size_t pos = 0;
		while ((pos = tmp.find(L"/", pos)) != String::npos)
		{
			tmp.replace(pos, 1, L"\\");
			pos++;
		}

		if (tmp.at(tmp.length() - 1) != L'\\')
		{
			tmp.append(L"\\");
		}

		auto iter = std::find(search_paths_.cbegin(), search_paths_.cend(), tmp);
		if (iter == search_paths_.cend())
		{
			search_paths_.push_front(tmp);
		}
	}

	Resource ResLoader::FindRes(Resource const & res) const
	{
		if (res.IsFile())
		{
			String path = Search(res.GetFileName(), search_paths_);
			return Resource(path.c_str());
		}
		return res;
	}

}