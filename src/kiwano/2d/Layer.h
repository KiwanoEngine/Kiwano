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
#include <kiwano/render/LayerArea.h>
#include <kiwano/render/RenderContext.h>

namespace kiwano
{
KGE_DECLARE_SMART_PTR(Layer);

/**
 * \addtogroup Actors
 * @{
 */

/**
 * \~chinese
 * @brief 图层
 */
class KGE_API Layer : public Actor
{
public:
    Layer();

    virtual ~Layer();

    /// \~chinese
    /// @brief 是否开启消息吞没
    bool IsSwallowEventsEnabled() const;

    /// \~chinese
    /// @brief 设置消息吞没功能
    /// @param enabled 是否启用
    void SetSwallowEvents(bool enabled);

    /// \~chinese
    /// @brief 设置裁剪区域
    /// @param clip_rect 裁剪矩形
    void SetClipRect(Rect const& clip_rect);

    /// \~chinese
    /// @brief 设置图层透明度
    /// @param opacity 透明度
    void SetOpacity(float opacity) override;

    /// \~chinese
    /// @brief 设置几何蒙层
    /// @param mask 蒙层的几何形状
    void SetMaskShape(ShapePtr mask);

    /// \~chinese
    /// @brief 设置几何蒙层的二维变换
    /// @param transform 应用于蒙层的二维变换
    void SetMaskTransform(Matrix3x2 const& transform);

    /// \~chinese
    /// @brief 设置图层区域
    /// @param area 图层区域属性
    void SetArea(LayerArea const& area);

    /// \~chinese
    /// @brief 获取图层区域
    LayerArea const& GetArea() const;

    bool DispatchEvent(Event* evt) override;

protected:
    void Render(RenderContext& ctx) override;

    bool CheckVisibility(RenderContext& ctx) const override;

private:
    bool      swallow_;
    LayerArea area_;
};

/** @} */

inline bool Layer::IsSwallowEventsEnabled() const
{
    return swallow_;
}

inline void Layer::SetSwallowEvents(bool enabled)
{
    swallow_ = enabled;
}

inline void Layer::SetArea(LayerArea const& area)
{
    area_ = area;
}

inline LayerArea const& Layer::GetArea() const
{
    return area_;
}
}  // namespace kiwano
