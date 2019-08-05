// Copyright (C) 2019 Nomango

#include "kiwano/kiwano.h"

using namespace kiwano;

class MainScene
	: public Scene
{
public:
	MainScene()
	{
		TextPtr text = new Text(L"Hello Kiwano!");	// 创建文字节点
		text->SetSize(text->GetLayoutSize());		// 设置节点大小为文字布局大小
		text->SetPosition(640 / 2, 480 / 2);		// 修改节点位置, 使节点在屏幕上居中
		text->SetAnchor(0.5, 0.5);					// 修改节点锚点, 使文字中心对齐屏幕中心
		this->AddChild(text);						// 添加到场景中
	}
};

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
	// 创建 Kiwano 程序实例
	Application app;

	// 在初始化选项中设置窗口宽高
	Options options;
	options.width = 640;
	options.height = 480;

	// 初始化 app
	app.Init(options);

	// 创建场景并进入
	ScenePtr scene = new MainScene;
	app.EnterScene(scene);

	// 运行
	app.Run();
	return 0;
}
