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
#include <kiwano/core/UUID.h>
#include <kiwano/render/Bitmap.h>

namespace kiwano
{

/**
 * \addtogroup Render
 * @{
 */

/**
 * \~chinese
 * @brief ��ɫ��
 */
class KGE_API Shader : public NativeObject
{
public:
    /// \~chinese
    /// @brief ����ͼ��
    virtual RefPtr<Image> Process(RefPtr<Image> input) = 0;

protected:
    Shader() = default;
};

/**
 * \~chinese
 * @brief ������ɫ��
 */
class KGE_API PixelShader : public Shader
{
public:
    /// \~chinese
    /// @brief Ԥ����������ɫ��
    /// @param cso_path ��ɫ����ִ���ļ�·��
    static RefPtr<PixelShader> Preload(const kiwano::UUID& uuid, StringView cso_path);

    /// \~chinese
    /// @brief Ԥ����������ɫ��
    /// @param cso_path ��ɫ����ִ���ļ�
    static RefPtr<PixelShader> Preload(const kiwano::UUID& uuid, BinaryData cso_data);

    /// \~chinese
    /// @brief ����ͼ��
    RefPtr<Image> Process(RefPtr<Image> input) override;

    /// \~chinese
    /// @brief ���� Pixel Shader ������
    template <typename T>
    void SetConstants(const T& value)
    {
        SetConstants(reinterpret_cast<const uint8_t*>(&value), sizeof(value));
    }

    /// \~chinese
    /// @brief ���� Pixel Shader ������
    void SetConstants(const uint8_t* data, uint32_t size);

    /// \~chinese
    /// @brief ����ͼ����չ��Χ
    void SetExpansion(PixelSize left_top, PixelSize right_bottom);

    /// \~chinese
    /// @brief ��ȡΨһ��ʶ
    kiwano::UUID GetUUID() const;

protected:
    PixelShader(const kiwano::UUID& uuid);

protected:
    kiwano::UUID uuid_;
};

/** @} */

inline kiwano::UUID PixelShader::GetUUID() const
{
    return uuid_;
}

}  // namespace kiwano
