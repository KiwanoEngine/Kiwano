// Copyright (C) 2019 Nomango

#include "Demo1.h"
#include "Demo2.h"
#include "Demo3.h"
#include "Demo4.h"
#include "Demo5.h"
#include "resource.h"

namespace
{
	struct Demo
	{
		String title;
		ScenePtr(*Create)();
	};

	Demo s_Demos[] = {
		{ L"����ʾ��", Demo1::Create },
		{ L"����ʾ��", Demo2::Create },
		{ L"��Ƶ����ʾ��", Demo3::Create },
		{ L"֡����ʾ��", Demo4::Create },
		{ L"��������ʾ��", Demo5::Create },
	};
	int s_CurrIndex = -1;
	int s_DemoNum = sizeof(s_Demos) / sizeof(Demo);
}

// ��Դ���ع���
ResLoader g_Loader;

class DemoApp
	: public Application
{
public:
	DemoApp()
	{
		// ʹ�� Audio ���
		Use(Audio::Instance());

		// ʹ�� HttpClient ���
		Use(HttpClient::Instance());

		Options options(L"Kiwanoʾ������", WINDOW_WIDTH, WINDOW_HEIGHT, MAKEINTRESOURCE(IDI_ICON1));
		Init(options);
	}

	void OnStart() override
	{
		// �� JSON �ļ��ж�ȡ��Դ��Ϣ
		//g_Loader.LoadFromJsonFile(L"res/index.json");

		// �� XML �ļ��ж�ȡ��Դ��Ϣ
		g_Loader.LoadFromXmlFile(L"res/index.xml");

		// �л�����һ������
		ChangeDemoScene(0);
	}

	void OnDestroy() override
	{
		// �˳���Ϸʱ������Դ
		g_Loader.Destroy();
	}

	void ChangeDemoScene(int index)
	{
		if (s_CurrIndex != index)
		{
			s_CurrIndex = index;

			String title = s_Demos[index].title;
			Window::Instance()->SetTitle(L"Kiwanoʾ������ - " + title);

			ScenePtr scene = s_Demos[index].Create();
			EnterScene(scene);

			// ��Ӱ�������
			scene->AddListener(Event::KeyUp, MakeClosure(this, &DemoApp::KeyPressed));

			// ��ʾ��ʾ����
			String intro_str = String::format(L"���� 1~%d ���л�ʾ��\n", s_DemoNum);
			TextPtr intro = new Text(intro_str + title);
			intro->SetFontSize(16.f);
			intro->SetPosition(10, 10);
			scene->AddChild(intro);
		}
	}

	void KeyPressed(Event const& evt)
	{
		KGE_ASSERT(evt.type == Event::KeyUp);

		if (evt.key.code > KeyCode::Num0 &&
			evt.key.code <= (KeyCode::Num0 + s_DemoNum))
		{
			int index = evt.key.code - KeyCode::Num1;
			ChangeDemoScene(index);
		}
	}
};

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
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
