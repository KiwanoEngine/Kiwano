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
#include <kiwano/core/common.h>
#include <kiwano/core/time.h>
#include <kiwano/core/ObjectBase.h>
#include <kiwano/math/math.h>
#include <kiwano/2d/Transform.h>
#include <kiwano/2d/action/ActionManager.h>
#include <kiwano/core/TimerManager.h>
#include <kiwano/core/EventDispatcher.h>

namespace kiwano
{
	class Stage;
	class Director;
	class RenderTarget;

	KGE_DECLARE_SMART_PTR(Actor);

	/**
	* \~chinese
	* \defgroup Actors ��ɫ
	*/

	/**
	* \addtogroup Actors
	* @{
	*/

	/**
	* \~chinese
	* @brief ��ɫ
	* @details ��ɫ����̨���������Ԫ�أ��������Ⱦ�����¡��¼��ַ��ȹ��ܵ���С��λ��Ҳ�Ƕ�������ʱ�����¼������ȹ��ܵ�����
	*/
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

		/// \~chinese
		/// @brief ���½�ɫ
		/// @details ÿ֡����ˢ��ǰ���øú��������ظú�����ʵ�ֽ�ɫ�ĸ��´���
		/// @param dt ����һ�θ��µ�ʱ����
		virtual void OnUpdate(Duration dt);

		/// \~chinese
		/// @brief ��Ⱦ��ɫ
		/// @details ÿ֡����ˢ��ʱ���øú�����Ĭ�ϲ�������Ⱦ�����ظú�����ʵ�־�����Ⱦ����
		/// @param rt ��ȾĿ��
		virtual void OnRender(RenderTarget* rt);

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
		/// @brief ��ȡ���Ƶ� Hash ֵ
		size_t GetHashName() const;

		/// \~chinese
		/// @brief ��ȡ Z ��˳��
		int GetZOrder() const;

		/// \~chinese
		/// @brief ��ȡ����
		Point const& GetPosition() const;

		/// \~chinese
		/// @brief ��ȡ x ����
		float GetPositionX() const;

		/// \~chinese
		/// @brief ��ȡ y ����
		float GetPositionY() const;

		/// \~chinese
		/// @brief ��ȡ���ű���
		Point const& GetScale() const;

		/// \~chinese
		/// @brief ��ȡ�������ű���
		float GetScaleX() const;

		/// \~chinese
		/// @brief ��ȡ�������ű���
		float GetScaleY() const;

		/// \~chinese
		/// @brief ��ȡ���нǶ�
		Point const& GetSkew() const;

		/// \~chinese
		/// @brief ��ȡ������нǶ�
		float GetSkewX() const;

		/// \~chinese
		/// @brief ��ȡ������нǶ�
		float GetSkewY() const;

		/// \~chinese
		/// @brief ��ȡ��ת�Ƕ�
		float GetRotation() const;

		/// \~chinese
		/// @brief ��ȡ���
		float GetWidth() const;

		/// \~chinese
		/// @brief ��ȡ�߶�
		float GetHeight() const;

		/// \~chinese
		/// @brief ��ȡ��С
		Size const& GetSize() const;

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
		Point const& GetAnchor() const;

		/// \~chinese
		/// @brief ��ȡ x ����ê��
		float GetAnchorX() const;

		/// \~chinese
		/// @brief ��ȡ y ����ê��
		float GetAnchorY() const;

		/// \~chinese
		/// @brief ��ȡ͸����
		float GetOpacity() const;

		/// \~chinese
		/// @brief ��ȡ��ʾ͸����
		float GetDisplayedOpacity() const;

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
		Matrix3x2 const& GetTransformMatrix()  const;

		/// \~chinese
		/// @brief ��ȡ��ά�任�������
		Matrix3x2 const& GetTransformInverseMatrix()  const;

		/// \~chinese
		/// @brief ���ý�ɫ�Ƿ�ɼ�
		void SetVisible(bool val);

		/// \~chinese
		/// @brief ��������
		void SetName(String const& name);

		/// \~chinese
		/// @brief ��������
		virtual void SetPosition(Point const& point);

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
		/// @brief �ƶ�����
		void Move(Vec2 const& v);

		/// \~chinese
		/// @brief �ƶ�����
		void Move(float vx, float vy);

		/// \~chinese
		/// @brief �������ű�����Ĭ��Ϊ (1.0, 1.0)
		virtual void SetScale(Vec2 const& scale);

		/// \~chinese
		/// @brief �������ű�����Ĭ��Ϊ (1.0, 1.0)
		void SetScale(float scalex, float scaley);

