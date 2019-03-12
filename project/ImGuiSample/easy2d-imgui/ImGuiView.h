// Copyright (C) 2019 Nomango

#pragma once
#include "easy2d.h"

// ImGui
#include "imgui.h"

namespace easy2d
{
	class ImGuiView
		: public Singleton<ImGuiView>
		, public Component
	{
		E2D_DECLARE_SINGLETON(ImGuiView);

	public:
		void Setup(Application* app) override;

		void Destroy() override;

	public:
		void Init(HWND hwnd);

		void NewFrame();

		void Render();
	};

}
