// Copyright (C) 2019 Nomango

#include "kiwano.h"
#include "kiwano-imgui.h"
#include "MainScene.h"

using namespace kiwano;

class ImGuiApp
	: public Application
{
public:
	ImGuiApp()
	{
		// 添加 ImGui 组件
		Use(&ImGuiView::Instance());

		// 初始化
		Options options(L"ImGui Demo", 1280, 800);

		Init(options);
	}

	void OnStart() override
	{
		MainScenePtr scene = new MainScene;
		EnterScene(scene);
	}
};

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
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
