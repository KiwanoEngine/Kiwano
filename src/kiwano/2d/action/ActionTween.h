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
#include <kiwano/utils/Logger.h>

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

KGE_DECLARE_SMART_PTR(ActionTweenEntity);
KGE_DECLARE_SMART_PTR(ActionMoveByEntity);
KGE_DECLARE_SMART_PTR(ActionMoveToEntity);
KGE_DECLARE_SMART_PTR(ActionJumpByEntity);
KGE_DECLARE_SMART_PTR(ActionJumpToEntity);
KGE_DECLARE_SMART_PTR(ActionScaleByEntity);
KGE_DECLARE_SMART_PTR(ActionScaleToEntity);
KGE_DECLARE_SMART_PTR(ActionFadeToEntity);
KGE_DECLARE_SMART_PTR(ActionRotateByEntity);
KGE_DECLARE_SMART_PTR(ActionRotateToEntity);
KGE_DECLARE_SMART_PTR(ActionCustomEntity);

/**
 * \addtogroup Actions
 * @{
 */

/// \~chinese
/// @brief ���䶯��ʵ��
class KGE_API ActionTweenEntity : public ActionEntity
{
public:
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
    ActionTweenEntity();

    /// \~chinese
    /// @brief ���䶯��
    /// @param duration ����ʱ��
    /// @param func �����ٶȻ�������
    ActionTweenEntity(Duration duration);

    void Update(Actor* target, Duration dt) override;

    virtual void UpdateTween(Actor* target, float percent) = 0;

    void DoClone(ActionTweenEntity* to) const;

private:
    Duration dur_;
    EaseFunc ease_func_;
};

/// \~chinese
/// @brief ���䶯��
class ActionTween : public Action
{
public:
    ActionTween() = default;

    inline ActionTween(ActionTweenEntityPtr ptr)
    {
        SetEntity(ptr);
    }

    /// \~chinese
    /// @brief ���û�������
    inline ActionTween& Ease(EaseFunc ease)
    {
        auto tween_ptr = Get();
        if (tween_ptr)
            tween_ptr->SetEaseFunc(ease);
        return (*this);
    }

    /// \~chinese
    /// @brief ��ȡָ��
    inline ActionTweenEntity* Get() const
    {
        return static_cast<ActionTweenEntity*>(Action::Get());
    }

    /// \~chinese
    /// @brief ���ö���ʵ��
    inline void SetEntity(ActionTweenEntityPtr tween_ptr)
    {
        Action::SetEntity(tween_ptr.Get());
    }

    inline ActionTweenEntity* operator->() const
    {
        return Get();
    }
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
    ActionMoveBy(Duration duration, const Vec2& displacement);
};

/// \~chinese
/// @brief ���λ�ƶ���ʵ��
class KGE_API ActionMoveByEntity : public ActionTweenEntity
{
public:
    /// \~chinese
    /// @brief �������λ�ƶ���
    /// @param duration ����ʱ��
    /// @param displacement λ������
    ActionMoveByEntity(Duration duration, const Vec2& displacement);

    /// \~chinese
    /// @brief ��ȡλ������
    Vec2 GetDisplacement() const;

    /// \~chinese
    /// @brief ����λ������
    void SetDisplacement(const Vec2& displacement);

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    ActionMoveByEntity* Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    ActionMoveByEntity* Reverse() const override;

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
class KGE_API ActionMoveTo : public ActionTween
{
public:
    /// \~chinese
    /// @brief ����λ�ƶ���
    /// @param duration ����ʱ��
    /// @param distination Ŀ������
    ActionMoveTo(Duration duration, const Point& distination);
};

/// \~chinese
/// @brief λ�ƶ���ʵ��
class KGE_API ActionMoveToEntity : public ActionMoveByEntity
{
public:
    /// \~chinese
    /// @brief ����λ�ƶ���
    /// @param duration ����ʱ��
    /// @param distination Ŀ������
    ActionMoveToEntity(Duration duration, const Point& distination);

    /// \~chinese
    /// @brief ��ȡĿ������
    Point GetDistination() const;

