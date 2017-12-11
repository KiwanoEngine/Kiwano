#include "..\Easy2D\easy2d.h"
#include <iostream>

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	if (!EApp::init(L"Demo", 250, 150))
		return -1;

	EString str;
	str += L"123";
	str += L"4";
	UINT h1 = str.hash();

	EString s = L"abcdea";
	EString ss = s.sub(0);
	EString sss = s.sub(0, 5);
	EString ssss = s.sub(0, 9);
	EString ssssss = s.sub(1, 4);
	EString sssss = s.sub(-1, 3);
	int i = s.findFirstOf(L'a');
	int j = s.findLastOf(L'a');

	EString string = L"Hello";
	string = string + 2017 + L"!";

	EString str2;
	str2 += 1;
	str2 += 2L;
	str2 += 2.3;
	str2 += 4.6f;
	UINT h2 = str2.hash();

	str2 += std::wstring(L"sdf");
	EString str3;
	str3 += str2 + str;
	UINT h3 = str3.hash();

	auto scene = new EScene();
	EApp::enterScene(scene);

	for (int i = 0; i < 25; i++)
	{
		auto sprite = new ESprite(L"test.png");
		sprite->setScale(0.5f);
		sprite->setPos(ERandom::between(0, EApp::getWidth()), ERandom::between(0, EApp::getHeight()));
		scene->add(sprite);
	}

	//auto scene = new EScene();
	//scene->retain();

	//auto text = new EText(L"中文测试中文测试中文测试中文测试中文测试中文测试中文测试", L"楷体");
	//text->setPos(EApp::getWidth() / 2, EApp::getHeight() / 2);
	////text->setWordWrapping(true);
	////text->setWordWrappingWidth(130);
	//text->setRotation(40);
	//text->runAction(new EActionLoop(new EActionTwo(new EActionFadeOut(1), new EActionFadeIn(1))));
	//scene->add(text);

	//auto listener = new EListenerKeyboardPress([=]() {
	//	if (EKeyboardMsg::getKeyValue() == EKeyboardMsg::KEY::SPACE)
	//	{
	//		EApp::backScene(new ETransitionMove(0.5f, ETransitionMove::DOWN));
	//	}
	//});
	//listener->bindWith(scene);

	//auto scene2 = new EScene();

	//auto bird = new ESprite(L"atlas.png", 5, 982, 34, 24);
	//auto animation = new EAnimation();
	//animation->addFrame(new ESpriteFrame(L"atlas.png", 5, 982, 34, 24));
	//animation->addFrame(new ESpriteFrame(L"atlas.png", 61, 982, 34, 24));
	//animation->addFrame(new ESpriteFrame(L"atlas.png", 117, 982, 34, 24));
	//animation->addFrame(new ESpriteFrame(L"atlas.png", 61, 982, 34, 24));
	//bird->runAction(new EActionLoop(animation));
	//bird->setPivot(0.5f, 0.5f);
	//bird->setPos(EApp::getWidth() / 2, EApp::getHeight() / 2);
	//scene2->add(bird);

	//auto btnStart = new ESprite(L"atlas.png", 702, 234, 116, 70);
	//auto btnStartSelected = new ESprite(L"atlas.png", 702, 234, 116, 70);
	//btnStartSelected->setPosY(5);
	//auto button = new EButton(btnStart, btnStartSelected, [=] {
	//	/*if (EApp::isPaused())
	//	{
	//		EApp::resume();
	//	}
	//	else
	//	{
	//		EApp::pause();
	//	}*/
	//	EApp::enterScene(scene, new ETransitionMove(1, ETransitionMove::RIGHT));
	//});
	//button->setPivot(0.5f, 0.5f);
	//button->setPos(EApp::getWidth() / 2, EApp::getHeight() / 2 + 100);
	//scene2->add(button);

	//EMusicUtils::playMusic(L"music.wav", -1);

	///*scene2->runAction(new EActionSequence(5,
	//	new EActionCallback([]() { EMusicUtils::playMusic(L"music.wav", -1); }),
	//	new EActionDelay(3),
	//	new EActionCallback([]() { EMusicUtils::pauseMusic(L"music.wav"); }),
	//	new EActionDelay(10),
	//	new EActionCallback([]() { EMusicUtils::resumeMusic(L"music.wav"); })));*/

	//EApp::enterScene(scene2, new ETransitionMove(1, ETransitionMove::UP));

	return EApp::run();
}
