#include "..\eshape.h"
#include "..\emanagers.h"
#include "..\enodes.h"

e2d::ShapeBase::ShapeBase()
	: m_bIsVisiable(true)
	, m_nColor(Color::RED)
	, m_fOpacity(1)
	, m_pParentNode(nullptr)
	, m_pTransformedShape(nullptr)
	, m_bEnable(true)
	, m_bAutoResize(true)
{
}

e2d::ShapeBase::~ShapeBase()
{
	SafeReleaseInterface(&m_pTransformedShape);
}

e2d::Node * e2d::ShapeBase::getParentNode() const
{
	return m_pParentNode;
}

void e2d::ShapeBase::setEnable(bool bEnable)
{
	m_bEnable = bEnable;
}

void e2d::ShapeBase::setVisiable(bool bVisiable)
{
	m_bIsVisiable = bVisiable;
}

void e2d::ShapeBase::setColor(UINT32 color)
{
	m_nColor = color;
}

void e2d::ShapeBase::setOpacity(double opacity)
{
	m_fOpacity = min(max(static_cast<float>(opacity), 0), 1);
}

void e2d::ShapeBase::setAutoResize(bool bEnable)
{
	m_bAutoResize = bEnable;
}

void e2d::ShapeBase::_render()
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

e2d::Relation e2d::ShapeBase::getRelationWith(ShapeBase * pShape) const
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

			return Relation(relation);
		}
	}
	return Relation::UNKNOWN;
}

void e2d::ShapeBase::_transform()
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
