// Copyright (C) 2019 Nomango

#include "easy2d.h"
#include "easy2d-imgui/easy2d-imgui.h"

using namespace easy2d;

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 800;


E2D_DECLARE_SMART_PTR(ImGuiScene);
class ImGuiScene
	: public Scene
{
	bool show_demo_window = true;

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
