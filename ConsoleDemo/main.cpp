#include "..\Easy2D\easy2d.h"

int main()
{
	EApp app;

	if (app.init(L"Easy2D Demo", 640, 480, true))
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

		auto listener = new EKeyboardPressListener([=] {
			if (EKeyboardMsg::isCapitalLockOn())
			{
				if (EKeyboardMsg::getVal() == EKeyboardMsg::KEY::LEFT)
				{
					node->move(-3, 0);
				}
				if (EKeyboardMsg::getVal() == EKeyboardMsg::KEY::RIGHT)
				{
					node->move(3, 0);
				}
			}
		});

		listener->bindWith(node);

		EMsgManager::bindListener(listener, scene);

		app.enterScene(scene);

		app.run();
	}

	return 0;
}
