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

#include <fstream>
#include <kiwano/platform/FileSystem.h>
#include <kiwano/utils/Logger.h>
#include <kiwano/utils/ResourceCache.h>

namespace kiwano
{
namespace resource_cache_01
{

bool LoadJsonData(ResourceCache* loader, const Json& json_data);
bool LoadXmlData(ResourceCache* loader, const XmlNode& elem);

}  // namespace resource_cache_01

namespace
{

Map<String, Function<bool(ResourceCache*, const Json&)>> load_json_funcs = {
    { "latest", resource_cache_01::LoadJsonData },
    { "0.1", resource_cache_01::LoadJsonData },
};

Map<String, Function<bool(ResourceCache*, const XmlNode&)>> load_xml_funcs = {
    { "latest", resource_cache_01::LoadXmlData },
    { "0.1", resource_cache_01::LoadXmlData },
};

}  // namespace

ResourceCache::ResourceCache() {}

ResourceCache::~ResourceCache()
{
    Clear();
}

bool ResourceCache::LoadFromJsonFile(const String& file_path)
{
    if (!FileSystem::GetInstance().IsFileExists(file_path))
    {
        KGE_ERRORF("%s failed: File not found.", __FUNCTION__);
        return false;
    }

    Json json_data;

    std::ifstream ifs;
    ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        String full_path = FileSystem::GetInstance().GetFullPathForFile(file_path);
        ifs.open(full_path.c_str());
        ifs >> json_data;
        ifs.close();
    }
    catch (std::ios_base::failure& e)
    {
        KGE_ERRORF("%s failed: cannot open file. (%s)", __FUNCTION__, e.what());
        return false;
    }
    catch (Json::exception& e)
    {
        KGE_ERRORF("%s failed: cannot parse JSON. (%s)", __FUNCTION__, e.what());
        return false;
    }
    return LoadFromJson(json_data);
}

bool ResourceCache::LoadFromJson(const Json& json_data)
{
    try
    {
        String version = json_data["version"];

        auto load = load_json_funcs.find(version);
        if (load != load_json_funcs.end())
        {
            return load->second(this, json_data);
        }
        else if (version.empty())
        {
            return load_json_funcs["latest"](this, json_data);
        }
        else
        {
            KGE_ERRORF("%s failed: unknown resource data version", __FUNCTION__);
        }
    }
    catch (Json::exception& e)
    {
        KGE_ERRORF("%s failed: JSON data is invalid. (%s)", __FUNCTION__, e.what());
        return false;
    }
    return false;
}

bool ResourceCache::LoadFromXmlFile(const String& file_path)
{
    if (!FileSystem::GetInstance().IsFileExists(file_path))
    {
        KGE_ERRORF("%s failed: File not found.", __FUNCTION__);
        return false;
    }

    String full_path = FileSystem::GetInstance().GetFullPathForFile(file_path);

    XmlDocument doc;

    auto result = doc.load_file(full_path.c_str());
    if (result)
    {
        return LoadFromXml(doc);
    }
    else
    {
        KGE_ERRORF("%s failed: XML [%s] parsed with errors: %s", __FUNCTION__, full_path.c_str(), result.description());
        return false;
    }
}

bool ResourceCache::LoadFromXml(const XmlDocument& doc)
{
    if (XmlNode root = doc.child("resources"))
    {
        String version;
        if (auto version_node = root.child("version"))
            version = version_node.child_value();

        auto load = load_xml_funcs.find(version);
        if (load != load_xml_funcs.end())
        {
            return load->second(this, root);
        }
        else if (version.empty())
        {
            return load_xml_funcs["latest"](this, root);
        }
        else
        {
            KGE_ERRORF("%s failed: unknown resource data version", __FUNCTION__);
        }
    }
    return false;
}

bool ResourceCache::AddObject(const String& id, ObjectBasePtr obj)
{
    if (obj)
    {
        object_cache_.insert(std::make_pair(id, obj));
        return true;
    }
    return false;
}

void ResourceCache::Remove(const String& id)
{
    object_cache_.erase(id);
}

void ResourceCache::Clear()
{
    object_cache_.clear();
}

ObjectBasePtr ResourceCache::Get(const String& id) const
{
    auto iter = object_cache_.find(id);
    if (iter == object_cache_.end())
        return nullptr;
    return (*iter).second;
}

}  // namespace kiwano

