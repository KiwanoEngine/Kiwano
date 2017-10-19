#include "..\Easy2D\easy2d.h"


int WINAPI WinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow
)
{
	EApp app;

	if (app.init(L"Easy2D Demo", 640, 480))
	{
		auto scene = new EScene();

		auto text = new EText(L"中文测试中文测试中文测试中文测试中文测试中文测试中文测试", EColor::WHITE, L"Microsoft Yahei");
		text->setPos(EApp::getWidth() / 2, EApp::getHeight() / 2);
		//text->setWordWrapping(true);
		//text->setWordWrappingWidth(130);
		text->setRotation(40);
		text->getFont()->setItalic(true);
		text->setAnchor(0.5f, 0.5f);
		text->setColor(EColor::WHITE);
		scene->add(text);

		text->runAction(new EActionLoop(new EActionTwo(new EActionFadeOut(1), new EActionFadeIn(1))));

		app.enterScene(scene);
		app.run();
	}

	return 0;
}
