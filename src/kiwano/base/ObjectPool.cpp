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

#include <kiwano/base/ObjectPool.h>

namespace kiwano
{

List<ObjectPool*> ObjectPool::pools_;

ObjectPool& ObjectPool::GetInstance()
{
    static ObjectPool instance;
    return *pools_.back();
}

ObjectPool::ObjectPool()
{
    pools_.push_back(this);
}

ObjectPool::~ObjectPool()
{
    Clear();

    auto iter = std::find(pools_.begin(), pools_.end(), this);
    if (iter != pools_.end())
        pools_.erase(iter);
}

void ObjectPool::AddObject(ObjectBase* obj)
{
    if (obj)
    {
        if (!Contains(obj))
        {
            obj->Retain();

            std::lock_guard<std::mutex> lock(mutex_);
            objects_.push_back(obj);
        }
    }
}

bool ObjectPool::Contains(ObjectBase* obj) const
{
    std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(mutex_));

    for (auto iter = pools_.rbegin(); iter != pools_.rend(); iter++)
        for (const auto o : (*iter)->objects_)
            if (obj == o)
                return true;
    return false;
}

void ObjectPool::Clear()
{
    Vector<ObjectBase*> copied;

    {
        std::lock_guard<std::mutex> lock(mutex_);
        copied = std::move(objects_);
    }

    for (auto obj : copied)
        obj->Release();
}

}  // namespace kiwano
