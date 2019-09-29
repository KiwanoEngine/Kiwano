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
#include <kiwano/base/Component.h>
#include <kiwano/core/singleton.hpp>

namespace kiwano
{
	namespace imgui
	{
		class ImGuiModule
			: public Singleton<ImGuiModule>
			, public RenderComponent
			, public UpdateComponent
			, public EventComponent
		{
			KGE_DECLARE_SINGLETON(ImGuiModule);

		private:
			void Init(HWND hwnd);

			void NewFrame();

			void Render();

		public:
			ImGuiModule();

			void SetupComponent() override;

			void DestroyComponent() override;

			void OnUpdate(Duration dt) override;

			void BeforeRender() override;

			void AfterRender() override;

			void HandleMessage(HWND hwnd, std::uint32_t msg, WPARAM wparam, LPARAM lparam) override;

			void UpdateMousePos();

			void UpdateMouseCursor();

			void UpdateGamepads();

		private:
			bool has_gamepad_;
			bool want_update_has_gamepad_;
			HWND target_window_;
		};
	}
}
