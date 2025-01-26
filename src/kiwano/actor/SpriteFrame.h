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
#include <kiwano/core/Common.h>
#include <kiwano/math/Math.h>
#include <kiwano/render/Texture.h>

namespace kiwano
{

/**
 * \~chinese
 * @brief ����֡
 */
class KGE_API SpriteFrame
{
public:
    SpriteFrame();

    /// \~chinese
    /// @brief ��������֡
    /// @param file_path ͼ��·��
    SpriteFrame(StringView file_path);

    /// \~chinese
    /// @brief ��������֡
    /// @param res ͼ����Դ
    SpriteFrame(const Resource& res);

    /// \~chinese
    /// @brief ��������֡
    /// @param texture ����
    SpriteFrame(RefPtr<Texture> texture);

    /// \~chinese
    /// @brief ��������֡
    /// @param file_path ͼ��·��
    /// @param crop_rect �ü�����
    SpriteFrame(StringView file_path, const Rect& crop_rect);

    /// \~chinese
    /// @brief ��������֡
    /// @param res ͼ����Դ
    /// @param crop_rect �ü�����
    SpriteFrame(const Resource& res, const Rect& crop_rect);

    /// \~chinese
    /// @brief ��������֡
    /// @param texture ����
    /// @param crop_rect �ü�����
    SpriteFrame(RefPtr<Texture> texture, const Rect& crop_rect);

    /// \~chinese
    /// @brief ����ͼ��
    /// @param file_path ͼ��·��
    bool Load(StringView file_path);

    /// \~chinese
    /// @brief ����ͼ��
    /// @param res ͼ����Դ
    bool Load(const Resource& res);

    /// \~chinese
    /// @brief �Ƿ���Ч
    bool IsValid() const;

    /// \~chinese
    /// @brief ��ȡ�ü�����
    const Rect& GetCropRect() const;

    /// \~chinese
    /// @brief ��ȡ����
    RefPtr<Texture> GetTexture() const;

    /// \~chinese
    /// @brief ��ȡ����֡��С
    Size GetSize() const;

    /// \~chinese
    /// @brief �ü�����֡Ϊ����
    /// @param crop_rect �ü�����
    void SetCropRect(const Rect& crop_rect);

    /// \~chinese
    /// @brief �����������òü�����
    /// @param texture ����
    void SetTexture(RefPtr<Texture> texture);

    /// \~chinese
    /// @brief �����зָ��֡
    /// @param cols ����
    /// @param rows ����
    /// @param max_num ���֡��������-1Ϊ���ָ���ͼ��ȫ����Ϊ����֡
    /// @param padding_x X������
    /// @param padding_y Y������
    Vector<SpriteFrame> Split(int cols, int rows, int max_num = -1, float padding_x = 0, float padding_y = 0);

private:
    RefPtr<Texture> texture_;
    Rect            crop_rect_;
};

inline bool SpriteFrame::IsValid() const
{
    return texture_ && texture_->IsValid();
}

inline const Rect& SpriteFrame::GetCropRect() const
{
    return crop_rect_;
}

inline RefPtr<Texture> SpriteFrame::GetTexture() const
{
    return texture_;
}

inline Size SpriteFrame::GetSize() const
{
    return crop_rect_.GetSize();
}

}  // namespace kiwano