    /// \~chinese
    /// @brief ����Ŀ������
    void SetDistination(const Point& distination);

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    ActionMoveToEntity* Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    ActionMoveToEntity* Reverse() const override
    {
        KGE_ERRORF("Reverse() not supported in ActionMoveToEntity");
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
    ActionJumpBy(Duration duration, const Vec2& displacement, float height, int count = 1);
};

/// \~chinese
/// @brief �����Ծ����ʵ��
class KGE_API ActionJumpByEntity : public ActionTweenEntity
{
public:
    /// \~chinese
    /// @brief ���������Ծ����
    /// @param duration ����ʱ��
    /// @param displacement ��Ծλ������
    /// @param height ��Ծ�߶�
    /// @param count ��Ծ����
    ActionJumpByEntity(Duration duration, const Vec2& displacement, float height, int count = 1);

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
    ActionJumpByEntity* Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    ActionJumpByEntity* Reverse() const override;

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
class KGE_API ActionJumpTo : public ActionTween
{
public:
    /// \~chinese
    /// @brief ������Ծ����
    /// @param duration ����ʱ��
    /// @param distination Ŀ������
    /// @param height ��Ծ�߶�
    /// @param count ��Ծ����
    ActionJumpTo(Duration duration, const Point& distination, float height, int count = 1);
};

/// \~chinese
/// @brief ��Ծ����ʵ��
class KGE_API ActionJumpToEntity : public ActionJumpByEntity
{
public:
    /// \~chinese
    /// @brief ������Ծ����
    /// @param duration ����ʱ��
    /// @param distination Ŀ������
    /// @param height ��Ծ�߶�
    /// @param count ��Ծ����
    ActionJumpToEntity(Duration duration, const Point& distination, float height, int count = 1);

    /// \~chinese
    /// @brief ��ȡĿ������
    Point GetDistination() const;

    /// \~chinese
    /// @brief ����Ŀ������
    void SetDistination(const Point& distination);

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    ActionJumpToEntity* Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    ActionJumpToEntity* Reverse() const override
    {
        KGE_ERRORF("Reverse() not supported in ActionJumpToEntity");
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
    ActionScaleBy(Duration duration, float scale_x, float scale_y);

    /// \~chinese
    /// @brief ����������Ŷ���
    /// @param duration ����ʱ��
    /// @param scale ������Ա仯ֵ
    ActionScaleBy(Duration duration, Vec2 scale);
};

/// \~chinese
/// @brief ������Ŷ���ʵ��
class KGE_API ActionScaleByEntity : public ActionTweenEntity
{
public:
    /// \~chinese
    /// @brief ����������Ŷ���
    /// @param duration ����ʱ��
    /// @param scale_x ����������Ա仯ֵ
    /// @param scale_y ����������Ա仯ֵ
    ActionScaleByEntity(Duration duration, float scale_x, float scale_y);

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
    ActionScaleByEntity* Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    ActionScaleByEntity* Reverse() const override;

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
class KGE_API ActionScaleTo : public ActionTween
{
public:
    /// \~chinese
    /// @brief �������Ŷ���
    /// @param duration ����ʱ��
    /// @param scale_x ��������Ŀ��ֵ
    /// @param scale_y ��������Ŀ��ֵ
    ActionScaleTo(Duration duration, float scale_x, float scale_y);

