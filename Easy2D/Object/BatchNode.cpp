#include "..\easy2d.h"
#include "..\EasyX\easyx.h"

BatchNode::BatchNode()
{
}


BatchNode::~BatchNode()
{
	clearAllChildren();
}

bool BatchNode::_exec(bool active)
{
	// 批量节点是否显示
	if (!m_bDisplay)
	{
		return false;
	}
	// 逆序遍历所有子节点
	for (int i = int(m_vChildren.size() - 1); i >= 0; i--)
	{
		if (m_vChildren[i]->_exec(active))
		{
			active = false;
		}
	}
	// 若子节点取得了画面焦点，则该节点也取得了焦点
	return !active;
}

void BatchNode::_onDraw()
{
	// 节点是否显示
	if (!m_bDisplay)
	{
		return;
	}
	
	// 在相对位置绘制子节点
	App::setOrigin(App::getOriginX() + getX(), App::getOriginY() + getY());
	for (auto child : m_vChildren)
	{
		child->_onDraw();
	}
	App::setOrigin(App::getOriginX() - getX(), App::getOriginY() - getY());
}

void BatchNode::add(Node * child, int z_Order)
{
	if (child == nullptr) return;

	// 设置节点的父场景
	child->setParentScene(this->getParentScene());
	// 设置节点在批量节点中的 z 轴顺序
	child->setZOrder(z_Order);
	// 对象的引用计数加一
	child->retain();

	// 按 z 轴顺序插入节点
	size_t size = m_vChildren.size();
	for (unsigned i = 0; i <= size; i++)
	{
		if (i != size)
		{
			if (z_Order < m_vChildren.at(i)->getZOrder())
			{
				m_vChildren.insert(m_vChildren.begin() + i, child);
				break;
			}
		}
		else
		{
			m_vChildren.push_back(child);
			break;
		}
	}
}

bool BatchNode::del(Node * child)
{
	if (child == nullptr) return false;

	// 寻找是否有相同节点
	std::vector<Node*>::iterator iter;
	for (iter = m_vChildren.begin(); iter != m_vChildren.end(); iter++)
	{
		// 找到相同节点
		if ((*iter) == child)
		{
			// 对象的引用计数减一
			(*iter)->release();
			// 去掉该节点
			m_vChildren.erase(iter);
			return true;
		}
	}
	return false;
}

void BatchNode::clearAllChildren()
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
