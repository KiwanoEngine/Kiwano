#include "..\e2dcollider.h"
#include "..\e2dmanager.h"
#include "..\e2dnode.h"

e2d::Collider::Collider()
	: _visiable(true)
	, _color(Color::Red, 0.7)
	, _parentNode(nullptr)
	, _transformed(nullptr)
	, _enabled(true)
	, _autoResize(false)
{
}

e2d::Collider::~Collider()
{
	SafeRelease(_transformed);
}

e2d::Node * e2d::Collider::getParentNode() const
{
	return _parentNode;
}

e2d::Color e2d::Collider::getColor() const
{
	return _color;
}

void e2d::Collider::setEnabled(bool enabled)
{
	_enabled = enabled;
}

void e2d::Collider::setVisiable(bool bVisiable)
{
	_visiable = bVisiable;
}

void e2d::Collider::setColor(Color color)
{
	_color = color;
}

void e2d::Collider::setAutoResize(bool enabled)
{
	_autoResize = enabled;
}

void e2d::Collider::_render()
{
	if (_transformed && _enabled)
	{
		auto renderer = Renderer::getInstance();
		// 获取纯色画刷
		ID2D1SolidColorBrush * pBrush = renderer->getSolidColorBrush();
		// 设置画刷颜色和透明度
		pBrush->SetColor(_color.toD2DColorF());
		// 绘制几何碰撞体
		renderer->getRenderTarget()->DrawGeometry(_transformed, pBrush);
	}
}

e2d::Collider::Relation e2d::Collider::getRelationWith(Collider * pCollider) const
{
	if (_transformed && pCollider->_transformed)
	{
		if (_enabled && pCollider->_enabled)
		{
			D2D1_GEOMETRY_RELATION relation;

			_transformed->CompareWithGeometry(
				pCollider->_transformed,
				D2D1::Matrix3x2F::Identity(),
				&relation
			);

			return Relation(relation);
		}
	}
	return Relation::Unknown;
}

void e2d::Collider::_transform()
{
	if (_parentNode && _enabled)
	{
		if (_autoResize)
		{
			this->_resize();
		}

		// 释放原碰撞体
		SafeRelease(_transformed);

		// 根据父节点转换几何图形
		Renderer::getFactory()->CreateTransformedGeometry(
			getD2dGeometry(),
			_parentNode->_finalMatri,
			&_transformed
		);

		ColliderManager::__updateCollider(this);
	}
}
