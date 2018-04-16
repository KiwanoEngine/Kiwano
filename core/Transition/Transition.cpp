#include "..\ebase.h"
#include "..\etransition.h"

e2d::Transition::Transition(double duration)
	: m_bEnd(false)
	, m_fLast(0)
	, m_fRateOfProgress(0)
	, m_pPrevScene(nullptr)
	, m_pNextScene(nullptr)
	, m_pPrevLayer(nullptr)
	, m_pNextLayer(nullptr)
	, m_sPrevLayerParam()
	, m_sNextLayerParam()
{
	m_fDuration = max(duration, 0);
}

e2d::Transition::~Transition()
{
	SafeRelease(&m_pPrevScene);
	SafeRelease(&m_pNextScene);
	SafeReleaseInterface(&m_pPrevLayer);
	SafeReleaseInterface(&m_pNextLayer);
}

bool e2d::Transition::isEnding()
{
	return m_bEnd;
}

void e2d::Transition::_init(Scene * prev, Scene * next)
{
	// 创建图层
	HRESULT hr = Renderer::getRenderTarget()->CreateLayer(&m_pNextLayer);

	if (SUCCEEDED(hr))
	{
		hr = Renderer::getRenderTarget()->CreateLayer(&m_pPrevLayer);
	}

	if (FAILED(hr))
	{
		ASSERT(false, L"Create layer failed!");
	}

	m_fLast = Time::getTotalTime();
	m_pPrevScene = prev;
	m_pNextScene = next;
	if (m_pPrevScene) m_pPrevScene->retain();
	if (m_pNextScene) m_pNextScene->retain();

	auto size = Window::getSize();
	m_sPrevLayerParam = m_sNextLayerParam = D2D1::LayerParameters(
		D2D1::RectF(0, 0, float(size.width), float(size.height))
	);
}

void e2d::Transition::_update()
{
	// 计算动画进度
	if (m_fDuration == 0)
	{
		m_fRateOfProgress = 1;
	}
	else
	{
		m_fRateOfProgress = min((Time::getTotalTime() - m_fLast) / m_fDuration, 1);
	}

	this->_updateCustom();

	// 更新场景内容
	if (m_pPrevScene)
	{
		m_pPrevScene->_update();
	}
	if (m_pNextScene)
	{
		m_pNextScene->_update();
	}
}

void e2d::Transition::_render()
{
	auto pRT = Renderer::getRenderTarget();

	if (m_pPrevScene)
	{
		pRT->SetTransform(D2D1::Matrix3x2F::Identity());
		pRT->PushLayer(m_sPrevLayerParam, m_pPrevLayer);

		// 渲染场景
		m_pPrevScene->_render();

		pRT->PopLayer();
	}

	if (m_pNextScene)
	{
		pRT->SetTransform(D2D1::Matrix3x2F::Identity());
		pRT->PushLayer(m_sNextLayerParam, m_pNextLayer);

		// 渲染场景
		m_pNextScene->_render();

		pRT->PopLayer();
	}
}

void e2d::Transition::_stop()
{
	m_bEnd = true;
	_reset();
}
