#include "..\Easy2D\easy2d.h"
#pragma comment(lib, "d2d1.lib")


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	EApp app;

	if (app.init(L"Easy2D Demo", 640, 480))
	{
		auto scene = new EScene();

		auto node = new ENode();
		node->setPos(50, 80);
		node->setSize(30, 180);
		scene->add(node);

		/*auto listener = new EMouseListener([=] {
			if (!EMouseMsg::isLButtonDown())
			{
				if (EMouseMsg::getMsg() == EMouseMsg::MOVE)
				{
					node->setPos(EMouseMsg::getPos());
				}
			}
		});*/

		auto listener = new EKeyPressListener([=] {
			if (EKeyMsg::isCapitalLockOn())
			{
				if (EKeyMsg::getVal() == EKeyMsg::KEY::LEFT)
				{
					node->move(-3, 0);
				}
				if (EKeyMsg::getVal() == EKeyMsg::KEY::RIGHT)
				{
					node->move(3, 0);
				}
			}
		});

		EMsgManager::addListener(listener);

		app.enterScene(scene);

		app.run();
	}

	return 0;
}
