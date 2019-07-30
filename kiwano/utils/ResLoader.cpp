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
#include "../base/logs.h"
#include "../2d/Image.h"
#include "../2d/Frames.h"
#include "../2d/GifImage.h"
#include "FileUtil.h"
#include <fstream>

namespace kiwano
{
	namespace
	{
		void LoadJsonData09(Json const& json_data, ResLoader* loader)
		{
			String path;
			if (json_data.count(L"path"))
			{
				path = json_data[L"path"];
			}

			if (json_data.count(L"images"))
			{
				for (const auto& image : json_data[L"images"])
				{
					String file = image[L"file"];
					String id = image.count(L"id") ? image[L"id"] : file;

					if (image.count(L"type") && image[L"type"].as_string() == L"gif")
						loader->AddGifImage(id, Resource(path + file));
					else
						loader->AddImage(id, Resource(path + file));
				}
			}
			
			/*if (json_data.count(L"sounds"))
			{
				for (const auto& sound : json_data[L"sounds"])
				{
					loader->AddObj();
				}
			}*/
		}
	}

	bool ResLoader::LoadFromJsonFile(String const& file_path)
	{
		Json json_data;
		std::wifstream ifs;
		ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			ifs.open(file_path.c_str());
			ifs >> json_data;
			ifs.close();
		}
		catch (std::wifstream::failure& e)
		{
			String msg(e.what());
			KGE_WARNING_LOG(L"ResLoader::LoadFromJsonFile failed: Cannot open file. (%s)", msg.c_str());
			return false;
		}
		catch (json_exception& e)
		{
			String msg(e.what());
			KGE_WARNING_LOG(L"ResLoader::LoadFromJsonFile failed: Cannot parse to JSON. (%s)", msg.c_str());
			return false;
		}
		return LoadFromJson(json_data);
	}

	bool ResLoader::LoadFromJson(Json const& json_data)
	{
		try
		{
			String version = json_data[L"version"];
			if (version.empty() || version == L"0.9")
			{
				LoadJsonData09(json_data, this);
			}
			else
			{
				throw std::runtime_error("unknown JSON data version");
			}
		}
		catch (std::exception& e)
		{
			String msg(e.what());
			KGE_WARNING_LOG(L"ResLoader::LoadFromJson failed: JSON data is invalid. (%s)", msg.c_str());
			return false;
		}
		return true;
	}

	bool ResLoader::AddImage(String const& id, Resource const& image)
	{
		ImagePtr ptr = new (std::nothrow) Image;
		if (ptr)
		{
			if (ptr->Load(image))
			{
				return AddImage(id, ptr);
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

	bool ResLoader::AddGifImage(String const& id, Resource const& image)
	{
		GifImagePtr ptr = new (std::nothrow) GifImage;
		if (ptr)
		{
			if (ptr->Load(image))
			{
				return AddGifImage(id, ptr);
			}
		}
		return false;
	}

	bool ResLoader::AddGifImage(String const& id, GifImagePtr image)
	{
		if (image)
		{
			res_.insert(std::make_pair(id, image));
			return true;
		}
		return false;
	}

	size_t ResLoader::AddFrames(String const& id, Array<Resource> const& images)
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
				if (ptr->Load(image))
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

	size_t ResLoader::AddFrames(String const& id, Array<ImagePtr> const& images)
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

	size_t ResLoader::AddFrames(String const & id, Resource const & image, int cols, int rows)
	{
		if (cols <= 0 || rows <= 0)
			return 0;

		ImagePtr raw = new (std::nothrow) Image;
		if (!raw || !raw->Load(image))
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

	size_t ResLoader::AddFrames(String const & id, Resource const & image, Array<Rect> const & crop_rects)
	{
		ImagePtr raw = new (std::nothrow) Image;
		if (!raw || !raw->Load(image))
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

	GifImagePtr ResLoader::GetGifImage(String const& id) const
	{
		return Get<GifImage>(id);
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

}