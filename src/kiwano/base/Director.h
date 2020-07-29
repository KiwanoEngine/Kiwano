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
#include <kiwano/2d/Stage.h>
#include <kiwano/2d/Transition.h>
#include <kiwano/base/Module.h>

namespace kiwano
{
/**
 * \~chinese
 * @brief ����
 * @details ���������̨����Ⱦ�����¡��¼��ַ��Լ�������̨����ת
 * @see kiwano::Stage
 */
class KGE_API Director
    : public Singleton<Director>
    , public Module
{
    friend Singleton<Director>;

public:
    /**
     * \~chinese
     * @brief �л���̨
     * @param[in] stage ��̨
     * @param[in] transition ���ɶ���
     */
    void EnterStage(StagePtr stage, TransitionPtr transition = nullptr);

    /**
     * \~chinese
     * @brief �л���̨��������ǰ��̨���浽ջ��
     * @param[in] stage ��̨
     * @param[in] transition ���ɶ���
     */
    void PushStage(StagePtr stage, TransitionPtr transition = nullptr);

    /**
     * \~chinese
     * @brief �˳���ǰ��̨�����л�����һ����̨
     * @param[in] transition ���ɶ���
     */
    void PopStage(TransitionPtr transition = nullptr);

    /**
     * \~chinese
     * @brief ��ȡ��ǰ��̨
     * @return ���ص�ǰ��̨��ָ��
     */
    StagePtr GetCurrentStage();

    /**
     * \~chinese
     * @brief ���û���ý�ɫ�߽���Ⱦ����
     * @param enabled �Ƿ�����
     */
    void SetRenderBorderEnabled(bool enabled);

    /**
     * \~chinese
     * @brief ��ʾ�����ص�����Ϣ
     * @param show �Ƿ���ʾ
     */
    void ShowDebugInfo(bool show = true);

    /**
     * \~chinese
     * @brief �˳���ǰ��̨�������̨ջ
     */
    void ClearStages();

public:
    void OnUpdate(UpdateModuleContext& ctx) override;

    void OnRender(RenderModuleContext& ctx) override;

    void HandleEvent(EventModuleContext& ctx) override;

    virtual ~Director();

private:
    Director();

private:
    bool            render_border_enabled_;
    Stack<StagePtr> stages_;
    StagePtr        current_stage_;
    StagePtr        next_stage_;
    ActorPtr        debug_actor_;
    TransitionPtr   transition_;
};
}  // namespace kiwano
