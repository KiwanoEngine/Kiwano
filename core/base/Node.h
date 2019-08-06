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
#include "base.h"
#include "RefCounter.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "../math/Transform.h"
#include "../math/Matrix.hpp"

namespace easy2d
{
    class Game;
    class Scene;
    class Action;
    class Task;

	// �ڵ�
	class Node
		: public RefCounter
	{
		friend class Game;
		friend class Scene;

	public:
		typedef std::vector<Node*> Nodes;
		typedef std::vector<Action*> Actions;
		typedef std::vector<Task*> Tasks;

		Node();

		virtual ~Node();

		// ��Ⱦ�ڵ�
		virtual void OnDraw() const {}

		// ���½ڵ�
		virtual void OnUpdate(float dt) {}

		// ��ȡ�ڵ���ʾ״̬
		bool IsVisible() const;

		// ��ȡ�ڵ�����
		const String& GetName() const;

		// ��ȡ�ڵ����Ƶ� Hash ֵ
		size_t GetHashName() const;

		// ��ȡ�ڵ��ͼ˳��
		int GetOrder() const;

		// ��ȡ�ڵ�����
		const Point& GetPosition() const;

		// ��ȡ�ڵ���
		float GetWidth() const;

		// ��ȡ�ڵ�߶�
		float GetHeight() const;

		// ��ȡ�ڵ��ȣ����������ţ�
		float GetRealWidth() const;

		// ��ȡ�ڵ�߶ȣ����������ţ�
		float GetRealHeight() const;

		// ��ȡ�ڵ��С�����������ţ�
		const Size& GetRealSize() const;

		// ��ȡ�ڵ��֧��
		float GetPivotX() const;

		// ��ȡ�ڵ��֧��
		float GetPivotY() const;

		// ��ȡ�ڵ��С
		Size GetSize() const;

		// ��ȡ�ڵ�������ű���
		float GetScaleX() const;

		// ��ȡ�ڵ��������ű���
		float GetScaleY() const;

		// ��ȡ�ڵ������б�Ƕ�
		float GetSkewX() const;

		// ��ȡ�ڵ�������б�Ƕ�
		float GetSkewY() const;

		// ��ȡ�ڵ���ת�Ƕ�
		float GetRotation() const;

		// ��ȡ��άת������
		const math::Transform& GetTransform() const;

		// ��ȡ�ڵ�͸����
		float GetOpacity() const;

		// ��ȡ��ʾ͸����
		float GetDisplayOpacity() const;

		// ��ȡ���ڵ�
		Node * GetParent() const;

		// ��ȡ�ڵ����ڳ���
		Scene * GetParentScene() const;

		// ���ýڵ��Ƿ���ʾ
		void SetVisible(
			bool val
		);

		// ���ýڵ�����
		void SetName(
			const String& name
		);

		// ���ýڵ������
		virtual void SetPositionX(
			float x
		);

		// ���ýڵ�������
		virtual void SetPositionY(
			float y
		);

		// ���ýڵ�����
		virtual void SetPosition(
			const Point & point
		);

		// ���ýڵ�����
		virtual void SetPosition(
			float x,
			float y
		);

		// �ƶ��ڵ�
		virtual void MoveBy(
			float x,
			float y
		);

		// �ƶ��ڵ�
		virtual void MoveBy(
			const Point & vector
		);

		// ���ýڵ��ͼ˳��
		// Ĭ��Ϊ 0
		virtual void SetOrder(
			int order
		);

		// ���ú������ű���
		// Ĭ��Ϊ 1.0
		virtual void SetScaleX(
			float scale_x
		);

		// �����������ű���
		// Ĭ��Ϊ 1.0
		virtual void SetScaleY(
			float scale_y
		);

		// �������ű���
		// Ĭ��Ϊ (1.0, 1.0)
		virtual void SetScale(
			float scale_x,
			float scale_y
		);

		// �������ű���
		// Ĭ��Ϊ 1.0
		virtual void SetScale(
			float scale
		);

		// ���ú�����б�Ƕ�
		// Ĭ��Ϊ 0
		virtual void SetSkewX(
			float skew_x
		);

		// ����������б�Ƕ�
		// Ĭ��Ϊ 0
		virtual void SetSkewY(
			float skew_y
		);

		// ������б�Ƕ�
		// Ĭ��Ϊ (0, 0)
		virtual void SetSkew(
			float skew_x,
			float skew_y
		);

		// ������ת�Ƕ�
		// Ĭ��Ϊ 0
		virtual void SetRotation(
			float rotation
		);

		// ����͸����
		// Ĭ��Ϊ 1.0, ��Χ [0, 1]
		virtual void SetOpacity(
			float opacity
		);

		// ����֧��ĺ���λ��
		// Ĭ��Ϊ 0, ��Χ [0, 1]
		virtual void SetPivotX(
			float pivot_x
		);

		// ����֧�������λ��
		// Ĭ��Ϊ 0, ��Χ [0, 1]
		virtual void SetPivotY(
			float pivot_y
		);

		// ����֧��λ��
		// Ĭ��Ϊ (0, 0), ��Χ [0, 1]
		virtual void SetPivot(
			float pivot_x,
			float pivot_y
		);

		// �޸Ľڵ���
		virtual void SetWidth(
			float width
		);

		// �޸Ľڵ�߶�
		virtual void SetHeight(
			float height
		);

		// �޸Ľڵ��С
		virtual void SetSize(
			float width,
			float height
		);

		// �޸Ľڵ��С
		virtual void SetSize(
			const Size & size
		);

		// ���ö�άת��
		virtual void SetTransform(
			const math::Transform& transform
		);

		// ���û�ر���Ⱦ����ü�
		virtual void SetClipEnabled(
			bool enabled
		);

		// ���ýڵ��Ե��ɫ
		virtual void SetBorderColor(
			const Color& color
		);

		// �жϵ��Ƿ��ڽڵ���
		bool ContainsPoint(
			const Point& point
		);

		// �ж��������Ƿ��ཻ
		bool Intersects(
			Node * node
		);

		// ����ӽڵ�
		void AddChild(
			Node * child,
			int order = 0		/* ��Ⱦ˳�� */
		);

		// ��Ӷ���ӽڵ�
		void AddChild(
			const Nodes& nodes,	/* �ڵ����� */
			int order = 0		/* ��Ⱦ˳�� */
		);

		// ��ȡ����������ͬ���ӽڵ�
		Nodes GetChildren(
			const String& name
		) const;

		// ��ȡ������ͬ���ӽڵ�
		Node* GetChild(
			const String& name
		) const;

		// ��ȡ�����ӽڵ�
		const Nodes& GetAllChildren() const;

		// ��ȡ�ӽڵ�����
		int GetChildrenCount() const;

		// �Ƴ��ӽڵ�
		bool RemoveChild(
			Node * child
		);

		// �Ƴ�����������ͬ���ӽڵ�
		void RemoveChildren(
			const String& child_name
		);

		// �Ƴ����нڵ�
		void RemoveAllChildren();

		// �Ӹ��ڵ��Ƴ�
		void RemoveFromParent();

		// ִ�ж���
		void RunAction(
			Action * action
		);

		// ��������
		void ResumeAction(
			const String& name
		);

		// ��ͣ����
		void PauseAction(
			const String& name
		);

		// ֹͣ����
		void StopAction(
			const String& name
		);

		// ����������ͣ����
		void ResumeAllActions();

		// ��ͣ���ж���
		void PauseAllActions();

		// ֹͣ���ж���
		void StopAllActions();

		// ��ȡ���ж���
		const Actions& GetAllActions() const;

		// �������
		void AddTask(
			Task * task
		);

		// ��������
		void StartTasks(
			const String& task_name
		);

		// ֹͣ����
		void StopTasks(
			const String& task_name
		);

		// �Ƴ�����
		void RemoveTasks(
			const String& task_name
		);

		// ������������
		void StartAllTasks();

		// ֹͣ��������
		void StopAllTasks();

		// �Ƴ���������
		void RemoveAllTasks();

		// ��ȡ��������
		const Tasks& GetAllTasks() const;

	protected:
		// �����ڵ�
		virtual void Visit();

		// �ַ������Ϣ
		virtual bool Dispatch(
			const MouseEvent& e,
			bool handled
		);

		// �ַ�������Ϣ
		virtual bool Dispatch(
			const KeyEvent& e,
			bool handled
		);

	private:
		E2D_DISABLE_COPY(Node);

		// ��Ⱦ�ڵ��Ե
		void DrawBorder();

		// ���ýڵ����ڳ���
		void SetParentScene(
			Scene * scene
		);

		// �����ӽڵ�
		void UpdateChildren(float dt);

		// ����ת������
		void UpdateTransform();

		// ���½ڵ�͸����
		void UpdateOpacity();

		// ���¶���
		void UpdateActions();

		// ��������
		void UpdateTasks();

		// ���½ڵ�ʱ��
		void UpdateTime();

	private:
		String				name_;
		size_t				hash_name_;
		float				display_opacity_;
		float				real_opacity_;
		int					order_;
		bool				visible_;
		bool				clip_enabled_;
		bool				dirty_sort_;
		bool				dirty_transform_;
		Scene*				parent_scene_;
		Node*				parent_;
		Color				border_color_;
		Actions				actions_;
		Tasks				tasks_;
		Nodes				children_;
		ID2D1Geometry*		border_;
		math::Transform		transform_;
		math::Matrix		initial_matrix_;
		math::Matrix		final_matrix_;
	};
}
