#include "..\Easy2D\easy2d.h"


int WINAPI WinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow
)
{
	EApp app;

	if (app.init(L"Easy2D Demo", 640, 480, app.NO_CLOSE | app.NO_MINI_SIZE | app.TOP_MOST))
	{
		auto scene = new EScene();
		auto scene2 = new EScene();

		auto listener = new EKeyboardPressListener([=]() {
			if (EKeyboardMsg::getVal() == EKeyboardMsg::KEY::SPACE)
			{
				EApp::enterScene(scene, new ETransitionFade(2, 2));
			}
		});
		listener->bindWith(scene2);

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

		auto bird = new ESprite();
		auto animation = new EAnimation();
		animation->addFrame(new ESpriteFrame(L"atlas.png", 5, 982, 34, 24));
		animation->addFrame(new ESpriteFrame(L"atlas.png", 61, 982, 34, 24));
		animation->addFrame(new ESpriteFrame(L"atlas.png", 117, 982, 34, 24));
		bird->runAction(new EActionLoop(animation));
		bird->setAnchor(0.5f, 0.5f);
		bird->setPos(EApp::getWidth() / 2, EApp::getHeight() / 2);
		scene2->add(bird);

		auto listener2 = new EKeyboardPressListener([=]() {
			if (EKeyboardMsg::getVal() == EKeyboardMsg::KEY::SPACE)
			{
				EApp::backScene(new ETransitionFade(0.5f, 0.5f));
			}
		});
		listener2->bindWith(scene);

		app.enterScene(scene2, new ETransitionFade(2, 4));
		app.run();
	}

	return 0;
}