		/// \~chinese
		/// @brief ���ô��нǶȣ�Ĭ��Ϊ (0, 0)
		virtual void SetSkew(Vec2 const& skew);

		/// \~chinese
		/// @brief ���ô��нǶȣ�Ĭ��Ϊ (0, 0)
		void SetSkew(float skewx, float skewy);

		/// \~chinese
		/// @brief ������ת�Ƕȣ�Ĭ��Ϊ 0
		virtual void SetRotation(float rotation);

		/// \~chinese
		/// @brief ����ê��λ�ã�Ĭ��Ϊ (0, 0), ��Χ [0, 1]
		virtual void SetAnchor(Vec2 const& anchor);

		/// \~chinese
		/// @brief ����ê��λ�ã�Ĭ��Ϊ (0, 0), ��Χ [0, 1]
		void SetAnchor(float anchorx, float anchory);

		/// \~chinese
		/// @brief �޸Ŀ��
		virtual void SetWidth(float width);

		/// \~chinese
		/// @brief �޸ĸ߶�
		virtual void SetHeight(float height);

		/// \~chinese
		/// @brief �޸Ĵ�С
		virtual void SetSize(Size const& size);

		/// \~chinese
		/// @brief �޸Ĵ�С
		void SetSize(float width, float height);

		/// \~chinese
		/// @brief ����͸���ȣ�Ĭ��Ϊ 1.0, ��Χ [0, 1]
		virtual void SetOpacity(float opacity);

		/// \~chinese
		/// @brief ���û���ü���͸����
		void SetCascadeOpacityEnabled(bool enabled);

		/// \~chinese
		/// @brief ���ö�ά����任
		void SetTransform(Transform const& transform);

		/// \~chinese
		/// @brief ���� Z ��˳��Ĭ��Ϊ 0
		void SetZOrder(int zorder);

		/// \~chinese
		/// @brief ���ý�ɫ�Ƿ����Ӧ��Ĭ��Ϊ false
		/// @details ����Ӧ�Ľ�ɫ���յ����� Hover | Out | Click ��Ϣ
		void SetResponsible(bool enable);

		/// \~chinese
		/// @brief ����ӽ�ɫ
		void AddChild(ActorPtr child, int zorder = 0);

		/// \~chinese
		/// @brief ����ӽ�ɫ
		void AddChild(Actor* child, int zorder = 0);

		/// \~chinese
		/// @brief ��Ӷ���ӽ�ɫ
		void AddChildren(Vector<ActorPtr> const& children);

		/// \~chinese
		/// @brief ��ȡ������ͬ���ӽ�ɫ
		Actor* GetChild(String const& name) const;

		/// \~chinese
		/// @brief ��ȡ����������ͬ���ӽ�ɫ
		Vector<ActorPtr> GetChildren(String const& name) const;

		/// \~chinese
		/// @brief ��ȡȫ���ӽ�ɫ
		Children& GetAllChildren();

		/// \~chinese
		/// @brief ��ȡȫ���ӽ�ɫ
		Children const& GetAllChildren() const;

		/// \~chinese
		/// @brief �Ƴ��ӽ�ɫ
		void RemoveChild(ActorPtr child);

		/// \~chinese
		/// @brief �Ƴ��ӽ�ɫ
		void RemoveChild(Actor* child);

		/// \~chinese
		/// @brief �Ƴ�����������ͬ���ӽ�ɫ
		void RemoveChildren(String const& child_name);

		/// \~chinese
		/// @brief �Ƴ����н�ɫ
		void RemoveAllChildren();

		/// \~chinese
		/// @brief �Ӹ���ɫ�Ƴ�
		void RemoveFromParent();

		/// \~chinese
		/// @brief �жϵ��Ƿ��ڽ�ɫ��
		virtual bool ContainsPoint(const Point& point) const;

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
		void SetCallbackOnUpdate(UpdateCallback const& cb);

		/// \~chinese
		/// @brief ��ȡ����ʱ�Ļص�����
		UpdateCallback GetCallbackOnUpdate() const;

		/// \~chinese
		/// @brief ��Ⱦ��ɫ�߽�
		void ShowBorder(bool show);

		/// \~chinese
		/// @brief �ַ��¼�
		void Dispatch(Event& evt) override;

		/// \~chinese
		/// @brief ����Ĭ��ê��
		static void SetDefaultAnchor(float anchor_x, float anchor_y);

	protected:
		virtual void Update(Duration dt);

		virtual void Render(RenderTarget* rt);

		virtual void PrepareToRender(RenderTarget* rt);

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

	/** @} */


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
