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
#include <kiwano/2d/include-forwards.h>
#include <kiwano/2d/Transform.h>
#include <kiwano/2d/action/ActionManager.h>
#include <kiwano/base/TimerManager.h>
#include <kiwano/base/EventDispatcher.h>

namespace kiwano
{
	class Director;
	class RenderTarget;

	// ��ɫ
	class KGE_API Actor
		: public ObjectBase
		, public TimerManager
		, public ActionManager
		, public EventDispatcher
		, public IntrusiveListItem<ActorPtr>
	{
		friend class Director;
		friend class Transition;
		friend IntrusiveList<ActorPtr>;

	public:
		using Children			= IntrusiveList<ActorPtr>;
		using UpdateCallback	= Function<void(Duration)>;

		Actor();

		// ���½�ɫ
		virtual void OnUpdate(Duration dt);

		// ��Ⱦ��ɫ
		virtual void OnRender(RenderTarget* rt);

		// ��ȡ��ʾ״̬
		bool IsVisible() const;

		// ��ȡ��Ӧ״̬
		bool IsResponsible() const;

		// �Ƿ����ü���͸����
		bool IsCascadeOpacityEnabled() const;

		// ��ȡ���Ƶ� Hash ֵ
		size_t GetHashName() const;

		// ��ȡ Z ��˳��
		int GetZOrder() const;

		// ��ȡ����
		Point const& GetPosition() const;

		// ��ȡ x ����
		float GetPositionX() const;

		// ��ȡ y ����
		float GetPositionY() const;

		// ��ȡ���ű���
		Point const& GetScale() const;

		// ��ȡ�������ű���
		float GetScaleX() const;

		// ��ȡ�������ű���
		float GetScaleY() const;

		// ��ȡ���нǶ�
		Point const& GetSkew() const;

		// ��ȡ������нǶ�
		float GetSkewX() const;

		// ��ȡ������нǶ�
		float GetSkewY() const;

		// ��ȡ��ת�Ƕ�
		float GetRotation() const;

		// ��ȡ���
		float GetWidth() const;

		// ��ȡ�߶�
		float GetHeight() const;

		// ��ȡ��С
		Size const& GetSize() const;

		// ��ȡ���ź�Ŀ��
		float GetScaledWidth() const;

		// ��ȡ���ź�ĸ߶�
		float GetScaledHeight() const;

		// ��ȡ���ź�Ĵ�С
		Size GetScaledSize() const;

		// ��ȡê��
		Point const& GetAnchor() const;

		// ��ȡ x ����ê��
		float GetAnchorX() const;

		// ��ȡ y ����ê��
		float GetAnchorY() const;

		// ��ȡ͸����
		float GetOpacity() const;

		// ��ȡ��ʾ͸����
		float GetDisplayedOpacity() const;

		// ��ȡ�任
		Transform GetTransform() const;

		// ��ȡ����ɫ
		Actor* GetParent() const;

		// ��ȡ������̨
		Stage* GetStage() const;

		// ��ȡ�߿�
		virtual Rect GetBounds() const;

		// ��ȡ���а�Χ��
		virtual Rect GetBoundingBox() const;

		// ��ȡ��ά�任����
		Matrix3x2 const& GetTransformMatrix()  const;

		// ��ȡ��ά�任�������
		Matrix3x2 const& GetTransformInverseMatrix()  const;

		// �����Ƿ���ʾ
		void SetVisible(
			bool val
		);

		// ��������
		void SetName(
			String const& name
		);

		// ��������
		virtual void SetPosition(
			Point const& point
		);

		// ��������
		void SetPosition(
			float x,
			float y
		);

		// ���ú�����
		void SetPositionX(
			float x
		);

		// ����������
		void SetPositionY(
			float y
		);

		// �ƶ�����
		void Move(
			Vec2 const& v
		);

		// �ƶ�����
		void Move(
			float vx,
			float vy
		);

		// �������ű���
		// Ĭ��Ϊ (1.0, 1.0)
		virtual void SetScale(
			Vec2 const& scale
		);

		// �������ű���
		// Ĭ��Ϊ (1.0, 1.0)
		void SetScale(
			float scalex,
			float scaley
		);

		// ���ô��нǶ�
		// Ĭ��Ϊ (0, 0)
		virtual void SetSkew(
			Vec2 const& skew
		);

		// ���ô��нǶ�
		// Ĭ��Ϊ (0, 0)
		void SetSkew(
			float skewx,
			float skewy
		);

		// ������ת�Ƕ�
		// Ĭ��Ϊ 0
		virtual void SetRotation(
			float rotation
		);

		// ����ê��λ��
		// Ĭ��Ϊ (0, 0), ��Χ [0, 1]
		virtual void SetAnchor(
			Vec2 const& anchor
		);

		// ����ê��λ��
		// Ĭ��Ϊ (0, 0), ��Χ [0, 1]
		void SetAnchor(
			float anchorx,
			float anchory
		);

		// �޸Ŀ��
		virtual void SetWidth(
			float width
		);

		// �޸ĸ߶�
		virtual void SetHeight(
			float height
		);

		// �޸Ĵ�С
		virtual void SetSize(
			Size const& size
		);

		// �޸Ĵ�С
		void SetSize(
			float width,
			float height
		);

		// ����͸����
		// Ĭ��Ϊ 1.0, ��Χ [0, 1]
		virtual void SetOpacity(
			float opacity
		);

		// ���û���ü���͸����
		void SetCascadeOpacityEnabled(
			bool enabled
		);

		// ���ö�ά����任
		void SetTransform(
			Transform const& transform
		);

		// ���� Z ��˳��
		// Ĭ��Ϊ 0
		void SetZOrder(
			int zorder
		);

		// �Ƿ����Ӧ (��� Hover | Out | Click ��Ϣ)
		// Ĭ��Ϊ false
		void SetResponsible(
			bool enable
		);

		// ����ӽ�ɫ
		void AddChild(
			ActorPtr child,
			int zorder = 0
		);

		// ����ӽ�ɫ
		void AddChild(
			Actor* child,
			int zorder = 0
		);

		// ��Ӷ���ӽ�ɫ
		void AddChildren(
			Vector<ActorPtr> const& children
		);

		// ��ȡ������ͬ���ӽ�ɫ
		Actor* GetChild(
			String const& name
		) const;

		// ��ȡ����������ͬ���ӽ�ɫ
		Vector<ActorPtr> GetChildren(
			String const& name
		) const;

		// ��ȡȫ���ӽ�ɫ
		Children& GetAllChildren();

		// ��ȡȫ���ӽ�ɫ
		Children const& GetAllChildren() const;

		// �Ƴ��ӽ�ɫ
		void RemoveChild(
			ActorPtr child
		);

		// �Ƴ��ӽ�ɫ
		void RemoveChild(
			Actor* child
		);

		// �Ƴ�����������ͬ���ӽ�ɫ
		void RemoveChildren(
			String const& child_name
		);

		// �Ƴ����н�ɫ
		void RemoveAllChildren();

		// �Ӹ���ɫ�Ƴ�
		void RemoveFromParent();

		// �жϵ��Ƿ��ڽ�ɫ��
		virtual bool ContainsPoint(const Point& point) const;

		// ��ͣ��ɫ����
		void PauseUpdating();

		// ������ɫ����
		void ResumeUpdating();

		// ��ɫ�����Ƿ���ͣ
		bool IsUpdatePausing() const;

		// ���ø���ʱ�Ļص�����
		void SetCallbackOnUpdate(UpdateCallback const& cb);

		// ��ȡ����ʱ�Ļص�����
		UpdateCallback GetCallbackOnUpdate() const;

		// ��Ⱦ��ɫ�߽�
		void ShowBorder(bool show);

		// �¼��ַ�
		void Dispatch(Event& evt) override;

		// ����Ĭ��ê��
		static void SetDefaultAnchor(
			float anchor_x,
			float anchor_y
		);

	protected:
		virtual void Update(Duration dt);

		virtual void Render(RenderTarget* rt);

		virtual void PrepareRender(RenderTarget* rt);

		virtual void RenderBorder(RenderTarget* rt);

		virtual bool CheckVisibilty(RenderTarget* rt) const;

		void UpdateTransform() const;

		void UpdateOpacity();

		void Reorder();

		void SetStage(Stage* stage);

	private:
		bool			visible_;
		bool			update_pausing_;
		bool			cascade_opacity_;
		bool			show_border_;
		bool			hover_;
		bool			pressed_;
		bool			responsible_;
		int				z_order_;
		float			opacity_;
		float			displayed_opacity_;
		Actor*			parent_;
		Stage*			stage_;
		size_t			hash_name_;
		Point			anchor_;
		Size			size_;
		Children		children_;
		UpdateCallback	cb_update_;
		Transform		transform_;

		bool				is_fast_transform_;
		mutable bool		visible_in_rt_;
		mutable	bool		dirty_visibility_;
		mutable bool		dirty_transform_;
		mutable bool		dirty_transform_inverse_;
		mutable Matrix3x2	transform_matrix_;
		mutable Matrix3x2	transform_matrix_inverse_;
	};

