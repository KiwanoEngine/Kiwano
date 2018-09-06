#include "..\e2dmodule.h"
#include "..\e2dtool.h"


e2d::GC e2d::GC::instance_;

e2d::GC::GC()
{
}

e2d::GC::~GC()
{
	Image::ClearCache();

	Player::DestroyInstance();
	Audio::DestroyInstance();
	Renderer::DestroyInstance();
	Input::DestroyInstance();
	Window::DestroyInstance();
	Game::DestroyInstance();
}
