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

	// 节点
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

		// 更新节点
		virtual void OnUpdate(Duration const& dt) {}

		// 渲染节点
		virtual void OnRender() {}

		// 获取显示状态
		bool IsVisible()				const	{ return visible_; }

		// 获取名称
		String const& GetName()	const	{ return name_; }

		// 获取名称的 Hash 值
		size_t GetHashName()			const	{ return hash_name_; }

		// 获取 Z 轴顺序
		int GetZOrder()					const	{ return z_order_; }

		// 获取坐标
		Point const& GetPosition()		const	{ return transform_.position; }

		// 获取 x 坐标
		float GetPositionX()			const	{ return transform_.position.x; }

		// 获取 y 坐标
		float GetPositionY()			const	{ return transform_.position.y; }

		// 获取横向缩放比例
		float GetScaleX()				const	{ return transform_.scale.x; }

		// 获取纵向缩放比例
		float GetScaleY()				const	{ return transform_.scale.y; }

		// 获取横向错切角度
		float GetSkewX()				const	{ return transform_.skew.x; }

		// 获取纵向错切角度
		float GetSkewY()				const	{ return transform_.skew.y; }

		// 获取旋转角度
		float GetRotation()				const	{ return transform_.rotation; }

		// 获取宽度
		float GetWidth()				const	{ return size_.x; }

		// 获取高度
		float GetHeight()				const	{ return size_.y; }

		// 获取大小
		Size const& GetSize()			const	{ return size_; }

		// 获取缩放后的宽度
		float GetScaledWidth()			const	{ return size_.x * transform_.scale.x; }

		// 获取缩放后的高度
		float GetScaledHeight()			const	{ return size_.y * transform_.scale.y; }

		// 获取缩放后的大小
		Size GetScaledSize()			const	{ return Size{ GetScaledWidth(), GetScaledHeight() }; }

		// 获取 x 方向支点
		float GetPivotX()				const	{ return pivot_.x; }

		// 获取 y 方向支点
		float GetPivotY()				const	{ return pivot_.y; }

		// 获取透明度
		float GetOpacity()				const	{ return opacity_; }

		// 获取变换
		Transform const& GetTransform()	const	{ return transform_; }

		// 获取包围盒
		Rect GetBounds() const;

		// 获取二维变换矩阵
		Matrix const& GetTransformMatrix()  const;

		// 获取二维变换的逆矩阵
		Matrix const& GetTransformInverseMatrix()  const;

		// 获取父节点
		Node* GetParent() const;

		// 获取所在场景
		Scene* GetScene() const;

		// 设置是否显示
		void SetVisible(
			bool val
		);

		// 设置名称
		void SetName(
			String const& name
		);

		// 设置横坐标
		void SetPositionX(
			float x
		);

		// 设置纵坐标
		void SetPositionY(
			float y
		);

		// 设置坐标
		void SetPosition(
			const Point & point
		);

		// 设置坐标
		void SetPosition(
			float x,
			float y
		);

		// 移动
		void Move(
			float x,
			float y
		);

		// 移动
		void Move(
			const Point & vector
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
		void SetPivot(
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

		void SetTransform(
			Transform const& transform
		);

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

		// 判断点是否在节点内
		bool ContainsPoint(
			const Point& point
		)  const;

		// 添加子节点
		void AddChild(
			SpNode const& child
		);

		// 添加多个子节点
		void AddChildren(
			Array<SpNode> const& children
		);

		// 获取所有名称相同的子节点
		Array<SpNode> GetChildren(
			String const& name
		) const;

		// 获取名称相同的子节点
		SpNode GetChild(
			String const& name
		) const;

		// 获取全部子节点
		Children const& GetChildren() const;

		// 移除子节点
		bool RemoveChild(
			SpNode const& child
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
