#include "..\e2dbase.h"
#include "..\e2dmanager.h"
#include "..\e2dnode.h"


e2d::Renderer*		e2d::Renderer::_instance = nullptr;
ID2D1Factory*		e2d::Renderer::_d2dFactory = nullptr;
IWICImagingFactory*	e2d::Renderer::_imagingFactory = nullptr;
IDWriteFactory*		e2d::Renderer::_writeFactory = nullptr;
IDWriteTextFormat*	e2d::Renderer::_textFormat = nullptr;
ID2D1StrokeStyle*	e2d::Renderer::_miterStrokeStyle = nullptr;
ID2D1StrokeStyle*	e2d::Renderer::_bevelStrokeStyle = nullptr;
ID2D1StrokeStyle*	e2d::Renderer::_roundStrokeStyle = nullptr;

e2d::Renderer * e2d::Renderer::getInstance()
{
	if (!_instance)
	{
		_instance = new (std::nothrow) Renderer;
	}
	return _instance;
}

void e2d::Renderer::destroyInstance()
{
	if (_instance)
	{
		delete _instance;
		_instance = nullptr;

		SafeRelease(_textFormat);
		SafeRelease(_d2dFactory);
		SafeRelease(_miterStrokeStyle);
		SafeRelease(_bevelStrokeStyle);
		SafeRelease(_roundStrokeStyle);
		SafeRelease(_imagingFactory);
		SafeRelease(_writeFactory);
	}
}

e2d::Renderer::Renderer()
	: _showFps(false)
	, _renderTarget(nullptr)
	, _solidBrush(nullptr)
	, _textRenderer(nullptr)
	, _clearColor(D2D1::ColorF(D2D1::ColorF::Black))
{
	CoInitialize(nullptr);

	this->__createDeviceResources();
}

e2d::Renderer::~Renderer()
{
	SafeRelease(_renderTarget);
	SafeRelease(_solidBrush);
	SafeRelease(_textRenderer);

	CoUninitialize();
}

bool e2d::Renderer::__createDeviceResources()
{
	HRESULT hr = S_OK;

	if (!_renderTarget)
	{
		HWND hWnd = Window::getInstance()->getHWnd();

		// 创建设备相关资源。这些资源应在 Direct3D 设备消失时重建，
		// 比如当 isVisiable 被修改，等等
		RECT rc;
		GetClientRect(hWnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		// 创建一个 Direct2D 渲染目标
		hr = Renderer::getFactory()->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(
				hWnd,
				size),
			&_renderTarget
		);

		if (FAILED(hr))
		{
			throw SystemException(L"Create ID2D1HwndRenderTarget failed");
		}
		else
		{
			// 创建画刷
			hr = _renderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::White),
				&_solidBrush
			);
		}

		if (FAILED(hr))
		{
			throw SystemException(L"Create ID2D1SolidColorBrush failed");
		}
	}

	return SUCCEEDED(hr);
}

void e2d::Renderer::__discardDeviceResources()
{
	SafeRelease(_renderTarget);
	SafeRelease(_solidBrush);
	SafeRelease(_textRenderer);
	SafeRelease(_miterStrokeStyle);
	SafeRelease(_bevelStrokeStyle);
	SafeRelease(_roundStrokeStyle);
}

void e2d::Renderer::render()
{
	HRESULT hr = S_OK;

	// 创建设备相关资源
	Renderer::__createDeviceResources();

	// 开始渲染
	_renderTarget->BeginDraw();
	// 使用背景色清空屏幕
	_renderTarget->Clear(_clearColor);

	// 渲染场景
	SceneManager::getInstance()->render();

	// 渲染 FPS
	if (_showFps)
	{
		static int s_nRenderTimes = 0;
		static double s_fLastRenderTime = 0;
		static String s_sFpsText;

		++s_nRenderTimes;

		double fDelay = Time::getTotalTime() - s_fLastRenderTime;
		if (fDelay >= 0.3)
		{
			s_sFpsText = String::format(L"FPS: %.1lf", (1 / fDelay) * s_nRenderTimes);
			s_fLastRenderTime = Time::getTotalTime();
			s_nRenderTimes = 0;
		}

		IDWriteTextLayout * pTextLayout = nullptr;
		IDWriteTextFormat * pTextFormat = Renderer::getFpsTextFormat();

		hr = _writeFactory->CreateTextLayout(
			(const WCHAR *)s_sFpsText,
			(UINT32)s_sFpsText.getLength(),
			pTextFormat,
			0,
			0,
			&pTextLayout
		);

		if (SUCCEEDED(hr))
		{
			_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			_solidBrush->SetOpacity(1.0f);
			_textRenderer->SetTextStyle(
				D2D1::ColorF(D2D1::ColorF::White),
				TRUE,
				D2D1::ColorF(D2D1::ColorF::Black, 0.4f),
				1.5f,
				D2D1_LINE_JOIN_ROUND
			);

			pTextLayout->Draw(nullptr, _textRenderer, 10, 0);

			SafeRelease(pTextLayout);
		}
	}

	// 终止渲染
	hr = _renderTarget->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
	{
		// 如果 Direct3D 设备在执行过程中消失，将丢弃当前的设备相关资源
		// 并在下一次调用时重建资源
		hr = S_OK;
		this->__discardDeviceResources();
	}

	if (FAILED(hr))
	{
		throw SystemException(L"Device loss recovery failed");
	}
}

