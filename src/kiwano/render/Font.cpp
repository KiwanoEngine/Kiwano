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
#include <cctype>  // std::tolower

namespace kiwano
{

FontPtr Font::Preload(const String& file)
{
    size_t hash_code = std::hash<String>{}(file);
    if (FontPtr ptr = FontCache::GetInstance().GetFont(hash_code))
    {
        return ptr;
    }

    FontPtr ptr = MakePtr<Font>();
    if (ptr)
    {
        Vector<String> family_names;
        Renderer::GetInstance().CreateFontCollection(*ptr, family_names, file);
        if (ptr->IsValid())
        {
            FontCache::GetInstance().AddFont(hash_code, ptr);
            if (!family_names.empty())
            {
                ptr->SetFamilyName(family_names[0]);
            }
            for (const auto& name : family_names)
            {
                FontCache::GetInstance().AddFontByFamily(name, ptr);
            }
        }
    }
    return ptr;
}

FontPtr Font::Preload(const Resource& resource)
{
    size_t hash_code = resource.GetId();
    if (FontPtr ptr = FontCache::GetInstance().GetFont(hash_code))
    {
        return ptr;
    }

    FontPtr ptr = MakePtr<Font>();
    if (ptr)
    {
        Vector<String> family_names;
        Renderer::GetInstance().CreateFontCollection(*ptr, family_names, resource);
        if (ptr->IsValid())
        {
            FontCache::GetInstance().AddFont(hash_code, ptr);
            if (!family_names.empty())
            {
                ptr->SetFamilyName(family_names[0]);
            }
            for (const auto& name : family_names)
            {
                FontCache::GetInstance().AddFontByFamily(name, ptr);
            }
        }
    }
    return ptr;
}

Font::Font()
    : size_(18.0f)
    , weight_(FontWeight::Normal)
    , posture_(FontPosture::Normal)
    , stretch_(FontStretch::Normal)
{
}

Font::Font(const String& family_name, float size, uint32_t weight, FontPosture posture, FontStretch stretch)
    : size_(size)
    , weight_(weight)
    , posture_(posture)
    , stretch_(stretch)
{
    if (family_name.empty())
        return;

    FontPtr found = FontCache::GetInstance().GetFontByFamily(family_name);
    if (found)
    {
        this->ResetNativePointer(found->GetNativePointer());
        this->SetFamilyName(family_name);
    }
    else
    {
        this->Fail(strings::Format("Font::Load failed: cannot find family name \"%s\"", family_name.c_str()));
    }
}

FontCache::FontCache() {}

FontCache::~FontCache() {}

void FontCache::AddFont(size_t key, FontPtr font)
{
    font_cache_.insert(std::make_pair(key, font));
}

void FontCache::AddFontByFamily(const String& font_family, FontPtr font)
{
    String family = TransformFamily(font_family);
    font_family_cache_.insert(std::make_pair(family, font));
}

FontPtr FontCache::GetFont(size_t key) const
{
    if (font_cache_.count(key))
    {
        return font_cache_.at(key);
    }
    return FontPtr();
}

FontPtr FontCache::GetFontByFamily(const String& font_family) const
{
    String family = TransformFamily(font_family);
    if (font_family_cache_.count(family))
    {
        return font_family_cache_.at(family);
    }
    return FontPtr();
}

void FontCache::RemoveFont(size_t key)
{
    font_cache_.erase(key);
}

void FontCache::RemoveFontByFamily(const String& font_family)
{
    String family = TransformFamily(font_family);
    font_family_cache_.erase(family);
}

void FontCache::Clear()
{
    font_cache_.clear();
    font_family_cache_.clear();
}

String FontCache::TransformFamily(String family) const
{
    std::transform(family.begin(), family.end(), family.begin(), [](unsigned char c) { return std::tolower(c); });
    return family;
}

}  // namespace kiwano
