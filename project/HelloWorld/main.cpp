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
		// �������ֽڵ�
		easy2d::TextPtr text = new easy2d::Text(L"Hello Easy2D!");
		// �޸Ľڵ�λ��
		text->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
		// �޸Ľڵ�ê��, ʹ�ڵ�����Ļ�Ͼ���
		text->SetAnchor(0.5, 0.5);
		// ��ӵ�������
		this->AddChild(text);
	}
};

int main()
{
	try
	{
		// ���� Easy2D ����ʵ��
		easy2d::Application app;

		// ������ʼ��ѡ��
		easy2d::Options options;
		// ���ô��ڿ��
		options.width = WINDOW_WIDTH;
		options.height = WINDOW_HEIGHT;
		// ��ʼ�� app
		app.Init(options);

		// ��������������
		easy2d::ScenePtr scene = new MainScene;
		app.EnterScene(scene);

		// ����
		app.Run();
	}
	catch (std::exception& e)
	{
		::MessageBoxA(nullptr, e.what(), "An exception has occurred!", MB_ICONERROR | MB_TASKMODAL);
	}
	return 0;
}
