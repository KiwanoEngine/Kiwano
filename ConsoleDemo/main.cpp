#include <easy2d.h>

class Scene2 :
	public EScene
{
public:
	Scene2()
	{
		auto text = new EText(L"²âÊÔ°´Å¥");
		auto text2 = new EText(L"²âÊÔ°´Å¥", L"", 22, EColor::BLUE);
		auto button = new EButton(text, text2, text);
		button->setPos(EApp::getWidth() / 2, EApp::getHeight() / 2);
		button->setCallback([]() {
			EApp::backScene(new ETransitionScaleEmerge(1, ETransitionScaleEmerge::ENTER));
		});
		this->add(button);
	}
};

class Scene :
	public EScene
{
public:
	Scene()
	{
		/*auto sprite = new ESprite(L"test2.png");
		auto button = new EButton(sprite);
		button->setPos(EApp::getWidth() / 2, EApp::getHeight() / 2);
		button->setCallback([]() {
			EApp::enterScene(new Scene2(), new ETransitionScaleEmerge(1, ETransitionScaleEmerge::ENTER));
		});
		this->add(button);*/
		auto sprite = new ESprite(L"test2.png");
		sprite->setPivot(-1, 0);
		sprite->setPos(EApp::getWidth() / 2, EApp::getHeight() / 2);
		this->add(sprite);

		sprite->runAction(new EActionLoop(new EActionRotateBy(1, 60)));
	}
};

int main()
{
	EApp app;

	if (app.init(L"Easy2D Demo", 640, 480))
	{
		ENode::setDefaultPiovt(0.5f, 0.5f);

		auto scene = new Scene();
		app.enterScene(scene);

		app.run();
	}

	return 0;
}
