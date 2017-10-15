#include "winbase.h"

HWND hwnd = nullptr;
ID2D1Factory * pDirect2dFactory = nullptr;
ID2D1HwndRenderTarget * pRenderTarget = nullptr;
ID2D1SolidColorBrush * m_pSolidBrush = nullptr;
IWICImagingFactory * pIWICFactory = nullptr;


HWND &GetHWnd()
{
	return hwnd;
}

ID2D1Factory * &GetFactory()
{
	return pDirect2dFactory;
}

IWICImagingFactory * &GetImagingFactory()
{
	if (!pIWICFactory)
	{
		CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory,
			reinterpret_cast<void **>(&pIWICFactory)
		);
	}
	return pIWICFactory;
}

ID2D1HwndRenderTarget * &GetRenderTarget()
{
	if (!pRenderTarget)
	{
		RECT rc;
		GetClientRect(hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		// Create a Direct2D render target.
		HRESULT hr;
		hr = pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(hwnd, size),
			&pRenderTarget
		);
		
		ASSERT(SUCCEEDED(hr), "Create Render Target Failed!");
	}
	return pRenderTarget;
}

ID2D1SolidColorBrush * &GetSolidColorBrush()
{
	if (!m_pSolidBrush)
	{
		pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pSolidBrush);
	}
	return m_pSolidBrush;
}

