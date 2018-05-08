#include "..\e2dbase.h"
#include "..\e2dtransition.h"
#include "..\e2dnode.h"

e2d::TransitionBase::TransitionBase(double duration)
	: _bEnd(false)
	, _fLast(0)
	, _delta(0)
	, _pPrevScene(nullptr)
	, _pNextScene(nullptr)
	, _pPrevLayer(nullptr)
	, _pNextLayer(nullptr)
	, _sPrevLayerParam()
	, _sNextLayerParam()
{
	_duration = max(duration, 0);
}

e2d::TransitionBase::~TransitionBase()
{
	SafeReleaseInterface(&_pPrevLayer);
	SafeReleaseInterface(&_pNextLayer);
}

bool e2d::TransitionBase::isDone()
{
	return _bEnd;
}

void e2d::TransitionBase::onDestroy()
{
	SafeRelease(&_pPrevScene);
	SafeRelease(&_pNextScene);
}

void e2d::TransitionBase::_init(Scene * prev, Scene * next)
{
	// 创建图层
	HRESULT hr = Renderer::getRenderTarget()->CreateLayer(&_pNextLayer);

	if (SUCCEEDED(hr))
	{
		hr = Renderer::getRenderTarget()->CreateLayer(&_pPrevLayer);
	}

	if (FAILED(hr))
	{
		ASSERT(false, "Create layer failed!");
	}

	_fLast = Time::getTotalTime();
	_pPrevScene = prev;
	_pNextScene = next;
	if (_pPrevScene) _pPrevScene->retain();
	if (_pNextScene) _pNextScene->retain();

	_WindowSize = Window::getSize();
	_sPrevLayerParam = _sNextLayerParam = D2D1::LayerParameters();
}

void e2d::TransitionBase::_update()
{
	// 计算动画进度
	if (_duration == 0)
	{
		_delta = 1;
	}
	else
	{
		_delta = min((Time::getTotalTime() - _fLast) / _duration, 1);
	}

	this->_updateCustom();

	// 更新场景内容
	if (_pPrevScene)
	{
		_pPrevScene->_update();
	}
	if (_pNextScene)
	{
		_pNextScene->_update();
	}
}

void e2d::TransitionBase::_render()
{
	auto pRT = Renderer::getRenderTarget();

	if (_pPrevScene)
	{
		Point rootPos = _pPrevScene->getRoot()->getPos();
		auto clipRect = D2D1::RectF(
			float(max(rootPos.x, 0)),
			float(max(rootPos.y, 0)),
			float(min(rootPos.x + _WindowSize.width, _WindowSize.width)),
			float(min(rootPos.y + _WindowSize.height, _WindowSize.height))
		);
		pRT->SetTransform(D2D1::Matrix3x2F::Identity());
		pRT->PushAxisAlignedClip(clipRect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
		pRT->PushLayer(_sPrevLayerParam, _pPrevLayer);

		// 渲染场景
		_pPrevScene->_render();

		pRT->PopLayer();
		pRT->PopAxisAlignedClip();
	}

	if (_pNextScene)
	{
		Point rootPos = _pNextScene->getRoot()->getPos();
		auto clipRect = D2D1::RectF(
			float(max(rootPos.x, 0)),
			float(max(rootPos.y, 0)),
			float(min(rootPos.x + _WindowSize.width, _WindowSize.width)),
			float(min(rootPos.y + _WindowSize.height, _WindowSize.height))
		);
		pRT->SetTransform(D2D1::Matrix3x2F::Identity());
		pRT->PushAxisAlignedClip(clipRect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
		pRT->PushLayer(_sNextLayerParam, _pNextLayer);

		// 渲染场景
		_pNextScene->_render();

		pRT->PopLayer();
		pRT->PopAxisAlignedClip();
	}
}

void e2d::TransitionBase::_stop()
{
	_bEnd = true;
	_reset();
}
