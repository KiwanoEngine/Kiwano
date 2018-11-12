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
#include "RefCounter.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "../math/Matrix.hpp"

namespace easy2d
{
	class Node;

	// 场景
	class Scene
		: public RefCounter
	{
		E2D_DISABLE_COPY(Scene);

	public:
		Scene();

		explicit Scene(
			Node * root
		);

		virtual ~Scene();

		// 进入场景
		virtual void OnEnter() {}

		// 退出场景
		virtual void OnExit() {}

		// 更新场景
		virtual void OnUpdate(float dt) {}

		// 设置根节点
		void SetRoot(
			Node * root
		);

		// 获取根节点
		Node* GetRoot() const;

		// 渲染场景
		void Draw();

		// 分发鼠标消息
		virtual void Dispatch(
			const MouseEvent& e
		);

		// 分发按键消息
		virtual void Dispatch(
			const KeyEvent& e
		);

		// 设置转换矩阵
		void SetTransform(
			const math::Matrix& matrix
		);

		// 获取转换矩阵
		const math::Matrix& GetTransform() const;

	private:
		Node*			root_;
		math::Matrix	transform_;
	};
}
