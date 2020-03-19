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
    ActionTween(Duration duration, EaseFunc ease);

    /// \~chinese
    /// @brief ��ȡ����ʱ��
    Duration GetDuration() const;

    /// \~chinese
    /// @brief ���ö���ʱ��
    void SetDuration(Duration duration);

    /// \~chinese
    /// @brief ��ȡ�����ٶȻ�������
    const EaseFunc& GetEaseFunc() const;

    /// \~chinese
    /// @brief ���ö����ٶȻ�������
    void SetEaseFunc(const EaseFunc& func);

protected:
    void Update(Actor* target, Duration dt) override;

    virtual void UpdateTween(Actor* target, float percent) = 0;

    ActionPtr InnerClone(ActionTweenPtr to) const;

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
    /// @param displacement λ������
    static ActionMoveByPtr Create(Duration duration, const Vec2& displacement);

    ActionMoveBy();

    /// \~chinese
    /// @brief ��ȡλ������
    Vec2 GetDisplacement() const;

    /// \~chinese
    /// @brief ����λ������
    void SetDisplacement(const Vec2& displacement);

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
    Vec2  displacement_;
};

/// \~chinese
/// @brief λ�ƶ���
class KGE_API ActionMoveTo : public ActionMoveBy
{
public:
    /// \~chinese
    /// @brief ����λ�ƶ���
    /// @param duration ����ʱ��
    /// @param distination Ŀ������
    static ActionMoveToPtr Create(Duration duration, const Point& distination);

    ActionMoveTo();

    /// \~chinese
    /// @brief ��ȡĿ������
    Point GetDistination() const;

    /// \~chinese
    /// @brief ����Ŀ������
    void SetDistination(const Point& distination);

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
    Point distination_;
};

/// \~chinese
/// @brief �����Ծ����
class KGE_API ActionJumpBy : public ActionTween
{
public:
    /// \~chinese
    /// @brief ���������Ծ����
    /// @param duration ����ʱ��
    /// @param displacement ��Ծλ������
    /// @param height ��Ծ�߶�
    /// @param count ��Ծ����
    static ActionJumpByPtr Create(Duration duration, const Vec2& displacement, float height, int count = 1,
                                  EaseFunc ease = nullptr);

    ActionJumpBy();

    /// \~chinese
    /// @brief ��ȡ��Ծλ��
    Vec2 GetDisplacement() const;

    /// \~chinese
    /// @brief ��ȡ��Ծ�߶�
    float GetJumpHeight() const;

    /// \~chinese
    /// @brief ��ȡ��Ծ����
    int GetJumpCount() const;

    /// \~chinese
    /// @brief ������Ծλ��
    void SetDisplacement(const Vec2& displacement);

    /// \~chinese
    /// @brief ������Ծ�߶�
    void SetJumpHeight(float height);

    /// \~chinese
    /// @brief ������Ծ����
    void SetJumpCount(int count);

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
    float height_;
    int   jump_count_;
    Point start_pos_;
    Point displacement_;
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
    /// @param distination Ŀ������
    /// @param height ��Ծ�߶�
    /// @param count ��Ծ����
    static ActionJumpToPtr Create(Duration duration, const Point& distination, float height, int count = 1,
                                  EaseFunc ease = nullptr);

    ActionJumpTo();

    /// \~chinese
    /// @brief ��ȡĿ������
    Point GetDistination() const;

    /// \~chinese
    /// @brief ����Ŀ������
    void SetDistination(const Point& distination);

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
    Point distination_;
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
    static ActionScaleByPtr Create(Duration duration, float scale_x, float scale_y);

    ActionScaleBy();

    /// \~chinese
    /// @brief ��ȡ����������Ա仯ֵ
    float GetScaleX() const;

    /// \~chinese
    /// @brief ��ȡ����������Ա仯ֵ
    float GetScaleY() const;

    /// \~chinese
    /// @brief ��������������Ա仯ֵ
    void SetScaleX(float scale_x);

    /// \~chinese
    /// @brief ��������������Ա仯ֵ
    void SetScaleY(float scale_y);

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
    static ActionScaleToPtr Create(Duration duration, float scale_x, float scale_y);

    ActionScaleTo();

    /// \~chinese
    /// @brief ��ȡ��������Ŀ��ֵ
    float GetTargetScaleX() const;

    /// \~chinese
    /// @brief ��ȡ��������Ŀ��ֵ
    float GetTargetScaleY() const;

    /// \~chinese
    /// @brief ������������Ŀ��ֵ
    void SetTargetScaleX(float scale_x);

    /// \~chinese
    /// @brief ������������Ŀ��ֵ
    void SetTargetScaleY(float scale_y);

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
    static ActionFadeToPtr Create(Duration duration, float opacity);

    ActionFadeTo();

