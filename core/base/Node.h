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

	// 节点
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

		E2D_DISABLE_COPY(Node);

		using Nodes = std::vector<spNode>;
		using Children = intrusive::List<spNode>;

	public:
		Node();

		virtual ~Node();

		// 渲染节点
		virtual void OnDraw() {}

		// 更新节点
		virtual void OnUpdate(Duration const& dt) {}

		// 获取节点显示状态
		bool IsVisible() const;

		// 获取节点名称
		String const& GetName() const;

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

		// 获取节点横向错切角度
		float GetSkewX() const;

		// 获取节点纵向错切角度
		float GetSkewY() const;

		// 获取节点旋转角度
		float GetRotation() const;

		// 获取节点透明度
		float GetOpacity() const;

		// 获取显示透明度
		float GetDisplayOpacity() const;

		// 获取父节点
		spNode GetParent() const;

		// 获取包围盒
		virtual Rect GetBounds();

		// 设置节点是否显示
		void SetVisible(
			bool val
		);

		// 设置节点名称
		void SetName(
			String const& name
		);

		// 设置节点横坐标
		void SetPositionX(
			float x
		);

		// 设置节点纵坐标
		void SetPositionY(
			float y
		);

		// 设置节点坐标
		void SetPosition(
			const Point & point
		);

		// 设置节点坐标
		void SetPosition(
			float x,
			float y
		);

		// 移动节点
		void MoveBy(
			float x,
			float y
		);

		// 移动节点
		void MoveBy(
			const Point & vector
		);

		// 设置节点绘图顺序
		// 默认为 0
		void SetOrder(
			int order
		);

		// 设置横向缩放比例
		// 默认为 1.0
		void SetScaleX(
			float scale_x
		);

		// 设置纵向缩放比例
		// 默认为 1.0
		void SetScaleY(
			float scale_y
		);

		// 设置缩放比例
		// 默认为 (1.0, 1.0)
		void SetScale(
			float scale_x,
			float scale_y
		);

		// 设置缩放比例
		// 默认为 1.0
		void SetScale(
			float scale
		);

		// 设置横向错切角度
		// 默认为 0
		void SetSkewX(
			float skew_x
		);

		// 设置纵向错切角度
		// 默认为 0
		void SetSkewY(
			float skew_y
		);

		// 设置错切角度
		// 默认为 (0, 0)
		void SetSkew(
			float skew_x,
			float skew_y
		);

		// 设置旋转角度
		// 默认为 0
		void SetRotation(
			float rotation
		);

		// 设置透明度
		// 默认为 1.0, 范围 [0, 1]
		void SetOpacity(
			float opacity
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

		// 修改节点宽度
		virtual void SetWidth(
			float width
		);

		// 修改节点高度
		void SetHeight(
			float height
		);

		// 修改节点大小
		void SetSize(
			float width,
			float height
		);

		// 修改节点大小
		void SetSize(
			const Size & size
		);

		// 启用或关闭渲染区域裁剪
		void SetClipEnabled(
			bool enabled
		);

		// 设置节点边缘颜色
		void SetBorderColor(
			const Color& color
		);

		Transform const& GetTransform() const;

		void SetTransform(
			Transform const& transform
		);

		// 判断点是否在节点内
		bool ContainsPoint(
			const Point& point
		);

		// 判断两物体是否相交
		bool Intersects(
			spNode const& node
		);

		// 添加子节点
		void AddChild(
			spNode const& child,
			int order = 0		/* 渲染顺序 */
		);

		// 添加多个子节点
		void AddChild(
			const Nodes& nodes,	/* 节点数组 */
			int order = 0		/* 渲染顺序 */
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

		// 移除所有名称相同的子节点
		void RemoveChildren(
			String const& child_name
		);

		// 移除所有节点
		void RemoveAllChildren();

		// 从父节点移除
		void RemoveFromParent();

		// 设置默认支点
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
		String				name_;
		size_t				hash_name_;
		float				display_opacity_;
		float				real_opacity_;
		int					order_;
		bool				visible_;
		bool				clip_enabled_;
		bool				dirty_sort_;
		bool				dirty_transform_;
		Node*				parent_;
		Color				border_color_;
		Children			children_;
		cpGeometry			border_;
		Transform			transform_;
		math::Matrix		initial_matrix_;
		math::Matrix		final_matrix_;
	};
}
