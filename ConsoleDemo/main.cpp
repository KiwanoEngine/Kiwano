#include <easy2d.h>

int main()
{
	EApp app;

	if (app.init(L"Easy2D Demo", 640, 480))
	{
		app.showConsole();
		auto scene = new EScene();

		// 创建一个文本
		auto btnNormal = new EText(L"开");
		auto btnSelected = new EText(L"开");
		btnSelected->movePosY(2);
		auto btnNormal2 = new EText(L"关");
		auto btnSelected2 = new EText(L"关");
		btnSelected2->movePosY(2);
		// 创建一个按钮
		auto button = new EButtonToggle(btnNormal, btnNormal2, btnSelected, btnSelected2);
		button->setCallback([=]() {
			//button->setEnable(false);
		});
		button->toggle();
		button->setPos(EApp::getWidth() / 2, EApp::getHeight() / 2);
		scene->add(button);

		// 创建按钮
		auto button2 = new EButton(new EText(L"关闭"), [=]() { button->setEnable(!button->isEnable()); });
		button2->setPos(40, 40);
		scene->add(button2);

		app.enterScene(scene);

		app.run();
	}

	return 0;
}
