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
#include <kiwano/render/Frame.h>
#include <kiwano/render/ShapeMaker.h>
#include <kiwano/render/RenderContext.h>

namespace kiwano
{

KGE_DECLARE_SMART_PTR(Canvas);

/**
 * \addtogroup Actors
 * @{
 */

/**
 * \~chinese
 * @brief ����
 * @details ���ڻ���ͼ�Ρ�ͼ�����ֵȸ������͵�ͼԪ��ͬʱ���Խ��������ݵ�����ͼ��
 */
class KGE_API Canvas : public Actor
{
public:
    /// \~chinese
    /// @brief ��������
    /// @param size ������С
    Canvas(const Size& size);

    /// \~chinese
    /// @brief ��ȡ2D��ͼ������
    RenderContextPtr GetContext2D() const;

    /// \~chinese
    /// @brief ��ջ�����С�����軭����С
    /// @warning �ú����ᵼ��ԭ��ͼ������ʧЧ
    void ResizeAndClear(Size size);

    /// \~chinese
    /// @brief ��������
    TexturePtr ExportToTexture() const;

    void OnRender(RenderContext& ctx) override;

private:
    TexturePtr       texture_cached_;
    RenderContextPtr render_ctx_;
};

/** @} */

}  // namespace kiwano
