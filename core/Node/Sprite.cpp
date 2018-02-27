#include "..\enodes.h"


e2d::Sprite::Sprite()
	: m_pImage(nullptr)
{
}

e2d::Sprite::Sprite(Image * image)
	: m_pImage(nullptr)
{
	loadFrom(image);
}

e2d::Sprite::Sprite(LPCTSTR imageFileName)
	: m_pImage(nullptr)
{
	loadFrom(imageFileName);
}

e2d::Sprite::Sprite(LPCTSTR imageFileName, double x, double y, double width, double height)
	: m_pImage(nullptr)
{
	loadFrom(imageFileName);
	clip(x, y, width, height);
}

e2d::Sprite::~Sprite()
{
	SafeRelease(&m_pImage);
}

void e2d::Sprite::loadFrom(Image * image)
{
	if (image)
	{
		SafeRelease(&m_pImage);
		m_pImage = image;
		m_pImage->retain();

		Node::_setSize(m_pImage->getWidth(), m_pImage->getHeight());
	}
}

void e2d::Sprite::loadFrom(LPCTSTR imageFileName)
{
	loadFrom(new Image(imageFileName));
}

void e2d::Sprite::clip(double x, double y, double width, double height)
{
	m_pImage->clip(x, y, width, height);
	Node::_setSize(
		min(max(width, 0), m_pImage->getSourceWidth() - m_pImage->getClipX()),
		min(max(height, 0), m_pImage->getSourceHeight() - m_pImage->getClipY())
	);
}

e2d::Image * e2d::Sprite::getImage() const
{
	return m_pImage;
}

void e2d::Sprite::onRender()
{
	if (m_pImage && m_pImage->getBitmap())
	{
		float fClipX = static_cast<float>(m_pImage->getClipX());
		float fClipY = static_cast<float>(m_pImage->getClipY());
		// äÖÈ¾Í¼Æ¬
		Renderer::getRenderTarget()->DrawBitmap(
			m_pImage->getBitmap(),
			D2D1::RectF(0, 0, m_fWidth, m_fHeight),
			m_fDisplayOpacity,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			D2D1::RectF(
				fClipX,
				fClipY,
				fClipX + m_fWidth,
				fClipY + m_fHeight
			)
		);
	}
}
