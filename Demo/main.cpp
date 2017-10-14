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

		auto node = new ENode(L"node1");
		node->setPos(50, 80);
		node->setSize(30, 30);
		scene->add(node);

		auto node2 = new ENode(L"node2");
		node2->setPos(20, 20);
		node2->setSize(40, 40);
		node->addChild(node2);

		auto mlistener = new EMouseClickListener([](EPoint p) {
			EApp::getCurrentScene()->getChild(L"node1")->setPos(p.x, p.y);
		});

		mlistener->bindWith(node);

		app.enterScene(scene);

		app.run();
	}

	return 0;
}
