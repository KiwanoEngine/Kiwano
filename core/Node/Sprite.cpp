#include "..\enodes.h"


e2d::ESprite::ESprite()
	: m_pImage(nullptr)
{
}

e2d::ESprite::ESprite(EImage * image)
	: m_pImage(nullptr)
{
	loadFrom(image);
}

e2d::ESprite::ESprite(LPCTSTR imageFileName)
	: m_pImage(nullptr)
{
	loadFrom(imageFileName);
}

e2d::ESprite::ESprite(LPCTSTR imageFileName, float x, float y, float width, float height)
	: m_pImage(nullptr)
{
	loadFrom(imageFileName);
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

		ENode::_setSize(m_pImage->getWidth(), m_pImage->getHeight());
	}
}

void e2d::ESprite::loadFrom(LPCTSTR imageFileName)
{
	loadFrom(new EImage(imageFileName));
}

void e2d::ESprite::clip(float x, float y, float width, float height)
{
	m_pImage->clip(x, y, width, height);
	ENode::_setSize(
		min(max(width, 0), m_pImage->getSourceWidth() - m_pImage->getClipX()),
		min(max(height, 0), m_pImage->getSourceHeight() - m_pImage->getClipY())
	);
}

e2d::EImage * e2d::ESprite::getImage() const
{
	return m_pImage;
}

void e2d::ESprite::onRender()
{
	if (m_pImage && m_pImage->getBitmap())
	{
		// äÖÈ¾Í¼Æ¬
		ERenderer::getRenderTarget()->DrawBitmap(
			m_pImage->getBitmap(),
			D2D1::RectF(0, 0, getRealWidth(), getRealHeight()),
			m_fDisplayOpacity,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			D2D1::RectF(
				m_pImage->getClipX(),
				m_pImage->getClipY(),
				m_pImage->getClipX() + getRealWidth(),
				m_pImage->getClipY() + getRealHeight()
			)
		);
	}
}
