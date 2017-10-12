#include "..\ebase.h"
#include "..\enodes.h"

e2d::EScene::EScene()
{
	EApp::get()->setLoadingScene(this);
}

e2d::EScene::~EScene()
{
	clearAllChildren();
}

void e2d::EScene::_exec()
{
	// active 标志画面是否取得焦点
	bool active = true;
	// 逆序执行，最后绘制的节点（即位于画面最上方）最先被访问
	for (int i = int(m_vChildren.size()) - 1; i >= 0; i--)
	{
		if (m_vChildren[i]->_exec(active))	// 执行节点程序
		{
			active = false;					// 若子节点取得焦点，将标志置 false
		}
	}
}

void e2d::EScene::_onDraw()
{
	// 绘制所有节点
	for (auto child : m_vChildren)
	{
		child->_onDraw();
	}
}

void e2d::EScene::init()
{
}

void e2d::EScene::onEnter()
{
}

void e2d::EScene::onExit()
{
}

void e2d::EScene::add(ENode * child, int zOrder)
{
	// 断言添加的节点非空
	ASSERT(child != nullptr);
	// 忽略空指针
	if (child == nullptr) return;
	// 设置节点的父场景
	child->setParentScene(this);
	// 设置 z 轴顺序
	child->setZOrder(zOrder);
	// 对象的引用计数加一
	child->retain();

	// 按 z 轴顺序插入节点
	size_t size = m_vChildren.size();
	for (unsigned i = 0; i <= size; i++)
	{
		if (i == size)
		{
			m_vChildren.push_back(child);
			break;
		}
		else
		{
			auto temp = m_vChildren.at(i);
			if (temp->getZOrder() > zOrder)
			{
				m_vChildren.insert(m_vChildren.begin() + i, child);
				break;
			}
		}
	}
}

bool e2d::EScene::del(ENode * child)
{
	if (child == nullptr) return false;

	// 寻找是否有相同节点
	std::vector<ENode*>::iterator iter;
	for (iter = m_vChildren.begin(); iter != m_vChildren.end(); iter++)
	{
		// 找到相同节点
		if (*iter == child)
		{
			// 对象的引用计数减一
			(*iter)->release();
			// 去掉该节点
			m_vChildren.erase(iter);
			return true;
		}
	}
	// 未找到该节点返回 false
	return false;
}

std::vector<e2d::ENode*>& e2d::EScene::getChildren()
{
	return m_vChildren;
}

void e2d::EScene::clearAllChildren()
{
	// 所有节点的引用计数减一
	for (auto child : m_vChildren)
	{
		child->autoRelease();
		child->release();
	}
	// 清空储存节点的容器
	m_vChildren.clear();
}