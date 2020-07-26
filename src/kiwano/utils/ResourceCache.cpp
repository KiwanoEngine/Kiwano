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

#include <kiwano/utils/ResourceCache.h>

namespace kiwano
{

ResourceCache::ResourceCache() {}

ResourceCache::~ResourceCache()
{
    Clear();
}

void ResourceCache::AddObject(const String& id, ObjectBasePtr obj)
{
    object_cache_.insert(std::make_pair(id, obj));
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
