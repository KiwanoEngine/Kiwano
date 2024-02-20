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
#include <kiwano/2d/animation/Animation.h>
#include <kiwano/2d/animation/EaseFunc.h>
#include <kiwano/utils/Logger.h>

namespace kiwano
{

/**
 * \addtogroup Animation
 * @{
 */

/// \~chinese
/// @brief ���䶯��
class KGE_API TweenAnimation : public Animation
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
    TweenAnimation();

    TweenAnimation(Duration duration);

    float Interpolate(float frac);

    void Update(Actor* target, Duration dt) override;

    virtual void UpdateTween(Actor* target, float frac) = 0;

    void DoClone(TweenAnimation* to) const;

private:
    Duration dur_;
    EaseFunc ease_func_;
};


/// \~chinese
/// @brief ���λ�ƶ���
class KGE_API MoveByAnimation : public TweenAnimation
{
public:
    /// \~chinese
    /// @brief �������λ�ƶ���
    /// @param duration ����ʱ��
    /// @param displacement λ������
    MoveByAnimation(Duration duration, const Vec2& displacement);

    /// \~chinese
    /// @brief ��ȡλ������
    Vec2 GetDisplacement() const;

    /// \~chinese
    /// @brief ����λ������
    void SetDisplacement(const Vec2& displacement);

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    MoveByAnimation* Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    MoveByAnimation* Reverse() const override;

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float frac) override;

protected:
    Point start_pos_;
    Point prev_pos_;
    Vec2  displacement_;
};


/// \~chinese
/// @brief λ�ƶ���
class KGE_API MoveToAnimation : public MoveByAnimation
{
public:
    /// \~chinese
    /// @brief ����λ�ƶ���
    /// @param duration ����ʱ��
    /// @param distination Ŀ������
    MoveToAnimation(Duration duration, const Point& distination);

    /// \~chinese
    /// @brief ��ȡĿ������
    Point GetDistination() const;

    /// \~chinese
    /// @brief ����Ŀ������
    void SetDistination(const Point& distination);

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    MoveToAnimation* Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    MoveToAnimation* Reverse() const override
    {
        KGE_ERRORF("Reverse() not supported in MoveToAnimation");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

private:
    Point distination_;
};


/// \~chinese
/// @brief �����Ծ����
class KGE_API JumpByAnimation : public TweenAnimation
{
public:
    /// \~chinese
    /// @brief ���������Ծ����
    /// @param duration ����ʱ��
    /// @param displacement ��Ծλ������
    /// @param height ��Ծ�߶�
    /// @param count ��Ծ����
    JumpByAnimation(Duration duration, const Vec2& displacement, float height, int count = 1);

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
    JumpByAnimation* Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    JumpByAnimation* Reverse() const override;

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float frac) override;

protected:
    float height_;
    int   jump_count_;
    Point start_pos_;
    Point displacement_;
    Point prev_pos_;
};


/// \~chinese
/// @brief ��Ծ����
class KGE_API JumpToAnimation : public JumpByAnimation
{
public:
    /// \~chinese
    /// @brief ������Ծ����
    /// @param duration ����ʱ��
    /// @param distination Ŀ������
    /// @param height ��Ծ�߶�
    /// @param count ��Ծ����
    JumpToAnimation(Duration duration, const Point& distination, float height, int count = 1);

    /// \~chinese
    /// @brief ��ȡĿ������
    Point GetDistination() const;

    /// \~chinese
    /// @brief ����Ŀ������
    void SetDistination(const Point& distination);

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    JumpToAnimation* Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    JumpToAnimation* Reverse() const override
    {
        KGE_ERRORF("Reverse() not supported in JumpToAnimation");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

private:
    Point distination_;
};


/// \~chinese
/// @brief ������Ŷ���
class KGE_API ScaleByAnimation : public TweenAnimation
{
public:
    /// \~chinese
    /// @brief ����������Ŷ���
    /// @param duration ����ʱ��
    /// @param scale ������Ա仯ֵ
    ScaleByAnimation(Duration duration, const Vec2& scale);

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
    ScaleByAnimation* Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    ScaleByAnimation* Reverse() const override;

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float frac) override;

protected:
    Vec2 start_val_;
    Vec2 delta_;
};


/// \~chinese
/// @brief ���Ŷ���
class KGE_API ScaleToAnimation : public ScaleByAnimation
{
public:
    /// \~chinese
    /// @brief �������Ŷ���
    /// @param duration ����ʱ��
    /// @param scale ����Ŀ��ֵ
    ScaleToAnimation(Duration duration, const Vec2& scale);

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
    ScaleToAnimation* Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    ScaleToAnimation* Reverse() const override
    {
        KGE_ERRORF("Reverse() not supported in ScaleToAnimation");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

private:
    Vec2 end_val_;
};


/// \~chinese
/// @brief ͸���Ƚ��䶯��
class KGE_API FadeToAnimation : public TweenAnimation
{
public:
    /// \~chinese
    /// @brief ����͸���Ƚ��䶯��
    /// @param duration ����ʱ��
    /// @param opacity Ŀ��͸����
    FadeToAnimation(Duration duration, float opacity);

