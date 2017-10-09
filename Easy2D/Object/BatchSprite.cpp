#include "..\easy2d.h"

BatchSprite::BatchSprite()
{
}

BatchSprite::~BatchSprite()
{
}

void BatchSprite::addSprite(Sprite * sprite, int z_Order)
{
	if (sprite == nullptr) return;

	// 设置节点的父场景
	sprite->setParentScene(this->getParentScene());
	// 设置节点在批量节点中的 z 轴顺序
	sprite->setZOrder(z_Order);
	// 对象的引用计数加一
	sprite->retain();

	// 按 z 轴顺序插入节点
	size_t size = m_vSprites.size();
	for (unsigned i = 0; i <= size; i++)
	{
		if (i != size)
		{
			if (z_Order < m_vSprites.at(i)->getZOrder())
			{
				m_vSprites.insert(m_vSprites.begin() + i, sprite);
				break;
			}
		}
		else
		{
			m_vSprites.push_back(sprite);
			break;
		}
	}
}

bool BatchSprite::delSprite(Sprite * sprite)
{
	if (sprite == nullptr) return false;

	// 寻找是否有相同节点
	std::vector<Sprite*>::iterator iter;
	for (iter = m_vSprites.begin(); iter != m_vSprites.end(); iter++)
	{
		// 找到相同节点
		if ((*iter) == sprite)
		{
			// 对象的引用计数减一
			(*iter)->release();
			// 去掉该节点
			m_vSprites.erase(iter);
			return true;
		}
	}
	return false;
}

void BatchSprite::clearAllSprites()
{
	// 所有节点的引用计数减一
	for (auto s : m_vSprites)
	{
		s->release();
	}
	// 清空储存节点的容器
	m_vSprites.clear();
}

bool BatchSprite::_exec(bool active)
{
	// 批量节点是否显示
	if (!m_bDisplay)
	{
		return false;
	}
	// 逆序遍历所有子节点
	for (int i = int(m_vSprites.size() - 1); i >= 0; i--)
	{
		if (m_vSprites[i]->_exec(active))
		{
			active = false;
		}
	}
	// 若子节点取得了画面焦点，则该节点也取得了焦点
	return !active;
}

void BatchSprite::_onDraw()
{
	// 节点是否显示
	if (!m_bDisplay)
	{
		return;
	}

	// 在相对位置绘制子节点
	for (auto sprite : m_vSprites)
	{
		// 将子节点移动到相对位置
		sprite->move(getX(), getY());
		// 绘制子节点
		sprite->_onDraw();
		// 将子节点移回原位
		sprite->move(-getX(), -getY());
	}
}

Sprite * BatchSprite::isCollisionWith(Sprite * sprite)
{
	for (int i = int(m_vSprites.size() - 1); i >= 0; i--)
	{
		if (m_vSprites[i]->isCollisionWith(sprite))
		{
			return m_vSprites[i];
		}
	}
	return nullptr;
}

Sprite * BatchSprite::isPointIn(CPoint point)
{
	for (int i = int(m_vSprites.size() - 1); i >= 0; i--)
	{
		if (m_vSprites[i]->isPointIn(point))
		{
			return m_vSprites[i];
		}
	}
	return nullptr;
}

void BatchSprite::addAction(Action * action)
{
	Sprite::addAction(action);
}

float BatchSprite::getScaleX() const
{
	return m_fScaleX;
}

float BatchSprite::getScaleY() const
{
	return m_fScaleY;
}

float BatchSprite::getOpacity() const
{
	return m_nAlpha / 255.0f;
}

void BatchSprite::setScale(float scaleX, float scaleY)
{
	m_fScaleX = scaleX;
	m_fScaleY = scaleY;
	for (auto s : m_vSprites)
	{
		s->setScale(scaleX, scaleY);
	}
}

void BatchSprite::setOpacity(float opacity)
{
	m_nAlpha = BYTE(min(max(opacity, 0), 1) * 255);
	for (auto s : m_vSprites)
	{
		s->setOpacity(opacity);
	}
}

void BatchSprite::setImage(Image * image)
{
	for (auto s : m_vSprites)
	{
		s->setImage(image);
	}
}

void BatchSprite::setX(int x)
{
	// 计算相对移动位置
	int var = x - getX();
	// 移动所有子节点的位置
	for (auto s : m_vSprites)
	{
		s->move(var, 0);
	}
	RectNode::setX(x);
}

void BatchSprite::setY(int y)
{
	// 计算相对移动位置
	int var = y - getY();
	// 移动所有子节点的位置
	for (auto s : m_vSprites)
	{
		s->move(0, var);
	}
	RectNode::setX(y);
}

void BatchSprite::setPos(int x, int y)
{
	// 计算相对移动位置
	CPoint var(x - getX(), y - getY());
	// 移动所有子节点的位置
	for (auto s : m_vSprites)
	{
		s->move(var);
	}
	RectNode::setPos(x, y);
}

void BatchSprite::setPos(CPoint p)
{
	// 计算相对移动位置
	CPoint var(p - getPos());
	// 移动所有子节点的位置
	for (auto s : m_vSprites)
	{
		s->move(var);
	}
	RectNode::setPos(p);
}

void BatchSprite::move(int x, int y)
{
	// 移动所有子节点的位置
	for (auto s : m_vSprites)
	{
		s->move(x, y);
	}
	RectNode::setPos(x, y);
}

void BatchSprite::move(CVector v)
{
	// 移动所有子节点的位置
	for (auto s : m_vSprites)
	{
		s->move(v);
	}
	RectNode::setPos(v);
}