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
#include <kiwano/2d/animation/TweenAnimation.h>
#include <kiwano/render/Bitmap.h>

namespace kiwano
{

/**
 * \addtogroup Animation
 * @{
 */

/**
 * \~chinese
 * @brief 精灵帧
 */
struct KGE_API SpriteFrame
{
    SpriteFrame() = default;

    /// \~chinese
    /// @brief 创建精灵帧
    /// @param image 位图
    /// @param src_rect 源矩形（裁剪矩形）
    explicit SpriteFrame(RefPtr<Image> image, const Rect& src_rect = Rect(), bool reset_sprite_size = true);

    /// \~chinese
    /// @brief 按行列分割位图
    /// @param bitmap 位图
    /// @param cols 列数
    /// @param rows 行数
    /// @param max_num 最大帧数量，设-1为将分割后的图像全部作为序列帧
    /// @param padding_x X方向间隔
    /// @param padding_y Y方向间隔
    inline static Vector<SpriteFrame> SplitBitmap(RefPtr<Bitmap> bitmap, int cols, int rows, int max_num = -1,
                                                  float padding_x = 0, float padding_y = 0)
    {
        return SpriteFrame::SplitBitmap(bitmap, Rect(Point(), bitmap->GetSize()), cols, rows, max_num, padding_x,
                                        padding_y);
    }

    /// \~chinese
    /// @brief 按行列分割位图
    /// @param bitmap 位图
    /// @param src_rect 位图源矩形（裁剪矩形）
    /// @param cols 列数
    /// @param rows 行数
    /// @param max_num 最大帧数量，设-1为将分割后的图像全部作为序列帧
    /// @param padding_x X方向间隔
    /// @param padding_y Y方向间隔
    static Vector<SpriteFrame> SplitBitmap(RefPtr<Bitmap> bitmap, const Rect& src_rect, int cols, int rows,
                                           int max_num = -1, float padding_x = 0, float padding_y = 0);

    bool           reset_sprite_size = true;
    RefPtr<Image>  image;
    RefPtr<Bitmap> bitmap;
    Rect           src_rect;
};

/**
 * \~chinese
 * @brief 序列帧
 */
struct KGE_API FrameSequence
{
    /// \~chinese
    /// @brief 构建空序列帧
    FrameSequence() = default;

    /// \~chinese
    /// @brief 创建序列帧
    /// @param dur 动画时长
    /// @param frames 精灵帧集合
    inline FrameSequence(Duration dur, const Vector<SpriteFrame>& frames)
        : duration(dur)
        , frames(frames)
    {
    }

    /// \~chinese
    /// @brief 获取序列帧的倒转
    FrameSequence Reverse() const;

    Duration            duration;
    Vector<SpriteFrame> frames;
};

/// \~chinese
/// @brief 帧动画
class KGE_API FrameAnimation : public TweenAnimation
{
public:
    FrameAnimation();

    /// \~chinese
    /// @brief 创建帧动画
    /// @param frame_seq_list 序列帧集合
    explicit FrameAnimation(const Vector<FrameSequence>& frame_seq_list);

    /// \~chinese
    /// @brief 获取序列帧
    const Vector<FrameSequence>& GetFrameSequences() const;

    /// \~chinese
    /// @brief 获取序列帧
    const FrameSequence& GetCurrentFrameSequence() const;

    /// \~chinese
    /// @brief 设置序列帧集合
    /// @param frame_seq_list 序列帧集合
    void SetFrameSequences(const Vector<FrameSequence>& frame_seq_list);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    FrameAnimation* Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    FrameAnimation* Reverse() const override;

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float percent) override;

    void SetTargetFrame(Actor* target, const SpriteFrame& frame);

private:
    size_t                current_index_;
    Vector<float>         progress_;
    Vector<FrameSequence> frame_seq_list_;
};

/** @} */

inline const Vector<FrameSequence>& FrameAnimation::GetFrameSequences() const
{
    return frame_seq_list_;
}

inline const FrameSequence& FrameAnimation::GetCurrentFrameSequence() const
{
    return frame_seq_list_[current_index_];
}

}  // namespace kiwano
