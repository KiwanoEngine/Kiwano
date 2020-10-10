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
#include <kiwano/render/Texture.h>

namespace kiwano
{
KGE_DECLARE_SMART_PTR(KeyFrame);

/**
 * \~chinese
 * @brief �ؼ�֡
 */
class KGE_API KeyFrame : public ObjectBase
{
public:
    KeyFrame();

    /// \~chinese
    /// @brief �����ؼ�֡
    /// @param file_path ͼ��·��
    KeyFrame(const String& file_path);

    /// \~chinese
    /// @brief �����ؼ�֡
    /// @param res ͼ����Դ
    KeyFrame(const Resource& res);

    /// \~chinese
    /// @brief �����ؼ�֡
    /// @param texture ����
    KeyFrame(TexturePtr texture);

    /// \~chinese
    /// @brief �����ؼ�֡
    /// @param file_path ͼ��·��
    /// @param crop_rect �ü�����
    KeyFrame(const String& file_path, const Rect& crop_rect);

    /// \~chinese
    /// @brief �����ؼ�֡
    /// @param res ͼ����Դ
    /// @param crop_rect �ü�����
    KeyFrame(const Resource& res, const Rect& crop_rect);

    /// \~chinese
    /// @brief �����ؼ�֡
    /// @param texture ����
    /// @param crop_rect �ü�����
    KeyFrame(TexturePtr texture, const Rect& crop_rect);

    /// \~chinese
    /// @brief ����ͼ��
    /// @param file_path ͼ��·��
    bool Load(const String& file_path);

    /// \~chinese
    /// @brief ����ͼ��
    /// @param res ͼ����Դ
    bool Load(const Resource& res);

    /// \~chinese
    /// @brief �Ƿ���Ч
    bool IsValid() const;

    /// \~chinese
    /// @brief �ü��ؼ�֡Ϊ����
    /// @param crop_rect �ü�����
    void SetCropRect(const Rect& crop_rect);

    /// \~chinese
    /// @brief ��������
    /// @param texture ����
    void SetTexture(TexturePtr texture);

    /// \~chinese
    /// @brief ��ȡ�ü�����
    const Rect& GetCropRect() const;

    /// \~chinese
    /// @brief ��ȡ����
    TexturePtr GetTexture() const;

private:
    TexturePtr texture_;
    Rect       crop_rect_;
};

inline bool KeyFrame::IsValid() const
{
    return texture_ && texture_->IsValid();
}

inline const Rect& KeyFrame::GetCropRect() const
{
    return crop_rect_;
}

inline TexturePtr KeyFrame::GetTexture() const
{
    return texture_;
}

}  // namespace kiwano
