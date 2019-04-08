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

namespace easy2d
{
	class ImGuiView;

	E2D_DECLARE_SMART_PTR(ImGuiLayer);

	using ImGuiPipeline = Closure<void()>;

	class ImGuiLayer
		: public Layer
	{
		friend class ImGuiView;

	public:
		virtual ~ImGuiLayer();

		// 添加 ImGui 元素
		void AddItem(
			ImGuiPipeline const& item,
			String const& name
		);

		// 移除 ImGui 元素
		void RemoveItem(
			String const& name
		);

		// 移除所有元素
		void RemoveAllItems();

	public:
		void OnMouseButtonDown(int btn, Point const& p) override;
		void OnMouseButtonUp(int btn, Point const& p) override;
		void OnMouseWheel(float wheel) override;

		void OnKeyDown(int key) override;
		void OnKeyUp(int key) override;
		void OnChar(char c) override;

	public:
		void OnUpdate(Duration dt) override;
		void OnRender() override;

		void UpdateMousePos();
		void UpdateMouseCursor();

	protected:
		ImGuiLayer();

	protected:
		HWND target_window_;
		Map<String, ImGuiPipeline> pipelines_;
	};
}
