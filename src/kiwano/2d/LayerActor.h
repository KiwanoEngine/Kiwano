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
#include <kiwano/2d/Actor.h>
#include <kiwano/render/Layer.h>
#include <kiwano/render/RenderContext.h>

namespace kiwano
{

/**
 * \addtogroup Actors
 * @{
 */

/**
 * \~chinese
 * @brief 图层角色
 */
class KGE_API LayerActor : public Actor
{
public:
    LayerActor();

    virtual ~LayerActor();

    /// \~chinese
    /// @brief 是否开启消息吞没
    bool IsSwallowEventsEnabled() const;

    /// \~chinese
    /// @brief 获取图层
    RefPtr<Layer> GetLayer() const;

    /// \~chinese
    /// @brief 设置图层
    /// @param layer 图层
    void SetLayer(RefPtr<Layer> layer);

    /// \~chinese
    /// @brief 设置消息吞没功能
    /// @param enabled 是否启用
    void SetSwallowEvents(bool enabled);

    /// \~chinese
    /// @brief 设置图层透明度
    /// @param opacity 透明度
    void SetOpacity(float opacity) override;

    bool DispatchEvent(Event* evt) override;

protected:
    void Render(RenderContext& ctx) override;

    bool CheckVisibility(RenderContext& ctx) const override;

private:
    bool     swallow_;
    RefPtr<Layer> layer_;
};

/** @} */

inline bool LayerActor::IsSwallowEventsEnabled() const
{
    return swallow_;
}

inline void LayerActor::SetSwallowEvents(bool enabled)
{
    swallow_ = enabled;
}

inline void LayerActor::SetLayer(RefPtr<Layer> layer)
{
    layer_ = layer;
}

inline RefPtr<Layer> LayerActor::GetLayer() const
{
    return layer_;
}

}  // namespace kiwano
