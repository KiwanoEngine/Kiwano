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
#include <kiwano/2d/SpriteFrame.h>

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
    Sprite();

    /// \~chinese
    /// @brief ��������
    /// @param file_path ����ͼƬ·��
    Sprite(StringView file_path);

    /// \~chinese
    /// @brief ��������
    /// @param res ͼƬ��Դ
    Sprite(const Resource& res);

    /// \~chinese
    /// @brief ��������
    /// @param texture ͼ��
    Sprite(TexturePtr texture);

    /// \~chinese
    /// @brief ��������
    /// @param file_path ����ͼƬ·��
    /// @param crop_rect �ü�����
    Sprite(StringView file_path, const Rect& crop_rect);

    /// \~chinese
    /// @brief ��������
    /// @param res ͼƬ��Դ
    /// @param crop_rect �ü�����
    Sprite(const Resource& res, const Rect& crop_rect);

    /// \~chinese
    /// @brief ��������
    /// @param texture ͼ��
    /// @param crop_rect �ü�����
    Sprite(TexturePtr texture, const Rect& crop_rect);

    /// \~chinese
    /// @brief ��������
    /// @param frame ����֡
    Sprite(const SpriteFrame& frame);

    virtual ~Sprite();

    /// \~chinese
    /// @brief ���ر���ͼƬ�����òü�����
    /// @param file_path ����ͼƬ·��
    bool Load(StringView file_path);

    /// \~chinese
    /// @brief ����ͼ����Դ�����òü�����
    /// @param res ͼƬ��Դ
    bool Load(const Resource& res);

    /// \~chinese
    /// @brief ��ȡͼ��
    TexturePtr GetTexture() const;

    /// \~chinese
    /// @brief ��ȡ�ü�����
    Rect GetCropRect() const;

    /// \~chinese
    /// @brief ��ȡ����֡
    SpriteFrame GetFrame() const;

    /// \~chinese
    /// @brief ʹ�þ�������ü�����
    /// @param crop_rect �ü�����
    void SetCropRect(const Rect& crop_rect);

    /// \~chinese
    /// @brief ���þ���֡
    /// @param[in] frame ����֡
    void SetFrame(const SpriteFrame& frame);

    void OnRender(RenderContext& ctx) override;

protected:
    bool CheckVisibility(RenderContext& ctx) const override;

private:
    SpriteFrame frame_;
};

/** @} */

inline TexturePtr Sprite::GetTexture() const
{
    return frame_.GetTexture();
}

inline Rect Sprite::GetCropRect() const
{
    return frame_.GetCropRect();
}

inline SpriteFrame Sprite::GetFrame() const
{
    return frame_;
}

inline void Sprite::SetCropRect(const Rect& crop_rect)
{
    frame_.SetCropRect(crop_rect);
}

}  // namespace kiwano
