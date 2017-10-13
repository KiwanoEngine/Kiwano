#include "..\etools.h"
#include "..\Win\winbase.h"
#include <vector>


// 鼠标消息
e2d::EMouseMsg mouseMsg;
// 鼠标消息监听器
std::vector<e2d::EMouseListener*> m_vMouseListeners;
// 按键消息监听器
//std::vector<e2d::EKeyListener*> m_vKeyListeners;


DWORD e2d::EMouseMsg::getX()
{
	return LOWORD(mouseMsg.m_lParam);
}

DWORD e2d::EMouseMsg::getY()
{
	return HIWORD(mouseMsg.m_lParam);
}

e2d::EPoint e2d::EMouseMsg::getPos()
{
	return EPoint(LOWORD(mouseMsg.m_lParam), HIWORD(mouseMsg.m_lParam));
}

bool e2d::EMouseMsg::isLButtonDown()
{
	return GET_KEYSTATE_WPARAM(mouseMsg.m_wParam) == MK_LBUTTON;
}

bool e2d::EMouseMsg::isMButtonDown()
{
	return GET_KEYSTATE_WPARAM(mouseMsg.m_wParam) == MK_MBUTTON;
}

bool e2d::EMouseMsg::isRButtonDown()
{
	return GET_KEYSTATE_WPARAM(mouseMsg.m_wParam) == MK_RBUTTON;
}

bool e2d::EMouseMsg::isShiftDown()
{
	return GET_KEYSTATE_WPARAM(mouseMsg.m_wParam) == MK_SHIFT;
}

bool e2d::EMouseMsg::isCtrlDown()
{
	return GET_KEYSTATE_WPARAM(mouseMsg.m_wParam) == MK_CONTROL;
}

DWORD e2d::EMouseMsg::getWheelDelta()
{
	return GET_WHEEL_DELTA_WPARAM(mouseMsg.m_wParam);
}

e2d::EMouseMsg::MSG e2d::EMouseMsg::getMsg()
{
	return MSG(mouseMsg.m_nMsg);
}



void e2d::EMsgManager::MouseProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// 保存鼠标消息
	mouseMsg.m_nMsg = message;
	mouseMsg.m_wParam = wParam;
	mouseMsg.m_lParam = lParam;
	// 执行鼠标消息监听函数
	for (auto mlistener : m_vMouseListeners)
	{
		if (mlistener->isRunning())
		{
			mlistener->runCallback();
		}
	}
}

void e2d::EMsgManager::KeyboardProc(UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
		break;
	}

	for (auto mlistener : m_vMouseListeners)
	{
		if (mlistener->isRunning())
		{
			mlistener->runCallback();
		}
	}
}

void e2d::EMsgManager::addListener(e2d::EMouseListener * listener)
{
	if (listener)
	{
		listener->start();
		listener->retain();
		listener->setParentScene(EApp::get()->getLoadingScene());
		m_vMouseListeners.push_back(listener);
	}
}

void e2d::EMsgManager::__exec()
{
}
