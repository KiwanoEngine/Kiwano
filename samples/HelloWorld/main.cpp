// Copyright (C) 2019 Nomango

#include "kiwano/kiwano.h"

using namespace kiwano;

class MainScene
	: public Scene
{
public:
	MainScene()
	{
		TextPtr text = new Text(L"Hello Kiwano!");	// �������ֽڵ�
		text->SetSize(text->GetLayoutSize());		// ���ýڵ��СΪ���ֲ��ִ�С
		text->SetPosition(640 / 2, 480 / 2);		// �޸Ľڵ�λ��, ʹ�ڵ�����Ļ�Ͼ���
		text->SetAnchor(0.5, 0.5);					// �޸Ľڵ�ê��, ʹ�������Ķ�����Ļ����
		this->AddChild(text);						// ��ӵ�������
	}
};

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	// ���� Kiwano ����ʵ��
	Application app;

	// �ڳ�ʼ��ѡ�������ô��ڿ��
	Options options;
	options.width = 640;
	options.height = 480;

	// ��ʼ�� app
	app.Init(options);

	// ��������������
	ScenePtr scene = new MainScene;
	app.EnterScene(scene);

	// ����
	app.Run();
	return 0;
}
