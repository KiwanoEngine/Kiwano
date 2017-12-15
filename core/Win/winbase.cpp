#include "winbase.h"


static HWND s_HWnd = nullptr;
static ID2D1Factory * s_pDirect2dFactory = nullptr;
static ID2D1HwndRenderTarget * s_pRenderTarget = nullptr;
static ID2D1SolidColorBrush * s_pSolidBrush = nullptr;
static IWICImagingFactory * s_pIWICFactory = nullptr;
static IDWriteFactory * s_pDWriteFactory = nullptr;
static LARGE_INTEGER s_tNow;
static LARGE_INTEGER s_tFreq;


HWND &GetHWnd()
{
	return s_HWnd;
}

ID2D1Factory * &GetFactory()
{
	if (!s_pDirect2dFactory)
	{
		// 创建设备无关资源，它们的生命周期和程序的时长相同
		HRESULT hr = S_OK;

		// 创建一个 Direct2D 工厂
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &s_pDirect2dFactory);

		ASSERT(SUCCEEDED(hr), "Create Device Independent Resources Failed!");
	}
	return s_pDirect2dFactory;
}

ID2D1HwndRenderTarget * &GetRenderTarget()
{
	if (!s_pRenderTarget)
	{
		// 创建设备相关资源。这些资源应在 Direct3D 设备消失时重建，
		// 比如当 isVisiable 被修改，等等
		RECT rc;
		GetClientRect(GetHWnd(), &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		// 创建一个 Direct2D 渲染目标
		HRESULT hr;
		hr = GetFactory()->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(
				GetHWnd(), 
				size),
			&s_pRenderTarget
		);
		
		ASSERT(SUCCEEDED(hr), "Create Render Target Failed! Maybe you should initalize EApp first.");
	}
	return s_pRenderTarget;
}

IWICImagingFactory * &GetImagingFactory()
{
	if (!s_pIWICFactory)
	{
		// 创建 WIC 绘图工厂，用于统一处理各种格式的图片
		HRESULT hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory,
			reinterpret_cast<void**>(&s_pIWICFactory)
		);
		ASSERT(SUCCEEDED(hr), "Create WICImagingFactory Failed!");
	}
	return s_pIWICFactory;
}

IDWriteFactory * &GetDirectWriteFactory()
{
	if (!s_pDWriteFactory)
	{
		// 创建 DirectWrite 工厂
		HRESULT hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&s_pDWriteFactory)
		);
		ASSERT(SUCCEEDED(hr), "Create DirectWrite Factory Failed!");
	}
	return s_pDWriteFactory;
}

ID2D1SolidColorBrush * &GetSolidColorBrush()
{
	if (!s_pSolidBrush)
	{
		HRESULT hr;
		hr = GetRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::White), 
			&s_pSolidBrush
		);
		ASSERT(SUCCEEDED(hr), "Create Solid Color Brush Failed!");
	}
	return s_pSolidBrush;
}

LARGE_INTEGER &GetFreq()
{
	if (s_tFreq.QuadPart == 0)
	{
		QueryPerformanceFrequency(&s_tFreq);	// 获取时钟频率
	}
	return s_tFreq;
}

LARGE_INTEGER &GetNow()
{
	return s_tNow;
}

bool IsIntervalFull(const LARGE_INTEGER& tLast, const LARGE_INTEGER& tInterval)
{
	return (s_tNow.QuadPart - tLast.QuadPart) >= tInterval.QuadPart;
}

LONGLONG ToMilliseconds(LONGLONG tLast)
{
	return tLast * 1000LL / GetFreq().QuadPart;
}

void SetInterval(LARGE_INTEGER& nInterval, LONGLONG nIntervalMS)
{
	nInterval.QuadPart = static_cast<LONGLONG>(GetFreq().QuadPart * nIntervalMS / 1000.0);
}