e2d::Color e2d::Renderer::getBackgroundColor()
{
	return Color(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
}

void e2d::Renderer::setBackgroundColor(Color color)
{
	_clearColor = color.toD2DColorF();
}

void e2d::Renderer::showFps(bool show)
{
	_showFps = show;
}

ID2D1HwndRenderTarget * e2d::Renderer::getRenderTarget()
{
	return _renderTarget;
}

ID2D1SolidColorBrush * e2d::Renderer::getSolidColorBrush()
{
	return _solidBrush;
}

e2d::TextRenderer * e2d::Renderer::getTextRenderer()
{
	if (!_textRenderer)
	{
		// 创建自定义的文字渲染器
		_textRenderer = TextRenderer::Create(
			Renderer::getFactory(),
			this->getRenderTarget(),
			this->getSolidColorBrush()
		);
	}
	return _textRenderer;
}

ID2D1Factory * e2d::Renderer::getFactory()
{
	if (!_d2dFactory)
	{
		HRESULT hr = D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			&_d2dFactory
		);

		if (FAILED(hr))
		{
			throw SystemException(L"Create ID2D1Factory failed");
		}
	}
	return _d2dFactory;
}

IWICImagingFactory * e2d::Renderer::getImagingFactory()
{
	if (!_imagingFactory)
	{
		// 创建 WIC 绘图工厂，用于统一处理各种格式的图片
		HRESULT hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory,
			reinterpret_cast<void**>(&_imagingFactory)
		);

		if (FAILED(hr))
		{
			throw SystemException(L"Create IWICImagingFactory failed");
		}
	}
	return _imagingFactory;
}

IDWriteFactory * e2d::Renderer::getWriteFactory()
{
	if (!_writeFactory)
	{
		// 创建 DirectWrite 工厂
		HRESULT hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&_writeFactory)
		);

		if (FAILED(hr))
		{
			throw SystemException(L"Create IDWriteFactory failed");
		}
	}
	return _writeFactory;
}

IDWriteTextFormat * e2d::Renderer::getFpsTextFormat()
{
	if (!_textFormat)
	{
		// 创建 FPS 文本格式化对象
		HRESULT hr = Renderer::getWriteFactory()->CreateTextFormat(
			L"",
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			20,
			L"",
			&_textFormat
		);

		if (SUCCEEDED(hr))
		{
			_textFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
		}
	}
	return _textFormat;
}

ID2D1StrokeStyle * e2d::Renderer::getMiterStrokeStyle()
{
	if (!_miterStrokeStyle)
	{
		HRESULT hr = Renderer::getFactory()->CreateStrokeStyle(
			D2D1::StrokeStyleProperties(
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_LINE_JOIN_MITER,
				2.0f,
				D2D1_DASH_STYLE_SOLID,
				0.0f),
			nullptr,
			0,
			&_miterStrokeStyle
		);

		if (FAILED(hr))
		{
			throw SystemException(L"Create ID2D1StrokeStyle failed");
		}
	}
	return _miterStrokeStyle;
}

ID2D1StrokeStyle * e2d::Renderer::getBevelStrokeStyle()
{
	if (!_bevelStrokeStyle)
	{
		HRESULT hr = Renderer::getFactory()->CreateStrokeStyle(
			D2D1::StrokeStyleProperties(
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_LINE_JOIN_BEVEL,
				2.0f,
				D2D1_DASH_STYLE_SOLID,
				0.0f),
			nullptr,
			0,
			&_bevelStrokeStyle
		);

		if (FAILED(hr))
		{
			throw SystemException(L"Create ID2D1StrokeStyle failed");
		}
	}
	return _bevelStrokeStyle;
}

ID2D1StrokeStyle * e2d::Renderer::getRoundStrokeStyle()
{
	if (!_roundStrokeStyle)
	{
		HRESULT hr = Renderer::getFactory()->CreateStrokeStyle(
			D2D1::StrokeStyleProperties(
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_LINE_JOIN_ROUND,
				2.0f,
				D2D1_DASH_STYLE_SOLID,
				0.0f),
			nullptr,
			0,
			&_roundStrokeStyle
		);

		if (FAILED(hr))
		{
			throw SystemException(L"Create ID2D1StrokeStyle failed");
		}
	}
	return _roundStrokeStyle;
}
