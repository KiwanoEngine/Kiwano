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
#include <kiwano/core/Resource.h>
#include <kiwano/render/GifImage.h>
#include <kiwano/render/RenderContext.h>

namespace kiwano
{

/**
 * \addtogroup Actors
 * @{
 */

/**
 * \~chinese
 * @brief GIF 精灵
 */
class KGE_API GifSprite : public Actor
{
public:
    /// \~chinese
    /// @brief GIF播放循环结束回调
    using LoopDoneCallback = Function<void(int /* times */)>;

    /// \~chinese
    /// @brief GIF播放结束回调
    using DoneCallback = Function<void()>;

    GifSprite();

    /// \~chinese
    /// @brief 创建GIF精灵
    /// @param file_path GIF图片路径
    GifSprite(StringView file_path);

    /// \~chinese
    /// @brief 创建GIF精灵
    /// @param res GIF图片资源
    GifSprite(const Resource& res);

    /// \~chinese
    /// @brief 创建GIF精灵
    /// @param gif GIF图片
    GifSprite(RefPtr<GifImage> gif);

    /// \~chinese
    /// @brief 加载GIF图片
    /// @param file_path GIF图片路径
    bool Load(StringView file_path);

    /// \~chinese
    /// @brief 加载GIF图片
    /// @param res GIF图片资源
    bool Load(const Resource& res);

    /// \~chinese
    /// @brief 加载GIF图片
    /// @param gif GIF图片
    bool Load(RefPtr<GifImage> gif);

    /// \~chinese
    /// @brief 设置 GIF 动画循环次数
    void SetLoopCount(int loops);

    /// \~chinese
    /// @brief 设置 GIF 动画每次循环结束回调函数
    void SetLoopDoneCallback(const LoopDoneCallback& cb);

    /// \~chinese
    /// @brief 设置 GIF 动画结束回调函数
    void SetDoneCallback(const DoneCallback& cb);

    /// \~chinese
    /// @brief 设置 GIF 图像
    void SetGifImage(RefPtr<GifImage> gif);

    /// \~chinese
    /// @brief 重新播放 GIF 动画
    void RestartAnimation();

    /// \~chinese
    /// @brief 获取 GIF 动画循环结束回调
    LoopDoneCallback GetLoopDoneCallback() const;

    /// \~chinese
    /// @brief 获取 GIF 动画播放结束回调
    DoneCallback GetDoneCallback() const;

    /// \~chinese
    /// @brief 获取 GIF 图片
    RefPtr<GifImage> GetGifImage() const;

    void OnRender(RenderContext& ctx) override;

private:
    void Update(Duration dt) override;

    /// \~chinese
    /// @brief 是否是最后一帧
    bool IsLastFrame() const;

    /// \~chinese
    /// @brief 动画是否已结束
    bool EndOfAnimation() const;

    /// \~chinese
    /// @brief 合成下一帧
    void ComposeNextFrame();

    /// \~chinese
    /// @brief 解析当前关键帧
    void DisposeCurrentFrame();

    /// \~chinese
    /// @brief 覆盖下一帧
    void OverlayNextFrame();

    /// \~chinese
    /// @brief 保存合成后的关键帧
    void SaveComposedFrame();

    /// \~chinese
    /// @brief 恢复已保存的关键帧
    void RestoreSavedFrame();

    /// \~chinese
    /// @brief 清空当前图像区域
    void ClearCurrentFrameArea();

private:
    bool                  animating_;
    int                   total_loop_count_;
    int                   loop_count_;
    size_t                next_index_;
    Duration              frame_elapsed_;
    LoopDoneCallback      loop_cb_;
    DoneCallback          done_cb_;
    RefPtr<GifImage>      gif_;
    GifImage::Frame       frame_;
    RefPtr<Bitmap>        saved_frame_;
    RefPtr<Bitmap>        frame_to_render_;
    RefPtr<RenderContext> frame_rt_;
};

/** @} */

inline void GifSprite::SetLoopCount(int loops)
{
    total_loop_count_ = loops;
}

inline void GifSprite::SetLoopDoneCallback(const LoopDoneCallback& cb)
{
    loop_cb_ = cb;
}

inline void GifSprite::SetDoneCallback(const DoneCallback& cb)
{
    done_cb_ = cb;
}

inline GifSprite::LoopDoneCallback GifSprite::GetLoopDoneCallback() const
{
    return loop_cb_;
}

inline GifSprite::DoneCallback GifSprite::GetDoneCallback() const
{
    return done_cb_;
}

inline RefPtr<GifImage> GifSprite::GetGifImage() const
{
    return gif_;
}

inline bool GifSprite::IsLastFrame() const
{
    return (next_index_ == 0);
}

inline bool GifSprite::EndOfAnimation() const
{
    return IsLastFrame() && loop_count_ == total_loop_count_ + 1;
}
}  // namespace kiwano
