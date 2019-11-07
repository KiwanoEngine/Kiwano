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
#include <kiwano/2d/include-forwards.h>
#include <kiwano/2d/Transform.h>
#include <kiwano/2d/action/ActionManager.h>
#include <kiwano/base/TimerManager.h>
#include <kiwano/base/EventDispatcher.h>

namespace kiwano
{
	class Director;
	class RenderTarget;

	// 角色
	class KGE_API Actor
		: public ObjectBase
		, public TimerManager
		, public ActionManager
		, public EventDispatcher
		, public IntrusiveListItem<ActorPtr>
	{
		friend class Director;
		friend class Transition;
		friend IntrusiveList<ActorPtr>;

	public:
		using Children			= IntrusiveList<ActorPtr>;
		using UpdateCallback	= Function<void(Duration)>;

		Actor();

		// 更新角色
		virtual void OnUpdate(Duration dt);

		// 渲染角色
		virtual void OnRender(RenderTarget* rt);

		// 获取显示状态
		bool IsVisible() const;

		// 获取响应状态
		bool IsResponsible() const;

		// 是否启用级联透明度
		bool IsCascadeOpacityEnabled() const;

		// 获取名称的 Hash 值
		size_t GetHashName() const;

		// 获取 Z 轴顺序
		int GetZOrder() const;

		// 获取坐标
		Point const& GetPosition() const;

		// 获取 x 坐标
		float GetPositionX() const;

		// 获取 y 坐标
		float GetPositionY() const;

		// 获取缩放比例
		Point const& GetScale() const;

		// 获取横向缩放比例
		float GetScaleX() const;

		// 获取纵向缩放比例
		float GetScaleY() const;

		// 获取错切角度
		Point const& GetSkew() const;

		// 获取横向错切角度
		float GetSkewX() const;

		// 获取纵向错切角度
		float GetSkewY() const;

		// 获取旋转角度
		float GetRotation() const;

		// 获取宽度
		float GetWidth() const;

		// 获取高度
		float GetHeight() const;

		// 获取大小
		Size const& GetSize() const;

		// 获取缩放后的宽度
		float GetScaledWidth() const;

		// 获取缩放后的高度
		float GetScaledHeight() const;

		// 获取缩放后的大小
		Size GetScaledSize() const;

		// 获取锚点
		Point const& GetAnchor() const;

		// 获取 x 方向锚点
		float GetAnchorX() const;

		// 获取 y 方向锚点
		float GetAnchorY() const;

		// 获取透明度
		float GetOpacity() const;

		// 获取显示透明度
		float GetDisplayedOpacity() const;

		// 获取变换
		Transform GetTransform() const;

		// 获取父角色
		Actor* GetParent() const;

		// 获取所在舞台
		Stage* GetStage() const;

		// 获取边框
		virtual Rect GetBounds() const;

		// 获取外切包围盒
		virtual Rect GetBoundingBox() const;

		// 获取二维变换矩阵
		Matrix3x2 const& GetTransformMatrix()  const;

		// 获取二维变换的逆矩阵
		Matrix3x2 const& GetTransformInverseMatrix()  const;

		// 设置是否显示
		void SetVisible(
			bool val
		);

		// 设置名称
		void SetName(
			String const& name
		);

		// 设置坐标
		virtual void SetPosition(
			Point const& point
		);

		// 设置坐标
		void SetPosition(
			float x,
			float y
		);

		// 设置横坐标
		void SetPositionX(
			float x
		);

		// 设置纵坐标
		void SetPositionY(
			float y
		);

		// 移动坐标
		void Move(
			Vec2 const& v
		);

		// 移动坐标
		void Move(
			float vx,
			float vy
		);

		// 设置缩放比例
		// 默认为 (1.0, 1.0)
		virtual void SetScale(
			Vec2 const& scale
		);

		// 设置缩放比例
		// 默认为 (1.0, 1.0)
		void SetScale(
			float scalex,
			float scaley
		);

		// 设置错切角度
		// 默认为 (0, 0)
		virtual void SetSkew(
			Vec2 const& skew
		);

		// 设置错切角度
		// 默认为 (0, 0)
		void SetSkew(
			float skewx,
			float skewy
		);

		// 设置旋转角度
		// 默认为 0
		virtual void SetRotation(
			float rotation
		);

		// 设置锚点位置
		// 默认为 (0, 0), 范围 [0, 1]
		virtual void SetAnchor(
			Vec2 const& anchor
		);

		// 设置锚点位置
		// 默认为 (0, 0), 范围 [0, 1]
		void SetAnchor(
			float anchorx,
			float anchory
		);

		// 修改宽度
		virtual void SetWidth(
			float width
		);

		// 修改高度
		virtual void SetHeight(
			float height
		);

		// 修改大小
		virtual void SetSize(
			Size const& size
		);

		// 修改大小
		void SetSize(
			float width,
			float height
		);

		// 设置透明度
		// 默认为 1.0, 范围 [0, 1]
		virtual void SetOpacity(
			float opacity
		);

		// 启用或禁用级联透明度
		void SetCascadeOpacityEnabled(
			bool enabled
		);

		// 设置二维仿射变换
		void SetTransform(
			Transform const& transform
		);

		// 设置 Z 轴顺序
		// 默认为 0
		void SetZOrder(
			int zorder
		);

		// 是否可响应 (鼠标 Hover | Out | Click 消息)
		// 默认为 false
		void SetResponsible(
			bool enable
		);

		// 添加子角色
		void AddChild(
			ActorPtr child,
			int zorder = 0
		);

		// 添加子角色
		void AddChild(
			Actor* child,
			int zorder = 0
		);

		// 添加多个子角色
		void AddChildren(
			Vector<ActorPtr> const& children
		);

		// 获取名称相同的子角色
		Actor* GetChild(
			String const& name
		) const;

		// 获取所有名称相同的子角色
		Vector<ActorPtr> GetChildren(
			String const& name
		) const;

		// 获取全部子角色
		Children& GetAllChildren();

		// 获取全部子角色
		Children const& GetAllChildren() const;

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

		// 判断点是否在角色内
		virtual bool ContainsPoint(const Point& point) const;

		// 暂停角色更新
		void PauseUpdating();

		// 继续角色更新
		void ResumeUpdating();

		// 角色更新是否暂停
		bool IsUpdatePausing() const;

		// 设置更新时的回调函数
		void SetCallbackOnUpdate(UpdateCallback const& cb);

		// 获取更新时的回调函数
		UpdateCallback GetCallbackOnUpdate() const;

		// 渲染角色边界
		void ShowBorder(bool show);

		// 事件分发
		void Dispatch(Event& evt) override;

		// 设置默认锚点
		static void SetDefaultAnchor(
			float anchor_x,
			float anchor_y
		);

	protected:
		virtual void Update(Duration dt);

		virtual void Render(RenderTarget* rt);

		virtual void PrepareRender(RenderTarget* rt);

		virtual void RenderBorder(RenderTarget* rt);

		virtual bool CheckVisibilty(RenderTarget* rt) const;

		void UpdateTransform() const;

		void UpdateOpacity();

		void Reorder();

		void SetStage(Stage* stage);

	private:
		bool			visible_;
		bool			update_pausing_;
		bool			cascade_opacity_;
		bool			show_border_;
		bool			hover_;
		bool			pressed_;
		bool			responsible_;
		int				z_order_;
		float			opacity_;
		float			displayed_opacity_;
		Actor*			parent_;
		Stage*			stage_;
		size_t			hash_name_;
		Point			anchor_;
		Size			size_;
		Children		children_;
		UpdateCallback	cb_update_;
		Transform		transform_;

		bool				is_fast_transform_;
		mutable bool		visible_in_rt_;
		mutable	bool		dirty_visibility_;
		mutable bool		dirty_transform_;
		mutable bool		dirty_transform_inverse_;
		mutable Matrix3x2	transform_matrix_;
		mutable Matrix3x2	transform_matrix_inverse_;
	};

