#include "..\easy2d.h"
#include "..\EasyX\easyx.h"
#include <map>
using namespace std;

// 图片缓存
static map<TString, CImage*> s_mCImages;
// 从图片缓存中读取图片
static CImage* GetCImage(TString name, bool fromRes = false);
// 对 PNG 图像进行像素转换
static void CrossImage(CImage &img);


Image::Image() :
	m_pCImage(nullptr),
	m_nAlpha(255),
	m_fScaleX(1),
	m_fScaleY(1)
{
}

Image::Image(LPCTSTR ImageFile) :
	Image()
{
	setImage(ImageFile);	// 设置图片资源
}

Image::Image(LPCTSTR ImageFile, int x, int y, int width, int height) :
	Image()
{
	setImage(ImageFile, x, y, width, height);	// 设置图片资源和裁剪大小
}

Image::~Image()
{
}

void Image::_onDraw()
{
	// display 属性为 false，或未设置图片资源时，不绘制该图片
	if (!m_bDisplay || !m_pCImage)
	{
		return;
	}
	// 绘制图片
	if (m_pCImage->GetBPP() == 32)
	{
		m_pCImage->AlphaBlend(GetImageHDC(), m_Rect, m_SrcRect, m_nAlpha, AC_SRC_OVER);
	}
	else
	{
		m_pCImage->Draw(GetImageHDC(), m_Rect, m_SrcRect);
	}
}

float Image::getScaleX() const
{
	return m_fScaleX;
}

float Image::getScaleY() const
{
	return m_fScaleY;
}

float Image::getOpacity() const
{
	return m_nAlpha / 255.0f;
}

bool Image::setImage(LPCTSTR ImageFile)
{
	m_pCImage = GetCImage(ImageFile);
	if (m_pCImage)
	{
		reset();
		return true;
	}
	return false;
}

bool Image::setImage(LPCTSTR ImageFile, int x, int y, int width, int height)
{
	if (!setImage(ImageFile))
	{
		return false;
	}
	// 裁剪图片大小
	crop(x, y, width, height);

	return true;
}

bool Image::setImageFromRes(LPCTSTR pResName)
{
	m_pCImage = GetCImage(pResName, true);
	if (m_pCImage)
	{
		reset();
		return true;
	}
	return false;
}

bool Image::setImageFromRes(LPCTSTR pResName, int x, int y, int width, int height)
{
	if (!setImageFromRes(pResName))
	{
		return false;
	}
	// 裁剪图片大小
	crop(x, y, width, height);

	return true;
}

void Image::crop(int x, int y, int width, int height)
{
	width = min(max(width, 0), m_pCImage->GetWidth() - x);
	height = min(max(height, 0), m_pCImage->GetHeight() - y);
	// 设置源矩形的位置和大小（用于裁剪）
	m_SrcRect.SetRect(x, y, x + width, y + height);
	// 设置目标矩形（即绘制到窗口的位置和大小）
	setSize(int(width * m_fScaleX), int(height * m_fScaleY));
}

void Image::stretch(int width, int height)
{
	// 设置目标矩形的位置和大小（即绘制到窗口的位置和大小，用于拉伸图片）
	setSize(max(width, 0), max(height, 0));
	// 重置比例缩放属性
	m_fScaleX = 1;
	m_fScaleY = 1;
}

void Image::setScale(float scaleX, float scaleY)
{
	m_fScaleX = max(scaleX, 0);
	m_fScaleY = max(scaleY, 0);
	setSize(int(m_SrcRect.Width() * scaleX), int(m_SrcRect.Height() * scaleY));
}

void Image::setOpacity(float value)
{
	if (m_pCImage->GetBPP() == 32)
	{
		m_nAlpha = BYTE(min(max(value, 0), 1) * 255);
	}
}

void Image::setTransparentColor(COLORREF value)
{
	// 设置透明色
	m_pCImage->SetTransparentColor(value);
}

void Image::reset()
{
	// 设置目标矩形（即绘制到窗口的位置和大小）
	setSize(m_pCImage->GetWidth(), m_pCImage->GetHeight());
	// 设置源矩形（即截取图片的大小）
	m_SrcRect.SetRect(0, 0, m_pCImage->GetWidth(), m_pCImage->GetHeight());
	// 重置缩放属性
	m_fScaleX = 1;
	m_fScaleY = 1;
	// 重置透明度
	m_nAlpha = 255;
}

bool Image::preload(LPCTSTR fileName, bool fromRes)
{
	// 判断图片是否已经加载
	if (s_mCImages.find(fileName) != s_mCImages.end())
	{
		return true;
	}
	// 加载图片
	CImage* cImage = nullptr;
	if (fromRes)
	{
		cImage = new CImage();
		// 从资源加载图片（不支持 PNG）
		cImage->LoadFromResource(GetModuleHandle(NULL), fileName);
	}
	else
	{
		//判断图片路径是否存在
		if (!PathFileExists(fileName))
		{
			return false;
		}
		cImage = new CImage();
		cImage->Load(fileName);
	}
	// 加载失败
	if (!cImage || cImage->IsNull())
	{
		return false;
	}
	// 确认该图像包含 Alpha 通道
	if (cImage->GetBPP() == 32)
	{
		// 透明图片处理
		CrossImage(*cImage);
	}
	s_mCImages.insert(map<TString, CImage*>::value_type(fileName, cImage));

	return true;
}

void Image::saveScreenshot()
{
	TString savePath;
	// 获取保存位置
	if (FileUtils::getSaveFilePath(savePath, _T("截图保存到"), _T("jpg")))
	{
		// 保存窗口截图
		IMAGE image;
		getimage(&image, 0, 0, App::getWidth(), App::getHeight());
		saveimage(savePath.c_str(), &image);
	}
}


// 对 PNG 图像进行像素转换
void CrossImage(CImage &img)
{
	// 进行像素转换
	for (int i = 0; i < img.GetWidth(); i++)
	{
		for (int j = 0; j < img.GetHeight(); j++)
		{
			UCHAR *cr = (UCHAR*)img.GetPixelAddress(i, j);
			cr[0] = cr[0] * cr[3] / 255;
			cr[1] = cr[1] * cr[3] / 255;
			cr[2] = cr[2] * cr[3] / 255;
		}
	}
}

CImage* GetCImage(TString name, bool fromRes)
{
	if (Image::preload(name.c_str()))
	{
		return s_mCImages.at(name);
	}
	else
	{
		return nullptr;
	}
}