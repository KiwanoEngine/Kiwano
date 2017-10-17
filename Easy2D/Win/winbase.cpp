#include "winbase.h"
using namespace std::chrono;


HWND s_HWnd = nullptr;
ID2D1Factory * s_pDirect2dFactory = nullptr;
ID2D1HwndRenderTarget * s_pRenderTarget = nullptr;
ID2D1SolidColorBrush * s_pSolidBrush = nullptr;
IWICImagingFactory * s_pIWICFactory = nullptr;
steady_clock::time_point s_tNow;


HWND &GetHWnd()
{
	return s_HWnd;
}

ID2D1Factory * &GetFactory()
{
	return s_pDirect2dFactory;
}

IWICImagingFactory * &GetImagingFactory()
{
	if (!s_pIWICFactory)
	{
		CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory,
			reinterpret_cast<void **>(&s_pIWICFactory)
		);
	}
	return s_pIWICFactory;
}

ID2D1HwndRenderTarget * &GetRenderTarget()
{
	if (!s_pRenderTarget)
	{
		RECT rc;
		GetClientRect(s_HWnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		// Create a Direct2D render target.
		HRESULT hr;
		hr = s_pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(s_HWnd, size),
			&s_pRenderTarget
		);
		
		ASSERT(SUCCEEDED(hr), "Create Render Target Failed!");
	}
	return s_pRenderTarget;
}

ID2D1SolidColorBrush * &GetSolidColorBrush()
{
	if (!s_pSolidBrush)
	{
		s_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &s_pSolidBrush);
	}
	return s_pSolidBrush;
}

steady_clock::time_point &GetNow()
{
	return s_tNow;
}

long long GetInterval(steady_clock::time_point tLast)
{
	return duration_cast<milliseconds>(s_tNow - tLast).count();
}