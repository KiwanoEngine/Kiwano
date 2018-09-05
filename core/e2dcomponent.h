#pragma once
#include "e2dnode.h"

namespace e2d
{


	class Button :
		public Node
	{
	public:
		Button();

		explicit Button(
			Node * normal,					/* 普通状态 */
			const Function& func = nullptr	/* 按钮点击后的回调函数 */
		);

		explicit Button(
			Node * normal,					/* 普通状态 */
			Node * selected,				/* 鼠标按下状态 */
			const Function& func = nullptr	/* 按钮点击后的回调函数 */
		);

		explicit Button(
			Node * normal,					/* 普通状态 */
			Node * mouseover,				/* 鼠标移入状态 */
			Node * selected,				/* 鼠标按下状态 */
			const Function& func = nullptr	/* 按钮点击后的回调函数 */
		);

		explicit Button(
			Node * normal,					/* 普通状态 */
			Node * mouseover,				/* 鼠标移入状态 */
			Node * selected,				/* 鼠标移入状态 */
			Node * disabled,				/* 按钮禁用状态 */
			const Function& func = nullptr	/* 按钮点击后的回调函数 */
		);

		// 获取按钮状态是启用还是禁用
		bool IsEnable() const;

		// 设置按钮启用或禁用
		void SetEnabled(
			bool enabled
		);

		// 设置一般情况下显示的按钮
		virtual void SetNormal(
			Node * normal
		);

		// 设置鼠标移入按钮时显示的按钮
		virtual void SetMouseOver(
			Node * mouseover
		);

		// 设置鼠标按下按钮时显示的按钮
		virtual void SetSelected(
			Node * selected
		);

		// 设置按钮被禁用时显示的按钮
		virtual void SetDisabled(
			Node * disabled
		);

		// 设置按钮点击后的回调函数
		void SetCallbackOnClick(
			const Function& func
		);

		// 设置锚点位置
		// 默认为 (0, 0), 范围 [0, 1]
		virtual void SetAnchor(
			float anchor_x,
			float anchor_y
		) override;

		// 分发鼠标消息
		virtual bool Dispatch(
			const MouseEvent& e,
			bool handled
		) override;

		// 遍历节点
		virtual void Visit() override;

	protected:
		E2D_DISABLE_COPY(Button);

		// 按钮状态枚举
		enum class Status { Normal, Mouseover, Selected };

		// 设置按钮状态
		virtual void SetStatus(
			Status status
		);

		// 刷新按钮显示
		virtual void UpdateVisible();

		// 点击回调
		virtual void OnClick();

	protected:
		Node * normal_;
		Node *		mouseover_;
		Node *		selected_;
		Node *		disabled_;
		bool		enabled_;
		bool		is_selected_;
		Status		status_;
		Function	callback_;
	};


	class ToggleButton :
		public Button
	{
	public:
		ToggleButton();

		explicit ToggleButton(
			Node * normal_on,				/* 按钮打开时，普通状态 */
			Node * normal_off,				/* 按钮关闭时，普通状态 */
			const Function& func = nullptr	/* 按钮点击后的回调函数 */
		);

		explicit ToggleButton(
			Node * normal_on,				/* 按钮打开时，普通状态 */
			Node * normal_off,				/* 按钮关闭时，普通状态 */
			Node * selected_on,				/* 按钮打开时，鼠标按下状态 */
			Node * selected_off,			/* 按钮关闭时，鼠标按下状态 */
			const Function& func = nullptr	/* 按钮点击后的回调函数 */
		);

		explicit ToggleButton(
			Node * normal_on,				/* 按钮打开时，普通状态 */
			Node * normal_off,				/* 按钮关闭时，普通状态 */
			Node * mouseover_on,			/* 按钮打开时，鼠标移入状态 */
			Node * mouseover_off,			/* 按钮关闭时，鼠标移入状态 */
			Node * selected_on,				/* 按钮打开时，鼠标按下状态 */
			Node * selected_off,			/* 按钮关闭时，鼠标按下状态 */
			const Function& func = nullptr	/* 按钮点击后的回调函数 */
		);

		explicit ToggleButton(
			Node * normal_on,				/* 按钮打开时，普通状态 */
			Node * normal_off,				/* 按钮关闭时，普通状态 */
			Node * mouseover_on,			/* 按钮打开时，鼠标移入状态 */
			Node * mouseover_off,			/* 按钮关闭时，鼠标移入状态 */
			Node * selected_on,				/* 按钮打开时，鼠标按下状态 */
			Node * selected_off,			/* 按钮关闭时，鼠标按下状态 */
			Node * disabled_on,				/* 按钮打开时，禁用状态 */
			Node * disabled_off,			/* 按钮关闭时，禁用状态 */
			const Function& func = nullptr	/* 按钮点击后的回调函数 */
		);

		// 获取开关状态
		bool IsChecked() const;

		// 设置开关按钮的状态
		void SetChecked(
			bool checked
		);

		// 设置按钮打开状态下显示的按钮
		virtual void SetNormal(
			Node * normal
		) override;

		// 设置按钮打开状态下，鼠标移入按钮时显示的按钮
		virtual void SetMouseOver(
			Node * mouseover
		) override;

		// 设置按钮打开状态下，鼠标按下按钮时显示的按钮
		virtual void SetSelected(
			Node * selected
		) override;

		// 设置按钮打开状态下，被禁用时显示的按钮
		virtual void SetDisabled(
			Node * disabled
		) override;

		// 设置按钮关闭状态下显示的按钮
		void SetNormalOff(
			Node * normal
		);

		// 设置按钮关闭状态下，鼠标移入按钮时显示的按钮
		void SetMouseOverOff(
			Node * mouseover
		);

		// 设置按钮关闭状态下，鼠标按下按钮时显示的按钮
		void SetSelectedOff(
			Node * selected
		);

		// 设置按钮关闭状态下，按钮被禁用时显示的按钮
		void SetDisabledOff(
			Node * disabled
		);

		// 设置锚点位置
		// 默认为 (0, 0), 范围 [0, 1]
		virtual void SetAnchor(
			float anchor_x,
			float anchor_y
		) override;

	protected:
		E2D_DISABLE_COPY(ToggleButton);

		// 刷新按钮开关
		virtual void UpdateStatus();

		// 执行按钮函数对象
		virtual void OnClick() override;

	protected:
		Node * normal_on_;
		Node*	mouseover_on_;
		Node*	selected_on_;
		Node*	disabled_on_;
		Node*	normal_off_;
		Node*	mouseover_off_;
		Node*	selected_off_;
		Node*	disabled_off_;
		bool	checked_;
	};


	// 菜单
	class Menu :
		public Node
	{
	public:
		Menu();

		explicit Menu(
			const std::vector<Button*>& buttons	/* 按钮数组 */
		);

		// 获取菜单是否禁用
		bool IsEnable() const;

		// 获取菜单中的按钮数量
		size_t GetButtonCount() const;

		// 设置菜单启用或禁用
		void SetEnabled(
			bool enabled
		);

		// 添加按钮
		void AddButton(
			Button * button
		);

		// 移除按钮
		bool RemoveButton(
			Button * button
		);

		// 获取所有按钮
		const std::vector<Button*>& GetAllButtons() const;

	protected:
		E2D_DISABLE_COPY(Menu);

	protected:
		bool enabled_;
		std::vector<Button*> buttons_;
	};


}
