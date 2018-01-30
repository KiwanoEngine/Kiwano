#include "..\ecommon.h"

e2d::EKeyframe::EKeyframe()
	: m_fSourceClipX(0)
	, m_fSourceClipY(0)
	, m_fSourceClipWidth(0)
	, m_fSourceClipHeight(0)
	, m_pImage(nullptr)
{
}

e2d::EKeyframe::EKeyframe(EImage * texture)
	: m_fSourceClipX(0)
	, m_fSourceClipY(0)
	, m_fSourceClipWidth(0)
	, m_fSourceClipHeight(0)
	, m_pImage(nullptr)
{
	_setImage(texture);
}

e2d::EKeyframe::EKeyframe(const EString & imageFileName)
	: m_fSourceClipX(0)
	, m_fSourceClipY(0)
	, m_fSourceClipWidth(0)
	, m_fSourceClipHeight(0)
	, m_pImage(nullptr)
{
	_setImage(new EImage(imageFileName));
}

e2d::EKeyframe::EKeyframe(LPCTSTR resourceName, LPCTSTR resourceType)
	: m_fSourceClipX(0)
	, m_fSourceClipY(0)
	, m_fSourceClipWidth(0)
	, m_fSourceClipHeight(0)
	, m_pImage(nullptr)
{
	_setImage(new EImage(resourceName, resourceType));
}

e2d::EKeyframe::EKeyframe(EImage * texture, float x, float y, float width, float height)
	: m_fSourceClipX(0)
	, m_fSourceClipY(0)
	, m_fSourceClipWidth(0)
	, m_fSourceClipHeight(0)
	, m_pImage(nullptr)
{
	_setImage(texture);
	_clipTexture(x, y, width, height);
}

e2d::EKeyframe::EKeyframe(const EString & imageFileName, float x, float y, float width, float height)
	: m_fSourceClipX(0)
	, m_fSourceClipY(0)
	, m_fSourceClipWidth(0)
	, m_fSourceClipHeight(0)
	, m_pImage(nullptr)
{
	_setImage(new EImage(imageFileName));
	_clipTexture(x, y, width, height);
}

e2d::EKeyframe::EKeyframe(LPCTSTR resourceName, LPCTSTR resourceType, float x, float y, float width, float height)
	: m_fSourceClipX(0)
	, m_fSourceClipY(0)
	, m_fSourceClipWidth(0)
	, m_fSourceClipHeight(0)
	, m_pImage(nullptr)
{
	_setImage(new EImage(resourceName, resourceType));
	_clipTexture(x, y, width, height);
}

e2d::EKeyframe::~EKeyframe()
{
	SafeRelease(&m_pImage);
}

float e2d::EKeyframe::getWidth() const
{
	return m_fSourceClipWidth;
}

float e2d::EKeyframe::getHeight() const
{
	return m_fSourceClipHeight;
}

e2d::EImage * e2d::EKeyframe::getImage() const
{
	return m_pImage;
}

void e2d::EKeyframe::_setImage(EImage * texture)
{
	if (texture)
	{
		SafeRelease(&m_pImage);
		m_pImage = texture;
		m_pImage->retain();
		m_fSourceClipX = 0;
		m_fSourceClipY = 0;
		m_fSourceClipWidth = texture->getSourceWidth();
		m_fSourceClipHeight = texture->getSourceHeight();
	}
}

void e2d::EKeyframe::_clipTexture(float x, float y, float width, float height)
{
	if (m_pImage)
	{
		m_fSourceClipX = min(max(x, 0), m_pImage->getSourceWidth());
		m_fSourceClipY = min(max(y, 0), m_pImage->getSourceHeight());
		m_fSourceClipWidth = min(max(width, 0), m_pImage->getSourceWidth() - m_fSourceClipX);
		m_fSourceClipHeight = min(max(height, 0), m_pImage->getSourceHeight() - m_fSourceClipY);
	}
}
