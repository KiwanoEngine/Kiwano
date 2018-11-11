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

	// 节点
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

		// 渲染节点
		virtual void OnDraw() const {}

		// 更新节点
		virtual void OnUpdate(float dt) {}

		// 获取节点显示状态
		bool IsVisible() const;

		// 获取节点名称
		const String& GetName() const;

		// 获取节点名称的 Hash 值
		size_t GetHashName() const;

		// 获取节点绘图顺序
		int GetOrder() const;

		// 获取节点坐标
		const Point& GetPosition() const;

		// 获取节点宽度
		float GetWidth() const;

		// 获取节点高度
		float GetHeight() const;

		// 获取节点宽度（不考虑缩放）
		float GetRealWidth() const;

		// 获取节点高度（不考虑缩放）
		float GetRealHeight() const;

		// 获取节点大小（不考虑缩放）
		const Size& GetRealSize() const;

		// 获取节点的支点
		float GetPivotX() const;

		// 获取节点的支点
		float GetPivotY() const;

		// 获取节点大小
		Size GetSize() const;

		// 获取节点横向缩放比例
		float GetScaleX() const;

		// 获取节点纵向缩放比例
		float GetScaleY() const;

		// 获取节点横向倾斜角度
		float GetSkewX() const;

		// 获取节点纵向倾斜角度
		float GetSkewY() const;

		// 获取节点旋转角度
		float GetRotation() const;

		// 获取二维转换矩阵
		const math::Transform& GetTransform() const;

		// 获取节点透明度
		float GetOpacity() const;

		// 获取显示透明度
		float GetDisplayOpacity() const;

		// 获取父节点
		Node * GetParent() const;

		// 获取节点所在场景
		Scene * GetParentScene() const;

		// 设置节点是否显示
		void SetVisible(
			bool val
		);

		// 设置节点名称
		void SetName(
			const String& name
		);

		// 设置节点横坐标
		virtual void SetPositionX(
			float x
		);

		// 设置节点纵坐标
		virtual void SetPositionY(
			float y
		);

		// 设置节点坐标
		virtual void SetPosition(
			const Point & point
		);

		// 设置节点坐标
		virtual void SetPosition(
			float x,
			float y
		);

		// 移动节点
		virtual void MoveBy(
			float x,
			float y
		);

		// 移动节点
		virtual void MoveBy(
			const Point & vector
		);

		// 设置节点绘图顺序
		// 默认为 0
		virtual void SetOrder(
			int order
		);

		// 设置横向缩放比例
		// 默认为 1.0
		virtual void SetScaleX(
			float scale_x
		);

		// 设置纵向缩放比例
		// 默认为 1.0
		virtual void SetScaleY(
			float scale_y
		);

		// 设置缩放比例
		// 默认为 (1.0, 1.0)
		virtual void SetScale(
			float scale_x,
			float scale_y
		);

		// 设置缩放比例
		// 默认为 1.0
		virtual void SetScale(
			float scale
		);

		// 设置横向倾斜角度
		// 默认为 0
		virtual void SetSkewX(
			float skew_x
		);

		// 设置纵向倾斜角度
		// 默认为 0
		virtual void SetSkewY(
			float skew_y
		);

		// 设置倾斜角度
		// 默认为 (0, 0)
		virtual void SetSkew(
			float skew_x,
			float skew_y
		);

		// 设置旋转角度
		// 默认为 0
		virtual void SetRotation(
			float rotation
		);

		// 设置透明度
		// 默认为 1.0, 范围 [0, 1]
		virtual void SetOpacity(
			float opacity
		);

		// 设置支点的横向位置
		// 默认为 0, 范围 [0, 1]
		virtual void SetPivotX(
			float pivot_x
		);

		// 设置支点的纵向位置
		// 默认为 0, 范围 [0, 1]
		virtual void SetPivotY(
			float pivot_y
		);

		// 设置支点位置
		// 默认为 (0, 0), 范围 [0, 1]
		virtual void SetPivot(
			float pivot_x,
			float pivot_y
		);

		// 修改节点宽度
		virtual void SetWidth(
			float width
		);

		// 修改节点高度
		virtual void SetHeight(
			float height
		);

		// 修改节点大小
		virtual void SetSize(
			float width,
			float height
		);

		// 修改节点大小
		virtual void SetSize(
			const Size & size
		);

		// 设置二维转换
		virtual void SetTransform(
			const math::Transform& transform
		);

		// 启用或关闭渲染区域裁剪
		virtual void SetClipEnabled(
			bool enabled
		);

		// 设置节点边缘颜色
		virtual void SetBorderColor(
			const Color& color
		);

		// 判断点是否在节点内
		bool ContainsPoint(
			const Point& point
		);

		// 判断两物体是否相交
		bool Intersects(
			Node * node
		);

		// 添加子节点
		void AddChild(
			Node * child,
			int order = 0		/* 渲染顺序 */
		);

		// 添加多个子节点
		void AddChild(
			const Nodes& nodes,	/* 节点数组 */
			int order = 0		/* 渲染顺序 */
		);

		// 获取所有名称相同的子节点
		Nodes GetChildren(
			const String& name
		) const;

		// 获取名称相同的子节点
		Node* GetChild(
			const String& name
		) const;

		// 获取所有子节点
		const Nodes& GetAllChildren() const;

		// 获取子节点数量
		int GetChildrenCount() const;

		// 移除子节点
		bool RemoveChild(
			Node * child
		);

		// 移除所有名称相同的子节点
		void RemoveChildren(
			const String& child_name
		);

		// 移除所有节点
		void RemoveAllChildren();

		// 从父节点移除
		void RemoveFromParent();

		// 执行动作
		void RunAction(
			Action * action
		);

		// 继续动作
		void ResumeAction(
			const String& name
		);

		// 暂停动作
		void PauseAction(
			const String& name
		);

		// 停止动作
		void StopAction(
			const String& name
		);

		// 继续所有暂停动作
		void ResumeAllActions();

		// 暂停所有动作
		void PauseAllActions();

		// 停止所有动作
		void StopAllActions();

		// 获取所有动作
		const Actions& GetAllActions() const;

		// 添加任务
		void AddTask(
			Task * task
		);

		// 启动任务
		void StartTasks(
			const String& task_name
		);

		// 停止任务
		void StopTasks(
			const String& task_name
		);

		// 移除任务
		void RemoveTasks(
			const String& task_name
		);

		// 启动所有任务
		void StartAllTasks();

		// 停止所有任务
		void StopAllTasks();

		// 移除所有任务
		void RemoveAllTasks();

		// 获取所有任务
		const Tasks& GetAllTasks() const;

	protected:
		// 遍历节点
		virtual void Visit();

		// 分发鼠标消息
		virtual bool Dispatch(
			const MouseEvent& e,
			bool handled
		);

		// 分发按键消息
		virtual bool Dispatch(
			const KeyEvent& e,
			bool handled
		);

	private:
		E2D_DISABLE_COPY(Node);

		// 渲染节点边缘
		void DrawBorder();

		// 设置节点所在场景
		void SetParentScene(
			Scene * scene
		);

		// 更新子节点
		void UpdateChildren(float dt);

		// 更新转换矩阵
		void UpdateTransform();

		// 更新节点透明度
		void UpdateOpacity();

		// 更新动作
		void UpdateActions();

		// 更新任务
		void UpdateTasks();

		// 更新节点时间
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
