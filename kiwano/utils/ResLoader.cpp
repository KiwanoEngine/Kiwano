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
	namespace __res_loader_01
	{
		struct GlobalData
		{
			String path;
		};

		bool LoadImagesFromData(ResLoader* loader, GlobalData* gdata, const String* id, const String* type,
			const String* file, const Array<const wchar_t*>* files, int rows, int cols)
		{
			if (!gdata || !id) return false;

			if (file)
			{
				// Gif image
				if (type && (*type) == L"gif")
				{
					return loader->AddGifImage(*id, Resource(gdata->path + (*file)));
				}

				if (!(*file).empty())
				{
					if (rows || cols)
					{
						// Image slices
						return loader->AddFrames(*id, Resource(gdata->path + (*file)), std::max(cols, 1), std::max(rows, 1));
					}
					else
					{
						// Simple image
						return loader->AddImage(*id, Resource(gdata->path + (*file)));
					}
				}
			}

			// Frames
			if (files)
			{
				Array<ImagePtr> images;
				images.reserve(files->size());
				for (const auto& file : (*files))
				{
					ImagePtr image = new Image(gdata->path + (file));
					if (image->IsValid())
					{
						images.push_back(image);
					}
				}
				return !!loader->AddFrames(*id, images);
			}
			return false;
		}

		bool LoadJsonData(ResLoader* loader, Json const& json_data)
		{
			GlobalData global_data;
			if (json_data.count(L"path"))
			{
				global_data.path = json_data[L"path"];
			}

			if (json_data.count(L"images"))
			{
				for (const auto& image : json_data[L"images"])
				{
					const String* id = nullptr, *type = nullptr, *file = nullptr;
					int rows = 0, cols = 0;

					if (image.count(L"id")) id = &image[L"id"].as_string();
					if (image.count(L"type")) type = &image[L"type"].as_string();
					if (image.count(L"file")) file = &image[L"file"].as_string();
					if (image.count(L"rows")) rows = image[L"rows"].as_int();
					if (image.count(L"cols")) cols = image[L"cols"].as_int();

					if (image.count(L"files"))
					{
						Array<const wchar_t*> files;
						files.reserve(image[L"files"].size());
						for (const auto& file : image[L"files"])
						{
							files.push_back(file.as_string().c_str());
						}
						if (!LoadImagesFromData(loader, &global_data, id, type, file, &files, rows, cols))
							return false;
					}
					else
					{
						if (!LoadImagesFromData(loader, &global_data, id, type, file, nullptr, rows, cols))
							return false;
					}
				}
			}
			return true;
		}

		bool LoadXmlData(ResLoader* loader, tinyxml2::XMLElement* elem)
		{
			GlobalData global_data;
			if (auto path = elem->FirstChildElement(L"path"))
			{
				global_data.path = path->GetText();
			}

			if (auto images = elem->FirstChildElement(L"images"))
			{
				for (auto image = images->FirstChildElement(); image; image = image->NextSiblingElement())
				{
					String id, type, file;
					int rows = 0, cols = 0;

					if (auto attr = image->Attribute(L"id"))      id.assign(attr); // assign() copies attr content
					if (auto attr = image->Attribute(L"type"))    type = attr;     // operator=() just holds attr pointer
					if (auto attr = image->Attribute(L"file"))    file = attr;
					if (auto attr = image->IntAttribute(L"rows")) rows = attr;
					if (auto attr = image->IntAttribute(L"cols")) cols = attr;

					if (file.empty() && !image->NoChildren())
					{
						Array<const wchar_t*> files_arr;
						for (auto file = image->FirstChildElement(); file; file = file->NextSiblingElement())
						{
							if (auto path = file->Attribute(L"path"))
							{
								files_arr.push_back(path);
							}
						}
						if (!LoadImagesFromData(loader, &global_data, &id, &type, &file, &files_arr, rows, cols))
							return false;
					}
					else
					{
						if (!LoadImagesFromData(loader, &global_data, &id, &type, &file, nullptr, rows, cols))
							return false;
					}
				}
			}
			return true;
		}
	}

	namespace
	{
		Map<String, Closure<bool(ResLoader*, Json const&)>> load_json_funcs = {
			{ L"latest", __res_loader_01::LoadJsonData },
			{ L"0.1", __res_loader_01::LoadJsonData },
		};

		Map<String, Closure<bool(ResLoader*, tinyxml2::XMLElement*)>> load_xml_funcs = {
			{ L"latest", __res_loader_01::LoadXmlData },
			{ L"0.1", __res_loader_01::LoadXmlData },
		};
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
			KGE_WARNING_LOG(L"ResLoader::LoadFromJsonFile failed: Cannot open file. (%s)", string_to_wide(e.what()).c_str());
			return false;
		}
		catch (json_exception& e)
		{
			KGE_WARNING_LOG(L"ResLoader::LoadFromJsonFile failed: Cannot parse to JSON. (%s)", string_to_wide(e.what()).c_str());
			return false;
		}
		return LoadFromJson(json_data);
	}

	bool ResLoader::LoadFromJson(Json const& json_data)
	{
		try
		{
			String version = json_data[L"version"];

			auto load = load_json_funcs.find(version);
			if (load != load_json_funcs.end())
			{
				return load->second(this, json_data);
			}
			else if (version.empty())
			{
				return load_json_funcs[L"latest"](this, json_data);
			}
			else
			{
				throw std::runtime_error("unknown JSON data version");
			}
		}
		catch (std::exception& e)
		{
			KGE_WARNING_LOG(L"ResLoader::LoadFromJson failed: JSON data is invalid. (%s)", string_to_wide(e.what()).c_str());
			return false;
		}
		return false;
	}

	bool ResLoader::LoadFromXmlFile(String const& file_path)
	{
		tinyxml2::XMLDocument doc;

		std::wifstream ifs;
		ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			ifs.open(file_path.c_str());

			std::wstringstream ss;
			ss << ifs.rdbuf();

			if (tinyxml2::XML_SUCCESS != doc.Parse(ss.str().c_str()))
			{
				KGE_WARNING_LOG(L"ResLoader::LoadFromXmlFile failed: %s (%s)",
					tinyxml2::XMLDocument::ErrorIDToName(doc.ErrorID()), doc.ErrorStr());
				return false;
			}
		}
		catch (std::wifstream::failure& e)
		{
			KGE_WARNING_LOG(L"ResLoader::LoadFromXmlFile failed: Cannot open file. (%s)", string_to_wide(e.what()).c_str());
			return false;
		}

		return LoadFromXml(&doc);
	}

	bool ResLoader::LoadFromXml(tinyxml2::XMLDocument* doc)
	{
		if (doc)
		{
			try
			{
				if (auto root = doc->FirstChildElement(L"resources"))
				{
					kiwano::wstring version;
					if (auto ver = root->FirstChildElement(L"version")) version = ver->GetText();

					auto load = load_xml_funcs.find(version);
					if (load != load_xml_funcs.end())
					{
						return load->second(this, root);
					}
					else if (version.empty())
					{
						return load_xml_funcs[L"latest"](this, root);
					}
					else
					{
						throw std::runtime_error("unknown JSON data version");
					}
				}
			}
			catch (std::exception& e)
			{
				KGE_WARNING_LOG(L"ResLoader::LoadFromXml failed: %s", string_to_wide(e.what()).c_str());
				return false;
			}
		}
		return false;
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
			return AddFrames(id, frames);
		}
		return 0;
	}

	size_t ResLoader::AddFrames(String const& id, Array<ImagePtr> const& images)
	{
		if (images.empty())
			return 0;

		FramesPtr frames = new (std::nothrow) Frames(images);
		return AddFrames(id, frames);
	}

	size_t ResLoader::AddFrames(String const & id, Resource const & image, int cols, int rows)
	{
		if (cols <= 0 || rows <= 0)
			return 0;

		ImagePtr raw = new (std::nothrow) Image;
		if (!raw || !raw->Load(image))
			return false;

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
					ptr->Crop(Rect{ j * width, i * height, width, height });
					image_arr.push_back(ptr);
				}
			}
		}

		FramesPtr frames = new (std::nothrow) Frames(image_arr);
		return AddFrames(id, frames);
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
		return AddFrames(id, frames);
	}

	size_t ResLoader::AddFrames(String const & id, FramesPtr frames)
	{
		if (frames)
		{
			res_.insert(std::make_pair(id, frames));
			return frames->GetFrames().size();
		}
		return 0;
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

	void ResLoader::Delete(String const & id)
	{
		res_.erase(id);
	}

	void ResLoader::Destroy()
	{
		res_.clear();
	}

	ResLoader::ResLoader()
	{
	}

	ResLoader::~ResLoader()
	{
		Destroy();
	}

}