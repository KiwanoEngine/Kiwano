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
#include <kiwano/render/LayerArea.h>
#include <kiwano/render/RenderContext.h>

namespace kiwano
{
KGE_DECLARE_SMART_PTR(Layer);

/**
 * \addtogroup Actors
 * @{
 */

/**
 * \~chinese
 * @brief ͼ��
 */
class KGE_API Layer : public Actor
{
public:
    Layer();

    virtual ~Layer();

    /// \~chinese
    /// @brief �Ƿ�����Ϣ��û
    bool IsSwallowEventsEnabled() const;

    /// \~chinese
    /// @brief ������Ϣ��û����
    /// @param enabled �Ƿ�����
    void SetSwallowEvents(bool enabled);

    /// \~chinese
    /// @brief ���òü�����
    /// @param clip_rect �ü�����
    void SetClipRect(Rect const& clip_rect);

    /// \~chinese
    /// @brief ����ͼ��͸����
    /// @param opacity ͸����
    void SetOpacity(float opacity) override;

    /// \~chinese
    /// @brief ���ü����ɲ�
    /// @param mask �ɲ�ļ�����״
    void SetMaskShape(ShapePtr mask);

    /// \~chinese
    /// @brief ���ü����ɲ�Ķ�ά�任
    /// @param transform Ӧ�����ɲ�Ķ�ά�任
    void SetMaskTransform(Matrix3x2 const& transform);

    /// \~chinese
    /// @brief ����ͼ������
    /// @param area ͼ����������
    void SetArea(LayerArea const& area);

    /// \~chinese
    /// @brief ��ȡͼ������
    LayerArea const& GetArea() const;

    bool DispatchEvent(Event* evt) override;

protected:
    void Render(RenderContext& ctx) override;

    bool CheckVisibility(RenderContext& ctx) const override;

private:
    bool      swallow_;
    LayerArea area_;
};

/** @} */

inline bool Layer::IsSwallowEventsEnabled() const
{
    return swallow_;
}

inline void Layer::SetSwallowEvents(bool enabled)
{
    swallow_ = enabled;
}

inline void Layer::SetArea(LayerArea const& area)
{
    area_ = area;
}

inline LayerArea const& Layer::GetArea() const
{
    return area_;
}
}  // namespace kiwano
