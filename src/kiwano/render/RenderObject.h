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
#include <kiwano/core/Function.h>
#include <kiwano/core/IntrusiveList.hpp>
#include <kiwano/core/BaseObject.h>

namespace kiwano
{

class RenderContext;

/**
 * \addtogroup Render
 * @{
 */

/**
 * \~chinese
 * @brief 渲染对象
 */
class KGE_API RenderObject : protected IntrusiveListValue<RenderObject*>
{
    friend IntrusiveList<RenderObject*>;

public:
    /// \~chinese
    /// @brief 执行渲染
    /// @details 每帧画面刷新时调用该函数，重载该函数以实现具体渲染过程
    /// @param ctx 渲染上下文
    virtual void OnRender(RenderContext& ctx) = 0;

    /// \~chinese
    /// @brief 渲染完成后
    /// @param ctx 渲染上下文
    virtual inline void AfterRender(RenderContext& ctx) {}
};

/**
 * \~chinese
 * @brief 内联渲染对象
 */
class KGE_API InlineRenderObject : public RenderObject
{
public:
    InlineRenderObject() = default;

    InlineRenderObject(Function<void(RenderContext&)> render_func);

    void OnRender(RenderContext& ctx) override;

    Function<void(RenderContext&)> render_func;
};

/**
 * \~chinese
 * @brief 渲染对象组
 */
class KGE_API RenderGroup : public RenderObject
{
public:
    RenderGroup() = default;

    void PushRenderObject(RenderObject* render_object);

    const IntrusiveList<RenderObject*>& GetRenderList() const;

    void Clear();

    void OnRender(RenderContext& ctx) override;

    void AfterRender(RenderContext& ctx) override;

private:
    IntrusiveList<RenderObject*> render_list_;
};

/** @} */

inline InlineRenderObject::InlineRenderObject(Function<void(RenderContext&)> f)
    : render_func(f)
{
}

inline void InlineRenderObject::OnRender(RenderContext& ctx)
{
    render_func(ctx);
}

inline const IntrusiveList<RenderObject*>& RenderGroup::GetRenderList() const
{
    return render_list_;
}

}  // namespace kiwano
