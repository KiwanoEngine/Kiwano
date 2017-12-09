#include "..\Easy2D\easy2d.h"


int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	if (!EApp::init(L"Easy2D Demo", 640, 480))
		return -1;

	auto scene = new EScene();
	scene->retain();

	auto text = new EText(L"中文测试中文测试中文测试中文测试中文测试中文测试中文测试", L"楷体");
	text->setPos(EApp::getWidth() / 2, EApp::getHeight() / 2);
	//text->setWordWrapping(true);
	//text->setWordWrappingWidth(130);
	text->setRotation(40);
	text->runAction(new EActionLoop(new EActionTwo(new EActionFadeOut(1), new EActionFadeIn(1))));
	scene->add(text);

	auto listener = new EListenerKeyboardPress([=]() {
		if (EKeyboardMsg::getKeyValue() == EKeyboardMsg::KEY::SPACE)
		{
			EApp::backScene(new ETransitionMove(0.5f, ETransitionMove::DOWN));
		}
	});
	listener->bindWith(scene);

	auto scene2 = new EScene();

	auto bird = new ESprite(L"atlas.png", 5, 982, 34, 24);
	auto animation = new EAnimation();
	animation->addFrame(new ESpriteFrame(L"atlas.png", 5, 982, 34, 24));
	animation->addFrame(new ESpriteFrame(L"atlas.png", 61, 982, 34, 24));
	animation->addFrame(new ESpriteFrame(L"atlas.png", 117, 982, 34, 24));
	animation->addFrame(new ESpriteFrame(L"atlas.png", 61, 982, 34, 24));
	bird->runAction(new EActionLoop(animation));
	bird->setPivot(0.5f, 0.5f);
	bird->setPos(EApp::getWidth() / 2, EApp::getHeight() / 2);
	scene2->add(bird);

	auto btnStart = new ESprite(L"atlas.png", 702, 234, 116, 70);
	auto btnStartSelected = new ESprite(L"atlas.png", 702, 234, 116, 70);
	btnStartSelected->setPosY(5);
	auto button = new EButton(btnStart, btnStartSelected, [=] {
		/*if (EApp::isPaused())
		{
			EApp::resume();
		}
		else
		{
			EApp::pause();
		}*/
		EApp::enterScene(scene, new ETransitionMove(1, ETransitionMove::RIGHT));
	});
	button->setPivot(0.5f, 0.5f);
	button->setPos(EApp::getWidth() / 2, EApp::getHeight() / 2 + 100);
	scene2->add(button);

	EMusicUtils::playMusic(L"music.wav", -1);

	/*scene2->runAction(new EActionSequence(5,
		new EActionCallback([]() { EMusicUtils::playMusic(L"music.wav", -1); }),
		new EActionDelay(3),
		new EActionCallback([]() { EMusicUtils::pauseMusic(L"music.wav"); }),
		new EActionDelay(10),
		new EActionCallback([]() { EMusicUtils::resumeMusic(L"music.wav"); })));*/

	EApp::enterScene(scene2, new ETransitionMove(1, ETransitionMove::UP));

	return EApp::run();
}
