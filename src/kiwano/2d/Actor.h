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
#include <kiwano/math/Math.h>
#include <kiwano/core/Time.h>
#include <kiwano/base/ObjectBase.h>
#include <kiwano/base/component/ComponentManager.h>
#include <kiwano/event/EventDispatcher.h>
#include <kiwano/utils/TaskScheduler.h>
#include <kiwano/2d/action/ActionScheduler.h>

namespace kiwano
{
class Stage;
class Director;
class RenderContext;

namespace physics
{
class PhysicBody;
}

KGE_DECLARE_SMART_PTR(Actor);

/// \~chinese
/// @brief ��ɫ�б�
typedef IntrusiveList<ActorPtr> ActorList;

/**
 * \~chinese
 * \defgroup Actors ������ɫ
 */

/**
 * \addtogroup Actors
 * @{
 */

/**
 * \~chinese
 * @brief ��ɫ
 * @details
 * ��ɫ����̨���������Ԫ�أ��������Ⱦ�����¡��¼��ַ��ȹ��ܵ���С��λ��Ҳ�Ƕ����������¼������ȹ��ܵ�����
 */
class KGE_API Actor
    : public ObjectBase
    , public TaskScheduler
    , public ActionScheduler
    , public EventDispatcher
    , public ComponentManager
    , protected IntrusiveListValue<ActorPtr>
{
    friend class Director;
    friend class Transition;
    friend IntrusiveList<ActorPtr>;

public:
    /// \~chinese
    /// @brief ��ɫ���»ص�����
    typedef Function<void(Duration)> UpdateCallback;

    Actor();

    virtual ~Actor();

    /// \~chinese
    /// @brief ���½�ɫ
    /// @details ÿ֡����ˢ��ǰ���øú��������ظú�����ʵ�ֽ�ɫ�ĸ��´���
    /// @param dt ����һ�θ��µ�ʱ����
    virtual void OnUpdate(Duration dt);

    /// \~chinese
    /// @brief ��Ⱦ��ɫ
    /// @details
    /// ÿ֡����ˢ��ʱ���øú�����Ĭ�ϲ�������Ⱦ�����ظú�����ʵ�־�����Ⱦ����
    /// @param ctx ��Ⱦ������
    virtual void OnRender(RenderContext& ctx);

    /// \~chinese
    /// @brief ��ȡ��ʾ״̬
    bool IsVisible() const;

    /// \~chinese
    /// @brief ��ȡ��Ӧ״̬
    bool IsResponsible() const;

    /// \~chinese
    /// @brief �Ƿ����ü���͸����
    bool IsCascadeOpacityEnabled() const;

    /// \~chinese
    /// @brief �Ƿ������¼��ַ�
    bool IsEventDispatchEnabled() const;

    /// \~chinese
    /// @brief ��ȡ���Ƶ� Hash ֵ
    size_t GetHashName() const;

    /// \~chinese
    /// @brief ��ȡ Z ��˳��
    int GetZOrder() const;

    /// \~chinese
    /// @brief ��ȡ����
    virtual Point GetPosition() const;

    /// \~chinese
    /// @brief ��ȡ x ����
    float GetPositionX() const;

    /// \~chinese
    /// @brief ��ȡ y ����
    float GetPositionY() const;

    /// \~chinese
    /// @brief ��ȡ��С
    virtual Size GetSize() const;

    /// \~chinese
    /// @brief ��ȡ���
    float GetWidth() const;

    /// \~chinese
    /// @brief ��ȡ�߶�
    float GetHeight() const;

    /// \~chinese
    /// @brief ��ȡ���ź�Ŀ��
    float GetScaledWidth() const;

    /// \~chinese
    /// @brief ��ȡ���ź�ĸ߶�
    float GetScaledHeight() const;

    /// \~chinese
    /// @brief ��ȡ���ź�Ĵ�С
    Size GetScaledSize() const;

    /// \~chinese
    /// @brief ��ȡê��
    virtual Point GetAnchor() const;

    /// \~chinese
    /// @brief ��ȡ x ����ê��
    float GetAnchorX() const;

    /// \~chinese
    /// @brief ��ȡ y ����ê��
    float GetAnchorY() const;

    /// \~chinese
    /// @brief ��ȡ͸����
    virtual float GetOpacity() const;

    /// \~chinese
    /// @brief ��ȡ��ʾ͸����
    float GetDisplayedOpacity() const;

    /// \~chinese
    /// @brief ��ȡ��ת�Ƕ�
    virtual float GetRotation() const;

    /// \~chinese
    /// @brief ��ȡ���ű���
    virtual Point GetScale() const;

    /// \~chinese
    /// @brief ��ȡ�������ű���
    float GetScaleX() const;

    /// \~chinese
    /// @brief ��ȡ�������ű���
    float GetScaleY() const;

    /// \~chinese
    /// @brief ��ȡ���нǶ�
    virtual Point GetSkew() const;

    /// \~chinese
    /// @brief ��ȡ������нǶ�
    float GetSkewX() const;

    /// \~chinese
    /// @brief ��ȡ������нǶ�
    float GetSkewY() const;

    /// \~chinese
    /// @brief ��ȡ�任
    Transform GetTransform() const;

    /// \~chinese
    /// @brief ��ȡ����ɫ
    Actor* GetParent() const;

    /// \~chinese
    /// @brief ��ȡ������̨
    Stage* GetStage() const;

    /// \~chinese
    /// @brief ��ȡ�߿�
    virtual Rect GetBounds() const;

    /// \~chinese
    /// @brief ��ȡ���а�Χ��
    virtual Rect GetBoundingBox() const;

    /// \~chinese
    /// @brief ��ȡ��ά�任����
    const Matrix3x2& GetTransformMatrix() const;

    /// \~chinese
    /// @brief ��ȡ��ά�任�������
    const Matrix3x2& GetTransformInverseMatrix() const;

    /// \~chinese
    /// @brief ��ȡ�任������ɫ�Ķ�ά�任����
    const Matrix3x2& GetTransformMatrixToParent() const;

    /// \~chinese
    /// @brief ���ý�ɫ�Ƿ�ɼ�
    void SetVisible(bool val);

    /// \~chinese
    /// @brief ��������
    void SetName(const String& name);

    /// \~chinese
    /// @brief ��������
    virtual void SetPosition(const Point& point);

    /// \~chinese
    /// @brief ��������
    void SetPosition(float x, float y);

    /// \~chinese
    /// @brief ���ú�����
    void SetPositionX(float x);

    /// \~chinese
    /// @brief ����������
    void SetPositionY(float y);

    /// \~chinese
    /// @brief �ƶ�������
    void MoveTo(const Point& p);

    /// \~chinese
    /// @brief �ƶ�������
    void MoveTo(float x, float y);

    /// \~chinese
    /// @brief �ƶ��������
    void MoveBy(const Vec2& trans);

    /// \~chinese
    /// @brief �ƶ��������
    void MoveBy(float trans_x, float trans_y);

    /// \~chinese
    /// @brief �������ű�����Ĭ��Ϊ (1.0, 1.0)
    virtual void SetScale(const Vec2& scale);

    /// \~chinese
    /// @brief �������ű�����Ĭ��Ϊ (1.0, 1.0)
    void SetScale(float scalex, float scaley);

    /// \~chinese
    /// @brief ���ô��нǶȣ�Ĭ��Ϊ (0, 0)
    virtual void SetSkew(const Vec2& skew);

    /// \~chinese
    /// @brief ���ô��нǶȣ�Ĭ��Ϊ (0, 0)
    void SetSkew(float skewx, float skewy);

    /// \~chinese
    /// @brief ������ת�Ƕȣ�Ĭ��Ϊ 0
    virtual void SetRotation(float rotation);

    /// \~chinese
    /// @brief ����ê��λ�ã�Ĭ��Ϊ (0, 0), ��Χ [0, 1]
    virtual void SetAnchor(const Vec2& anchor);

    /// \~chinese
    /// @brief ����ê��λ�ã�Ĭ��Ϊ (0, 0), ��Χ [0, 1]
    void SetAnchor(float anchorx, float anchory);

    /// \~chinese
    /// @brief �޸Ĵ�С
    virtual void SetSize(const Size& size);

    /// \~chinese
    /// @brief �޸Ĵ�С
    void SetSize(float width, float height);

    /// \~chinese
    /// @brief �޸Ŀ��
    void SetWidth(float width);

    /// \~chinese
    /// @brief �޸ĸ߶�
    void SetHeight(float height);

    /// \~chinese
    /// @brief ����͸���ȣ�Ĭ��Ϊ 1.0, ��Χ [0, 1]
    virtual void SetOpacity(float opacity);

    /// \~chinese
    /// @brief ���û���ü���͸����
    void SetCascadeOpacityEnabled(bool enabled);

    /// \~chinese
    /// @brief ���ö�ά����任
    void SetTransform(const Transform& transform);

    /// \~chinese
    /// @brief ���� Z ��˳��Ĭ��Ϊ 0
    void SetZOrder(int zorder);

    /// \~chinese
    /// @brief ���ý�ɫ�Ƿ����Ӧ��Ĭ��Ϊ false
    /// @details ����Ӧ�Ľ�ɫ���յ����� Hover | Out | Click ��Ϣ
    void SetResponsible(bool enable);

    /// \~chinese
    /// @brief ����ӽ�ɫ
    void AddChild(ActorPtr child);

    /// \~chinese
    /// @brief ����ӽ�ɫ
    void AddChild(ActorPtr child, int zorder);

    /// \~chinese
    /// @brief ��Ӷ���ӽ�ɫ
    void AddChildren(const Vector<ActorPtr>& children);

    /// \~chinese
    /// @brief ��ȡ������ͬ���ӽ�ɫ
    ActorPtr GetChild(const String& name) const;

    /// \~chinese
    /// @brief ��ȡ����������ͬ���ӽ�ɫ
    Vector<ActorPtr> GetChildren(const String& name) const;

    /// \~chinese
    /// @brief ��ȡȫ���ӽ�ɫ
    ActorList& GetAllChildren();

    /// \~chinese
    /// @brief ��ȡȫ���ӽ�ɫ
    const ActorList& GetAllChildren() const;

    /// \~chinese
    /// @brief �Ƴ��ӽ�ɫ
    void RemoveChild(ActorPtr child);

    /// \~chinese
    /// @brief �Ƴ�����������ͬ���ӽ�ɫ
    void RemoveChildren(const String& child_name);

    /// \~chinese
    /// @brief �Ƴ����н�ɫ
    void RemoveAllChildren();

    /// \~chinese
    /// @brief �Ӹ���ɫ�Ƴ�
    void RemoveFromParent();

    /// \~chinese
    /// @brief ��ͣ��ɫ����
    void PauseUpdating();

    /// \~chinese
    /// @brief ������ɫ����
    void ResumeUpdating();

    /// \~chinese
    /// @brief ��ɫ�����Ƿ���ͣ
    bool IsUpdatePausing() const;

    /// \~chinese
    /// @brief ���ø���ʱ�Ļص�����
    void SetCallbackOnUpdate(const UpdateCallback& cb);

    /// \~chinese
    /// @brief ��ȡ����ʱ�Ļص�����
    UpdateCallback GetCallbackOnUpdate() const;

    /// \~chinese
    /// @brief �жϵ��Ƿ��ڽ�ɫ��
    virtual bool ContainsPoint(const Point& point) const;

    /// \~chinese
    /// @brief ����������ϵ��ת��Ϊ�ֲ�����ϵ��
    Point ConvertToLocal(const Point& point) const;

    /// \~chinese
    /// @brief ���ֲ�����ϵ��ת��Ϊ��������ϵ��
    Point ConvertToWorld(const Point& point) const;

    /// \~chinese
    /// @brief ��Ⱦ��ɫ�߽�
    void ShowBorder(bool show);

    /// \~chinese
    /// @brief �ַ��¼�
    /// @param evt �¼�
    /// @return �Ƿ�����ַ����¼�
    virtual bool DispatchEvent(Event* evt);

    /// \~chinese
    /// @brief ������ر��¼��ַ�����
    /// @param enabled �Ƿ���
    void SetEventDispatchEnabled(bool enabled);

    /// \~chinese
    /// @brief ��ȡ��������
    physics::PhysicBody* GetPhysicBody() const;

    /// \~chinese
    /// @brief ���л�
    void DoSerialize(Serializer* serializer) const override;

    /// \~chinese
    /// @brief �����л�
    void DoDeserialize(Deserializer* deserializer) override;

    /// \~chinese
    /// @brief ����Ĭ��ê��
    static void SetDefaultAnchor(float anchor_x, float anchor_y);

    /// \~chinese
    /// @brief ��ȡ�ɼ�������
    inline Property<bool> VisibleProperty()
    {
        return Property<bool>(&visible_);
    }

    /// \~chinese
    /// @brief ��ȡ��͸��������
    inline Property<float, FlagUint8> OpacityProperty()
    {
        return Property<float, FlagUint8>(&opacity_, &dirty_flag_, DirtyFlag::DirtyOpacity);
    }

    /// \~chinese
    /// @brief ��ȡê������
    inline Property<Point, FlagUint8> AnchorProperty()
    {
        return Property<Point, FlagUint8>(&anchor_, &dirty_flag_, DirtyFlag::DirtyTransform);
    }

    /// \~chinese
    /// @brief ��ȡ��С����
    inline Property<Size, FlagUint8> SizeProperty()
    {
        return Property<Size, FlagUint8>(&size_, &dirty_flag_, DirtyFlag::DirtyTransform);
    }

    /// \~chinese
    /// @brief ��ȡλ������
    inline Property<Point, FlagUint8> PositionProperty()
    {
        return Property<Point, FlagUint8>(&transform_.position, &dirty_flag_, DirtyFlag::DirtyTransform);
    }

    /// \~chinese
    /// @brief ��ȡ��ת�Ƕ�����
    inline Property<float, FlagUint8> RotationProperty()
    {
        return Property<float, FlagUint8>(&transform_.rotation, &dirty_flag_, DirtyFlag::DirtyTransform);
    }

    /// \~chinese
    /// @brief ��ȡ��������
    inline Property<Point, FlagUint8> ScaleProperty()
    {
        return Property<Point, FlagUint8>(&transform_.scale, &dirty_flag_, DirtyFlag::DirtyTransform);
    }

    /// \~chinese
    /// @brief ��ȡ���нǶ�����
    inline Property<Point, FlagUint8> SkewProperty()
    {
        return Property<Point, FlagUint8>(&transform_.skew, &dirty_flag_, DirtyFlag::DirtyTransform);
    }

    /// \~chinese
    /// @brief ��ȡZ��˳������
    inline Property<int, Function<void()>> ZOrderProperty()
    {
        return Property<int, Function<void()>>(&z_order_, Closure(this, &Actor::Reorder));
    }

protected:
    /// \~chinese
    /// @brief ��������������ӽ�ɫ
    virtual void Update(Duration dt);

    /// \~chinese
    /// @brief ��Ⱦ����������ӽ�ɫ
    virtual void Render(RenderContext& ctx);

    /// \~chinese
    /// @brief ��������������ӽ�ɫ�ı߽�
    virtual void RenderBorder(RenderContext& ctx);

    /// \~chinese
    /// @brief ����Ƿ�����Ⱦ�����ĵ�������
    virtual bool CheckVisibility(RenderContext& ctx) const;

    /// \~chinese
    /// @brief ��Ⱦǰ��ʼ����Ⱦ������״̬������ CheckVisibility ������ʱ���øú���
    virtual void PrepareToRender(RenderContext& ctx);

    /// \~chinese
    /// @brief �����Լ��Ķ�ά�任����֪ͨ�����ӽ�ɫ
    void UpdateTransform() const;

    /// \~chinese
    /// @brief �����Լ��������ӽ�ɫ��͸����
    void UpdateOpacity();

    /// \~chinese
    /// @brief �������ӽ�ɫ��Z��˳������
    void Reorder();

    /// \~chinese
    /// @brief ���ýڵ�������̨
    void SetStage(Stage* stage);

    /// \~chinese
    /// @brief �����¼�
    bool HandleEvent(Event* evt);

    /// \~chinese
    /// @brief ������������
    void SetPhysicBody(physics::PhysicBody* body);

    friend physics::PhysicBody;

private:
    bool         visible_;
    bool         update_pausing_;
    bool         cascade_opacity_;
    bool         show_border_;
    bool         hover_;
    bool         pressed_;
    bool         responsible_;
    bool         evt_dispatch_enabled_;
    mutable bool visible_in_rt_;

    enum DirtyFlag : uint8_t
    {
        Clean                 = 0,
        DirtyTransform        = 1,
        DirtyTransformInverse = 1 << 1,
        DirtyOpacity          = 1 << 2,
        DirtyVisibility       = 1 << 3
    };
    mutable Flag<uint8_t> dirty_flag_;

    int                  z_order_;
    float                opacity_;
    float                displayed_opacity_;
    Actor*               parent_;
    Stage*               stage_;
    physics::PhysicBody* physic_body_;
    size_t               hash_name_;
    Point                anchor_;
    Size                 size_;
    ActorList            children_;
    UpdateCallback       cb_update_;
    Transform            transform_;

    mutable Matrix3x2 transform_matrix_;
    mutable Matrix3x2 transform_matrix_inverse_;
    mutable Matrix3x2 transform_matrix_to_parent_;
};

/** @} */

inline void Actor::OnUpdate(Duration dt)
{
    KGE_NOT_USED(dt);
}

inline void Actor::OnRender(RenderContext& ctx)
{
    KGE_NOT_USED(ctx);
}

inline bool Actor::IsVisible() const
{
    return visible_;
}

inline bool Actor::IsResponsible() const
{
    return responsible_;
}

inline bool Actor::IsCascadeOpacityEnabled() const
{
    return cascade_opacity_;
}

inline bool Actor::IsEventDispatchEnabled() const
{
    return evt_dispatch_enabled_;
}

inline size_t Actor::GetHashName() const
{
    return hash_name_;
}

inline int Actor::GetZOrder() const
{
    return z_order_;
}

inline Point Actor::GetPosition() const
{
    return transform_.position;
}

inline float Actor::GetPositionX() const
{
    return GetPosition().x;
}

inline float Actor::GetPositionY() const
{
    return GetPosition().y;
}

inline Point Actor::GetScale() const
{
    return transform_.scale;
}

inline float Actor::GetScaleX() const
{
    return GetScale().x;
}

inline float Actor::GetScaleY() const
{
    return GetScale().y;
}

inline Point Actor::GetSkew() const
{
    return transform_.skew;
}

inline float Actor::GetSkewX() const
{
    return GetSkew().x;
}

inline float Actor::GetSkewY() const
{
    return GetSkew().y;
}

inline float Actor::GetRotation() const
{
    return transform_.rotation;
}

inline float Actor::GetWidth() const
{
    return GetSize().x;
}

inline float Actor::GetHeight() const
{
    return GetSize().y;
}

inline Size Actor::GetSize() const
{
    return size_;
}

inline float Actor::GetScaledWidth() const
{
    return GetWidth() * GetScaleX();
}

inline float Actor::GetScaledHeight() const
{
    return GetHeight() * GetScaleY();
}

inline Size Actor::GetScaledSize() const
{
    return Size{ GetScaledWidth(), GetScaledHeight() };
}

inline Point Actor::GetAnchor() const
{
    return anchor_;
}

inline float Actor::GetAnchorX() const
{
    return GetAnchor().x;
}

inline float Actor::GetAnchorY() const
{
    return GetAnchor().y;
}

inline float Actor::GetOpacity() const
{
    return opacity_;
}

inline float Actor::GetDisplayedOpacity() const
{
    return displayed_opacity_;
}

inline Transform Actor::GetTransform() const
{
    return transform_;
}

inline Actor* Actor::GetParent() const
{
    return parent_;
}

inline Stage* Actor::GetStage() const
{
    return stage_;
}

inline void Actor::PauseUpdating()
{
    update_pausing_ = true;
}

inline void Actor::ResumeUpdating()
{
    update_pausing_ = false;
}

inline bool Actor::IsUpdatePausing() const
{
    return update_pausing_;
}

inline void Actor::SetCallbackOnUpdate(const UpdateCallback& cb)
{
    cb_update_ = cb;
}

inline Actor::UpdateCallback Actor::GetCallbackOnUpdate() const
{
    return cb_update_;
}

inline void Actor::ShowBorder(bool show)
{
    show_border_ = show;
}

inline void Actor::SetPosition(float x, float y)
{
    this->SetPosition(Point(x, y));
}

inline void Actor::SetPositionX(float x)
{
    this->SetPosition(Point(x, transform_.position.y));
}

inline void Actor::SetPositionY(float y)
{
    this->SetPosition(Point(transform_.position.x, y));
}

inline void Actor::MoveTo(const Point& p)
{
    this->SetPosition(p);
}

inline void Actor::MoveTo(float x, float y)
{
    this->SetPosition(Point(x, y));
}

inline void Actor::MoveBy(const Vec2& trans)
{
    this->SetPosition(transform_.position.x + trans.x, transform_.position.y + trans.y);
}

inline void Actor::MoveBy(float trans_x, float trans_y)
{
    this->MoveBy(Vec2(trans_x, trans_y));
}

inline void Actor::SetScale(float scalex, float scaley)
{
    this->SetScale(Vec2(scalex, scaley));
}

inline void Actor::SetAnchor(float anchorx, float anchory)
{
    this->SetAnchor(Vec2(anchorx, anchory));
}

inline void Actor::SetSize(float width, float height)
{
    this->SetSize(Size(width, height));
}

inline void Actor::SetWidth(float width)
{
    this->SetSize(Size(width, size_.y));
}

inline void Actor::SetHeight(float height)
{
    this->SetSize(Size(size_.x, height));
}

inline void Actor::SetSkew(float skewx, float skewy)
{
    this->SetSkew(Vec2(skewx, skewy));
}

inline physics::PhysicBody* Actor::GetPhysicBody() const
{
    return physic_body_;
}

inline void Actor::SetPhysicBody(physics::PhysicBody* body)
{
    physic_body_ = body;
}

}  // namespace kiwano
