// Copyright (C) 2019 Nomango

#pragma once

#include "kiwano/kiwano.h"
#include "kiwano-imgui/kiwano-imgui.h"

using namespace kiwano;
using namespace kiwano::imgui;

KGE_DECLARE_SMART_PTR(MainScene);
class MainScene
	: public Scene
{
	bool show_demo_window = true;
	bool show_another_window = false;
	Color clear_color = Color(0.45f, 0.55f, 0.6f, 1.f);

public:
	MainScene()
	{
		// 创建 ImGui 图层
		ImGuiLayerPtr layer = ImGuiView::Instance().CreateLayer(this);
		AddChild(layer);

		// 添加 ImGui 提供的 Demo 窗口
		layer->AddItem([=]() {
			if (show_demo_window)
				ImGui::ShowDemoWindow(&show_demo_window);
		}, L"DemoWindow");

		// 添加一个简单窗口
		layer->AddItem(MakeClosure(this, &MainScene::SimpleWindow), L"SimpleWindow");

		// 再添加一个窗口
		layer->AddItem(MakeClosure(this, &MainScene::AnotherWindow), L"AnotherWindow");
	}

	void SimpleWindow()
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");

		ImGui::Text("This is some useful text.");
		ImGui::Checkbox("Demo Window", &show_demo_window);
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		ImGui::ColorEdit3("clear color", (float*)&clear_color);

		if (ImGui::Button("Button"))
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		// 修改窗口背景色
		Renderer::Instance().SetClearColor(clear_color);
	}

	void AnotherWindow()
	{
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}
	}
};
