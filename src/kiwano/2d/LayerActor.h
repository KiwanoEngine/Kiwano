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
    /// \~chinese
    /// @brief 创建矩形图层
    /// @param bounds 图层边界
    /// @param opacity 图层透明度
    LayerActor(const Rect& bounds, float opacity = 1.f);

    /// \~chinese
    /// @brief 创建图层
    /// @param mask 几何蒙层
    /// @param mask_transform 几何蒙层变换
    /// @param opacity 图层透明度
    /// @param bounds 图层边界
    LayerActor(RefPtr<Shape> mask, const Matrix3x2& mask_transform = Matrix3x2(), float opacity = 1.f,
               const Rect& bounds = Rect::Infinite());

    virtual ~LayerActor();

    /// \~chinese
    /// @brief 获取图层
    const Layer& GetLayer() const;

    /// \~chinese
    /// @brief 获取图层
    Layer& GetLayer();

    /// \~chinese
    /// @brief 设置图层
    void SetLayer(const Layer& layer);

protected:
    void Update(Duration dt) override;

private:
    bool  swallow_;
    Layer layer_;
};

/** @} */

inline const Layer& LayerActor::GetLayer() const
{
    return layer_;
}

inline Layer& LayerActor::GetLayer()
{
    return layer_;
}

inline void LayerActor::SetLayer(const Layer& layer)
{
    layer_ = layer;
}

}  // namespace kiwano
