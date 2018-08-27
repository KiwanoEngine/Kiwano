#include "..\e2dbase.h"
#include "..\e2dmanager.h"
#include "..\e2dnode.h"


e2d::Renderer*		e2d::Renderer::_instance = nullptr;
ID2D1Factory*		e2d::Renderer::_factory = nullptr;
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
		SafeRelease(_factory);
		SafeRelease(_imagingFactory);
		SafeRelease(_writeFactory);
	}
}

e2d::Renderer::Renderer()
	: _showFps(false)
	, _lastRenderTime(Time::now())
	, _renderTimes(0)
	, _fpsFormat(nullptr)
	, _fpsLayout(nullptr)
	, _renderTarget(nullptr)
	, _solidBrush(nullptr)
	, _textRenderer(nullptr)
	, _clearColor(D2D1::ColorF(D2D1::ColorF::Black))
{
	CoInitialize(nullptr);

	HWND hWnd = Window::getInstance()->getHWnd();

	RECT rc;
	GetClientRect(hWnd, &rc);

	D2D1_SIZE_U size = D2D1::SizeU(
		rc.right - rc.left,
		rc.bottom - rc.top
	);

	// 创建设备相关资源。这些资源应在 Direct2D 设备消失时重建
	// 创建一个 Direct2D 渲染目标
	ThrowIfFailed(
		getFactory()->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(
				hWnd,
				size,
				D2D1_PRESENT_OPTIONS_NONE),
			&_renderTarget
		)
	);

	// 创建画刷
	ThrowIfFailed(
		_renderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::White),
			&_solidBrush
		)
	);

	// 创建自定义的文字渲染器
	ThrowIfFailed(
		TextRenderer::Create(
			&_textRenderer,
			getFactory(),
			_renderTarget,
			_solidBrush
		)
	);
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

void e2d::Renderer::beginDraw()
{
	// 开始渲染
	_renderTarget->BeginDraw();
	// 使用背景色清空屏幕
	_renderTarget->Clear(_clearColor);
}

void e2d::Renderer::endDraw()
{
	if (_showFps)
	{
		int duration = (Time::now() - _lastRenderTime).milliseconds();

		++_renderTimes;
		if (duration >= 100)
		{
			String fpsText = String::format(L"FPS: %.1f", (1000.f / duration * _renderTimes));
			_lastRenderTime = Time::now();
			_renderTimes = 0;

			if (!_fpsFormat)
			{
				ThrowIfFailed(
					getWriteFactory()->CreateTextFormat(
						L"",
						nullptr,
						DWRITE_FONT_WEIGHT_NORMAL,
						DWRITE_FONT_STYLE_NORMAL,
						DWRITE_FONT_STRETCH_NORMAL,
						20,
						L"",
						&_fpsFormat
					)
				);

				ThrowIfFailed(
					_fpsFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP)
				);
			}

			SafeRelease(_fpsLayout);

			ThrowIfFailed(
				getWriteFactory()->CreateTextLayout(
				(const WCHAR *)fpsText,
					(UINT32)fpsText.length(),
					_fpsFormat,
					0,
					0,
					&_fpsLayout
				)
			);
		}

		if (_fpsLayout)
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

			ThrowIfFailed(
				_fpsLayout->Draw(nullptr, _textRenderer, 10, 0)
			);
		}
	}

	// 终止渲染
	HRESULT hr = _renderTarget->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
	{
		// 如果 Direct3D 设备在执行过程中消失，将丢弃当前的设备相关资源
		// 并在下一次调用时重建资源
		hr = S_OK;

		SafeRelease(_fpsFormat);
		SafeRelease(_fpsLayout);
		SafeRelease(_textRenderer);
		SafeRelease(_solidBrush);
		SafeRelease(_renderTarget);
	}

	if (FAILED(hr))
	{
		throw SystemException("Device loss recovery failed");
	}
}

e2d::Color e2d::Renderer::getBackgroundColor()
{
	return _clearColor;
}

void e2d::Renderer::setBackgroundColor(Color color)
{
	_clearColor = (D2D1_COLOR_F)color;
}

void e2d::Renderer::showFps(bool show)
{
	_showFps = show;
}

ID2D1Factory * e2d::Renderer::getFactory()
{
	if (!_factory)
	{
		ThrowIfFailed(
			D2D1CreateFactory(
				D2D1_FACTORY_TYPE_SINGLE_THREADED,
				&_factory
			)
		);
	}
	return _factory;
}

IWICImagingFactory * e2d::Renderer::getImagingFactory()
{
	if (!_imagingFactory)
	{
		ThrowIfFailed(
			CoCreateInstance(
				CLSID_WICImagingFactory,
				nullptr,
				CLSCTX_INPROC_SERVER,
				IID_IWICImagingFactory,
				reinterpret_cast<void**>(&_imagingFactory)
			)
		);
	}
	return _imagingFactory;
}

IDWriteFactory * e2d::Renderer::getWriteFactory()
{
	if (!_writeFactory)
	{
		ThrowIfFailed(
			DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory),
				reinterpret_cast<IUnknown**>(&_writeFactory)
			)
		);
	}
	return _writeFactory;
}

ID2D1StrokeStyle * e2d::Renderer::getMiterStrokeStyle()
{
	if (!_miterStrokeStyle)
	{
		ThrowIfFailed(
			getFactory()->CreateStrokeStyle(
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
			)
		);
	}
	return _miterStrokeStyle;
}

ID2D1StrokeStyle * e2d::Renderer::getBevelStrokeStyle()
{
	if (!_bevelStrokeStyle)
	{
		ThrowIfFailed(
			getFactory()->CreateStrokeStyle(
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
			)
		);
	}
	return _bevelStrokeStyle;
}

ID2D1StrokeStyle * e2d::Renderer::getRoundStrokeStyle()
{
	if (!_roundStrokeStyle)
	{
		ThrowIfFailed(
			getFactory()->CreateStrokeStyle(
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
			)
		);
	}
	return _roundStrokeStyle;
}
