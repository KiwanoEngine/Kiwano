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

		auto sprite = new ESprite(L"test.png");
		sprite->setAnchor(0.5f, 0.5f);
		sprite->setPos(EApp::getWidth() / 2, sprite->getHeight() / 2);

		auto sprite2 = new ESprite(L"test.png");
		sprite2->setPos(30, 0);
		sprite2->setScale(0.5);
		sprite2->setRotation(45);

		sprite->addChild(sprite2);

		auto mlistener = new EMouseDoubleClickListener([=](EPoint p) {
			//sprite->setRotation(sprite->getRotation() + 10);
			EApp::setWindowSize(640, 480);
		});

		mlistener->bindWith(sprite);

		scene->add(sprite);
		app.enterScene(scene);
		app.run();
	}

	return 0;
}
