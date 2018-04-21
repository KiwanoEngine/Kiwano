#include "..\ebase.h"
#include "..\emanager.h"
#include "..\enode.h"

static bool s_bShowFps = false;
static IDWriteTextFormat * s_pTextFormat = nullptr;
static ID2D1Factory * s_pDirect2dFactory = nullptr;
static ID2D1HwndRenderTarget * s_pRenderTarget = nullptr;
static ID2D1SolidColorBrush * s_pSolidBrush = nullptr;
static IWICImagingFactory * s_pIWICFactory = nullptr;
static IDWriteFactory * s_pDWriteFactory = nullptr;
static D2D1_COLOR_F s_nClearColor = D2D1::ColorF(D2D1::ColorF::Black);


bool e2d::Renderer::__createDeviceIndependentResources()
{
	// 创建设备无关资源，它们的生命周期和程序的时长相同
	HRESULT hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED, 
		&s_pDirect2dFactory
	);

	ASSERT(SUCCEEDED(hr), "Create ID2D1Factory Failed!");

	if (SUCCEEDED(hr))
	{
		// 创建 WIC 绘图工厂，用于统一处理各种格式的图片
		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory,
			reinterpret_cast<void**>(&s_pIWICFactory)
		);
		ASSERT(SUCCEEDED(hr), "Create IWICImagingFactory Failed!");
	}

	if (SUCCEEDED(hr))
	{
		// 创建 DirectWrite 工厂
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&s_pDWriteFactory)
		);
		ASSERT(SUCCEEDED(hr), "Create IDWriteFactory Failed!");
	}

	if (SUCCEEDED(hr))
	{
		// 创建文本格式化对象
		hr = s_pDWriteFactory->CreateTextFormat(
			L"",
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			18,
			L"",
			&s_pTextFormat
		);

		if (s_pTextFormat)
		{
			s_pTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
		}
	}

	return SUCCEEDED(hr);
}

bool e2d::Renderer::__createDeviceResources()
{
	HRESULT hr = S_OK;
	
	if (!s_pRenderTarget)
	{
		HWND hWnd = Window::getHWnd();

		// 创建设备相关资源。这些资源应在 Direct3D 设备消失时重建，
		// 比如当 isVisiable 被修改，等等
		RECT rc;
		GetClientRect(hWnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		// 创建一个 Direct2D 渲染目标
		hr = s_pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(
				hWnd,
				size),
			&s_pRenderTarget
		);

		ASSERT(SUCCEEDED(hr), "Create ID2D1HwndRenderTarget Failed!");

		if (SUCCEEDED(hr))
		{
			// 创建画刷
			hr = s_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::White),
				&s_pSolidBrush
			);
			ASSERT(SUCCEEDED(hr), "Create ID2D1SolidColorBrush Failed!");
		}
	}
	
	return SUCCEEDED(hr);
}

void e2d::Renderer::__discardDeviceResources()
{
	SafeReleaseInterface(&s_pRenderTarget);
	SafeReleaseInterface(&s_pSolidBrush);
}

void e2d::Renderer::__discardResources()
{
	SafeReleaseInterface(&s_pTextFormat);
	SafeReleaseInterface(&s_pDirect2dFactory);
	SafeReleaseInterface(&s_pRenderTarget);
	SafeReleaseInterface(&s_pSolidBrush);
	SafeReleaseInterface(&s_pIWICFactory);
	SafeReleaseInterface(&s_pDWriteFactory);
}

void e2d::Renderer::__render()
{
	HRESULT hr = S_OK;

	// 创建设备相关资源
	Renderer::__createDeviceResources();

	// 开始渲染
	s_pRenderTarget->BeginDraw();
	// 使用背景色清空屏幕
	s_pRenderTarget->Clear(s_nClearColor);

	// 渲染场景
	SceneManager::__render();

	// 渲染 FPS
	if (s_bShowFps)
	{
		static int s_nRenderTimes = 0;
		static double s_fLastRenderTime = 0;
		static e2d::String s_sFpsText = L"";

		s_nRenderTimes++;

		double fDelay = Time::getTotalTime() - s_fLastRenderTime;
		if (fDelay >= 0.3)
		{
			s_sFpsText = String::format(L"FPS: %.1lf", (1 / fDelay) * s_nRenderTimes);
			s_fLastRenderTime = Time::getTotalTime();
			s_nRenderTimes = 0;
		}

		s_pSolidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
		s_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		s_pRenderTarget->DrawTextW(s_sFpsText, (UINT32)s_sFpsText.getLength(), s_pTextFormat, D2D1::RectF(), s_pSolidBrush);
	}

	// 终止渲染
	hr = s_pRenderTarget->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
	{
		// 如果 Direct3D 设备在执行过程中消失，将丢弃当前的设备相关资源
		// 并在下一次调用时重建资源
		hr = S_OK;
		Renderer::__discardDeviceResources();
	}

	if (FAILED(hr))
	{
		// 渲染时产生了未知的错误，退出游戏
		ASSERT(false, L"Renderer error: %#X!", hr);
		Game::quit();
	}
}


void e2d::Renderer::setBackgroundColor(UINT32 color)
{
	s_nClearColor = D2D1::ColorF(color);
}

void e2d::Renderer::showFps(bool show)
{
	s_bShowFps = show;
}

ID2D1Factory * e2d::Renderer::getID2D1Factory()
{
	return s_pDirect2dFactory;
}

ID2D1HwndRenderTarget * e2d::Renderer::getRenderTarget()
{
	return s_pRenderTarget;
}

ID2D1SolidColorBrush * e2d::Renderer::getSolidColorBrush()
{
	return s_pSolidBrush;
}

IWICImagingFactory * e2d::Renderer::getIWICImagingFactory()
{
	return s_pIWICFactory;
}

IDWriteFactory * e2d::Renderer::getIDWriteFactory()
{
	return s_pDWriteFactory;
}
