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
#include <kiwano/2d/action/Action.h>
#include <kiwano/core/Logger.h>

namespace kiwano
{
/// \~chinese
/// @brief ��������
using EaseFunc = Function<float(float)>;

/// \~chinese
/// @brief ��������ö��
/// @details �鿴 https://easings.net ��ȡ������Ϣ
struct Ease
{
    static KGE_API EaseFunc Linear;        ///< ����
    static KGE_API EaseFunc EaseIn;        ///< �������
    static KGE_API EaseFunc EaseOut;       ///< �ɿ����
    static KGE_API EaseFunc EaseInOut;     ///< �������, ���ɿ����
    static KGE_API EaseFunc ExpoIn;        ///< �����伫��
    static KGE_API EaseFunc ExpoOut;       ///< �ɼ������
    static KGE_API EaseFunc ExpoInOut;     ///< ����������, ���ɼ������
    static KGE_API EaseFunc ElasticIn;     ///< ����㸳�赯��
    static KGE_API EaseFunc ElasticOut;    ///< ���յ㸳�赯��
    static KGE_API EaseFunc ElasticInOut;  ///< �������յ㸳�赯��
    static KGE_API EaseFunc BounceIn;      ///< ����㸳�跴����
    static KGE_API EaseFunc BounceOut;     ///< ���յ㸳�跴����
    static KGE_API EaseFunc BounceInOut;   ///< �������յ㸳�跴����
    static KGE_API EaseFunc BackIn;
    static KGE_API EaseFunc BackOut;
    static KGE_API EaseFunc BackInOut;
    static KGE_API EaseFunc QuadIn;
    static KGE_API EaseFunc QuadOut;
    static KGE_API EaseFunc QuadInOut;
    static KGE_API EaseFunc CubicIn;
    static KGE_API EaseFunc CubicOut;
    static KGE_API EaseFunc CubicInOut;
    static KGE_API EaseFunc QuartIn;
    static KGE_API EaseFunc QuartOut;
    static KGE_API EaseFunc QuartInOut;
    static KGE_API EaseFunc QuintIn;
    static KGE_API EaseFunc QuintOut;
    static KGE_API EaseFunc QuintInOut;
    static KGE_API EaseFunc SineIn;
    static KGE_API EaseFunc SineOut;
    static KGE_API EaseFunc SineInOut;
};

KGE_DECLARE_SMART_PTR(ActionTween);
KGE_DECLARE_SMART_PTR(ActionMoveBy);
KGE_DECLARE_SMART_PTR(ActionMoveTo);
KGE_DECLARE_SMART_PTR(ActionJumpBy);
KGE_DECLARE_SMART_PTR(ActionJumpTo);
KGE_DECLARE_SMART_PTR(ActionScaleBy);
KGE_DECLARE_SMART_PTR(ActionScaleTo);
KGE_DECLARE_SMART_PTR(ActionFadeTo);
KGE_DECLARE_SMART_PTR(ActionFadeIn);
KGE_DECLARE_SMART_PTR(ActionFadeOut);
KGE_DECLARE_SMART_PTR(ActionRotateBy);
KGE_DECLARE_SMART_PTR(ActionRotateTo);
KGE_DECLARE_SMART_PTR(ActionCustom);

/**
 * \addtogroup Actions
 * @{
 */

/// \~chinese
/// @brief ���䶯��
class KGE_API ActionTween : public Action
{
public:
    ActionTween();

    /// \~chinese
    /// @brief ���䶯��
    /// @param duration ����ʱ��
    /// @param func �����ٶȻ�������
    ActionTween(Duration duration, EaseFunc func);

    /// \~chinese
    /// @brief ��ȡ����ʱ��
    Duration GetDuration() const;

    /// \~chinese
    /// @brief ���ö���ʱ��
    void SetDuration(Duration duration);

    /// \~chinese
    /// @brief ��ȡ�����ٶȻ�������
    EaseFunc const& GetEaseFunc() const;

    /// \~chinese
    /// @brief ���ö����ٶȻ�������
    void SetEaseFunc(EaseFunc const& func);

protected:
    void Update(Actor* target, Duration dt) override;

    virtual void UpdateTween(Actor* target, float percent) = 0;

private:
    Duration dur_;
    EaseFunc ease_func_;
};

/// \~chinese
/// @brief ���λ�ƶ���
class KGE_API ActionMoveBy : public ActionTween
{
public:
    /// \~chinese
    /// @brief �������λ�ƶ���
    /// @param duration ����ʱ��
    /// @param vector �ƶ�����
    /// @param func �����ٶȻ�������
    ActionMoveBy(Duration duration, Vec2 const& vector, EaseFunc func = nullptr);

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    ActionPtr Reverse() const override;

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float percent) override;

protected:
    Point start_pos_;
    Point prev_pos_;
    Vec2  delta_pos_;
};

/// \~chinese
/// @brief λ�ƶ���
class KGE_API ActionMoveTo : public ActionMoveBy
{
public:
    /// \~chinese
    /// @brief ����λ�ƶ���
    /// @param duration ����ʱ��
    /// @param pos Ŀ������
    /// @param func �����ٶȻ�������
    ActionMoveTo(Duration duration, Point const& pos, EaseFunc func = nullptr);

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    virtual ActionPtr Reverse() const override
    {
        KGE_ERROR("Reverse() not supported in ActionMoveTo");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

private:
    Point end_pos_;
};

/// \~chinese
/// @brief �����Ծ����
class KGE_API ActionJumpBy : public ActionTween
{
public:
    /// \~chinese
    /// @brief ���������Ծ����
    /// @param duration ����ʱ��
    /// @param vec ��Ծλ������
    /// @param height ��Ծ�߶�
    /// @param jumps ��Ծ����
    /// @param func �����ٶȻ�������
    ActionJumpBy(Duration duration, Vec2 const& vec, float height, int jumps = 1, EaseFunc func = nullptr);

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    ActionPtr Reverse() const override;

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float percent) override;

protected:
    Point start_pos_;
    Point delta_pos_;
    float height_;
    int   jumps_;
    Point prev_pos_;
};

