#include "..\enodes.h"


e2d::ESprite::ESprite()
	: m_fSourceClipX(0)
	, m_fSourceClipY(0)
	, m_pImage(nullptr)
{
}

e2d::ESprite::ESprite(EImage * image)
	: m_fSourceClipX(0)
	, m_fSourceClipY(0)
	, m_pImage(nullptr)
{
	loadFrom(image);
}

e2d::ESprite::ESprite(EKeyframe * spriteFrame)
	: m_fSourceClipX(0)
	, m_fSourceClipY(0)
	, m_pImage(nullptr)
{
	loadFrom(spriteFrame);
}

e2d::ESprite::ESprite(LPCTSTR imageFileName)
	: m_fSourceClipX(0)
	, m_fSourceClipY(0)
	, m_pImage(nullptr)
{
	loadFrom(imageFileName);
}

e2d::ESprite::ESprite(LPCTSTR imageFileName, float x, float y, float width, float height)
	: m_fSourceClipX(0)
	, m_fSourceClipY(0)
	, m_pImage(nullptr)
{
	loadFrom(imageFileName);
	clip(x, y, width, height);
}

e2d::ESprite::ESprite(LPCTSTR resourceName, LPCTSTR resourceType)
	: m_fSourceClipX(0)
	, m_fSourceClipY(0)
	, m_pImage(nullptr)
{
	loadFrom(resourceName, resourceType);
}

e2d::ESprite::ESprite(LPCTSTR resourceName, LPCTSTR resourceType, float x, float y, float width, float height)
	: m_fSourceClipX(0)
	, m_fSourceClipY(0)
	, m_pImage(nullptr)
{
	loadFrom(resourceName, resourceType);
	clip(x, y, width, height);
}

e2d::ESprite::~ESprite()
{
	SafeRelease(&m_pImage);
}

void e2d::ESprite::loadFrom(EImage * image)
{
	if (image)
	{
		SafeRelease(&m_pImage);
		m_pImage = image;
		m_pImage->retain();

		m_fSourceClipX = m_fSourceClipY = 0;
		ENode::_setWidth(m_pImage->getSourceWidth());
		ENode::_setHeight(m_pImage->getSourceHeight());
	}
}

void e2d::ESprite::loadFrom(LPCTSTR imageFileName)
{
	loadFrom(new EImage(imageFileName));
}

void e2d::ESprite::loadFrom(LPCTSTR resourceName, LPCTSTR resourceType)
{
	loadFrom(new EImage(resourceName, resourceType));
}

void e2d::ESprite::loadFrom(EImage * image, float x, float y, float width, float height)
{
	loadFrom(image);
	clip(x, y, width, height);
}

void e2d::ESprite::loadFrom(EKeyframe * frame)
{
	if (frame)
	{
		loadFrom(frame->m_pImage);
		clip(frame->m_fSourceClipX, frame->m_fSourceClipY, frame->m_fSourceClipWidth, frame->m_fSourceClipHeight);
	}
}

void e2d::ESprite::clip(float x, float y, float width, float height)
{
	m_fSourceClipX = min(max(x, 0), m_pImage->getSourceWidth());
	m_fSourceClipY = min(max(y, 0), m_pImage->getSourceHeight());
	ENode::_setWidth(min(max(width, 0), m_pImage->getSourceWidth() - m_fSourceClipX));
	ENode::_setHeight(min(max(height, 0), m_pImage->getSourceHeight() - m_fSourceClipY));
}

void e2d::ESprite::onRender()
{
	if (m_pImage && m_pImage->_getBitmap())
	{
		// Draw bitmap
		ERenderer::getRenderTarget()->DrawBitmap(
			m_pImage->_getBitmap(),
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