	inline void Actor::OnUpdate(Duration dt)
	{
		KGE_NOT_USED(dt);
	}

	inline void Actor::OnRender(RenderTarget* rt)
	{
		KGE_NOT_USED(rt);
	}

	inline bool Actor::IsVisible() const
	{
		return visible_;
	}

	inline bool Actor::IsResponsible() const
	{
		return responsible_;
	}

	inline bool Actor::IsCascadeOpacityEnabled() const
	{
		return cascade_opacity_;
	}

	inline size_t Actor::GetHashName() const
	{
		return hash_name_;
	}

	inline int Actor::GetZOrder() const
	{
		return z_order_;
	}

	inline Point const& Actor::GetPosition() const
	{
		return transform_.position;
	}

	inline float Actor::GetPositionX() const
	{
		return GetPosition().x;
	}

	inline float Actor::GetPositionY() const
	{
		return GetPosition().y;
	}

	inline Point const& Actor::GetScale() const
	{
		return transform_.scale;
	}

	inline float Actor::GetScaleX() const
	{
		return GetScale().x;
	}

	inline float Actor::GetScaleY() const
	{
		return GetScale().y;
	}

	inline Point const& Actor::GetSkew() const
	{
		return transform_.skew;
	}

	inline float Actor::GetSkewX() const
	{
		return GetSkew().x;
	}

