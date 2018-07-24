#include "..\e2dbase.h"
#include "..\e2dmanager.h"
#include "..\e2dnode.h"


e2d::Renderer*		e2d::Renderer::_instance = nullptr;
ID2D1Factory*		e2d::Renderer::_d2dFactory = nullptr;
IWICImagingFactory*	e2d::Renderer::_imagingFactory = nullptr;
IDWriteFactory*		e2d::Renderer::_writeFactory = nullptr;
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

		SafeRelease(_miterStrokeStyle);
		SafeRelease(_bevelStrokeStyle);
		SafeRelease(_roundStrokeStyle);
		SafeRelease(_d2dFactory);
		SafeRelease(_imagingFactory);
		SafeRelease(_writeFactory);
	}
}

e2d::Renderer::Renderer()
	: _renderTimes(0)
	, _lastRenderTime(0)
	, _fpsFormat(nullptr)
	, _fpsLayout(nullptr)
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
	SafeRelease(_fpsFormat);
	SafeRelease(_fpsLayout);
	SafeRelease(_textRenderer);
	SafeRelease(_solidBrush);
	SafeRelease(_renderTarget);

	CoUninitialize();
}

bool e2d::Renderer::__createDeviceResources()
{
	HRESULT hr = S_OK;

	if (!_renderTarget)
	{
		HWND hWnd = Window::getInstance()->getHWnd();

		// 创建设备相关资源。这些资源应在 Direct3D 设备消失时重建
		RECT rc;
		GetClientRect(hWnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		// 创建一个 Direct2D 渲染目标
		hr = Renderer::getFactory()->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(hWnd, size, D2D1_PRESENT_OPTIONS_IMMEDIATELY),
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
	if (Game::getInstance()->getConfig()->isFpsShow())
	{
		_renderFps();
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

void e2d::Renderer::_renderFps()
{
	++_renderTimes;

	double duration = Game::getInstance()->getTotalDuration().seconds();
	double delay = duration - _lastRenderTime;
	if (delay >= 0.1)
	{
		String fpsText = String::format(L"FPS: %.1lf", (1 / delay) * _renderTimes);
		_lastRenderTime = duration;
		_renderTimes = 0;

		auto writeFactory = Renderer::getWriteFactory();
		if (!_fpsFormat)
		{
			writeFactory->CreateTextFormat(
				L"",
				nullptr,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				20,
				L"",
				&_fpsFormat
			);
		}

		if (_fpsFormat)
		{
			_fpsFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

			SafeRelease(_fpsLayout);
			writeFactory->CreateTextLayout(
				(const WCHAR *)fpsText,
				(UINT32)fpsText.getLength(),
				_fpsFormat,
				0,
				0,
				&_fpsLayout
			);
		}
	}

	if (_fpsLayout)
	{
		_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		_solidBrush->SetOpacity(1.0f);

		auto textRenderer = this->getTextRenderer();
		textRenderer->SetTextStyle(
			D2D1::ColorF(D2D1::ColorF::White),
			TRUE,
			D2D1::ColorF(D2D1::ColorF::Black, 0.4f),
			1.5f,
			D2D1_LINE_JOIN_ROUND
		);

		_fpsLayout->Draw(nullptr, textRenderer, 10, 0);
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
		HRESULT hr = TextRenderer::Create(
			&_textRenderer,
			Renderer::getFactory(),
			this->getRenderTarget(),
			this->getSolidColorBrush()
		);

		if (FAILED(hr))
		{
			throw SystemException(L"Create TextRenderer failed");
		}
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
