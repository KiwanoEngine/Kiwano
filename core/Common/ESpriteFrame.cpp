#include "..\ecommon.h"

e2d::ESpriteFrame::ESpriteFrame()
	: m_fSourceClipX(0)
	, m_fSourceClipY(0)
	, m_fSourceClipWidth(0)
	, m_fSourceClipHeight(0)
	, m_pTexture(nullptr)
{
}

e2d::ESpriteFrame::ESpriteFrame(ETexture * texture)
	: m_fSourceClipX(0)
	, m_fSourceClipY(0)
	, m_fSourceClipWidth(0)
	, m_fSourceClipHeight(0)
	, m_pTexture(nullptr)
{
	_setTexture(texture);
}

e2d::ESpriteFrame::ESpriteFrame(const EString & imageFileName)
	: m_fSourceClipX(0)
	, m_fSourceClipY(0)
	, m_fSourceClipWidth(0)
	, m_fSourceClipHeight(0)
	, m_pTexture(nullptr)
{
	_setTexture(new ETexture(imageFileName));
}

e2d::ESpriteFrame::ESpriteFrame(LPCTSTR resourceName, LPCTSTR resourceType)
	: m_fSourceClipX(0)
	, m_fSourceClipY(0)
	, m_fSourceClipWidth(0)
	, m_fSourceClipHeight(0)
	, m_pTexture(nullptr)
{
	_setTexture(new ETexture(resourceName, resourceType));
}

e2d::ESpriteFrame::ESpriteFrame(ETexture * texture, float x, float y, float width, float height)
	: m_fSourceClipX(0)
	, m_fSourceClipY(0)
	, m_fSourceClipWidth(0)
	, m_fSourceClipHeight(0)
	, m_pTexture(nullptr)
{
	_setTexture(texture);
	_clipTexture(x, y, width, height);
}

e2d::ESpriteFrame::ESpriteFrame(const EString & imageFileName, float x, float y, float width, float height)
	: m_fSourceClipX(0)
	, m_fSourceClipY(0)
	, m_fSourceClipWidth(0)
	, m_fSourceClipHeight(0)
	, m_pTexture(nullptr)
{
	_setTexture(new ETexture(imageFileName));
	_clipTexture(x, y, width, height);
}

e2d::ESpriteFrame::ESpriteFrame(LPCTSTR resourceName, LPCTSTR resourceType, float x, float y, float width, float height)
	: m_fSourceClipX(0)
	, m_fSourceClipY(0)
	, m_fSourceClipWidth(0)
	, m_fSourceClipHeight(0)
	, m_pTexture(nullptr)
{
	_setTexture(new ETexture(resourceName, resourceType));
	_clipTexture(x, y, width, height);
}

e2d::ESpriteFrame::~ESpriteFrame()
{
	SafeRelease(&m_pTexture);
}

float e2d::ESpriteFrame::getWidth() const
{
	return m_fSourceClipWidth;
}

float e2d::ESpriteFrame::getHeight() const
{
	return m_fSourceClipHeight;
}

e2d::ETexture * e2d::ESpriteFrame::getTexture() const
{
	return m_pTexture;
}

void e2d::ESpriteFrame::_setTexture(ETexture * texture)
{
	if (texture)
	{
		SafeRelease(&m_pTexture);
		m_pTexture = texture;
		m_pTexture->retain();
		m_fSourceClipX = 0;
		m_fSourceClipY = 0;
		m_fSourceClipWidth = texture->getSourceWidth();
		m_fSourceClipHeight = texture->getSourceHeight();
	}
}

void e2d::ESpriteFrame::_clipTexture(float x, float y, float width, float height)
{
	if (m_pTexture)
	{
		m_fSourceClipX = min(max(x, 0), m_pTexture->getSourceWidth());
		m_fSourceClipY = min(max(y, 0), m_pTexture->getSourceHeight());
		m_fSourceClipWidth = min(max(width, 0), m_pTexture->getSourceWidth() - m_fSourceClipX);
		m_fSourceClipHeight = min(max(height, 0), m_pTexture->getSourceHeight() - m_fSourceClipY);
	}
}
