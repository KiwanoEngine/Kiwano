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
#include <kiwano/macros.h>
#include <kiwano/math/math.h>
#include <kiwano/core/common.h>
#include <kiwano/core/event/Event.h>
#include <kiwano/core/Component.h>

namespace kiwano
{
	/**
	* \~chinese
	* @brief 输入设备实例，可获取鼠标和键盘的按键状态
	*/
	class KGE_API Input
		: public Singleton<Input>
		, public UpdateComponent
		, public EventComponent
	{
		friend Singleton<Input>;

	public:
		/**
		* \~chinese
		* @brief 检测键盘按键是否正被按下
		* @param key 键值
		*/
		bool IsDown(KeyCode key) const;

		/**
		* \~chinese
		* @brief 检测键盘按键是否刚被点击
		* @param key 键值
		*/
		bool WasPressed(KeyCode key) const;

		/**
		* \~chinese
		* @brief 检测键盘按键是否刚抬起
		* @param key 键值
		*/
		bool WasReleased(KeyCode key) const;
		/**
		* \~chinese
		* @brief 检测鼠标按键是否正被按下
		* @param btn 鼠标键值
		*/
		bool IsDown(MouseButton btn) const;

		/**
		* \~chinese
		* @brief 检测鼠标按键是否刚被点击
		* @param btn 鼠标键值
		*/
		bool WasPressed(MouseButton btn) const;

		/**
		* \~chinese
		* @brief 检测鼠标按键是否刚抬起
		* @param btn 鼠标键值
		*/
		bool WasReleased(MouseButton btn) const;

		/**
		* \~chinese
		* @brief 获得鼠标 x 坐标
		* @return 鼠标 x 坐标
		*/
		float GetMouseX() const;

		/**
		* \~chinese
		* @brief 获得鼠标 y 坐标
		* @return 鼠标 y 坐标
		*/
		float GetMouseY() const;

		/**
		* \~chinese
		* @brief 获得鼠标坐标
		* @return 鼠标坐标
		*/
		Point GetMousePos() const;

	public:
		void SetupComponent() override {}

		void DestroyComponent() override {}

		void AfterUpdate() override;

		void HandleEvent(Event* evt) override;

	private:
		Input();

		~Input();

		void UpdateKey(KeyCode key, bool down);

		void UpdateButton(MouseButton btn, bool down);

		void UpdateMousePos(const Point& pos);

	private:
		static const int KEY_NUM = int(KeyCode::Last);
		static const int BUTTON_NUM = int(MouseButton::Last);

		bool want_update_keys_;
		bool want_update_buttons_;
		Point mouse_pos_;

		enum KeyIndex : size_t
		{
			Current = 0,
			Prev
		};

		std::array<bool, BUTTON_NUM> buttons_[2];
		std::array<bool, KEY_NUM> keys_[2];
	};
}
