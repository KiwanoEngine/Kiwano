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

	// 节点
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

		// 初始化节点
		virtual void Init();

		// 更新节点
		virtual void Update(Duration const& dt);

		// 渲染节点
		virtual void OnRender();

		// 处理事件
		virtual void HandleEvent(Event* e);

		// 获取显示状态
		bool IsVisible() const { return visible_; }

		// 获取名称
		String const& GetName() const { return name_; }

		// 获取名称的 Hash 值
		size_t GetHashName() const { return hash_name_; }

		// 获取 Z 轴顺序
		int GetZOrder() const { return z_order_; }

		// 获取宽度
		virtual float GetWidth() const { return size_.width * transform_.scale.x; }

		// 获取高度
		virtual float GetHeight() const { return size_.height * transform_.scale.y; }

		// 获取大小
		Size GetSize() const { return Size{ GetWidth(), GetHeight() }; }

		// 获取 x 方向支点
		virtual float GetPivotX() const { return pivot_.x; }

		// 获取 y 方向支点
		virtual float GetPivotY() const { return pivot_.y; }

		// 获取透明度
		virtual float GetOpacity() const { return opacity_; }

		// 获取包围盒
		virtual Rect GetBounds();

		// 获取二维变换矩阵
		virtual math::Matrix const& GetTransformMatrix() override;

		// 获取父节点
		spNode GetParent() const;

		// 获取所在场景
		spScene GetScene() const;

		// 设置是否显示
		void SetVisible(
			bool val
		);

		// 设置名称
		void SetName(
			String const& name
		);

		// 设置支点的横向位置
		// 默认为 0, 范围 [0, 1]
		void SetPivotX(
			float pivot_x
		);

		// 设置支点的纵向位置
		// 默认为 0, 范围 [0, 1]
		void SetPivotY(
			float pivot_y
		);

		// 设置支点位置
		// 默认为 (0, 0), 范围 [0, 1]
		virtual void SetPivot(
			float pivot_x,
			float pivot_y
		);

		// 修改宽度
		void SetWidth(
			float width
		);

		// 修改高度
		void SetHeight(
			float height
		);

		// 修改大小
		void SetSize(
			float width,
			float height
		);

		// 修改大小
		void SetSize(
			const Size & size
		);

		virtual void SetTransform(
			Transform const& transform
		) override;

		// 设置透明度
		// 默认为 1.0, 范围 [0, 1]
		void SetOpacity(
			float opacity
		);

		// 设置 Z 轴顺序
		// 默认为 0
		void SetZOrder(
			int zorder
		);

		// 设置边框颜色
		void SetBorderColor(
			const Color& color
		);

		// 判断点是否在节点内
		bool ContainsPoint(
			const Point& point
		);

		// 添加子节点
		void AddChild(
			spNode const& child
		);

		// 添加多个子节点
		void AddChildren(
			const Nodes& children
		);

		// 获取所有名称相同的子节点
		Nodes GetChildren(
			String const& name
		) const;

		// 获取名称相同的子节点
		spNode GetChild(
			String const& name
		) const;

		// 获取全部子节点
		Children const& GetChildren() const;

		// 移除子节点
		bool RemoveChild(
			spNode const& child
		);

		// 移除子节点
		bool RemoveChild(
			Node* child
		);

		// 移除所有名称相同的子节点
		void RemoveChildren(
			String const& child_name
		);

		// 移除所有节点
		void RemoveAllChildren();

		// 从父节点移除
		void RemoveFromParent();

		virtual void DispatchEvent(Event* e) override;

		// 设置默认支点
		static void SetDefaultPivot(
			float pivot_x,
			float pivot_y
		);

		// 启用边框自动生成
		static void EnableBorder();

		// 禁用边框自动生成
		static void DisableBorder();

	protected:
		void Render();

		void DrawBorder();

		void UpdateBorder();

		void UpdateTransform();

		void UpdateOpacity();

		void SetScene(Scene* scene);

	protected:
		bool			inited_;
		bool			visible_;
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
