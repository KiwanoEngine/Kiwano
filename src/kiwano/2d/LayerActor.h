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
#include <kiwano/render/Layer.h>
#include <kiwano/render/RenderContext.h>

namespace kiwano
{
KGE_DECLARE_SMART_PTR(LayerActor);

/**
 * \addtogroup Actors
 * @{
 */

/**
 * \~chinese
 * @brief ͼ���ɫ
 */
class KGE_API LayerActor : public Actor
{
public:
    /// \~chinese
    /// @brief ����ͼ��
    static LayerActorPtr Create();

    LayerActor();

    virtual ~LayerActor();

    /// \~chinese
    /// @brief �Ƿ�����Ϣ��û
    bool IsSwallowEventsEnabled() const;

    /// \~chinese
    /// @brief ��ȡͼ��
    LayerPtr GetLayer() const;

    /// \~chinese
    /// @brief ����ͼ��
    /// @param layer ͼ��
    void SetLayer(LayerPtr layer);

    /// \~chinese
    /// @brief ������Ϣ��û����
    /// @param enabled �Ƿ�����
    void SetSwallowEvents(bool enabled);

    /// \~chinese
    /// @brief ����ͼ��͸����
    /// @param opacity ͸����
    void SetOpacity(float opacity) override;

    bool DispatchEvent(Event* evt) override;

protected:
    void Render(RenderContext& ctx) override;

    bool CheckVisibility(RenderContext& ctx) const override;

private:
    bool     swallow_;
    LayerPtr layer_;
};

/** @} */

inline bool LayerActor::IsSwallowEventsEnabled() const
{
    return swallow_;
}

inline void LayerActor::SetSwallowEvents(bool enabled)
{
    swallow_ = enabled;
}

inline void LayerActor::SetLayer(LayerPtr layer)
{
    layer_ = layer;
}

inline LayerPtr LayerActor::GetLayer() const
{
    return layer_;
}

}  // namespace kiwano
