// Copyright (C) 2019 Nomango

#include "easy2d.h"

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

class MainScene
	: public easy2d::Scene
{
public:
	MainScene()
	{
		// 创建文字节点
		easy2d::TextPtr text = new easy2d::Text(L"Hello Easy2D!");
		// 修改节点位置
		text->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
		// 修改节点锚点, 使节点在屏幕上居中
		text->SetAnchor(0.5, 0.5);
		// 添加到场景中
		this->AddChild(text);
	}
};

int main()
{
	try
	{
		// 创建 Easy2D 程序实例
		easy2d::Application app;

		// 创建初始化选项
		easy2d::Options options;
		// 设置窗口宽高
		options.width = WINDOW_WIDTH;
		options.height = WINDOW_HEIGHT;
		// 初始化 app
		app.Init(options);

		// 创建场景并进入
		easy2d::ScenePtr scene = new MainScene;
		app.EnterScene(scene);

		// 运行
		app.Run();
	}
	catch (std::exception& e)
	{
		::MessageBoxA(nullptr, e.what(), "An exception has occurred!", MB_ICONERROR | MB_TASKMODAL);
	}
	return 0;
}
