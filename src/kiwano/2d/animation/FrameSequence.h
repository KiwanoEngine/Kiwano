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
#include <kiwano/2d/animation/KeyFrame.h>
#include <kiwano/core/Common.h>
#include <kiwano/core/Time.h>

namespace kiwano
{
KGE_DECLARE_SMART_PTR(FrameSequence);

/**
 * \~chinese
 * @brief ����֡
 */
class KGE_API FrameSequence : public ObjectBase
{
public:
    /// \~chinese
    /// @brief ��������֡
    /// @param frames �ؼ�֡����
    FrameSequence(const Vector<KeyFramePtr>& frames);

    /// \~chinese
    /// @brief ����������֡
    FrameSequence();

    virtual ~FrameSequence();

    /// \~chinese
    /// @brief ��ӹؼ�֡
    /// @param frame �ؼ�֡
    void AddFrame(KeyFramePtr frame);

    /// \~chinese
    /// @brief ��Ӷ���ؼ�֡
    /// @param frames �ؼ�֡����
    void AddFrames(const Vector<KeyFramePtr>& frames);

    /// \~chinese
    /// @brief ��ȡ�ؼ�֡
    /// @param index �ؼ�֡�±�
    KeyFramePtr GetFrame(size_t index) const;

    /// \~chinese
    /// @brief ��ȡ���йؼ�֡
    const Vector<KeyFramePtr>& GetFrames() const;

    /// \~chinese
    /// @brief ��ȡ�ؼ�֡����
    size_t GetFramesCount() const;

    /// \~chinese
    /// @brief ��ȡ����֡�Ŀ�������
    FrameSequencePtr Clone() const;

    /// \~chinese
    /// @brief ��ȡ����֡�ĵ�ת
    FrameSequencePtr Reverse() const;

private:
    Vector<KeyFramePtr> frames_;
};

/// \~chinese
/// @brief ����֡ͼ��ָ���
struct KGE_API KeyFrameSpliter
{
    TexturePtr texture;
    Rect       crop_rect;

    KeyFrameSpliter() = default;

    /// \~chinese
    /// @brief ��������֡ͼ��ָ���
    /// @param texture ͼ��
    KeyFrameSpliter(TexturePtr texture);

    /// \~chinese
    /// @brief ��������֡ͼ��ָ���
    /// @param texture ͼ��
    /// @param crop_rect �ü�����
    KeyFrameSpliter(TexturePtr texture, const Rect& crop_rect);

    /// \~chinese
    /// @brief �����зָ�ͼ�񲢴�������֡
    /// @param cols ����
    /// @param rows ����
    /// @param max_num ���֡��������-1Ϊ���ָ���ͼ��ȫ����Ϊ����֡
    /// @param padding_x X������
    /// @param padding_y Y������
    Vector<KeyFramePtr> Split(int cols, int rows = 1, int max_num = -1, float padding_x = 0, float padding_y = 0);
};

}  // namespace kiwano
