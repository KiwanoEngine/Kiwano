#include <easy2d.h>

int main()
{
	EApp app;

	if (app.init(L"Easy2D Demo", 640, 640))
	{
		app.showConsole();
		auto scene = new EScene();

		auto sprite = new ESprite(L"haha.png");
		// 获取窗口宽度
		float width = EApp::getWidth();
		// 获取窗口高度
		float height = EApp::getHeight();
		// 移动精灵的位置
		sprite->setPos(width / 2 + sprite->getWidth(), height / 2);
		scene->add(sprite);

		scene->getRoot()->runAction(new EActionLoop(new EActionRotateBy(1, 60)));

		app.enterScene(scene);

		app.run();
	}

	return 0;
}
