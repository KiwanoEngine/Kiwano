// Copyright (C) 2019 Nomango

#include "easy2d.h"
#include "ImGuiView.h"
#include "ImGuiLayer.h"

using namespace easy2d;

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 800;


E2D_DECLARE_SMART_PTR(ImGuiScene);

class ImGuiScene
	: public Scene
{
	bool show_demo_window = true;
	bool show_another_window = false;
	Color clear_color = Color(0.45f, 0.55f, 0.6f, 1.f);

public:
	ImGuiScene()
	{
		// ���� ImGui ͼ��
		ImGuiLayerPtr layer = new ImGuiLayer;
		AddChild(layer);

		// ��� ImGui �ṩ�� Demo ����
		layer->AddItem([=]() {
			if (show_demo_window)
				ImGui::ShowDemoWindow(&show_demo_window);
		}, L"DemoWindow");

		// ���һ�����״���
		layer->AddItem([=]() {
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

			Renderer::Instance().SetClearColor(clear_color);
		}, L"SimpleWindow");

		// �����һ�����״���
		layer->AddItem([=]() {
			if (show_another_window)
			{
				ImGui::Begin("Another Window", &show_another_window);
				ImGui::Text("Hello from another window!");
				if (ImGui::Button("Close Me"))
					show_another_window = false;
				ImGui::End();
			}
		}, L"AnotherSimpleWindow");
	}
};


class ImGuiApp
	: public Application
{
public:
	ImGuiApp()
	{
		// ��� ImGui ���
		Use(&ImGuiView::Instance());

		// ��ʼ��
		Options options(L"ImGui Demo", WINDOW_WIDTH, WINDOW_HEIGHT);
		options.clear_color = Color(0.45f, 0.55f, 0.6f, 1.f);

		Init(options);
	}

	void OnStart() override
	{
		ImGuiScenePtr scene = new ImGuiScene;
		EnterScene(scene);
	}
};

int main()
{
	try
	{
		ImGuiApp app;
		app.Run();
	}
	catch (std::exception& e)
	{
		::MessageBoxA(nullptr, e.what(), "An exception has occurred!", MB_ICONERROR | MB_OK);
	}
	return 0;
}
