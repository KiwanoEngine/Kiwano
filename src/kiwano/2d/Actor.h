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
#include "include-forwards.h"
#include "Transform.hpp"
#include "action/ActionManager.h"
#include "../base/TimerManager.h"
#include "../base/EventDispatcher.h"

namespace kiwano
{
	class Director;
	class Renderer;

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

		using Children = intrusive_list<ActorPtr>;
		using UpdateCallback = Function<void(Duration)>;

	public:
		Actor();

		// ���½�ɫ
		virtual void OnUpdate(Duration dt) { KGE_UNUSED(dt); }

		// ��Ⱦ��ɫ
		virtual void OnRender(Renderer* renderer) { KGE_UNUSED(renderer); }

		// ��ȡ��ʾ״̬
		bool IsVisible()				const	{ return visible_; }

		// ��ȡ��Ӧ״̬
		bool IsResponsible()			const	{ return responsible_; }

		// �Ƿ����ü���͸����
		bool IsCascadeOpacityEnabled()	const	{ return cascade_opacity_; }

		// ��ȡ���Ƶ� Hash ֵ
		UInt32 GetHashName()			const	{ return hash_name_; }

		// ��ȡ Z ��˳��
		Int32 GetZOrder()					const	{ return z_order_; }

		// ��ȡ����
		Point GetPosition()				const	{ return transform_.position; }

		// ��ȡ x ����
		Float32 GetPositionX()			const	{ return transform_.position.x; }

		// ��ȡ y ����
		Float32 GetPositionY()			const	{ return transform_.position.y; }

		// ��ȡ���ű���
		Point GetScale()				const	{ return transform_.scale; }

		// ��ȡ�������ű���
		Float32 GetScaleX()				const	{ return transform_.scale.x; }

		// ��ȡ�������ű���
		Float32 GetScaleY()				const	{ return transform_.scale.y; }

		// ��ȡ���нǶ�
		Point GetSkew()					const	{ return transform_.skew; }

		// ��ȡ������нǶ�
		Float32 GetSkewX()				const	{ return transform_.skew.x; }

		// ��ȡ������нǶ�
		Float32 GetSkewY()				const	{ return transform_.skew.y; }

		// ��ȡ��ת�Ƕ�
		Float32 GetRotation()				const	{ return transform_.rotation; }

		// ��ȡ���
		Float32 GetWidth()				const	{ return size_.x; }

		// ��ȡ�߶�
		Float32 GetHeight()				const	{ return size_.y; }

		// ��ȡ��С
		Size GetSize()					const	{ return size_; }

		// ��ȡ���ź�Ŀ��
		Float32 GetScaledWidth()			const	{ return size_.x * transform_.scale.x; }

		// ��ȡ���ź�ĸ߶�
		Float32 GetScaledHeight()			const	{ return size_.y * transform_.scale.y; }

		// ��ȡ���ź�Ĵ�С
		Size GetScaledSize()			const	{ return Size{ GetScaledWidth(), GetScaledHeight() }; }

		// ��ȡê��
		Point GetAnchor()				const	{ return anchor_; }

		// ��ȡ x ����ê��
		Float32 GetAnchorX()				const	{ return anchor_.x; }

		// ��ȡ y ����ê��
		Float32 GetAnchorY()				const	{ return anchor_.y; }

		// ��ȡ͸����
		Float32 GetOpacity()				const	{ return opacity_; }

		// ��ȡ��ʾ͸����
		Float32 GetDisplayedOpacity()		const	{ return displayed_opacity_; }

		// ��ȡ�任
		Transform GetTransform()		const	{ return transform_; }

		// ��ȡ�߿�
		virtual Rect GetBounds() const;

		// ��ȡ���а�Χ��
		virtual Rect GetBoundingBox() const;

		// ��ȡ��ά�任����
		Matrix3x2 const& GetTransformMatrix()  const;

		// ��ȡ��ά�任�������
		Matrix3x2 const& GetTransformInverseMatrix()  const;

		// ��ȡ����ɫ
		inline Actor* GetParent() const { return parent_; }

		// ��ȡ������̨
		inline Stage* GetStage() const { return stage_; }

		// �����Ƿ���ʾ
		void SetVisible(
			bool val
		);

		// ��������
		void SetName(
			String const& name
		);

		// ���ú�����
		void SetPositionX(
			Float32 x
		);

		// ����������
		void SetPositionY(
			Float32 y
		);

		// ��������
		void SetPosition(
			const Point & point
		);

		// ��������
		void SetPosition(
			Float32 x,
			Float32 y
		);

		// �ƶ�
		void Move(
			Float32 x,
			Float32 y
		);

		// �ƶ�
		void Move(
			const Point & vector
		);

		// ���ú������ű���
		// Ĭ��Ϊ 1.0
		void SetScaleX(
			Float32 scale_x
		);

		// �����������ű���
		// Ĭ��Ϊ 1.0
		void SetScaleY(
			Float32 scale_y
		);

		// �������ű���
		// Ĭ��Ϊ (1.0, 1.0)
		void SetScale(
			Float32 scale_x,
			Float32 scale_y
		);

		// �������ű���
		// Ĭ��Ϊ (1.0, 1.0)
		void SetScale(
			Point const& scale
		);

