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
	_duration = max(duration, 0);
}

e2d::Transition::~Transition()
{
	SafeReleaseInterface(_outLayer);
	SafeReleaseInterface(_inLayer);
}

bool e2d::Transition::isDone()
{
	return _end;
}

void e2d::Transition::onDestroy()
{
	SafeRelease(_outScene);
	SafeRelease(_inScene);
}

void e2d::Transition::_init(Scene * prev, Scene * next)
{
	// 创建图层
	HRESULT hr = Renderer::getRenderTarget()->CreateLayer(&_inLayer);

	if (SUCCEEDED(hr))
	{
		hr = Renderer::getRenderTarget()->CreateLayer(&_outLayer);
	}

	if (FAILED(hr))
	{
		ASSERT(false, "Create layer failed!");
	}

	_last = Time::getTotalTime();
	_outScene = prev;
	_inScene = next;
	if (_outScene) _outScene->retain();
	if (_inScene) _inScene->retain();

	_windowSize = Window::getSize();
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
		_delta = min((Time::getTotalTime() - _last) / _duration, 1);
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
	auto pRT = Renderer::getRenderTarget();

	if (_outScene)
	{
		Point rootPos = _outScene->getRoot()->getPos();
		auto clipRect = D2D1::RectF(
			float(max(rootPos.x, 0)),
			float(max(rootPos.y, 0)),
			float(min(rootPos.x + _windowSize.width, _windowSize.width)),
			float(min(rootPos.y + _windowSize.height, _windowSize.height))
		);
		pRT->SetTransform(D2D1::Matrix3x2F::Identity());
		pRT->PushAxisAlignedClip(clipRect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
		pRT->PushLayer(_outLayerParam, _outLayer);

		// 渲染场景
		_outScene->_render();

		pRT->PopLayer();
		pRT->PopAxisAlignedClip();
	}

	if (_inScene)
	{
		Point rootPos = _inScene->getRoot()->getPos();
		auto clipRect = D2D1::RectF(
			float(max(rootPos.x, 0)),
			float(max(rootPos.y, 0)),
			float(min(rootPos.x + _windowSize.width, _windowSize.width)),
			float(min(rootPos.y + _windowSize.height, _windowSize.height))
		);
		pRT->SetTransform(D2D1::Matrix3x2F::Identity());
		pRT->PushAxisAlignedClip(clipRect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
		pRT->PushLayer(_inLayerParam, _inLayer);

		// 渲染场景
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
