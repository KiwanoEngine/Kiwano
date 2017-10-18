#include "..\enodes.h"
#include "..\Win\winbase.h"


e2d::ESprite::ESprite()
	: m_fSourcePosX(0)
	, m_fSourcePosY(0)
	, m_pTexture(nullptr)
{
}

e2d::ESprite::ESprite(const EString & imageFileName)
	: ESprite()
{
	setTexture(new ETexture(imageFileName));
}

e2d::ESprite::ESprite(const EString & imageFileName, float x, float y, float width, float height)
{
	setTexture(new ETexture(imageFileName));
	clipTexture(x, y, width, height);
}

e2d::ESprite::ESprite(const EString & resourceName, const EString & resourceType)
	: ESprite()
{
	setTexture(new ETexture(resourceName, resourceType));
}

e2d::ESprite::ESprite(const EString & resourceName, const EString & resourceType, float x, float y, float width, float height)
{
	setTexture(new ETexture(resourceName, resourceType));
	clipTexture(x, y, width, height);
}

e2d::ESprite::~ESprite()
{
	SafeRelease(&m_pTexture);
}

void e2d::ESprite::setTexture(ETexture * texture)
{
	SafeRelease(&m_pTexture);
	m_pTexture = texture;
	m_pTexture->retain();

	m_fSourcePosX = m_fSourcePosY = 0;
	ENode::_setWidth(m_pTexture->getSourceWidth());
	ENode::_setHeight(m_pTexture->getSourceHeight());
}

void e2d::ESprite::setTexture(ETexture * texture, float x, float y, float width, float height)
{
	setTexture(texture);
	clipTexture(x, y, width, height);
}

void e2d::ESprite::clipTexture(float x, float y, float width, float height)
{
	m_fSourcePosX = max(x, 0);
	m_fSourcePosY = max(y, 0);
	ENode::_setWidth(min(max(width, 0), m_pTexture->getSourceWidth() - m_fSourcePosX));
	ENode::_setHeight(min(max(height, 0), m_pTexture->getSourceHeight() - m_fSourcePosX));
}

void e2d::ESprite::_onRender()
{
	if (m_pTexture && m_pTexture->_getBitmap())
	{
		// Draw bitmap
		GetRenderTarget()->DrawBitmap(
			m_pTexture->_getBitmap(),
			D2D1::RectF(0, 0, getRealWidth(), getRealHeight()),
			m_fDisplayOpacity,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			D2D1::RectF(m_fSourcePosX, m_fSourcePosY, getRealWidth(), getRealHeight())
		);
	}
}
