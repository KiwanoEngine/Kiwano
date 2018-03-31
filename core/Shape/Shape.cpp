#include "..\eshape.h"
#include "..\emanagers.h"
#include "..\enodes.h"

e2d::Shape::Shape()
	: m_bIsVisiable(true)
	, m_nColor(Color::RED)
	, m_fOpacity(1)
	, m_pParentNode(nullptr)
	, m_pTransformedShape(nullptr)
	, m_bEnable(true)
	, m_bAutoResize(true)
{
}

e2d::Shape::~Shape()
{
	SafeReleaseInterface(&m_pTransformedShape);
}

e2d::Node * e2d::Shape::getParentNode() const
{
	return m_pParentNode;
}

void e2d::Shape::setEnable(bool bEnable)
{
	m_bEnable = bEnable;
}

void e2d::Shape::setVisiable(bool bVisiable)
{
	m_bIsVisiable = bVisiable;
}

void e2d::Shape::setColor(UINT32 color)
{
	m_nColor = color;
}

void e2d::Shape::setOpacity(double opacity)
{
	m_fOpacity = min(max(static_cast<float>(opacity), 0), 1);
}

void e2d::Shape::setAutoResize(bool bEnable)
{
	m_bAutoResize = bEnable;
}

void e2d::Shape::_render()
{
	if (m_pTransformedShape && m_bEnable)
	{
		ID2D1SolidColorBrush * pBrush = Renderer::getSolidColorBrush();
		// 创建画刷
		Renderer::getRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(
				m_nColor,
				m_fOpacity),
			&pBrush
		);
		// 绘制几何形状
		Renderer::getRenderTarget()->DrawGeometry(m_pTransformedShape, pBrush);
	}
}

int e2d::Shape::getRelationWith(Shape * pShape) const
{
	if (m_pTransformedShape && pShape->m_pTransformedShape)
	{
		if (m_bEnable && pShape->m_bEnable)
		{
			D2D1_GEOMETRY_RELATION relation;

			m_pTransformedShape->CompareWithGeometry(
				pShape->m_pTransformedShape,
				D2D1::Matrix3x2F::Identity(),
				&relation
			);

			return relation;
		}
	}
	return 0;
}

void e2d::Shape::_transform()
{
	if (m_pParentNode && m_bEnable)
	{
		if (m_bAutoResize)
		{
			this->_resize();
		}

		// 释放原形状
		SafeReleaseInterface(&m_pTransformedShape);

		// 根据父节点转换几何图形
		Renderer::getID2D1Factory()->CreateTransformedGeometry(
			getD2dGeometry(),
			m_pParentNode->m_MatriFinal,
			&m_pTransformedShape
		);

		CollisionManager::__updateShape(this);
	}
}
