#pragma once
#include <e2dbase.h>

namespace easy2d 
{
	class FontStyle :
		public Object
	{
		friend class Text;

	public:
		FontStyle();
		/**
		*  使用 [字体名称、字号、粗细、字宽、斜体、下划线、删除线、字符串书写角度、
		*  每个字符书写角度、抗锯齿] 属性创建字体样式
		*/
		FontStyle(LPCTSTR fontfamily, LONG height = 18, LONG weight = 0, LONG width = 0,
			bool italic = 0, bool underline = 0, bool strikeout = 0, LONG escapement = 0,
			LONG orientation = 0, bool quality = true);
		virtual ~FontStyle();

		// 获取默认字体
		static FontStyle * getDefault();
		// 设置字符平均高度
		void setHeight(LONG value);
		// 设置字符平均宽度（0表示自适应）
		void setWidth(LONG value);
		// 设置字体
		void setFontFamily(LPCTSTR value);
		// 设置字符笔画粗细，范围0~1000，默认为0
		void setWeight(LONG value);
		// 设置斜体
		void setItalic(bool value);
		// 设置下划线
		void setUnderline(bool value);
		// 设置删除线
		void setStrikeOut(bool value);
		// 设置字符串的书写角度，单位0.1度，默认为0
		void setEscapement(LONG value);
		// 设置每个字符的书写角度，单位0.1度，默认为0
		void setOrientation(LONG value);
		// 设置字体抗锯齿，默认为true
		void setQuality(bool value);

	protected:
		LOGFONT m_font;
	};


	class FontWeight
	{
	public:
		static const LONG dontcare;		// 粗细值 0
		static const LONG thin;			// 粗细值 100
		static const LONG extraLight;	// 粗细值 200
		static const LONG light;		// 粗细值 300
		static const LONG normal;		// 粗细值 400
		static const LONG regular;		// 粗细值 400
		static const LONG medium;		// 粗细值 500
		static const LONG demiBlod;		// 粗细值 600
		static const LONG blod;			// 粗细值 700
		static const LONG extraBold;	// 粗细值 800
		static const LONG black;		// 粗细值 900
		static const LONG heavy;		// 粗细值 900
	};

	class Color
	{
	public:
		static const COLORREF black;		// 黑色
		static const COLORREF blue;			// 蓝色
		static const COLORREF green;		// 绿色
		static const COLORREF cyan;			// 青色
		static const COLORREF red;			// 红色
		static const COLORREF magenta;		// 紫色
		static const COLORREF brown;		// 棕色
		static const COLORREF lightgray;	// 亮灰色
		static const COLORREF darkgray;		// 深灰色
		static const COLORREF lightblue;	// 亮蓝色
		static const COLORREF lightgreen;	// 亮绿色
		static const COLORREF lightcyan;	// 亮青色
		static const COLORREF lightred;		// 亮红色
		static const COLORREF lightmagenta;	// 亮紫色
		static const COLORREF yellow;		// 亮黄色
		static const COLORREF white;		// 白色

											// 通过红、绿、蓝颜色分量合成颜色
		static COLORREF getFromRGB(BYTE r, BYTE g, BYTE b);
		// 通过色相、饱和度、亮度合成颜色
		static COLORREF getFromHSL(float H, float S, float L);
		// 通过色相、饱和度、明度合成颜色
		static COLORREF getFromHSV(float H, float S, float V);

		// 返回指定颜色中的红色值
		static BYTE getRValue(COLORREF color);
		// 返回指定颜色中的绿色值
		static BYTE getGValue(COLORREF color);
		// 返回指定颜色中的蓝色值
		static BYTE getBValue(COLORREF color);
		// 返回与指定颜色对应的灰度值颜色
		static COLORREF getGray(COLORREF color);
	};

}	// End of easy2d namespace