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

	// 角色
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

		// 更新角色
		virtual void OnUpdate(Duration dt) { KGE_UNUSED(dt); }

		// 渲染角色
		virtual void OnRender(Renderer* renderer) { KGE_UNUSED(renderer); }

		// 获取显示状态
		bool IsVisible()				const	{ return visible_; }

		// 获取响应状态
		bool IsResponsible()			const	{ return responsible_; }

		// 是否启用级联透明度
		bool IsCascadeOpacityEnabled()	const	{ return cascade_opacity_; }

		// 获取名称的 Hash 值
		UInt32 GetHashName()			const	{ return hash_name_; }

		// 获取 Z 轴顺序
		Int32 GetZOrder()					const	{ return z_order_; }

		// 获取坐标
		Point GetPosition()				const	{ return transform_.position; }

		// 获取 x 坐标
		Float32 GetPositionX()			const	{ return transform_.position.x; }

		// 获取 y 坐标
		Float32 GetPositionY()			const	{ return transform_.position.y; }

		// 获取缩放比例
		Point GetScale()				const	{ return transform_.scale; }

		// 获取横向缩放比例
		Float32 GetScaleX()				const	{ return transform_.scale.x; }

		// 获取纵向缩放比例
		Float32 GetScaleY()				const	{ return transform_.scale.y; }

		// 获取错切角度
		Point GetSkew()					const	{ return transform_.skew; }

		// 获取横向错切角度
		Float32 GetSkewX()				const	{ return transform_.skew.x; }

		// 获取纵向错切角度
		Float32 GetSkewY()				const	{ return transform_.skew.y; }

		// 获取旋转角度
		Float32 GetRotation()				const	{ return transform_.rotation; }

		// 获取宽度
		Float32 GetWidth()				const	{ return size_.x; }

		// 获取高度
		Float32 GetHeight()				const	{ return size_.y; }

		// 获取大小
		Size GetSize()					const	{ return size_; }

		// 获取缩放后的宽度
		Float32 GetScaledWidth()			const	{ return size_.x * transform_.scale.x; }

		// 获取缩放后的高度
		Float32 GetScaledHeight()			const	{ return size_.y * transform_.scale.y; }

		// 获取缩放后的大小
		Size GetScaledSize()			const	{ return Size{ GetScaledWidth(), GetScaledHeight() }; }

		// 获取锚点
		Point GetAnchor()				const	{ return anchor_; }

		// 获取 x 方向锚点
		Float32 GetAnchorX()				const	{ return anchor_.x; }

		// 获取 y 方向锚点
		Float32 GetAnchorY()				const	{ return anchor_.y; }

		// 获取透明度
		Float32 GetOpacity()				const	{ return opacity_; }

		// 获取显示透明度
		Float32 GetDisplayedOpacity()		const	{ return displayed_opacity_; }

		// 获取变换
		Transform GetTransform()		const	{ return transform_; }

		// 获取边框
		virtual Rect GetBounds() const;

		// 获取外切包围盒
		virtual Rect GetBoundingBox() const;

		// 获取二维变换矩阵
		Matrix3x2 const& GetTransformMatrix()  const;

		// 获取二维变换的逆矩阵
		Matrix3x2 const& GetTransformInverseMatrix()  const;

		// 获取父角色
		inline Actor* GetParent() const { return parent_; }

		// 获取所在舞台
		inline Stage* GetStage() const { return stage_; }

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
			Float32 x
		);

		// 设置纵坐标
		void SetPositionY(
			Float32 y
		);

		// 设置坐标
		void SetPosition(
			const Point & point
		);

		// 设置坐标
		void SetPosition(
			Float32 x,
			Float32 y
		);

		// 移动
		void Move(
			Float32 x,
			Float32 y
		);

		// 移动
		void Move(
			const Point & vector
		);

		// 设置横向缩放比例
		// 默认为 1.0
		void SetScaleX(
			Float32 scale_x
		);

		// 设置纵向缩放比例
		// 默认为 1.0
		void SetScaleY(
			Float32 scale_y
		);

		// 设置缩放比例
		// 默认为 (1.0, 1.0)
		void SetScale(
			Float32 scale_x,
			Float32 scale_y
		);

		// 设置缩放比例
		// 默认为 (1.0, 1.0)
		void SetScale(
			Point const& scale
		);

		// 设置缩放比例
		// 默认为 1.0
		void SetScale(
			Float32 scale
		);

		// 设置横向错切角度
		// 默认为 0
		void SetSkewX(
			Float32 skew_x
		);

		// 设置纵向错切角度
		// 默认为 0
		void SetSkewY(
			Float32 skew_y
		);

		// 设置错切角度
		// 默认为 (0, 0)
		void SetSkew(
			Float32 skew_x,
			Float32 skew_y
		);

		// 设置错切角度
		// 默认为 (0, 0)
		void SetSkew(
			Point const& skew
		);

		// 设置旋转角度
		// 默认为 0
		void SetRotation(
			Float32 rotation
		);

		// 设置锚点的横向位置
		// 默认为 0, 范围 [0, 1]
		void SetAnchorX(
			Float32 anchor_x
		);

		// 设置锚点的纵向位置
		// 默认为 0, 范围 [0, 1]
		void SetAnchorY(
			Float32 anchor_y
		);

		// 设置锚点位置
		// 默认为 (0, 0), 范围 [0, 1]
		void SetAnchor(
			Float32 anchor_x,
			Float32 anchor_y
		);

		// 设置锚点位置
		// 默认为 (0, 0), 范围 [0, 1]
		void SetAnchor(
			Point const& anchor
		);

		// 修改宽度
		void SetWidth(
			Float32 width
		);

		// 修改高度
		void SetHeight(
			Float32 height
		);

		// 修改大小
		void SetSize(
			Float32 width,
			Float32 height
		);

		// 修改大小
		void SetSize(
			const Size & size
		);

		// 设置二维仿射变换
		void SetTransform(
			Transform const& transform
		);

		// 设置透明度
		// 默认为 1.0, 范围 [0, 1]
		void SetOpacity(
			Float32 opacity
		);

		// 启用或禁用级联透明度
		void SetCascadeOpacityEnabled(
			bool enabled
		);

		// 设置 Z 轴顺序
		// 默认为 0
		void SetZOrder(
			Int32 zorder
		);

		// 是否可响应 (鼠标 Hover | Out | Click 消息)
		// 默认为 false
		void SetResponsible(
			bool enable
		);

		// 判断点是否在角色内
		bool ContainsPoint(
			const Point& point
		)  const;

		// 添加子角色
		void AddChild(
			ActorPtr child
		);

		// 添加多个子角色
		void AddChildren(
			Vector<ActorPtr> const& children
		);

		// 获取所有名称相同的子角色
		Vector<ActorPtr> GetChildren(
			String const& name
		) const;

		// 获取名称相同的子角色
		ActorPtr GetChild(
			String const& name
		) const;

		// 获取全部子角色
		Children const& GetChildren() const;

		// 移除子角色
		void RemoveChild(
			ActorPtr child
		);

		// 移除子角色
		void RemoveChild(
			Actor* child
		);

		// 移除所有名称相同的子角色
		void RemoveChildren(
			String const& child_name
		);

		// 移除所有角色
		void RemoveAllChildren();

		// 从父角色移除
		void RemoveFromParent();

		// 事件分发
		void Dispatch(Event& evt) override;

		// 暂停角色更新
		inline void PauseUpdating()									{ update_pausing_ = true; }

		// 继续角色更新
		inline void ResumeUpdating()								{ update_pausing_ = false; }

		// 角色更新是否暂停
		inline bool IsUpdatePausing() const							{ return update_pausing_; }

		// 设置更新时的回调函数
		inline void SetCallbackOnUpdate(UpdateCallback const& cb)	{ cb_update_ = cb; }

		// 获取更新时的回调函数
		inline UpdateCallback GetCallbackOnUpdate() const			{ return cb_update_; }

		// 渲染角色边界
		inline void ShowBorder(bool show)							{ show_border_ = show; }

		// 设置默认锚点
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
