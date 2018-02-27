#include "..\eshape.h"
#include "..\emanagers.h"
#include "..\enodes.h"

e2d::Shape::Shape()
	: m_nCategoryBitmask(0)
	, m_nCollisionBitmask(0)
	, m_bIsVisiable(true)
	, m_nColor(Color::RED)
	, m_fOpacity(1)
	, m_pParentNode(nullptr)
	, m_pTransformedShape(nullptr)
	, m_bEnable(true)
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

UINT32 e2d::Shape::getCategoryBitmask() const
{
	return m_nCategoryBitmask;
}

UINT32 e2d::Shape::getCollisionBitmask() const
{
	return m_nCollisionBitmask;
}

void e2d::Shape::setCategoryBitmask(UINT32 mask)
{
	m_nCategoryBitmask = mask;
}

void e2d::Shape::setCollisionBitmask(UINT32 mask)
{
	m_nCollisionBitmask = mask;
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
		// 释放原形状
		SafeReleaseInterface(&m_pTransformedShape);

		// 根据父节点转换几何图形
		Renderer::getID2D1Factory()->CreateTransformedGeometry(
			_getD2dGeometry(),
			m_pParentNode->m_MatriFinal,
			&m_pTransformedShape
		);

		ShapeManager::__updateShape(this);
	}
}
