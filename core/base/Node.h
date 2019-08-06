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
#include "base.hpp"
#include "time.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "ActionManager.h"
#include "TaskManager.h"
#include "Transform.hpp"
#include "intrusive/List.hpp"

namespace easy2d
{
    class Game;

	// �ڵ�
	class Node
		: public RefCounter
		, public ActionManager
		, public TaskManager
		, protected intrusive::ListItem<spNode>
	{
		friend class Game;
		friend class Scene;
		friend class Transition;
		friend class intrusive::List<spNode>;

		using Nodes = std::vector<spNode>;
		using Children = intrusive::List<spNode>;

	public:
		Node();

		virtual ~Node();

		// ��Ⱦ�ڵ�
		virtual void OnDraw() {}

		// ���½ڵ�
		virtual void OnUpdate(Duration const& dt) {}

		// ��ȡ�ڵ���ʾ״̬
		bool IsVisible() const { return visible_; }

		// ��ȡ�ڵ�����
		String const& GetName() const { return name_; }

		// ��ȡ�ڵ����Ƶ� Hash ֵ
		size_t GetHashName() const { return hash_name_; }

		// ��ȡ�ڵ��ͼ˳��
		int GetOrder() const { return order_; }

		// ��ȡ�ڵ�����
		virtual const Point& GetPosition() const { return transform_.position; }

		// ��ȡ�ڵ���
		virtual float GetWidth() const { return transform_.size.width * transform_.scale.x; }

		// ��ȡ�ڵ�߶�
		virtual float GetHeight() const { return transform_.size.height * transform_.scale.y; }

		// ��ȡ�ڵ��С
		Size GetSize() const { return Size{ GetWidth(), GetHeight() }; }

		// ��ȡ�ڵ��֧��
		virtual float GetPivotX() const { return transform_.pivot.x; }

		// ��ȡ�ڵ��֧��
		virtual float GetPivotY() const { return transform_.pivot.y; }

		// ��ȡ�ڵ�������ű���
		virtual float GetScaleX() const { return transform_.scale.x; }

		// ��ȡ�ڵ��������ű���
		virtual float GetScaleY() const { return transform_.scale.y; }

		// ��ȡ�ڵ������нǶ�
		virtual float GetSkewX() const { return transform_.skew.x; }

		// ��ȡ�ڵ�������нǶ�
		virtual float GetSkewY() const { return transform_.skew.y; }

		// ��ȡ�ڵ���ת�Ƕ�
		virtual float GetRotation() const { return transform_.rotation; }

		// ��ȡ�ڵ�͸����
		virtual float GetOpacity() const { return opacity_; }

		// ��ȡ��Χ��
		virtual Rect GetBounds();

		// ��ȡ���ڵ�
		virtual spNode GetParent() const { return parent_; }

		// ���ýڵ��Ƿ���ʾ
		void SetVisible(
			bool val
		);

		// ���ýڵ�����
		void SetName(
			String const& name
		);

		// ���ýڵ������
		void SetPositionX(
			float x
		);

		// ���ýڵ�������
		void SetPositionY(
			float y
		);

		// ���ýڵ�����
		void SetPosition(
			const Point & point
		);

		// ���ýڵ�����
		void SetPosition(
			float x,
			float y
		);

		// �ƶ��ڵ�
		void MoveBy(
			float x,
			float y
		);

		// �ƶ��ڵ�
		void MoveBy(
			const Point & vector
		);

		// ���ýڵ��ͼ˳��
		// Ĭ��Ϊ 0
		void SetOrder(
			int order
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

		// ����͸����
		// Ĭ��Ϊ 1.0, ��Χ [0, 1]
		void SetOpacity(
			float opacity
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
		virtual void SetPivot(
			float pivot_x,
			float pivot_y
		);

		// �޸Ľڵ���
		void SetWidth(
			float width
		);

		// �޸Ľڵ�߶�
		void SetHeight(
			float height
		);

		// �޸Ľڵ��С
		void SetSize(
			float width,
			float height
		);

		// �޸Ľڵ��С
		void SetSize(
			const Size & size
		);

		// ���û�ر���Ⱦ����ü�
		void SetClipEnabled(
			bool enabled
		);

		// ���ýڵ��Ե��ɫ
		void SetBorderColor(
			const Color& color
		);

		Transform const& GetTransform() const;

		void SetTransform(
			Transform const& transform
		);

		// �жϵ��Ƿ��ڽڵ���
		bool ContainsPoint(
			const Point& point
		);

		// �ж��������Ƿ��ཻ
		bool Intersects(
			spNode const& node
		);

		// ����ӽڵ�
		void AddChild(
			spNode const& child,
			int order = 0		/* ��Ⱦ˳�� */
		);

		// ��Ӷ���ӽڵ�
		void AddChild(
			const Nodes& nodes,	/* �ڵ����� */
			int order = 0		/* ��Ⱦ˳�� */
		);

		// ��ȡ����������ͬ���ӽڵ�
		Nodes GetChildren(
			String const& name
		) const;

		// ��ȡ������ͬ���ӽڵ�
		spNode GetChild(
			String const& name
		) const;

		// ��ȡȫ���ӽڵ�
		Children const& GetChildren() const;

		// �Ƴ��ӽڵ�
		bool RemoveChild(
			spNode const& child
		);

		// �Ƴ�����������ͬ���ӽڵ�
		void RemoveChildren(
			String const& child_name
		);

		// �Ƴ����нڵ�
		void RemoveAllChildren();

		// �Ӹ��ڵ��Ƴ�
		void RemoveFromParent();

		// ����Ĭ��֧��
		static void SetDefaultPivot(
			float pivot_x,
			float pivot_y
		);

	protected:
		virtual void Visit();

		virtual bool Dispatch(
			const MouseEvent& e,
			bool handled
		);

		virtual bool Dispatch(
			const KeyEvent& e,
			bool handled
		);

	protected:
		virtual void Update(Duration const& dt);

		void UpdateTransform();

		void UpdateOpacity();

		void DrawBorder();

	protected:
		String			name_;
		size_t			hash_name_;
		float			display_opacity_;
		float			opacity_;
		int				order_;
		bool			visible_;
		bool			clip_enabled_;
		bool			dirty_sort_;
		bool			dirty_transform_;
		Node*			parent_;
		Color			border_color_;
		Children		children_;
		cpGeometry		border_;
		Transform		transform_;
		math::Matrix	initial_matrix_;
		math::Matrix	final_matrix_;
	};
}
