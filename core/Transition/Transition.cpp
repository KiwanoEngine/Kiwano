#include "..\e2dbase.h"
#include "..\e2dtransition.h"
#include "..\e2dnode.h"

e2d::Transition::Transition(Scene* scene, float duration)
	: _end(false)
	, _started()
	, _delta(0)
	, _outScene(nullptr)
	, _inScene(scene)
	, _outLayer(nullptr)
	, _inLayer(nullptr)
	, _outLayerParam()
	, _inLayerParam()
{
	_duration = std::max(duration, 0.f);
	if (_inScene)
		_inScene->retain();
}

e2d::Transition::~Transition()
{
	SafeRelease(_outLayer);
	SafeRelease(_inLayer);
	GC::instance()->safeRelease(_outScene);
	GC::instance()->safeRelease(_inScene);
}

bool e2d::Transition::done()
{
	return _end;
}

bool e2d::Transition::_init(Game * game, Scene * prev)
{
	_started = Time::now();
	_outScene = prev;

	if (_outScene)
		_outScene->retain();
	
	HRESULT hr = S_OK;
	auto renderer = game->renderer();
	if (_inScene)
	{
		hr = renderer->renderTarget()->CreateLayer(&_inLayer);
	}

	if (SUCCEEDED(hr) && _outScene)
	{
		hr = renderer->renderTarget()->CreateLayer(&_outLayer);
	}

	if (FAILED(hr))
	{
		return false;
	}

	_windowSize = game->window()->size();
	_outLayerParam = _inLayerParam = D2D1::LayerParameters(
		D2D1::InfiniteRect(),
		nullptr,
		D2D1_ANTIALIAS_MODE_PER_PRIMITIVE,
		D2D1::Matrix3x2F::Identity(),
		1.f,
		renderer->solidBrush(),
		D2D1_LAYER_OPTIONS_NONE
	);

	return true;
}

void e2d::Transition::_update()
{
	if (_duration == 0)
	{
		_delta = 1;
	}
	else
	{
		_delta = (Time::now() - _started).seconds() / _duration;
		_delta = std::min(_delta, 1.f);
	}
}

void e2d::Transition::_render(Game * game)
{
	auto renderTarget = game->renderer()->renderTarget();

	if (_outScene)
	{
		Point rootPos = _outScene->position();
		auto clipRect = D2D1::RectF(
			std::max(rootPos.x, 0.f),
			std::max(rootPos.y, 0.f),
			std::min(rootPos.x + _windowSize.width, _windowSize.width),
			std::min(rootPos.y + _windowSize.height, _windowSize.height)
		);
		renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		renderTarget->PushAxisAlignedClip(clipRect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
		renderTarget->PushLayer(_outLayerParam, _outLayer);

		_outScene->visit(game);

		renderTarget->PopLayer();
		renderTarget->PopAxisAlignedClip();
	}

	if (_inScene)
	{
		Point rootPos = _inScene->position();
		auto clipRect = D2D1::RectF(
			std::max(rootPos.x, 0.f),
			std::max(rootPos.y, 0.f),
			std::min(rootPos.x + _windowSize.width, _windowSize.width),
			std::min(rootPos.y + _windowSize.height, _windowSize.height)
		);
		renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		renderTarget->PushAxisAlignedClip(clipRect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
		renderTarget->PushLayer(_inLayerParam, _inLayer);

		_inScene->visit(game);

		renderTarget->PopLayer();
		renderTarget->PopAxisAlignedClip();
	}
}

void e2d::Transition::_stop()
{
	_end = true;
	_reset();
}
