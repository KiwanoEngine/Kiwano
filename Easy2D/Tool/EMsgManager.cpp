#include "..\etools.h"
#include "..\Win\winbase.h"
#include <vector>

static POINT p;
static e2d::EMouseMsg mouseMsg;

std::vector<e2d::EMouseListener*> m_vMouseListeners;

void e2d::EMsgManager::setMouseMsg(UINT message)
{
	mouseMsg.m_nMsg = message;

	switch (message)
	{
	case WM_LBUTTONUP:
		mouseMsg.m_bLButtonDown = false;
		break;

	case WM_LBUTTONDOWN:
		mouseMsg.m_bLButtonDown = true;
		break;

	case WM_LBUTTONDBLCLK:
		mouseMsg.m_bLButtonDown = true;
		break;

	case WM_MBUTTONUP:
		mouseMsg.m_bMButtonDown = false;
		break;

	case WM_MBUTTONDOWN:
		mouseMsg.m_bMButtonDown = true;
		break;

	case WM_MBUTTONDBLCLK:
		mouseMsg.m_bMButtonDown = true;
		break;

	case WM_RBUTTONUP:
		mouseMsg.m_bRButtonDown = false;
		break;

	case WM_RBUTTONDOWN:
		mouseMsg.m_bRButtonDown = true;
		break;

	case WM_RBUTTONDBLCLK:
		mouseMsg.m_bRButtonDown = true;
		break;

	case WM_MOUSEMOVE:
	{
		GetCursorPos(&p);
		ScreenToClient(GetHWnd(), &p);
		mouseMsg.m_Pos = p;
		break;
	}
	
	case WM_MOUSEWHEEL:
		break;
	}

	for (auto mlistener : m_vMouseListeners)
	{
		//mlistener->
	}
}

e2d::EMouseMsg::MESSAGE e2d::EMsgManager::getMouseMsg()
{
	return e2d::EMouseMsg::MESSAGE(mouseMsg.m_nMsg);
}

void e2d::EMsgManager::addListener(e2d::EMouseListener * listener)
{
	if (listener)
	{
		listener->retain();
		m_vMouseListeners.push_back(listener);
	}
}

void e2d::EMsgManager::__exec()
{
}
