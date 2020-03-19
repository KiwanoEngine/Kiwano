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
 * @brief ����
 */
class KGE_API Sprite : public Actor
{
public:
    /// \~chinese
    /// @brief ��������
    /// @param file_path ����ͼƬ·��
    static SpritePtr Create(const String& file_path);

    /// \~chinese
    /// @brief ��������
    /// @param res ͼƬ��Դ
    static SpritePtr Create(const Resource& res);

    /// \~chinese
    /// @brief ��������
    /// @param frame ͼ��֡
    static SpritePtr Create(FramePtr frame);

    /// \~chinese
    /// @brief ��������
    /// @param file_path ����ͼƬ·��
    /// @param crop_rect �ü�����
    static SpritePtr Create(const String& file_path, const Rect& crop_rect);

    /// \~chinese
    /// @brief ��������
    /// @param res ͼƬ��Դ
    /// @param crop_rect �ü�����
    static SpritePtr Create(const Resource& res, const Rect& crop_rect);

    Sprite();

    virtual ~Sprite();

    /// \~chinese
    /// @brief ���ر���ͼƬ
    /// @param file_path ����ͼƬ·��
    /// @param autoresize �Ƿ��Զ����������СΪͼ���С
    bool Load(const String& file_path, bool autoresize = true);

    /// \~chinese
    /// @brief ����ͼ����Դ
    /// @param res ͼƬ��Դ
    /// @param autoresize �Ƿ��Զ����������СΪͼ���С
    bool Load(const Resource& res, bool autoresize = true);

    /// \~chinese
    /// @brief ��ȡͼ��ԭ���
    float GetSourceWidth() const;

    /// \~chinese
    /// @brief ��ȡͼ��ԭ�߶�
    float GetSourceHeight() const;

    /// \~chinese
    /// @brief ��ȡͼ��ԭ��С
    Size GetSourceSize() const;

    /// \~chinese
    /// @brief ��ȡ�ü�����
    Rect GetCropRect() const;

    /// \~chinese
    /// @brief ʹ�þ�������ü�����
    /// @param crop_rect �ü�����
    void SetCropRect(const Rect& crop_rect);

    /// \~chinese
    /// @brief ��ȡ֡ͼ��
    FramePtr GetFrame() const;

    /// \~chinese
    /// @brief ����ͼ��֡
    /// @param[in] frame ͼ��֡
    /// @param autoresize �Ƿ��Զ����������СΪͼ���С
    void SetFrame(FramePtr frame, bool autoresize = true);

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
