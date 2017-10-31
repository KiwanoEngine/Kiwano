#include "..\enodes.h"
#include "..\Win\winbase.h"


e2d::ESprite::ESprite()
	: m_fSourceClipX(0)
	, m_fSourceClipY(0)
	, m_pTexture(nullptr)
{
}

e2d::ESprite::ESprite(ETexture * texture)
	: ESprite()
{
	loadFrom(texture);
}

e2d::ESprite::ESprite(ESpriteFrame * spriteFrame)
	: ESprite()
{
	loadFrom(spriteFrame);
}

e2d::ESprite::ESprite(const EString & imageFileName)
	: ESprite()
{
	loadFrom(imageFileName);
}

e2d::ESprite::ESprite(const EString & imageFileName, float x, float y, float width, float height)
{
	loadFrom(imageFileName);
	clip(x, y, width, height);
}

e2d::ESprite::ESprite(const EString & resourceName, const EString & resourceType)
	: ESprite()
{
	loadFrom(resourceName, resourceType);
}

e2d::ESprite::ESprite(const EString & resourceName, const EString & resourceType, float x, float y, float width, float height)
{
	loadFrom(resourceName, resourceType);
	clip(x, y, width, height);
}

e2d::ESprite::~ESprite()
{
	SafeRelease(&m_pTexture);
}

void e2d::ESprite::loadFrom(ETexture * texture)
{
	if (texture)
	{
		SafeRelease(&m_pTexture);
		m_pTexture = texture;
		m_pTexture->retain();

		m_fSourceClipX = m_fSourceClipY = 0;
		ENode::_setWidth(m_pTexture->getSourceWidth());
		ENode::_setHeight(m_pTexture->getSourceHeight());
	}
}

void e2d::ESprite::loadFrom(const EString & imageFileName)
{
	loadFrom(new ETexture(imageFileName));
}

void e2d::ESprite::loadFrom(const EString & resourceName, const EString & resourceType)
{
	loadFrom(new ETexture(resourceName, resourceType));
}

void e2d::ESprite::loadFrom(ETexture * texture, float x, float y, float width, float height)
{
	loadFrom(texture);
	clip(x, y, width, height);
}

void e2d::ESprite::loadFrom(ESpriteFrame * frame)
{
	if (frame)
	{
		loadFrom(frame->m_pTexture);
		clip(frame->m_fSourceClipX, frame->m_fSourceClipY, frame->m_fSourceClipWidth, frame->m_fSourceClipHeight);
	}
}

void e2d::ESprite::clip(float x, float y, float width, float height)
{
	m_fSourceClipX = min(max(x, 0), m_pTexture->getSourceWidth());
	m_fSourceClipY = min(max(y, 0), m_pTexture->getSourceHeight());
	ENode::_setWidth(min(max(width, 0), m_pTexture->getSourceWidth() - m_fSourceClipX));
	ENode::_setHeight(min(max(height, 0), m_pTexture->getSourceHeight() - m_fSourceClipY));
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
			D2D1::RectF(
				m_fSourceClipX, 
				m_fSourceClipY, 
				m_fSourceClipX + getRealWidth(), 
				m_fSourceClipY + getRealHeight()
			)
		);
	}
}
