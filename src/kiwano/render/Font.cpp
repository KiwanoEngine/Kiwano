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

#include <kiwano/render/Font.h>
#include <kiwano/render/Renderer.h>
#include <functional>  // std::hash
#include <cctype>      // std::tolower

namespace kiwano
{

RefPtr<FontCollection> FontCollection::Preload(const Vector<String>& files)
{
    RefPtr<FontCollection> ptr = MakePtr<FontCollection>();
    if (ptr)
    {
        Renderer::GetInstance().CreateFontCollection(*ptr, ptr->family_names_, files);
        if (ptr->IsValid())
        {
            FontCache::GetInstance().AddFontCollection(ptr);
        }
    }
    return ptr;
}

RefPtr<FontCollection> FontCollection::Preload(const Vector<Resource>& resources)
{
    RefPtr<FontCollection> ptr = MakePtr<FontCollection>();
    if (ptr)
    {
        Vector<BinaryData> datas;
        datas.reserve(resources.size());
        for (const auto& res : resources)
        {
            datas.emplace_back(res.GetData());
        }

        Renderer::GetInstance().CreateFontCollection(*ptr, ptr->family_names_, datas);
        if (ptr->IsValid())
        {
            FontCache::GetInstance().AddFontCollection(ptr);
        }
    }
    return ptr;
}

Font::Font(StringView family_name, float size, uint32_t weight, FontPosture posture)
    : size(size)
    , weight(weight)
    , posture(posture)
    , family_name(family_name)
{
    collection = FontCache::GetInstance().GetFontCollection(family_name);
}

Font::Font(RefPtr<FontCollection> collection, float size, uint32_t weight, FontPosture posture)
    : collection(collection)
    , size(size)
    , weight(weight)
    , posture(posture)
    , family_name()
{
    const auto& names = collection->GetFamilyNames();
    if (names.size() > 0)
    {
        family_name = names[0];
    }
}

FontCache::FontCache() {}

FontCache::~FontCache() {}

void FontCache::AddFontCollection(RefPtr<FontCollection> collection)
{
    for (const auto& name : collection->GetFamilyNames())
    {
        FontCache::GetInstance().AddFontCollection(name, collection);
    }
}

void FontCache::AddFontCollection(StringView font_family, RefPtr<FontCollection> collection)
{
    String family             = TransformFamily(font_family);
    collection_cache_[family] = collection;
}

RefPtr<FontCollection> FontCache::GetFontCollection(StringView font_family) const
{
    String family = TransformFamily(font_family);
    if (collection_cache_.count(family))
    {
        return collection_cache_.at(family);
    }
    return {};
}

void FontCache::RemoveFontCollection(StringView font_family)
{
    String family = TransformFamily(font_family);
    collection_cache_.erase(family);
}

void FontCache::Clear()
{
    collection_cache_.clear();
}

String FontCache::TransformFamily(String family) const
{
    std::transform(family.begin(), family.end(), family.begin(), [](unsigned char c) { return std::tolower(c); });
    return family;
}

}  // namespace kiwano
