#include "..\e2dbase.h"
#include "..\e2dtransition.h"
#include "..\e2dnode.h"

e2d::Transition::Transition(double duration)
	: _end(false)
	, _last(0)
	, _delta(0)
	, _outScene(nullptr)
	, _inScene(nullptr)
	, _outLayer(nullptr)
	, _inLayer(nullptr)
	, _outLayerParam()
	, _inLayerParam()
{
	_duration = std::max(duration, 0.0);
}

e2d::Transition::~Transition()
{
	SafeRelease(_outLayer);
	SafeRelease(_inLayer);
}

bool e2d::Transition::isDone()
{
	return _end;
}

void e2d::Transition::onDestroy()
{
	GC::safeRelease(_outScene);
	GC::safeRelease(_inScene);
}

void e2d::Transition::_init(Scene * prev, Scene * next)
{
	auto renderer = Renderer::getInstance();
	// 创建图层
	HRESULT hr = renderer->getRenderTarget()->CreateLayer(&_inLayer);

	if (SUCCEEDED(hr))
	{
		hr = renderer->getRenderTarget()->CreateLayer(&_outLayer);
	}

	if (FAILED(hr))
	{
		throw SystemException(L"场景过渡动画图层创建失败");
	}

	_last = Time::getTotalTime();
	_outScene = prev;
	_inScene = next;
	if (_outScene) _outScene->retain();
	if (_inScene) _inScene->retain();

	_windowSize = Window::getInstance()->getSize();
	_outLayerParam = _inLayerParam = D2D1::LayerParameters();
}

void e2d::Transition::_update()
{
	// 计算动作进度
	if (_duration == 0)
	{
		_delta = 1;
	}
	else
	{
		_delta = std::min((Time::getTotalTime() - _last) / _duration, 1.0);
	}

	this->_updateCustom();

	// 更新场景内容
	if (_outScene)
	{
		_outScene->_update();
	}
	if (_inScene)
	{
		_inScene->_update();
	}
}

void e2d::Transition::_render()
{
	auto pRT = Renderer::getInstance()->getRenderTarget();

	if (_outScene)
	{
		Point rootPos = _outScene->getRoot()->getPos();
		auto clipRect = D2D1::RectF(
			float(std::max(rootPos.x, 0.0)),
			float(std::max(rootPos.y, 0.0)),
			float(std::min(rootPos.x + _windowSize.width, _windowSize.width)),
			float(std::min(rootPos.y + _windowSize.height, _windowSize.height))
		);
		pRT->SetTransform(D2D1::Matrix3x2F::Identity());
		pRT->PushAxisAlignedClip(clipRect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
		pRT->PushLayer(_outLayerParam, _outLayer);

		_outScene->_render();

		pRT->PopLayer();
		pRT->PopAxisAlignedClip();
	}

	if (_inScene)
	{
		Point rootPos = _inScene->getRoot()->getPos();
		auto clipRect = D2D1::RectF(
			float(std::max(rootPos.x, 0.0)),
			float(std::max(rootPos.y, 0.0)),
			float(std::min(rootPos.x + _windowSize.width, _windowSize.width)),
			float(std::min(rootPos.y + _windowSize.height, _windowSize.height))
		);
		pRT->SetTransform(D2D1::Matrix3x2F::Identity());
		pRT->PushAxisAlignedClip(clipRect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
		pRT->PushLayer(_inLayerParam, _inLayer);

		_inScene->_render();

		pRT->PopLayer();
		pRT->PopAxisAlignedClip();
	}
}

void e2d::Transition::_stop()
{
	_end = true;
	_reset();
}