    /// \~chinese
    /// @brief ��ȡĿ��͸����
    float GetTargetOpacity() const;

    /// \~chinese
    /// @brief ����Ŀ��͸����
    void SetTargetOpacity(float opacity);

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
    /// @brief �������붯��
    /// @param duration ����ʱ��
    static ActionFadeInPtr Create(Duration duration);
};

/// \~chinese
/// @brief ��������
class KGE_API ActionFadeOut : public ActionFadeTo
{
public:
    /// \~chinese
    /// @brief ������������
    /// @param duration ����ʱ��
    static ActionFadeOutPtr Create(Duration duration);
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
    static ActionRotateByPtr Create(Duration duration, float rotation);

    ActionRotateBy();

    /// \~chinese
    /// @brief ��ȡ�Ƕ���Ա仯ֵ
    float GetRotation() const;

    /// \~chinese
    /// @brief ���ýǶ���Ա仯ֵ
    void SetRotation(float rotation);

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
    static ActionRotateToPtr Create(Duration duration, float rotation);

    ActionRotateTo();

    /// \~chinese
    /// @brief ��ȡĿ��Ƕ�
    float GetTargetRotation() const;

    /// \~chinese
    /// @brief ����Ŀ��Ƕ�
    void SetTargetRotation(float rotation);

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
    static ActionCustomPtr Create(Duration duration, TweenFunc tween_func);

    ActionCustom();

    /// \~chinese
    /// @brief ��ȡ�����ص�����
    TweenFunc GetTweenFunc() const;

    /// \~chinese
    /// @brief ���ö����ص�����
    void SetTweenFunc(const TweenFunc& tween_func);

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

inline const EaseFunc& ActionTween::GetEaseFunc() const
{
    return ease_func_;
}

inline Duration ActionTween::GetDuration() const
{
    return dur_;
}

inline void ActionTween::SetDuration(Duration duration)
{
    dur_ = duration;
}

inline void ActionTween::SetEaseFunc(const EaseFunc& func)
{
    ease_func_ = func;
}

inline Vec2 ActionMoveBy::GetDisplacement() const
{
    return displacement_;
}

inline void ActionMoveBy::SetDisplacement(const Vec2& displacement)
{
    displacement_ = displacement;
}

inline Point ActionMoveTo::GetDistination() const
{
    return distination_;
}

inline void ActionMoveTo::SetDistination(const Point& distination)
{
    distination_ = distination;
}

inline Vec2 ActionJumpBy::GetDisplacement() const
{
    return displacement_;
}

inline float ActionJumpBy::GetJumpHeight() const
{
    return height_;
}

inline int ActionJumpBy::GetJumpCount() const
{
    return jump_count_;
}

inline void ActionJumpBy::SetDisplacement(const Vec2& displacement)
{
    displacement_ = displacement;
}

inline void ActionJumpBy::SetJumpHeight(float height)
{
    height_ = height;
}

inline void ActionJumpBy::SetJumpCount(int count)
{
    jump_count_ = count;
}

inline Point ActionJumpTo::GetDistination() const
{
    return distination_;
}

inline void ActionJumpTo::SetDistination(const Point& distination)
{
    distination_ = distination;
}

inline float ActionScaleBy::GetScaleX() const
{
    return delta_x_;
}

inline float ActionScaleBy::GetScaleY() const
{
    return delta_y_;
}

inline void ActionScaleBy::SetScaleX(float scale_x)
{
    delta_x_ = scale_x;
}

inline void ActionScaleBy::SetScaleY(float scale_y)
{
    delta_y_ = scale_y;
}

inline float ActionScaleTo::GetTargetScaleX() const
{
    return end_scale_x_;
}

inline float ActionScaleTo::GetTargetScaleY() const
{
    return end_scale_y_;
}

inline void ActionScaleTo::SetTargetScaleX(float scale_x)
{
    end_scale_x_ = scale_x;
}

inline void ActionScaleTo::SetTargetScaleY(float scale_y)
{
    end_scale_y_ = scale_y;
}

inline float ActionFadeTo::GetTargetOpacity() const
{
    return end_val_;
}

inline void ActionFadeTo::SetTargetOpacity(float opacity)
{
    end_val_ = opacity;
}

inline float ActionRotateBy::GetRotation() const
{
    return delta_val_;
}

inline void ActionRotateBy::SetRotation(float rotation)
{
    delta_val_ = rotation;
}

inline float ActionRotateTo::GetTargetRotation() const
{
    return end_val_;
}

inline void ActionRotateTo::SetTargetRotation(float rotation)
{
    end_val_ = rotation;
}

inline ActionCustom::TweenFunc ActionCustom::GetTweenFunc() const
{
    return tween_func_;
}

inline void ActionCustom::SetTweenFunc(const TweenFunc& tween_func)
{
    tween_func_ = tween_func;
}

}  // namespace kiwano