namespace kiwano
{
namespace resource_cache_01
{
struct GlobalData
{
    String path;
};

bool LoadTexturesFromData(ResourceCache* loader, GlobalData* gdata, const String& id, const String& type,
                          const String& file)
{
    if (!gdata)
        return false;

    if (type == "gif")
    {
        // GIF image
        GifImagePtr gif = MakePtr<GifImage>();
        if (gif && gif->Load(gdata->path + file))
        {
            return loader->AddObject(id, gif);
        }
    }

    if (!file.empty())
    {
        // Simple image
        FramePtr frame = MakePtr<Frame>();
        if (frame && frame->Load(gdata->path + file))
        {
            return loader->AddObject(id, frame);
        }
    }
    return false;
}

bool LoadTexturesFromData(ResourceCache* loader, GlobalData* gdata, const String& id, const Vector<String>& files)
{
    if (!gdata)
        return false;

    if (files.empty())
        return true;

    // Frames
    Vector<FramePtr> frames;
    frames.reserve(files.size());
    for (const auto& file : files)
    {
        FramePtr frame = MakePtr<Frame>();
        if (frame->Load(gdata->path + file))
        {
            frames.push_back(frame);
        }
    }
    FrameSequencePtr frame_seq = MakePtr<FrameSequence>(frames);
    if (frame_seq)
    {
        return !!loader->AddObject(id, frame_seq);
    }
    return false;
}

bool LoadTexturesFromData(ResourceCache* loader, GlobalData* gdata, const String& id, const String& file, int rows,
                          int cols, int max_num, float padding_x, float padding_y)
{
    if (!gdata)
        return false;

    if (!file.empty())
    {
        if (rows || cols)
        {
            // Frame slices
            FramePtr frame = MakePtr<Frame>();
            if (frame && frame->Load(gdata->path + file))
            {
                FrameSequencePtr frame_seq = MakePtr<FrameSequence>();
                if (frame_seq)
                {
                    frame_seq->AddFrames(frame, cols, rows, max_num, padding_x, padding_y);
                    return loader->AddObject(id, frame_seq);
                }
            }
        }
        else
        {
            // Simple image
            FramePtr frame = MakePtr<Frame>();
            if (frame && frame->Load(gdata->path + file))
            {
                return loader->AddObject(id, frame);
            }
        }
    }
    return false;
}

bool LoadFontsFromData(ResourceCache* loader, GlobalData* gdata, const String& id, const String& file)
{
    if (!gdata)
        return false;

    FontPtr font = MakePtr<Font>();
    if (font && font->Load(gdata->path + file))
    {
        return loader->AddObject(id, font);
    }
    return false;
}

bool LoadJsonData(ResourceCache* loader, const Json& json_data)
{
    GlobalData global_data;
    if (json_data.count("path"))
    {
        global_data.path = json_data["path"].get<String>();
    }

    if (json_data.count("images"))
    {
        for (const auto& image : json_data["images"])
        {
            String id, type, file;
            int    rows = 0, cols = 0, max_num = -1;

            if (image.count("id"))
                id = image["id"].get<String>();
            if (image.count("type"))
                type = image["type"].get<String>();
            if (image.count("file"))
                file = image["file"].get<String>();
            if (image.count("rows"))
                rows = image["rows"].get<int>();
            if (image.count("cols"))
                cols = image["cols"].get<int>();
            if (image.count("max_num"))
                max_num = image["max_num"].get<int>();

            if (rows || cols)
            {
                float padding_x = 0, padding_y = 0;
                if (image.count("padding-x"))
                    padding_x = image["padding-x"].get<float>();
                if (image.count("padding-y"))
                    padding_y = image["padding-y"].get<float>();

                if (!LoadTexturesFromData(loader, &global_data, id, file, rows, cols, max_num, padding_x, padding_y))
                    return false;
            }

            if (image.count("files"))
            {
                Vector<String> files;
                files.reserve(image["files"].size());
                for (const auto& file : image["files"])
                {
                    files.push_back(file.get<String>());
                }
                if (!LoadTexturesFromData(loader, &global_data, id, files))
                    return false;
            }
            else
            {
                if (!LoadTexturesFromData(loader, &global_data, id, type, file))
                    return false;
            }
        }
    }

    if (json_data.count("fonts"))
    {
        for (const auto& font : json_data["fonts"])
        {
            String id, file;

            if (font.count("id"))
                id = font["id"].get<String>();
            if (font.count("file"))
                file = font["file"].get<String>();

            if (!LoadFontsFromData(loader, &global_data, id, file))
                return false;
        }
    }
    return true;
}

bool LoadXmlData(ResourceCache* loader, const XmlNode& elem)
{
    GlobalData global_data;
    if (auto path = elem.child("path"))
    {
        global_data.path = path.child_value();
    }

    if (auto images = elem.child("images"))
    {
        for (auto image : images.children())
        {
            String id, type, file;
            int    rows = 0, cols = 0, max_num = -1;

            if (auto attr = image.attribute("id"))
                id = attr.value();
            if (auto attr = image.attribute("type"))
                type = attr.value();
            if (auto attr = image.attribute("file"))
                file = attr.value();
            if (auto attr = image.attribute("rows"))
                rows = attr.as_int(0);
            if (auto attr = image.attribute("cols"))
                cols = attr.as_int(0);
            if (auto attr = image.attribute("max_num"))
                max_num = attr.as_int(-1);

            if (rows || cols)
            {
                float padding_x = 0, padding_y = 0;
                if (auto attr = image.attribute("padding-x"))
                    padding_x = attr.as_float(0.0f);
                if (auto attr = image.attribute("padding-y"))
                    padding_y = attr.as_float(0.0f);

                if (!LoadTexturesFromData(loader, &global_data, id, file, rows, cols, max_num, padding_x, padding_y))
                    return false;
            }

            if (file.empty() && !image.empty())
            {
                Vector<String> files_arr;
                for (auto file : image.children())
                {
                    if (auto path = file.attribute("path"))
                    {
                        files_arr.push_back(path.value());
                    }
                }
                if (!LoadTexturesFromData(loader, &global_data, id, files_arr))
                    return false;
            }
            else
            {
                if (!LoadTexturesFromData(loader, &global_data, id, type, file))
                    return false;
            }
        }
    }

    if (auto fonts = elem.child("fonts"))
    {
        for (auto font : fonts.children())
        {
            String id, file;
            if (auto attr = font.attribute("id"))
                id = attr.value();
            if (auto attr = font.attribute("file"))
                file = attr.value();

            if (!LoadFontsFromData(loader, &global_data, id, file))
                return false;
        }
    }
    return true;
}
}  // namespace resource_cache_01
}  // namespace kiwano
