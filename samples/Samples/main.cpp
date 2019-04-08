// Copyright (C) 2019 Nomango

#include "Demo1.h"
#include "Demo2.h"
#include "Demo3.h"
#include "Demo4.h"
#include "Demo5.h"

namespace
{
	struct Demo
	{
		String title;
		ScenePtr(*Create)();
	};

	Demo s_Demos[] = {
		{ L"动画示例", Demo1::Create },
		{ L"输入示例", Demo2::Create },
		{ L"音频播放示例", Demo3::Create },
		{ L"帧动画示例", Demo4::Create },
		{ L"网络请求示例", Demo5::Create },
	};
	int s_CurrIndex = -1;
	int s_DemoNum = sizeof(s_Demos) / sizeof(Demo);
}

class DemoApp
	: public Application
{
public:
	DemoApp()
	{
		// 使用 Audio 组件
		Use(&Audio::Instance());

		// 使用 HttpClient 组件
		Use(&HttpClient::Instance());

		Options options(L"Easy2D示例程序", WINDOW_WIDTH, WINDOW_HEIGHT);
		Init(options);
	}

	void OnStart() override
	{
		ChangeDemoScene(0);
	}

	void ChangeDemoScene(int index)
	{
		if (s_CurrIndex != index)
		{
			s_CurrIndex = index;

			String title = s_Demos[index].title;
			GetWindow()->SetTitle(title);

			ScenePtr scene = s_Demos[index].Create();
			EnterScene(scene);

			// 添加按键监听
			scene->AddListener(Event::KeyUp, MakeClosure(this, &DemoApp::KeyPressed));

			// 显示提示文字
			String intro_str = format_wstring(L"按键 1~%d 可切换示例\n", s_DemoNum);
			TextPtr intro = new Text(intro_str + title);
			intro->SetFontSize(16.f);
			intro->SetPosition(10, 10);
			scene->AddChild(intro);
		}
	}

	void KeyPressed(Event const& evt)
	{
		E2D_ASSERT(evt.type == Event::KeyUp);

		if (evt.key.code > KeyCode::Num0 &&
			evt.key.code <= (KeyCode::Num0 + s_DemoNum))
		{
			int index = evt.key.code - KeyCode::Num1;
			ChangeDemoScene(index);
		}
	}
};

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	try
	{
		DemoApp app;
		app.Run();
	}
	catch (std::exception& e)
	{
		::MessageBoxA(nullptr, e.what(), "An exception has occurred!", MB_ICONERROR | MB_OK);
	}
	return 0;
}
