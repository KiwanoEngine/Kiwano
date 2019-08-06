// Copyright (c) 2016-2018 Easy2D - Nomango
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
#include "time.h"
#include "Transform.hpp"
#include "TaskManager.h"
#include "ActionManager.h"
#include "EventDispatcher.h"
#include "intrusive/List.hpp"

namespace easy2d
{
	class Game;

	// �ڵ�
	class Node
		: public Object
		, public TaskManager
		, public ActionManager
		, public EventDispatcher
		, protected intrusive::ListItem<SpNode>
	{
		friend class Game;
		friend class Scene;
		friend class Transition;
		friend class intrusive::List<SpNode>;

		using Children = intrusive::List<SpNode>;

	public:
		Node();

		// ���½ڵ�
		virtual void OnUpdate(Duration const& dt) {}

		// ��Ⱦ�ڵ�
		virtual void OnRender() {}

		// ��ȡ��ʾ״̬
		bool IsVisible()				const	{ return visible_; }

		// ��ȡ����
		String const& GetName()	const	{ return name_; }

		// ��ȡ���Ƶ� Hash ֵ
		size_t GetHashName()			const	{ return hash_name_; }

		// ��ȡ Z ��˳��
		int GetZOrder()					const	{ return z_order_; }

		// ��ȡ����
		Point const& GetPosition()		const	{ return transform_.position; }

		// ��ȡ x ����
		float GetPositionX()			const	{ return transform_.position.x; }

		// ��ȡ y ����
		float GetPositionY()			const	{ return transform_.position.y; }

		// ��ȡ�������ű���
		float GetScaleX()				const	{ return transform_.scale.x; }

		// ��ȡ�������ű���
		float GetScaleY()				const	{ return transform_.scale.y; }

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
		Size const& GetSize()			const	{ return size_; }

		// ��ȡ���ź�Ŀ��
		float GetScaledWidth()			const	{ return size_.x * transform_.scale.x; }

		// ��ȡ���ź�ĸ߶�
		float GetScaledHeight()			const	{ return size_.y * transform_.scale.y; }

		// ��ȡ���ź�Ĵ�С
		Size GetScaledSize()			const	{ return Size{ GetScaledWidth(), GetScaledHeight() }; }

		// ��ȡ x ����֧��
		float GetPivotX()				const	{ return pivot_.x; }

		// ��ȡ y ����֧��
		float GetPivotY()				const	{ return pivot_.y; }

		// ��ȡ͸����
		float GetOpacity()				const	{ return opacity_; }

		// ��ȡ�任
		Transform const& GetTransform()	const	{ return transform_; }

		// ��ȡ��Χ��
		Rect GetBounds() const;

		// ��ȡ��ά�任����
		Matrix const& GetTransformMatrix()  const;

		// ��ȡ��ά�任�������
		Matrix const& GetTransformInverseMatrix()  const;

		// ��ȡ���ڵ�
		Node* GetParent() const;

		// ��ȡ���ڳ���
		Scene* GetScene() const;

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

		// ������ת�Ƕ�
		// Ĭ��Ϊ 0
		void SetRotation(
			float rotation
		);

		// ����֧��ĺ���λ��
		// Ĭ��Ϊ 0, ��Χ [0, 1]
		void SetPivotX(
			float pivot_x
		);

		// ����֧�������λ��
		// Ĭ��Ϊ 0, ��Χ [0, 1]
		void SetPivotY(
			float pivot_y
		);

		// ����֧��λ��
		// Ĭ��Ϊ (0, 0), ��Χ [0, 1]
		void SetPivot(
			float pivot_x,
			float pivot_y
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

		// �жϵ��Ƿ��ڽڵ���
		bool ContainsPoint(
			const Point& point
		)  const;

		// ����ӽڵ�
		void AddChild(
			SpNode const& child
		);

		// ��Ӷ���ӽڵ�
		void AddChildren(
			Array<SpNode> const& children
		);

		// ��ȡ����������ͬ���ӽڵ�
		Array<SpNode> GetChildren(
			String const& name
		) const;

		// ��ȡ������ͬ���ӽڵ�
		SpNode GetChild(
			String const& name
		) const;

		// ��ȡȫ���ӽڵ�
		Children const& GetChildren() const;

		// �Ƴ��ӽڵ�
		bool RemoveChild(
			SpNode const& child
		);

		// �Ƴ��ӽڵ�
		bool RemoveChild(
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

		virtual void DispatchEvent(Event* e) override;

		// ����Ĭ��֧��
		static void SetDefaultPivot(
			float pivot_x,
			float pivot_y
		);

	protected:
		void Update(Duration const& dt);

		void Render();

		void UpdateTransform() const;

		void UpdateOpacity();

		void SetScene(Scene* scene);

	protected:
		bool		visible_;
		bool		hover_;
		bool		pressed_;
		int			z_order_;
		float		opacity_;
		float		display_opacity_;
		String		name_;
		size_t		hash_name_;
		Transform	transform_;
		Point		pivot_;
		Size		size_;
		Node*		parent_;
		Scene*		scene_;
		Children	children_;

		mutable bool	dirty_transform_;
		mutable bool	dirty_transform_inverse_;
		mutable Matrix	transform_matrix_;
		mutable Matrix	transform_matrix_inverse_;
	};
}
