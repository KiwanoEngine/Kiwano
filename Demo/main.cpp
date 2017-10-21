#include "..\Easy2D\easy2d.h"


int WINAPI WinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow
)
{
	EApp app;

	if (app.init(L"Easy2D Demo", 640, 480, app.NO_MINI_SIZE))
	{
		auto scene = new EScene();

		auto text = new EText(L"中文测试中文测试中文测试中文测试中文测试中文测试中文测试", EColor::WHITE, L"楷体");
		text->setPos(EApp::getWidth() / 2, EApp::getHeight() / 2);
		//text->setWordWrapping(true);
		//text->setWordWrappingWidth(130);
		text->setRotation(40);
		text->getFont()->setItalic(true);
		text->setAnchor(0.5f, 0.5f);
		text->setColor(EColor::WHITE);
		//text->runAction(new EActionLoop(new EActionTwo(new EActionFadeOut(1), new EActionFadeIn(1))));
		scene->add(text);

		auto listener = new EKeyboardPressListener([=]() {
			if (EKeyboardMsg::getVal() == EKeyboardMsg::KEY::SPACE)
			{
				EApp::backScene(new ETransitionFade(0.5f, 0.5f));
			}
		});
		listener->bindWith(scene);

		auto scene2 = new EScene();

		auto bird = new ESprite();
		auto animation = new EAnimation();
		animation->addFrame(new ESpriteFrame(L"atlas.png", 5, 982, 34, 24));
		animation->addFrame(new ESpriteFrame(L"atlas.png", 61, 982, 34, 24));
		animation->addFrame(new ESpriteFrame(L"atlas.png", 117, 982, 34, 24));
		bird->runAction(new EActionLoop(animation));
		bird->setAnchor(0.5f, 0.5f);
		bird->setPos(EApp::getWidth() / 2, EApp::getHeight() / 2);
		scene2->add(bird);

		auto btnStart = new ESprite(L"atlas.png", 702, 234, 116, 70);
		btnStart->setAnchor(0.5f, 0.5f);
		auto btnStartSelected = new ESprite(L"atlas.png", 702, 234, 116, 70);
		btnStartSelected->setAnchor(0.5f, 0.5f);
		btnStartSelected->setPosY(5);
		auto button = new EButton(btnStart, btnStartSelected, [=] {
			if (EApp::isPaused())
			{
				EApp::resume();
			}
			else
			{
				EApp::pause();
			}
			//EApp::enterScene(scene, new ETransitionScaleEmerge(1, ETransitionScaleEmerge::BACK));
		});
		button->setPos(EApp::getWidth() / 2, EApp::getHeight() / 2 + 100);
		scene2->add(button);

		app.enterScene(scene2, new ETransitionFade(0, 1));
		app.run();
	}

	return 0;
}