		// �������ű���
		// Ĭ��Ϊ 1.0
		void SetScale(
			Float32 scale
		);

		// ���ú�����нǶ�
		// Ĭ��Ϊ 0
		void SetSkewX(
			Float32 skew_x
		);

		// ����������нǶ�
		// Ĭ��Ϊ 0
		void SetSkewY(
			Float32 skew_y
		);

		// ���ô��нǶ�
		// Ĭ��Ϊ (0, 0)
		void SetSkew(
			Float32 skew_x,
			Float32 skew_y
		);

		// ���ô��нǶ�
		// Ĭ��Ϊ (0, 0)
		void SetSkew(
			Point const& skew
		);

		// ������ת�Ƕ�
		// Ĭ��Ϊ 0
		void SetRotation(
			Float32 rotation
		);

		// ����ê��ĺ���λ��
		// Ĭ��Ϊ 0, ��Χ [0, 1]
		void SetAnchorX(
			Float32 anchor_x
		);

		// ����ê�������λ��
		// Ĭ��Ϊ 0, ��Χ [0, 1]
		void SetAnchorY(
			Float32 anchor_y
		);

		// ����ê��λ��
		// Ĭ��Ϊ (0, 0), ��Χ [0, 1]
		void SetAnchor(
			Float32 anchor_x,
			Float32 anchor_y
		);

		// ����ê��λ��
		// Ĭ��Ϊ (0, 0), ��Χ [0, 1]
		void SetAnchor(
			Point const& anchor
		);

		// �޸Ŀ��
		void SetWidth(
			Float32 width
		);

		// �޸ĸ߶�
		void SetHeight(
			Float32 height
		);

		// �޸Ĵ�С
		void SetSize(
			Float32 width,
			Float32 height
		);

		// �޸Ĵ�С
		void SetSize(
			const Size & size
		);

		// ���ö�ά����任
		void SetTransform(
			Transform const& transform
		);

		// ����͸����
		// Ĭ��Ϊ 1.0, ��Χ [0, 1]
		void SetOpacity(
			Float32 opacity
		);

		// ���û���ü���͸����
		void SetCascadeOpacityEnabled(
			bool enabled
		);

		// ���� Z ��˳��
		// Ĭ��Ϊ 0
		void SetZOrder(
			Int32 zorder
		);

		// �Ƿ����Ӧ (��� Hover | Out | Click ��Ϣ)
		// Ĭ��Ϊ false
		void SetResponsible(
			bool enable
		);

		// �жϵ��Ƿ��ڽ�ɫ��
		bool ContainsPoint(
			const Point& point
		)  const;

		// ����ӽ�ɫ
		void AddChild(
			ActorPtr child
		);

		// ��Ӷ���ӽ�ɫ
		void AddChildren(
			Vector<ActorPtr> const& children
		);

		// ��ȡ����������ͬ���ӽ�ɫ
		Vector<ActorPtr> GetChildren(
			String const& name
		) const;

		// ��ȡ������ͬ���ӽ�ɫ
		ActorPtr GetChild(
			String const& name
		) const;

		// ��ȡȫ���ӽ�ɫ
		Children const& GetChildren() const;

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

		// �¼��ַ�
		void Dispatch(Event& evt) override;

		// ��ͣ��ɫ����
		inline void PauseUpdating()									{ update_pausing_ = true; }

		// ������ɫ����
		inline void ResumeUpdating()								{ update_pausing_ = false; }

		// ��ɫ�����Ƿ���ͣ
		inline bool IsUpdatePausing() const							{ return update_pausing_; }

		// ���ø���ʱ�Ļص�����
		inline void SetCallbackOnUpdate(UpdateCallback const& cb)	{ cb_update_ = cb; }

		// ��ȡ����ʱ�Ļص�����
		inline UpdateCallback GetCallbackOnUpdate() const			{ return cb_update_; }

		// ��Ⱦ��ɫ�߽�
		inline void ShowBorder(bool show)							{ show_border_ = show; }

		// ����Ĭ��ê��
		static void SetDefaultAnchor(
			Float32 anchor_x,
			Float32 anchor_y
		);

	protected:
		virtual void Update(Duration dt);

		virtual void Render(Renderer* renderer);

		void PrepareRender(Renderer* renderer);

		void RenderBorder();

		void UpdateTransform() const;

		void UpdateOpacity();

		void Reorder();

		void SetStage(Stage* scene);

	protected:
		bool			visible_;
		bool			hover_;
		bool			pressed_;
		bool			responsible_;
		bool			update_pausing_;
		bool			cascade_opacity_;
		bool			show_border_;
		Int32			z_order_;
		Float32			opacity_;
		Float32			displayed_opacity_;
		Actor*			parent_;
		Stage*			stage_;
		UInt32			hash_name_;
		Point			anchor_;
		Size			size_;
		Children		children_;
		UpdateCallback	cb_update_;
		Transform		transform_;

		bool				is_fast_transform_;
		mutable bool		dirty_transform_;
		mutable bool		dirty_transform_inverse_;
		mutable Matrix3x2	transform_matrix_;
		mutable Matrix3x2	transform_matrix_inverse_;
	};

}
