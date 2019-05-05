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

#include "ResLoader.h"
#include "../platform/modules.h"
#include "../2d/Image.h"
#include "../2d/Frames.h"

namespace kiwano
{
	namespace
	{
		Resource LocateRes(Resource const& res, List<String> const& paths)
		{
			if (res.IsFileType())
			{
				String file_name = res.GetFileName();
				for (const auto& path : paths)
				{
					if (modules::Shlwapi::Get().PathFileExistsW((path + file_name).c_str()))
					{
						return Resource{ path + file_name };
					}
				}
			}
			return res;
		}
	}

	bool ResLoader::AddImage(String const& id, Resource const& image)
	{
		ImagePtr ptr = new (std::nothrow) Image;
		if (ptr)
		{
			if (ptr->Load(LocateRes(image, search_paths_)))
			{
				res_.insert(std::make_pair(id, ptr));
				return true;
			}
		}
		return false;
	}

	bool ResLoader::AddImage(String const & id, ImagePtr image)
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

		Array<ImagePtr> image_arr;
		image_arr.reserve(images.size());

		for (const auto& image : images)
		{
			ImagePtr ptr = new (std::nothrow) Image;
			if (ptr)
			{
				if (ptr->Load(LocateRes(image, search_paths_)))
				{
					image_arr.push_back(ptr);
				}
			}
		}

		if (!image_arr.empty())
		{
			FramesPtr frames = new (std::nothrow) Frames(image_arr);
			if (frames)
			{
				res_.insert(std::make_pair(id, frames));
				return frames->GetFrames().size();
			}
		}
		return 0;
	}

	int ResLoader::AddFrames(String const& id, Array<ImagePtr> const& images)
	{
		if (images.empty())
			return 0;

		FramesPtr frames = new (std::nothrow) Frames(images);
		if (frames)
		{
			res_.insert(std::make_pair(id, frames));
			return frames->GetFrames().size();
		}
		return 0;
	}

	int ResLoader::AddFrames(String const & id, Resource const & image, int cols, int rows)
	{
		if (cols <= 0 || rows <= 0)
			return 0;

		ImagePtr raw = new (std::nothrow) Image;
		if (!raw || !raw->Load(LocateRes(image, search_paths_)))
			return 0;

		float raw_width = raw->GetSourceWidth();
		float raw_height = raw->GetSourceHeight();
		float width = raw_width / cols;
		float height = raw_height / rows;

		Array<ImagePtr> image_arr;
		image_arr.reserve(rows * cols);

		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				ImagePtr ptr = new (std::nothrow) Image(raw->GetBitmap());
				if (ptr)
				{
					ptr->Crop(Rect{ i * width, j * height, width, height });
					image_arr.push_back(ptr);
				}
			}
		}

		FramesPtr frames = new (std::nothrow) Frames(image_arr);
		if (frames)
		{
			res_.insert(std::make_pair(id, frames));
			return frames->GetFrames().size();
		}
		return 0;
	}

	int ResLoader::AddFrames(String const & id, Resource const & image, Array<Rect> const & crop_rects)
	{
		ImagePtr raw = new (std::nothrow) Image;
		if (!raw || !raw->Load(LocateRes(image, search_paths_)))
			return 0;

		Array<ImagePtr> image_arr;
		image_arr.reserve(crop_rects.size());

		for (const auto& rect : crop_rects)
		{
			ImagePtr ptr = new (std::nothrow) Image(raw->GetBitmap());
			if (ptr)
			{
				ptr->Crop(rect);
				image_arr.push_back(ptr);
			}
		}

		FramesPtr frames = new (std::nothrow) Frames(image_arr);
		if (frames)
		{
			res_.insert(std::make_pair(id, frames));
			return frames->GetFrames().size();
		}
		return 0;
	}

	bool ResLoader::AddFrames(String const & id, FramesPtr frames)
	{
		if (frames)
		{
			res_.insert(std::make_pair(id, frames));
			return true;
		}
		return false;
	}

	bool ResLoader::AddObj(String const& id, ObjectPtr obj)
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
		return Get<Image>(id);
	}

	FramesPtr ResLoader::GetFrames(String const & id) const
	{
		return Get<Frames>(id);
	}

	ObjectPtr ResLoader::GetObj(String const & id) const
	{
		return Get<Object>(id);
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

}