	inline void Actor::OnUpdate(Duration dt)
	{
		KGE_NOT_USED(dt);
	}

	inline void Actor::OnRender(RenderTarget* rt)
	{
		KGE_NOT_USED(rt);
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

	inline size_t Actor::GetHashName() const
	{
		return hash_name_;
	}

	inline int Actor::GetZOrder() const
	{
		return z_order_;
	}

	inline Point const& Actor::GetPosition() const
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

	inline Point const& Actor::GetScale() const
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

	inline Point const& Actor::GetSkew() const
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

	inline Size const& Actor::GetSize() const
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

	inline Point const& Actor::GetAnchor() const
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

	inline void Actor::SetCallbackOnUpdate(UpdateCallback const& cb)
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
		SetPosition(Point{ x, y });
	}

	inline void Actor::Move(float vx, float vy)
	{
		Move(Vec2{ vx, vy });
	}

	inline void Actor::SetScale(float scalex, float scaley)
	{
		SetScale(Vec2{ scalex, scaley });
	}

	inline void Actor::SetAnchor(float anchorx, float anchory)
	{
		SetAnchor(Vec2{ anchorx, anchory });
	}

	inline void Actor::SetSize(float width, float height)
	{
		SetSize(Size{ width, height });
	}

	inline void Actor::SetSkew(float skewx, float skewy)
	{
		SetSkew(Vec2{ skewx, skewy });
	}

}
