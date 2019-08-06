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
#include "ActionManager.h"
#include "../base/TimerManager.h"
#include "../base/EventDispatcher.h"

namespace kiwano
{
	class Application;

	// �ڵ�
	class KGE_API Node
		: public virtual Object
		, public TimerManager
		, public ActionManager
		, public EventDispatcher
		, public IntrusiveListItem<NodePtr>
	{
		friend class Application;
		friend class Transition;
		friend class IntrusiveList<NodePtr>;

		using Children = IntrusiveList<NodePtr>;
		using UpdateCallback = Closure<void(Duration)>;

	public:
		Node();

		// ���½ڵ�
		virtual void OnUpdate(Duration dt) { KGE_NOT_USED(dt); }

		// ��Ⱦ�ڵ�
		virtual void OnRender() {}

		// ��ȡ��ʾ״̬
		bool IsVisible()				const	{ return visible_; }

		// ��ȡ��Ӧ״̬
		bool IsResponsible()			const	{ return responsible_; }

		// ��ȡ���Ƶ� Hash ֵ
		size_t GetHashName()			const	{ return hash_name_; }

		// ��ȡ Z ��˳��
		int GetZOrder()					const	{ return z_order_; }

		// ��ȡ����
		Point GetPosition()				const	{ return transform_.position; }

		// ��ȡ x ����
		float GetPositionX()			const	{ return transform_.position.x; }

		// ��ȡ y ����
		float GetPositionY()			const	{ return transform_.position.y; }

		// ��ȡ�������ű���
		Point GetScale()				const	{ return transform_.scale; }

		// ��ȡ�������ű���
		float GetScaleX()				const	{ return transform_.scale.x; }

		// ��ȡ�������ű���
		float GetScaleY()				const	{ return transform_.scale.y; }

		// ��ȡ���нǶ�
		Point GetSkew()					const { return transform_.skew; }

		// ��ȡ������нǶ�
		float GetSkewX()				const	{ return transform_.skew.x; }

		// ��ȡ������нǶ�
		float GetSkewY()				const	{ return transform_.skew.y; }

		// ��ȡ��ת�Ƕ�
		float GetRotation()				const	{ return transform_.rotation; }

		// ��ȡ���
		float GetWidth()				const	{ return size_.x; }

		// ��ȡ�߶�
		float GetHeight()				const	{ return size_.y; }

		// ��ȡ��С
		Size GetSize()					const	{ return size_; }

		// ��ȡ���ź�Ŀ��
		float GetScaledWidth()			const	{ return size_.x * transform_.scale.x; }

		// ��ȡ���ź�ĸ߶�
		float GetScaledHeight()			const	{ return size_.y * transform_.scale.y; }

		// ��ȡ���ź�Ĵ�С
		Size GetScaledSize()			const	{ return Size{ GetScaledWidth(), GetScaledHeight() }; }

		// ��ȡê��
		Point GetAnchor()				const	{ return anchor_; }

		// ��ȡ x ����ê��
		float GetAnchorX()				const	{ return anchor_.x; }

		// ��ȡ y ����ê��
		float GetAnchorY()				const	{ return anchor_.y; }

		// ��ȡ͸����
		float GetOpacity()				const	{ return opacity_; }

		// ��ȡ�任
		Transform GetTransform()		const	{ return transform_; }

		// ��ȡ�߿�
		Rect GetBounds() const;

		// ��ȡ���а�Χ��
		Rect GetBoundingBox() const;

		// ��ȡ��ά�任����
		Matrix const& GetTransformMatrix()  const;

		// ��ȡ��ά�任�������
		Matrix const& GetTransformInverseMatrix()  const;

		// ��ȡ���ڵ�
		inline Node* GetParent() const { return parent_; }

		// ��ȡ���ڳ���
		inline Scene* GetScene() const { return scene_; }

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
			float x
		);

		// ����������
		void SetPositionY(
			float y
		);

		// ��������
		void SetPosition(
			const Point & point
		);

		// ��������
		void SetPosition(
			float x,
			float y
		);

		// �ƶ�
		void Move(
			float x,
			float y
		);

		// �ƶ�
		void Move(
			const Point & vector
		);

		// ���ú������ű���
		// Ĭ��Ϊ 1.0
		void SetScaleX(
			float scale_x
		);

		// �����������ű���
		// Ĭ��Ϊ 1.0
		void SetScaleY(
			float scale_y
		);

		// �������ű���
		// Ĭ��Ϊ (1.0, 1.0)
		void SetScale(
			float scale_x,
			float scale_y
		);

		// �������ű���
		// Ĭ��Ϊ (1.0, 1.0)
		void SetScale(
			Point const& scale
		);

		// �������ű���
		// Ĭ��Ϊ 1.0
		void SetScale(
			float scale
		);