    /// \~chinese
    /// @brief ��ȡĿ��͸����
    float GetTargetOpacity() const;

    /// \~chinese
    /// @brief ����Ŀ��͸����
    void SetTargetOpacity(float opacity);

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    FadeToAnimation* Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    FadeToAnimation* Reverse() const override
    {
        KGE_ERRORF("Reverse() not supported in FadeToAnimation");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float frac) override;

private:
    float start_val_;
    float delta_val_;
    float end_val_;
};


/// \~chinese
/// @brief �����ת����
class KGE_API RotateByAnimation : public TweenAnimation
{
public:
    /// \~chinese
    /// @brief ���������ת����
    /// @param duration ����ʱ��
    /// @param rotation �Ƕ���Ա仯ֵ
    RotateByAnimation(Duration duration, float rotation);

    /// \~chinese
    /// @brief ��ȡ�Ƕ���Ա仯ֵ
    float GetRotation() const;

    /// \~chinese
    /// @brief ���ýǶ���Ա仯ֵ
    void SetRotation(float rotation);

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    RotateByAnimation* Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    RotateByAnimation* Reverse() const override;

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float frac) override;

protected:
    float start_val_;
    float delta_val_;
};


/// \~chinese
/// @brief ��ת����
class KGE_API RotateToAnimation : public RotateByAnimation
{
public:
    /// \~chinese
    /// @brief ������ת����
    /// @param duration ����ʱ��
    /// @param rotation Ŀ��Ƕ�
    RotateToAnimation(Duration duration, float rotation);

    /// \~chinese
    /// @brief ��ȡĿ��Ƕ�
    float GetTargetRotation() const;

    /// \~chinese
    /// @brief ����Ŀ��Ƕ�
    void SetTargetRotation(float rotation);

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    RotateToAnimation* Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    RotateToAnimation* Reverse() const override
    {
        KGE_ERRORF("Reverse() not supported in RotateToAnimation");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

private:
    float end_val_;
};

/** @} */

inline const EaseFunc& TweenAnimation::GetEaseFunc() const
{
    return ease_func_;
}

inline Duration TweenAnimation::GetDuration() const
{
    return dur_;
}

inline void TweenAnimation::SetDuration(Duration duration)
{
    dur_ = duration;
}

inline void TweenAnimation::SetEaseFunc(const EaseFunc& func)
{
    ease_func_ = func;
}

inline Vec2 MoveByAnimation::GetDisplacement() const
{
    return displacement_;
}

inline void MoveByAnimation::SetDisplacement(const Vec2& displacement)
{
    displacement_ = displacement;
}

inline Point MoveToAnimation::GetDistination() const
{
    return distination_;
}

inline void MoveToAnimation::SetDistination(const Point& distination)
{
    distination_ = distination;
}

inline Vec2 JumpByAnimation::GetDisplacement() const
{
    return displacement_;
}

inline float JumpByAnimation::GetJumpHeight() const
{
    return height_;
}

inline int JumpByAnimation::GetJumpCount() const
{
    return jump_count_;
}

inline void JumpByAnimation::SetDisplacement(const Vec2& displacement)
{
    displacement_ = displacement;
}

inline void JumpByAnimation::SetJumpHeight(float height)
{
    height_ = height;
}

inline void JumpByAnimation::SetJumpCount(int count)
{
    jump_count_ = count;
}

inline Point JumpToAnimation::GetDistination() const
{
    return distination_;
}

inline void JumpToAnimation::SetDistination(const Point& distination)
{
    distination_ = distination;
}

inline float ScaleByAnimation::GetScaleX() const
{
    return delta_.x;
}

inline float ScaleByAnimation::GetScaleY() const
{
    return delta_.y;
}

inline void ScaleByAnimation::SetScaleX(float scale_x)
{
    delta_.x = scale_x;
}

inline void ScaleByAnimation::SetScaleY(float scale_y)
{
    delta_.y = scale_y;
}

inline float ScaleToAnimation::GetTargetScaleX() const
{
    return end_val_.x;
}

inline float ScaleToAnimation::GetTargetScaleY() const
{
    return end_val_.y;
}

inline void ScaleToAnimation::SetTargetScaleX(float scale_x)
{
    end_val_.x = scale_x;
}

inline void ScaleToAnimation::SetTargetScaleY(float scale_y)
{
    end_val_.y = scale_y;
}

inline float FadeToAnimation::GetTargetOpacity() const
{
    return end_val_;
}

inline void FadeToAnimation::SetTargetOpacity(float opacity)
{
    end_val_ = opacity;
}

inline float RotateByAnimation::GetRotation() const
{
    return delta_val_;
}

inline void RotateByAnimation::SetRotation(float rotation)
{
    delta_val_ = rotation;
}

inline float RotateToAnimation::GetTargetRotation() const
{
    return end_val_;
}

inline void RotateToAnimation::SetTargetRotation(float rotation)
{
    end_val_ = rotation;
}

}  // namespace kiwano