    /// \~chinese
    /// @brief �������Ŷ���
    /// @param duration ����ʱ��
    /// @param scale ����Ŀ��ֵ
    ActionScaleTo(Duration duration, Vec2 scale);
};

/// \~chinese
/// @brief ���Ŷ���ʵ��
class KGE_API ActionScaleToEntity : public ActionScaleByEntity
{
public:
    /// \~chinese
    /// @brief �������Ŷ���
    /// @param duration ����ʱ��
    /// @param scale_x ��������Ŀ��ֵ
    /// @param scale_y ��������Ŀ��ֵ
    ActionScaleToEntity(Duration duration, float scale_x, float scale_y);

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
    ActionScaleToEntity* Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    ActionScaleToEntity* Reverse() const override
    {
        KGE_ERRORF("Reverse() not supported in ActionScaleToEntity");
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
    ActionFadeTo(Duration duration, float opacity);
};

/// \~chinese
/// @brief ���붯��
class KGE_API ActionFadeIn : public ActionTween
{
public:
    /// \~chinese
    /// @brief �������붯��
    /// @param duration ����ʱ��
    ActionFadeIn(Duration duration);
};

/// \~chinese
/// @brief ��������
class KGE_API ActionFadeOut : public ActionTween
{
public:
    /// \~chinese
    /// @brief ������������
    /// @param duration ����ʱ��
    ActionFadeOut(Duration duration);
};

/// \~chinese
/// @brief ͸���Ƚ��䶯��ʵ��
class KGE_API ActionFadeToEntity : public ActionTweenEntity
{
public:
    /// \~chinese
    /// @brief ����͸���Ƚ��䶯��
    /// @param duration ����ʱ��
    /// @param opacity Ŀ��͸����
    ActionFadeToEntity(Duration duration, float opacity);

    /// \~chinese
    /// @brief ��ȡĿ��͸����
    float GetTargetOpacity() const;

    /// \~chinese
    /// @brief ����Ŀ��͸����
    void SetTargetOpacity(float opacity);

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    ActionFadeToEntity* Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    ActionFadeToEntity* Reverse() const override
    {
        KGE_ERRORF("Reverse() not supported in ActionFadeToEntity");
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
/// @brief �����ת����
class KGE_API ActionRotateBy : public ActionTween
{
public:
    /// \~chinese
    /// @brief ���������ת����
    /// @param duration ����ʱ��
    /// @param rotation �Ƕ���Ա仯ֵ
    ActionRotateBy(Duration duration, float rotation);
};

/// \~chinese
/// @brief �����ת����ʵ��
class KGE_API ActionRotateByEntity : public ActionTweenEntity
{
public:
    /// \~chinese
    /// @brief ���������ת����
    /// @param duration ����ʱ��
    /// @param rotation �Ƕ���Ա仯ֵ
    ActionRotateByEntity(Duration duration, float rotation);

    /// \~chinese
    /// @brief ��ȡ�Ƕ���Ա仯ֵ
    float GetRotation() const;

    /// \~chinese
    /// @brief ���ýǶ���Ա仯ֵ
    void SetRotation(float rotation);

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    ActionRotateByEntity* Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    ActionRotateByEntity* Reverse() const override;

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float percent) override;

protected:
    float start_val_;
    float delta_val_;
};

/// \~chinese
/// @brief ��ת����
class KGE_API ActionRotateTo : public ActionTween
{
public:
    /// \~chinese
    /// @brief ������ת����
    /// @param duration ����ʱ��
    /// @param rotation Ŀ��Ƕ�
    ActionRotateTo(Duration duration, float rotation);
};

/// \~chinese
/// @brief ��ת����ʵ��
class KGE_API ActionRotateToEntity : public ActionRotateByEntity
{
public:
    /// \~chinese
    /// @brief ������ת����
    /// @param duration ����ʱ��
    /// @param rotation Ŀ��Ƕ�
    ActionRotateToEntity(Duration duration, float rotation);

    /// \~chinese
    /// @brief ��ȡĿ��Ƕ�
    float GetTargetRotation() const;

    /// \~chinese
    /// @brief ����Ŀ��Ƕ�
    void SetTargetRotation(float rotation);

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    ActionRotateToEntity* Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    ActionRotateToEntity* Reverse() const override
    {
        KGE_ERRORF("Reverse() not supported in ActionRotateToEntity");
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
    ActionCustom(Duration duration, TweenFunc tween_func);
};

/// \~chinese
/// @brief �Զ��嶯��ʵ��
class KGE_API ActionCustomEntity : public ActionTweenEntity
{
public:
    /// \~chinese
    /// @brief �����Զ��嶯��
    /// @param duration ����ʱ��
    /// @param tween_func �����ص�����
    ActionCustomEntity(Duration duration, ActionCustom::TweenFunc tween_func);

