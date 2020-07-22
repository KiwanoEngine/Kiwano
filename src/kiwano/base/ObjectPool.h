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

#pragma once
#include <kiwano/core/Singleton.h>
#include <kiwano/base/ObjectBase.h>
#include <mutex>

namespace kiwano
{

/**
 * \~chinese
 * @brief 对象池
 */
class KGE_API ObjectPool : public Singleton<ObjectPool>
{
    friend Singleton<ObjectPool>;

public:
    ObjectPool();

    virtual ~ObjectPool();

    /**
     * \~chinese
     * @brief 添加对象到内存池
     * @param[in] obj 引用计数对象
     */
    void AddObject(RefObject* obj);

    /**
     * \~chinese
     * @brief 判断对象是否在对象池中
     * @param[in] obj 引用计数对象
     */
    bool Contains(RefObject* obj) const;

    /**
     * \~chinese
     * @brief 清空所有对象
     */
    void Clear();

private:
    ObjectPool(const ObjectPool&) = delete;

    ObjectPool& operator=(const ObjectPool&) = delete;

private:
    std::mutex          mutex_;
    Vector<RefObject*> objects_;
};

}  // namespace kiwano
