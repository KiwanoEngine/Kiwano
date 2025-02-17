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
 * @brief ����֡
 */
struct KGE_API SpriteFrame
{
    SpriteFrame() = default;

    /// \~chinese
    /// @brief ��������֡
    /// @param image λͼ
    /// @param src_rect Դ���Σ��ü����Σ�
    explicit SpriteFrame(RefPtr<Image> image, const Rect& src_rect = Rect(), bool reset_sprite_size = true);

    /// \~chinese
    /// @brief �����зָ�λͼ
    /// @param bitmap λͼ
    /// @param cols ����
    /// @param rows ����
    /// @param max_num ���֡��������-1Ϊ���ָ���ͼ��ȫ����Ϊ����֡
    /// @param padding_x X������
    /// @param padding_y Y������
    inline static Vector<SpriteFrame> SplitBitmap(RefPtr<Bitmap> bitmap, int cols, int rows, int max_num = -1,
                                                  float padding_x = 0, float padding_y = 0)
    {
        return SpriteFrame::SplitBitmap(bitmap, Rect(Point(), bitmap->GetSize()), cols, rows, max_num, padding_x,
                                        padding_y);
    }

    /// \~chinese
    /// @brief �����зָ�λͼ
    /// @param bitmap λͼ
    /// @param src_rect λͼԴ���Σ��ü����Σ�
    /// @param cols ����
    /// @param rows ����
    /// @param max_num ���֡��������-1Ϊ���ָ���ͼ��ȫ����Ϊ����֡
    /// @param padding_x X������
    /// @param padding_y Y������
    static Vector<SpriteFrame> SplitBitmap(RefPtr<Bitmap> bitmap, const Rect& src_rect, int cols, int rows,
                                           int max_num = -1, float padding_x = 0, float padding_y = 0);

    bool           reset_sprite_size = true;
    RefPtr<Image>  image;
    RefPtr<Bitmap> bitmap;
    Rect           src_rect;
};

/**
 * \~chinese
 * @brief ����֡
 */
struct KGE_API FrameSequence
{
    /// \~chinese
    /// @brief ����������֡
    FrameSequence() = default;

    /// \~chinese
    /// @brief ��������֡
    /// @param dur ����ʱ��
    /// @param frames ����֡����
    inline FrameSequence(Duration dur, const Vector<SpriteFrame>& frames)
        : duration(dur)
        , frames(frames)
    {
    }

    /// \~chinese
    /// @brief ��ȡ����֡�ĵ�ת
    FrameSequence Reverse() const;

    Duration            duration;
    Vector<SpriteFrame> frames;
};

/// \~chinese
/// @brief ֡����
class KGE_API FrameAnimation : public TweenAnimation
{
public:
    FrameAnimation();

    /// \~chinese
    /// @brief ����֡����
    /// @param frame_seq_list ����֡����
    explicit FrameAnimation(const Vector<FrameSequence>& frame_seq_list);

    /// \~chinese
    /// @brief ��ȡ����֡
    const Vector<FrameSequence>& GetFrameSequences() const;

    /// \~chinese
    /// @brief ��ȡ����֡
    const FrameSequence& GetCurrentFrameSequence() const;

    /// \~chinese
    /// @brief ��������֡����
    /// @param frame_seq_list ����֡����
    void SetFrameSequences(const Vector<FrameSequence>& frame_seq_list);

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    FrameAnimation* Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
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
