// Copyright (C) 2019 Nomango

#include "../kiwano-imgui.h"
#include "imgui_impl_dx11.h"

namespace kiwano
{
	void ImGuiView::SetupComponent(Application* app)
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer bindings
		Init(app->GetWindow()->GetHandle());
	}

	void ImGuiView::DestroyComponent()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiView::Init(HWND hwnd)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)
		io.BackendPlatformName = "imgui_impl_win32";
		io.ImeWindowHandle = hwnd;

		// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array that we will update during the application lifetime.
		io.KeyMap[ImGuiKey_Tab] = KeyCode::Tab;
		io.KeyMap[ImGuiKey_LeftArrow] = KeyCode::Left;
		io.KeyMap[ImGuiKey_RightArrow] = KeyCode::Right;
		io.KeyMap[ImGuiKey_UpArrow] = KeyCode::Up;
		io.KeyMap[ImGuiKey_DownArrow] = KeyCode::Down;
		io.KeyMap[ImGuiKey_Delete] = KeyCode::Delete;
		io.KeyMap[ImGuiKey_Backspace] = KeyCode::Back;
		io.KeyMap[ImGuiKey_Space] = KeyCode::Space;
		io.KeyMap[ImGuiKey_Enter] = KeyCode::Enter;
		io.KeyMap[ImGuiKey_Escape] = KeyCode::Esc;
		io.KeyMap[ImGuiKey_A] = KeyCode::A;
		io.KeyMap[ImGuiKey_C] = KeyCode::C;
		io.KeyMap[ImGuiKey_V] = KeyCode::V;
		io.KeyMap[ImGuiKey_X] = KeyCode::X;
		io.KeyMap[ImGuiKey_Y] = KeyCode::Y;
		io.KeyMap[ImGuiKey_Z] = KeyCode::Z;

		ImGui_ImplDX11_Init(
			Renderer::Instance().GetDeviceResources()->GetD3DDevice(),
			Renderer::Instance().GetDeviceResources()->GetD3DDeviceContext()
		);
	}

	void ImGuiView::NewFrame()
	{
		ImGui_ImplDX11_NewFrame();

		ImGuiIO& io = ImGui::GetIO();
		KGE_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built!");

		// Setup display size (every frame to accommodate for window resizing)
		Size display_size = Renderer::Instance().GetOutputSize();
		io.DisplaySize = ImVec2(display_size.x, display_size.y);

		ImGui::NewFrame();
	}

	void ImGuiView::Render()
	{
		ImGui::Render();

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	ImGuiLayerPtr ImGuiView::CreateLayer(Scene* scene)
	{
		auto iter = layers_.find(scene);
		if (iter == layers_.end())
		{
			ImGuiLayerPtr layer = new (std::nothrow) ImGuiLayer;
			if (layer)
			{
				layers_.insert(std::make_pair(scene, layer.Get()));
			}
			return layer;
		}
		else
		{
			return iter->second;
		}
	}

	void ImGuiView::RemoveLayer(ImGuiLayer* layer)
	{
		using value_type = Map<Scene*, Layer*>::value_type;

		auto iter = std::find_if(layers_.begin(), layers_.end(), [=](value_type const& value)
			{
				return value.second == layer;
			});
		if (iter != layers_.end())
		{
			layers_.erase(iter);
		}
	}

}