/// \~chinese
/// @brief ��Ծ����
class KGE_API ActionJumpTo : public ActionJumpBy
{
public:
    /// \~chinese
    /// @brief ������Ծ����
    /// @param duration ����ʱ��
    /// @param pos Ŀ������
    /// @param height ��Ծ�߶�
    /// @param jumps ��Ծ����
    /// @param func �����ٶȻ�������
    ActionJumpTo(Duration duration, Point const& pos, float height, int jumps = 1, EaseFunc func = nullptr);

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    virtual ActionPtr Reverse() const override
    {
        KGE_ERROR("Reverse() not supported in ActionJumpTo");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

private:
    Point end_pos_;
};

/// \~chinese
/// @brief ������Ŷ���
class KGE_API ActionScaleBy : public ActionTween
{
public:
    /// \~chinese
    /// @brief ����������Ŷ���
    /// @param duration ����ʱ��
    /// @param scale_x ����������Ա仯ֵ
    /// @param scale_y ����������Ա仯ֵ
    /// @param func �����ٶȻ�������
    ActionScaleBy(Duration duration, float scale_x, float scale_y, EaseFunc func = nullptr);

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    ActionPtr Reverse() const override;

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float percent) override;

protected:
    float start_scale_x_;
    float start_scale_y_;
    float delta_x_;
    float delta_y_;
};

/// \~chinese
/// @brief ���Ŷ���
class KGE_API ActionScaleTo : public ActionScaleBy
{
public:
    /// \~chinese
    /// @brief �������Ŷ���
    /// @param duration ����ʱ��
    /// @param scale_x ��������Ŀ��ֵ
    /// @param scale_y ��������Ŀ��ֵ
    /// @param func �����ٶȻ�������
    ActionScaleTo(Duration duration, float scale_x, float scale_y, EaseFunc func = nullptr);

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    virtual ActionPtr Reverse() const override
    {
        KGE_ERROR("Reverse() not supported in ActionScaleTo");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

private:
    float end_scale_x_;
    float end_scale_y_;
};

/// \~chinese
/// @brief ͸���Ƚ��䶯��
class KGE_API ActionFadeTo : public ActionTween
{
public:
    /// \~chinese
    /// @brief ����͸���Ƚ��䶯��
    /// @param duration ����ʱ��
    /// @param opacity Ŀ��͸����
    /// @param func �����ٶȻ�������
    ActionFadeTo(Duration duration, float opacity, EaseFunc func = nullptr);

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    virtual ActionPtr Reverse() const override
    {
        KGE_ERROR("Reverse() not supported in ActionFadeTo");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float percent) override;

private:
    float start_val_;
    float delta_val_;
    float end_val_;
};

/// \~chinese
/// @brief ���붯��
class KGE_API ActionFadeIn : public ActionFadeTo
{
public:
    /// \~chinese
    /// @brief ���쵭�붯��
    /// @param duration ����ʱ��
    /// @param func �����ٶȻ�������
    explicit ActionFadeIn(Duration duration, EaseFunc func = nullptr);
};

/// \~chinese
/// @brief ��������
class KGE_API ActionFadeOut : public ActionFadeTo
{
public:
    /// \~chinese
    /// @brief ���쵭������
    /// @param duration ����ʱ��
    /// @param func �����ٶȻ�������
    explicit ActionFadeOut(Duration duration, EaseFunc func = nullptr);
};

/// \~chinese
/// @brief �����ת����
class KGE_API ActionRotateBy : public ActionTween
{
public:
    /// \~chinese
    /// @brief ���������ת����
    /// @param duration ����ʱ��
    /// @param rotation �Ƕ���Ա仯ֵ
    /// @param func �����ٶȻ�������
    ActionRotateBy(Duration duration, float rotation, EaseFunc func = nullptr);

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    ActionPtr Reverse() const override;

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float percent) override;

protected:
    float start_val_;
    float delta_val_;
};

/// \~chinese
/// @brief ��ת����
class KGE_API ActionRotateTo : public ActionRotateBy
{
public:
    /// \~chinese
    /// @brief ������ת����
    /// @param duration ����ʱ��
    /// @param rotation Ŀ��Ƕ�
    /// @param func �����ٶȻ�������
    ActionRotateTo(Duration duration, float rotation, EaseFunc func = nullptr);

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    virtual ActionPtr Reverse() const override
    {
        KGE_ERROR("Reverse() not supported in ActionRotateTo");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

private:
    float end_val_;
};

/// \~chinese
/// @brief �Զ��嶯��
class KGE_API ActionCustom : public ActionTween
{
public:
    /// \~chinese
    /// @brief �����ص�����
    /// @details �ڶ�������ʱ�ص��ú�������һ��������ִ�ж�����Ŀ�꣬�ڶ��������Ƕ������ȣ�0.0 - 1.0��
    using TweenFunc = Function<void(Actor* /* target */, float /* percent */)>;

    /// \~chinese
    /// @brief �����Զ��嶯��
    /// @param duration ����ʱ��
    /// @param tween_func �����ص�����
    /// @param func �����ٶȻ�������
    ActionCustom(Duration duration, TweenFunc tween_func, EaseFunc func = nullptr);

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    ActionPtr Reverse() const override
    {
        KGE_ERROR("Reverse() not supported in ActionCustom");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float percent) override;

private:
    TweenFunc tween_func_;
};

/** @} */

}  // namespace kiwano