    /// \~chinese
    /// @brief ��ȡ�����ص�����
    ActionCustom::TweenFunc GetTweenFunc() const;

    /// \~chinese
    /// @brief ���ö����ص�����
    void SetTweenFunc(const ActionCustom::TweenFunc& tween_func);

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    ActionCustomEntity* Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    ActionCustomEntity* Reverse() const override
    {
        KGE_ERRORF("Reverse() not supported in ActionCustomEntity");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float percent) override;

private:
    ActionCustom::TweenFunc tween_func_;
};

/** @} */

inline const EaseFunc& ActionTweenEntity::GetEaseFunc() const
{
    return ease_func_;
}

inline Duration ActionTweenEntity::GetDuration() const
{
    return dur_;
}

inline void ActionTweenEntity::SetDuration(Duration duration)
{
    dur_ = duration;
}

inline void ActionTweenEntity::SetEaseFunc(const EaseFunc& func)
{
    ease_func_ = func;
}

inline Vec2 ActionMoveByEntity::GetDisplacement() const
{
    return displacement_;
}

inline void ActionMoveByEntity::SetDisplacement(const Vec2& displacement)
{
    displacement_ = displacement;
}

inline Point ActionMoveToEntity::GetDistination() const
{
    return distination_;
}

inline void ActionMoveToEntity::SetDistination(const Point& distination)
{
    distination_ = distination;
}

inline Vec2 ActionJumpByEntity::GetDisplacement() const
{
    return displacement_;
}

inline float ActionJumpByEntity::GetJumpHeight() const
{
    return height_;
}

inline int ActionJumpByEntity::GetJumpCount() const
{
    return jump_count_;
}

inline void ActionJumpByEntity::SetDisplacement(const Vec2& displacement)
{
    displacement_ = displacement;
}

inline void ActionJumpByEntity::SetJumpHeight(float height)
{
    height_ = height;
}

inline void ActionJumpByEntity::SetJumpCount(int count)
{
    jump_count_ = count;
}

inline Point ActionJumpToEntity::GetDistination() const
{
    return distination_;
}

inline void ActionJumpToEntity::SetDistination(const Point& distination)
{
    distination_ = distination;
}

inline float ActionScaleByEntity::GetScaleX() const
{
    return delta_x_;
}

inline float ActionScaleByEntity::GetScaleY() const
{
    return delta_y_;
}

inline void ActionScaleByEntity::SetScaleX(float scale_x)
{
    delta_x_ = scale_x;
}

inline void ActionScaleByEntity::SetScaleY(float scale_y)
{
    delta_y_ = scale_y;
}

inline float ActionScaleToEntity::GetTargetScaleX() const
{
    return end_scale_x_;
}

inline float ActionScaleToEntity::GetTargetScaleY() const
{
    return end_scale_y_;
}

inline void ActionScaleToEntity::SetTargetScaleX(float scale_x)
{
    end_scale_x_ = scale_x;
}

inline void ActionScaleToEntity::SetTargetScaleY(float scale_y)
{
    end_scale_y_ = scale_y;
}

inline float ActionFadeToEntity::GetTargetOpacity() const
{
    return end_val_;
}

inline void ActionFadeToEntity::SetTargetOpacity(float opacity)
{
    end_val_ = opacity;
}

inline float ActionRotateByEntity::GetRotation() const
{
    return delta_val_;
}

inline void ActionRotateByEntity::SetRotation(float rotation)
{
    delta_val_ = rotation;
}

inline float ActionRotateToEntity::GetTargetRotation() const
{
    return end_val_;
}

inline void ActionRotateToEntity::SetTargetRotation(float rotation)
{
    end_val_ = rotation;
}

inline ActionCustom::TweenFunc ActionCustomEntity::GetTweenFunc() const
{
    return tween_func_;
}

inline void ActionCustomEntity::SetTweenFunc(const ActionCustom::TweenFunc& tween_func)
{
    tween_func_ = tween_func;
}

}  // namespace kiwano
