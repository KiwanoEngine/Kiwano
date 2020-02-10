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
#include <kiwano/2d/Frame.h>

namespace kiwano
{
KGE_DECLARE_SMART_PTR(Sprite);

/**
 * \addtogroup Actors
 * @{
 */

/**
 * \~chinese
 * @brief 精灵
 */
class KGE_API Sprite : public Actor
{
public:
    /// \~chinese
    /// @brief 创建精灵
    /// @param file_path 本地图片路径
    static SpritePtr Create(String const& file_path);

    /// \~chinese
    /// @brief 创建精灵
    /// @param res 图片资源
    static SpritePtr Create(Resource const& res);

    /// \~chinese
    /// @brief 创建精灵
    /// @param frame 图像帧
    static SpritePtr Create(FramePtr frame);

    Sprite();

    virtual ~Sprite();

    /// \~chinese
    /// @brief 加载本地图片
    /// @param file_path 本地图片路径
    bool Load(String const& file_path);

    /// \~chinese
    /// @brief 加载图像资源
    /// @param res 图片资源
    bool Load(Resource const& res);

    /// \~chinese
    /// @brief 使用矩形区域裁剪精灵
    /// @param crop_rect 裁剪矩形
    void SetCropRect(const Rect& crop_rect);

    /// \~chinese
    /// @brief 获取帧图像
    FramePtr GetFrame() const;

    /// \~chinese
    /// @brief 设置图像帧
    /// @param[in] frame 图像帧
    void SetFrame(FramePtr frame);

    void OnRender(RenderContext& ctx) override;

protected:
    bool CheckVisibility(RenderContext& ctx) const override;

private:
    FramePtr frame_;
};

/** @} */

inline FramePtr Sprite::GetFrame() const
{
    return frame_;
}
}  // namespace kiwano
