#include <easy2d.h>

int main()
{
	EApp app;

	if (app.init(L"Easy2D Demo", 320, 320))
	{
		app.showConsole();
		auto scene = new EScene();

		auto sprite = new ESprite(L"elyse.png");
		sprite->setScale(0.3f);
		// 获取窗口宽度
		float width = EApp::getWidth();
		// 获取窗口高度
		float height = EApp::getHeight();
		// 移动精灵的位置
		sprite->setPos(width / 2, height / 2);
		//sprite->setAnchor(0, 0);
		//scene->add(sprite);
		auto text = new EText(L"balabalabalabalabala", L"宋体", 80, EColor::BLUE);
		scene->add(text, -1);
		sprite->setName(L"test");
		auto button = new EButton(sprite, [=] {
			EApp::enterScene(new EScene);
		});
		scene->add(button);

		app.enterScene(scene);

		app.run();
	}

	return 0;
}
