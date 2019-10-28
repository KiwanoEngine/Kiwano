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
		, public intrusive_list_item<ActorPtr>
	{
		friend class Director;
		friend class Transition;
		friend class intrusive_list<ActorPtr>;

	public:
		using Children			= intrusive_list<ActorPtr>;
		using UpdateCallback	= Function<void(Duration)>;

		Actor();

		// ���½�ɫ
		virtual void		OnUpdate(Duration dt)			{ KGE_UNUSED(dt); }

		// ��Ⱦ��ɫ
		virtual void		OnRender(RenderTarget* rt)		{ KGE_UNUSED(rt); }

		// ��ȡ��ʾ״̬
		bool				IsVisible() const				{ return visible_; }

		// ��ȡ��Ӧ״̬
		bool				IsResponsible() const			{ return responsible_; }

		// �Ƿ����ü���͸����
		bool				IsCascadeOpacityEnabled() const	{ return cascade_opacity_; }

		// ��ȡ���Ƶ� Hash ֵ
		size_t				GetHashName() const				{ return hash_name_; }

		// ��ȡ Z ��˳��
		int					GetZOrder() const				{ return z_order_; }

		// ��ȡ����
		Point				GetPosition() const				{ return transform_.position; }

		// ��ȡ x ����
		float				GetPositionX() const			{ return transform_.position.x; }

		// ��ȡ y ����
		float				GetPositionY() const			{ return transform_.position.y; }

		// ��ȡ���ű���
		Point				GetScale() const				{ return transform_.scale; }

		// ��ȡ�������ű���
		float				GetScaleX() const				{ return transform_.scale.x; }

		// ��ȡ�������ű���
		float				GetScaleY() const				{ return transform_.scale.y; }

		// ��ȡ���нǶ�
		Point				GetSkew() const					{ return transform_.skew; }

		// ��ȡ������нǶ�
		float				GetSkewX() const				{ return transform_.skew.x; }

		// ��ȡ������нǶ�
		float				GetSkewY() const				{ return transform_.skew.y; }

		// ��ȡ��ת�Ƕ�
		float				GetRotation() const				{ return transform_.rotation; }

		// ��ȡ���
		float				GetWidth() const				{ return size_.x; }

		// ��ȡ�߶�
		float				GetHeight() const				{ return size_.y; }

		// ��ȡ��С
		Size				GetSize() const					{ return size_; }

		// ��ȡ���ź�Ŀ��
		float				GetScaledWidth() const			{ return size_.x * transform_.scale.x; }

		// ��ȡ���ź�ĸ߶�
		float				GetScaledHeight() const			{ return size_.y * transform_.scale.y; }

		// ��ȡ���ź�Ĵ�С
		Size				GetScaledSize() const			{ return Size{ GetScaledWidth(), GetScaledHeight() }; }

		// ��ȡê��
		Point				GetAnchor() const				{ return anchor_; }

		// ��ȡ x ����ê��
		float				GetAnchorX() const				{ return anchor_.x; }

		// ��ȡ y ����ê��
		float				GetAnchorY() const				{ return anchor_.y; }

		// ��ȡ͸����
		float				GetOpacity() const				{ return opacity_; }

		// ��ȡ��ʾ͸����
		float				GetDisplayedOpacity() const		{ return displayed_opacity_; }

		// ��ȡ�任
		Transform			GetTransform() const			{ return transform_; }

		// ��ȡ����ɫ
		inline Actor*		GetParent() const				{ return parent_; }

		// ��ȡ������̨
		inline Stage*		GetStage() const				{ return stage_; }

		// ��ȡ�߿�
		virtual Rect		GetBounds() const;

		// ��ȡ���а�Χ��
		virtual Rect		GetBoundingBox() const;

		// ��ȡ��ά�任����
		Matrix3x2 const&	GetTransformMatrix()  const;

		// ��ȡ��ά�任�������
		Matrix3x2 const&	GetTransformInverseMatrix()  const;

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
		inline void SetPosition(
			float x,
			float y
		)
		{
			SetPosition(Point{ x, y });
		}

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
		inline void Move(
			float vx,
			float vy
		)
		{
			Move(Vec2{ vx, vy });
		}

		// �������ű���
		// Ĭ��Ϊ (1.0, 1.0)
		virtual void SetScale(
			Vec2 const& scale
		);

		// �������ű���
		// Ĭ��Ϊ (1.0, 1.0)
		inline void SetScale(
			float scalex,
			float scaley
		)
		{
			SetScale(Vec2{ scalex, scaley });
		}

		// ���ô��нǶ�
		// Ĭ��Ϊ (0, 0)
		virtual void SetSkew(
			Vec2 const& skew
		);

		// ���ô��нǶ�
		// Ĭ��Ϊ (0, 0)
		inline void SetSkew(
			float skewx,
			float skewy
		)
		{
			SetSkew(Vec2{ skewx, skewy });
		}

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
		inline void SetAnchor(
			float anchorx,
			float anchory
		)
		{
			SetAnchor(Vec2{ anchorx, anchory });
		}

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
		inline void SetSize(
			float width,
			float height
		)
		{
			SetSize(Size{ width, height });
		}

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

		// �¼��ַ�
		void Dispatch(Event& evt) override;

		// ����Ĭ��ê��
		static void SetDefaultAnchor(
			float anchor_x,
			float anchor_y
		);

		// ��ͣ��ɫ����
		inline void				PauseUpdating()									{ update_pausing_ = true; }

		// ������ɫ����
		inline void				ResumeUpdating()								{ update_pausing_ = false; }

		// ��ɫ�����Ƿ���ͣ
		inline bool				IsUpdatePausing() const							{ return update_pausing_; }

		// ���ø���ʱ�Ļص�����
		inline void				SetCallbackOnUpdate(UpdateCallback const& cb)	{ cb_update_ = cb; }

		// ��ȡ����ʱ�Ļص�����
		inline UpdateCallback	GetCallbackOnUpdate() const						{ return cb_update_; }

		// ��Ⱦ��ɫ�߽�
		inline void				ShowBorder(bool show)							{ show_border_ = show; }

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

	protected:
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

}
