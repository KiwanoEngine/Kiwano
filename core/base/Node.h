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
#include "Unit.h"
#include "TaskManager.h"
#include "ActionManager.h"
#include "EventDispatcher.h"
#include "intrusive/List.hpp"

namespace easy2d
{
    class Game;

	// �ڵ�
	class Node
		: public Unit
		, public TaskManager
		, public ActionManager
		, public EventDispatcher
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

		// ��ʼ���ڵ�
		virtual void Init();

		// ���½ڵ�
		virtual void Update(Duration const& dt);

		// ��Ⱦ�ڵ�
		virtual void OnRender();

		// �����¼�
		virtual void HandleEvent(Event* e);

		// ��ȡ��ʾ״̬
		bool IsVisible() const { return visible_; }

		// ��ȡ����
		String const& GetName() const { return name_; }

		// ��ȡ���Ƶ� Hash ֵ
		size_t GetHashName() const { return hash_name_; }

		// ��ȡ Z ��˳��
		int GetZOrder() const { return z_order_; }

		// ��ȡ���
		virtual float GetWidth() const { return size_.width * transform_.scale.x; }

		// ��ȡ�߶�
		virtual float GetHeight() const { return size_.height * transform_.scale.y; }

		// ��ȡ��С
		Size GetSize() const { return Size{ GetWidth(), GetHeight() }; }

		// ��ȡ x ����֧��
		virtual float GetPivotX() const { return pivot_.x; }

		// ��ȡ y ����֧��
		virtual float GetPivotY() const { return pivot_.y; }

		// ��ȡ͸����
		virtual float GetOpacity() const { return opacity_; }

		// ��ȡ��Χ��
		virtual Rect GetBounds();

		// ��ȡ��ά�任����
		virtual math::Matrix const& GetTransformMatrix() override;

		// ��ȡ���ڵ�
		spNode GetParent() const;

		// ��ȡ���ڳ���
		spScene GetScene() const;

		// �����Ƿ���ʾ
		void SetVisible(
			bool val
		);

		// ��������
		void SetName(
			String const& name
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

		virtual void SetTransform(
			Transform const& transform
		) override;

		// ����͸����
		// Ĭ��Ϊ 1.0, ��Χ [0, 1]
		void SetOpacity(
			float opacity
		);

		// ���� Z ��˳��
		// Ĭ��Ϊ 0
		void SetZOrder(
			int order
		);

		// ���ñ߿���ɫ
		void SetBorderColor(
			const Color& color
		);

		// �жϵ��Ƿ��ڽڵ���
		bool ContainsPoint(
			const Point& point
		);

		// ����ӽڵ�
		void AddChild(
			spNode const& child,
			int z_order = 0		/* Z ��˳�� */
		);

		// ��Ӷ���ӽڵ�
		void AddChild(
			const Nodes& nodes,	/* �ڵ����� */
			int z_order = 0		/* Z ��˳�� */
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

		virtual void DispatchEvent(Event* e) override;

		// ����Ĭ��֧��
		static void SetDefaultPivot(
			float pivot_x,
			float pivot_y
		);

		// ���ñ߿��Զ�����
		static void EnableBorder();

		// ���ñ߿��Զ�����
		static void DisableBorder();

	protected:
		void Render();

		void DrawBorder();

		void SortChildren();

		void UpdateBorder();

		void UpdateTransform();

		void UpdateOpacity();

		void SetScene(Scene* scene);

	protected:
		bool			inited_;
		bool			visible_;
		bool			dirty_sort_;
		int				z_order_;
		float			opacity_;
		float			display_opacity_;
		String			name_;
		size_t			hash_name_;
		Node*			parent_;
		Scene*			scene_;
		Color			border_color_;
		Children		children_;
		cpGeometry		border_;
		Point			pivot_;
		Size			size_;
		math::Matrix	initial_matrix_;
		math::Matrix	final_matrix_;
	};
}