		// ���ú�����нǶ�
		// Ĭ��Ϊ 0
		void SetSkewX(
			float skew_x
		);

		// ����������нǶ�
		// Ĭ��Ϊ 0
		void SetSkewY(
			float skew_y
		);

		// ���ô��нǶ�
		// Ĭ��Ϊ (0, 0)
		void SetSkew(
			float skew_x,
			float skew_y
		);

		// ���ô��нǶ�
		// Ĭ��Ϊ (0, 0)
		void SetSkew(
			Point const& skew
		);

		// ������ת�Ƕ�
		// Ĭ��Ϊ 0
		void SetRotation(
			float rotation
		);

		// ����ê��ĺ���λ��
		// Ĭ��Ϊ 0, ��Χ [0, 1]
		void SetAnchorX(
			float anchor_x
		);

		// ����ê�������λ��
		// Ĭ��Ϊ 0, ��Χ [0, 1]
		void SetAnchorY(
			float anchor_y
		);

		// ����ê��λ��
		// Ĭ��Ϊ (0, 0), ��Χ [0, 1]
		void SetAnchor(
			float anchor_x,
			float anchor_y
		);

		// ����ê��λ��
		// Ĭ��Ϊ (0, 0), ��Χ [0, 1]
		void SetAnchor(
			Point const& anchor
		);

		// �޸Ŀ��
		void SetWidth(
			float width
		);

		// �޸ĸ߶�
		void SetHeight(
			float height
		);

		// �޸Ĵ�С
		void SetSize(
			float width,
			float height
		);

		// �޸Ĵ�С
		void SetSize(
			const Size & size
		);

		void SetTransform(
			Transform const& transform
		);

		// ����͸����
		// Ĭ��Ϊ 1.0, ��Χ [0, 1]
		void SetOpacity(
			float opacity
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

		// �жϵ��Ƿ��ڽڵ���
		bool ContainsPoint(
			const Point& point
		)  const;

		// ����ӽڵ�
		void AddChild(
			NodePtr child
		);

		// ��Ӷ���ӽڵ�
		void AddChildren(
			Array<NodePtr> const& children
		);

		// ��ȡ����������ͬ���ӽڵ�
		Array<NodePtr> GetChildren(
			String const& name
		) const;

		// ��ȡ������ͬ���ӽڵ�
		NodePtr GetChild(
			String const& name
		) const;

		// ��ȡȫ���ӽڵ�
		Children const& GetChildren() const;

		// �Ƴ��ӽڵ�
		void RemoveChild(
			NodePtr child
		);

		// �Ƴ��ӽڵ�
		void RemoveChild(
			Node* child
		);

		// �Ƴ�����������ͬ���ӽڵ�
		void RemoveChildren(
			String const& child_name
		);

		// �Ƴ����нڵ�
		void RemoveAllChildren();

		// �Ӹ��ڵ��Ƴ�
		void RemoveFromParent();

		// ��ͣ�ڵ����
		inline void PauseUpdating()									{ update_pausing_ = true; }

		// �����ڵ����
		inline void ResumeUpdating()								{ update_pausing_ = false; }

		// �ڵ�����Ƿ���ͣ
		inline bool IsUpdatePausing() const							{ return update_pausing_; }

		// ���ø���ʱ�Ļص�����
		inline void SetCallbackOnUpdate(UpdateCallback const& cb)	{ cb_update_ = cb; }

		// ��ȡ����ʱ�Ļص�����
		inline UpdateCallback const& GetCallbackOnUpdate()			{ return cb_update_; }

		// ����Ĭ��ê��
		static void SetDefaultAnchor(
			float anchor_x,
			float anchor_y
		);

	public:
		// �¼��ַ�
		void Dispatch(Event& evt) override;

	protected:
		virtual void PrepareRender() {}

		virtual void Update(Duration dt);

		void Render();

		void UpdateTransform() const;

		void UpdateOpacity();

		void SetScene(Scene* scene);

	protected:
		bool			visible_;
		bool			hover_;
		bool			pressed_;
		bool			responsible_;
		bool			update_pausing_;
		int				z_order_;
		float			opacity_;
		float			display_opacity_;
		size_t			hash_name_;
		Transform		transform_;
		Point			anchor_;
		Size			size_;
		Node*			parent_;
		Scene*			scene_;
		Children		children_;
		UpdateCallback	cb_update_;

		mutable bool	dirty_transform_;
		mutable bool	dirty_transform_inverse_;
		mutable Matrix	transform_matrix_;
		mutable Matrix	transform_matrix_inverse_;
	};


	// ���ӻ��ڵ�
	class KGE_API VisualNode
		: public Node
	{
	public:
		virtual void PrepareRender() override;
	};

}