	inline float Actor::GetSkewY() const
	{
		return GetSkew().y;
	}

	inline float Actor::GetRotation() const
	{
		return transform_.rotation;
	}

	inline float Actor::GetWidth() const
	{
		return GetSize().x;
	}

	inline float Actor::GetHeight() const
	{
		return GetSize().y;
	}

	inline Size const& Actor::GetSize() const
	{
		return size_;
	}

	inline float Actor::GetScaledWidth() const
	{
		return GetWidth() * GetScaleX();
	}

	inline float Actor::GetScaledHeight() const
	{
		return GetHeight() * GetScaleY();
	}

	inline Size Actor::GetScaledSize() const
	{
		return Size{ GetScaledWidth(), GetScaledHeight() };
	}

	inline Point const& Actor::GetAnchor() const
	{
		return anchor_;
	}

	inline float Actor::GetAnchorX() const
	{
		return GetAnchor().x;
	}

	inline float Actor::GetAnchorY() const
	{
		return GetAnchor().y;
	}

	inline float Actor::GetOpacity() const
	{
		return opacity_;
	}

	inline float Actor::GetDisplayedOpacity() const
	{
		return displayed_opacity_;
	}

	inline Transform Actor::GetTransform() const
	{
		return transform_;
	}

	inline Actor* Actor::GetParent() const
	{
		return parent_;
	}

	inline Stage* Actor::GetStage() const
	{
		return stage_;
	}

	inline void Actor::PauseUpdating()
	{
		update_pausing_ = true;
	}

	inline void Actor::ResumeUpdating()
	{
		update_pausing_ = false;
	}

	inline bool Actor::IsUpdatePausing() const
	{
		return update_pausing_;
	}

	inline void Actor::SetCallbackOnUpdate(UpdateCallback const& cb)
	{
		cb_update_ = cb;
	}

	inline Actor::UpdateCallback Actor::GetCallbackOnUpdate() const
	{
		return cb_update_;
	}

	inline void Actor::ShowBorder(bool show)
	{
		show_border_ = show;
	}

	inline void Actor::SetPosition(float x, float y)
	{
		SetPosition(Point{ x, y });
	}

	inline void Actor::Move(float vx, float vy)
	{
		Move(Vec2{ vx, vy });
	}

	inline void Actor::SetScale(float scalex, float scaley)
	{
		SetScale(Vec2{ scalex, scaley });
	}

	inline void Actor::SetAnchor(float anchorx, float anchory)
	{
		SetAnchor(Vec2{ anchorx, anchory });
	}

	inline void Actor::SetSize(float width, float height)
	{
		SetSize(Size{ width, height });
	}

	inline void Actor::SetSkew(float skewx, float skewy)
	{
		SetSkew(Vec2{ skewx, skewy });
	}

}
