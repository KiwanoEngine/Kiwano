#include "winbase.h"

HWND hwnd = nullptr;
ID2D1Factory * pDirect2dFactory = nullptr;
ID2D1HwndRenderTarget * pRenderTarget = nullptr;
ID2D1SolidColorBrush * m_pSolidBrush = nullptr;


HWND &GetHWnd()
{
	return hwnd;
}

ID2D1Factory * &GetFactory()
{
	return pDirect2dFactory;
}

ID2D1HwndRenderTarget * &GetRenderTarget()
{
	return pRenderTarget;
}

ID2D1SolidColorBrush *& GetSolidColorBrush()
{
	if (!m_pSolidBrush)
	{
		pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pSolidBrush);
	}
	return m_pSolidBrush;
}

