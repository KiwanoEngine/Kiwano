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
#include <kiwano/utils/ResourceLoader.h>
#include <kiwano/utils/ResourceCache.h>

#include <kiwano/render/Font.h>
#include <kiwano/render/GifImage.h>
#include <kiwano/2d/animation/KeyFrame.h>
#include <kiwano/2d/animation/FrameSequence.h>

namespace kiwano
{
namespace resource_cache_01
{

void LoadJsonData(ResourceCache* cache, const Json& json_data);
void LoadXmlData(ResourceCache* cache, const XmlNode& elem);

}  // namespace resource_cache_01

namespace
{

Map<String, Function<void(ResourceCache*, const Json&)>> load_json_funcs = {
    { "latest", resource_cache_01::LoadJsonData },
    { "0.1", resource_cache_01::LoadJsonData },
};

Map<String, Function<void(ResourceCache*, const XmlNode&)>> load_xml_funcs = {
    { "latest", resource_cache_01::LoadXmlData },
    { "0.1", resource_cache_01::LoadXmlData },
};

}  // namespace

ResourceLoader::ResourceLoader(ResourceCache& cache)
    : cache_(cache)
{
}

void ResourceLoader::LoadFromJsonFile(const String& file_path)
{
    if (!FileSystem::GetInstance().IsFileExists(file_path))
    {
        cache_.Fail(strings::Format("ResourceLoader::LoadFromJsonFile failed: [%s] file not found.", file_path.c_str()));
        return;
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
        cache_.Fail(strings::Format("ResourceLoader::LoadFromJsonFile failed: cannot open file [%s]. %s",
                                    file_path.c_str(), e.what()));
        return;
    }
    catch (Json::exception& e)
    {
        cache_.Fail(strings::Format("ResourceLoader::LoadFromJsonFile failed: Json file [%s] parsed with errors: %s",
                                    file_path.c_str(), e.what()));
        return;
    }

    LoadFromJson(json_data);
}

void ResourceLoader::LoadFromJson(const Json& json_data)
{
    try
    {
        String version = json_data["version"];

        auto load = load_json_funcs.find(version);
        if (load != load_json_funcs.end())
        {
            load->second(&cache_, json_data);
        }
        else if (version.empty())
        {
            load_json_funcs["latest"](&cache_, json_data);
        }
        else
        {
            cache_.Fail("ResourceLoader::LoadFromJson failed: unknown resource data version");
        }
    }
    catch (Json::exception& e)
    {
        cache_.Fail(String("ResourceLoader::LoadFromJson failed: ") + e.what());
    }
}

void ResourceLoader::LoadFromXmlFile(const String& file_path)
{
    if (!FileSystem::GetInstance().IsFileExists(file_path))
    {
        cache_.Fail(strings::Format("ResourceLoader::LoadFromXmlFile failed: [%s] file not found.", file_path.c_str()));
        return;
    }

    String full_path = FileSystem::GetInstance().GetFullPathForFile(file_path);

    XmlDocument doc;

    auto result = doc.load_file(full_path.c_str());
    if (result)
    {
        LoadFromXml(doc);
    }
    else
    {
        cache_.Fail(strings::Format("ResourceLoader::LoadFromXmlFile failed: XML file [%s] parsed with errors: %s",
                                    file_path.c_str(), result.description()));
    }
}

void ResourceLoader::LoadFromXml(const XmlDocument& doc)
{
    if (XmlNode root = doc.child("resources"))
    {
        String version;
        if (auto version_node = root.child("version"))
            version = version_node.child_value();

        auto load = load_xml_funcs.find(version);
        if (load != load_xml_funcs.end())
        {
            load->second(&cache_, root);
        }
        else if (version.empty())
        {
            load_xml_funcs["latest"](&cache_, root);
        }
        else
        {
            cache_.Fail("ResourceLoader::LoadFromXml failed: unknown resource data version");
        }
    }
    else
    {
        cache_.Fail("ResourceLoader::LoadFromXml failed: unknown file format");
    }
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

void LoadTexturesFromData(ResourceCache* cache, GlobalData* gdata, const String& id, const String& type,
                          const String& file)
{
    if (type == "gif")
    {
        // GIF image
        GifImagePtr gif = GifImage::Preload(gdata->path + file);
        if (gif)
        {
            cache->AddObject(id, gif);
            return;
        }
    }
    else if (!file.empty())
    {
        // Simple image
        KeyFramePtr frame = MakePtr<KeyFrame>();
        if (frame && frame->Load(gdata->path + file))
        {
            cache->AddObject(id, frame);
            return;
        }
    }

    cache->Fail(strings::Format("%s failed", __FUNCTION__));
}

void LoadTexturesFromData(ResourceCache* cache, GlobalData* gdata, const String& id, const Vector<String>& files)
{
    if (files.empty())
        return;

    // Frames
    Vector<KeyFramePtr> frames;
    frames.reserve(files.size());
    for (const auto& file : files)
    {
        KeyFramePtr frame = MakePtr<KeyFrame>();
        if (frame->Load(gdata->path + file))
        {
            frames.push_back(frame);
        }
    }

    if (!frames.empty())
    {
        FrameSequencePtr frame_seq = MakePtr<FrameSequence>(frames);
        if (frame_seq)
        {
            cache->AddObject(id, frame_seq);
            return;
        }
    }

    cache->Fail(strings::Format("%s failed", __FUNCTION__));
}

void LoadTexturesFromData(ResourceCache* cache, GlobalData* gdata, const String& id, const String& file, int rows,
                          int cols, int max_num, float padding_x, float padding_y)
{
    if (!file.empty())
    {
        if (rows || cols)
        {
            // KeyFrame slices
            TexturePtr texture = MakePtr<Texture>();
            if (texture && texture->Load(gdata->path + file))
            {
                FrameSequencePtr frame_seq = MakePtr<FrameSequence>();
                if (frame_seq)
                {
                    KeyFrameSpliter spliter(texture);

                    auto frames = spliter.Split(cols, rows, max_num, padding_x, padding_y);
                    frame_seq->AddFrames(frames);
                    cache->AddObject(id, frame_seq);
                    return;
                }
            }
        }
        else
        {
            // Simple image
            KeyFramePtr frame = MakePtr<KeyFrame>();
            if (frame && frame->Load(gdata->path + file))
            {
                cache->AddObject(id, frame);
                return;
            }
        }
    }

    cache->Fail(strings::Format("%s failed", __FUNCTION__));
}

void LoadFontsFromData(ResourceCache* cache, GlobalData* gdata, const String& id, const String& file)
{
    FontPtr font = Font::Preload(gdata->path + file);
    if (font)
    {
        cache->AddObject(id, font);
        return;
    }
    cache->Fail(strings::Format("%s failed", __FUNCTION__));
}

void LoadJsonData(ResourceCache* cache, const Json& json_data)
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

                LoadTexturesFromData(cache, &global_data, id, file, rows, cols, max_num, padding_x, padding_y);
            }

            if (image.count("files"))
            {
                Vector<String> files;
                files.reserve(image["files"].size());
                for (const auto& file : image["files"])
                {
                    files.push_back(file.get<String>());
                }
                LoadTexturesFromData(cache, &global_data, id, files);
            }
            else
            {
                LoadTexturesFromData(cache, &global_data, id, type, file);
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

            LoadFontsFromData(cache, &global_data, id, file);
        }
    }
}

void LoadXmlData(ResourceCache* cache, const XmlNode& elem)
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

                LoadTexturesFromData(cache, &global_data, id, file, rows, cols, max_num, padding_x, padding_y);
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
                LoadTexturesFromData(cache, &global_data, id, files_arr);
            }
            else
            {
                LoadTexturesFromData(cache, &global_data, id, type, file);
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

            LoadFontsFromData(cache, &global_data, id, file);
        }
    }
}

}  // namespace resource_cache_01
}  // namespace kiwano
