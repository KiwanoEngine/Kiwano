#include "..\Easy2D\easy2d.h"


int WINAPI WinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow
)
{
	EApp app;

	if (app.init(L"Easy2D Demo", 640, 480, true))
	{
		auto scene = new EScene();

		auto node = new ENode();
		node->setPos(50, 80);
		node->setSize(30, 180);
		scene->add(node);

		auto mlistener = new EMouseClickListener([=](EPoint) {
			node->setPos(EMouseMsg::getPos());
		});

		auto klistener = new EKeyPressListener([=] {
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

		mlistener->bindWith(node);

		scene->bindListener(klistener);

		app.enterScene(scene);

		app.run();
	}

	return 0;
}
