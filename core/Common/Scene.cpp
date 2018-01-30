#include "..\ebase.h"
#include "..\enodes.h"
#include "..\emanagers.h"
#include "..\etools.h"
#include "..\eactions.h"
#include <algorithm>

e2d::EScene::EScene()
	: m_bWillSave(true)
	, m_bSortNeeded(false)
	, m_bGeometryVisiable(false)
	, m_pRoot(new ENode())
{
	m_pRoot->retain();
	m_pRoot->_onEnter();
	m_pRoot->_setParentScene(this);
	m_pRoot->setPivot(0, 0);
	m_pRoot->_setSize(EWindow::getWidth(), EWindow::getHeight());
}

e2d::EScene::~EScene()
{
	SafeRelease(&m_pRoot);
}

void e2d::EScene::_render()
{
	m_pRoot->_render();

	if (m_bGeometryVisiable)
	{
		// 恢复矩阵转换
		ERenderer::getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
		// 绘制所有几何图形
		m_pRoot->_drawGeometry();
	}
}

void e2d::EScene::_update()
{
	// 执行 onUpdate 函数
	this->onUpdate();
	// 更新根节点
	m_pRoot->_update();
}

void e2d::EScene::add(ENode * child, int order /* = 0 */)
{
	m_pRoot->addChild(child, order);
}

bool e2d::EScene::remove(ENode * child)
{
	return m_pRoot->removeChild(child);
}

void e2d::EScene::remove(const EString &childName)
{
	return m_pRoot->removeChild(childName);
}

std::vector<e2d::ENode*> e2d::EScene::getChildren()
{
	return m_pRoot->m_vChildren;
}

size_t e2d::EScene::getChildrenCount() const
{
	return m_pRoot->getChildrenCount();
}

e2d::ENode * e2d::EScene::getChild(const EString &childName)
{
	return m_pRoot->getChild(childName);
}

e2d::ENode * e2d::EScene::getRoot() const
{
	return m_pRoot;
}

void e2d::EScene::clearAllChildren()
{
	m_pRoot->clearAllChildren();
}

void e2d::EScene::runAction(EAction * action)
{
	this->m_pRoot->runAction(action);
}

void e2d::EScene::setGeometryVisiable(bool visiable)
{
	m_bGeometryVisiable = visiable;
}
