#pragma once
#include <e2dbase.h>
#include <atlimage.h>

namespace easy2d 
{
	class FontStyle;
	class Color;
	class Action;

	class Node :
		public Object
	{
		friend class Scene;
		friend class BatchNode;

	public:
		Node();
		Node(CPoint p);
		Node(int x, int y);
		virtual ~Node();

		// 获取节点横坐标
		virtual int getX() const;
		// 获取节点纵坐标
		virtual int getY() const;
		// 获取节点坐标
		virtual CPoint getPos() const;
		// 设置节点横坐标
		virtual void setX(int x);
		// 设置节点纵坐标
		virtual void setY(int y);
		// 设置节点横纵坐标
		virtual void setPos(int x, int y);
		// 设置节点横纵坐标
		virtual void setPos(CPoint p);
		// 移动节点
		virtual void move(int x, int y);
		// 设置节点横纵坐标
		virtual void move(CVector v);
		// 节点是否显示
		virtual bool display() const;
		// 设置节点是否显示
		virtual void setDisplay(bool value);
		// 获取节点绘图顺序
		virtual int getZOrder() const;
		// 设置节点绘图顺序（0为最先绘制，显示在最底层）
		virtual void setZOrder(int z);
		// 获取节点所在场景
		Scene * getParentScene();

	protected:
		int		m_nZOrder;
		bool	m_bDisplay;
		Scene*	m_pScene;
		CPoint	m_Pos;

	protected:
		virtual bool _exec(bool active);
		virtual void _onDraw() = 0;
		void setParentScene(Scene * scene);
	};


	class BatchNode :
		public Node
	{
	public:
		BatchNode();
		virtual ~BatchNode();

		// 添加子节点
		void add(Node *child, int z_Order = 0);
		// 删除子节点
		bool del(Node * child);
		// 清空所有子节点
		void clearAllChildren();

	protected:
		std::vector<Node*> m_vChildren;

	protected:
		virtual bool _exec(bool active) override;
		virtual void _onDraw() override;
	};


	class Layer :
		public BatchNode
	{
	public:
		Layer();
		virtual ~Layer();

		// 图层是否阻塞消息
		int getBlock() const;
		// 设置图层是否阻塞消息
		void setBlock(bool block);

	protected:
		bool m_bBlock;

	protected:
		virtual bool _exec(bool active) override;
	};


	class RectNode :
		public Node
	{
	public:
		RectNode();
		~RectNode();

		virtual bool isCollisionWith(RectNode * rectNode) const;
		virtual bool isPointIn(CPoint p) const;

		virtual void setWindowCenterX();
		virtual void setWindowCenterY();
		virtual void setWindowCenter();

		virtual int getX() const override;
		virtual int getY() const override;
		virtual CPoint getPos() const override;
		virtual int getWidth() const;
		virtual int getHeight() const;
		virtual CSize getSize() const;
		virtual CRect getRect() const;

		virtual void setX(int x) override;
		virtual void setY(int y) override;
		virtual void setPos(int x, int y) override;
		virtual void setPos(CPoint p) override;
		virtual void move(int x, int y) override;
		virtual void move(CVector v) override;
		virtual void setWidth(int width);
		virtual void setHeight(int height);
		virtual void setSize(int width, int height);
		virtual void setSize(CSize size);
		virtual void setRect(int x1, int y1, int x2, int y2);
		virtual void setRect(CPoint leftTop, CPoint rightBottom);
		virtual void setRect(CRect rect);

	protected:
		CRect m_Rect;
	};


	class Text :
		public RectNode
	{
		friend class TextButton;

	public:
		Text();
		// 根据字符串、颜色和字体创建文字
		Text(tstring text, COLORREF color = Color::white, FontStyle * font = FontStyle::getDefault());
		// 根据横纵坐标、字符串、颜色和字体创建文字
		Text(int x, int y, tstring text, COLORREF color = Color::white, FontStyle * font = FontStyle::getDefault());
		virtual ~Text();

		// 获取当前颜色
		COLORREF getColor() const;
		// 获取当前文字
		tstring getText() const;
		// 获取当前字体
		FontStyle * getFontStyle();
		// 文本是否为空
		bool isEmpty() const;

		// 设置文字
		void setText(tstring text);
		// 设置文字颜色
		void setColor(COLORREF color);
		// 设置字体
		void setFontStyle(FontStyle * style);

	protected:
		tstring		m_sText;
		COLORREF	m_color;
		FontStyle *	m_pFontStyle;

	protected:
		virtual void _onDraw() override;
	};


	class Image :
		public RectNode
	{
		friend class Sprite;
		friend class ImageButton;
	public:
		Image();
		// 从图片文件获取图像
		Image(LPCTSTR ImageFile);
		/**
		*  从图片文件获取图像
		*  参数：图片文件名，图片裁剪坐标，图片裁剪宽度和高度
		*/
		Image(LPCTSTR ImageFile, int x, int y, int width, int height);
		virtual ~Image();

		// 获取横向拉伸比例
		float getScaleX() const;
		// 获取纵向拉伸比例
		float getScaleY() const;
		// 获取透明度
		float getOpacity() const;

		/**
		*  从图片文件获取图像
		*  返回值：图片加载是否成功
		*/
		bool setImage(LPCTSTR ImageFile);
		/**
		*  从图片文件获取图像
		*  参数：图片文件名，图片裁剪起始坐标，图片裁剪宽度和高度
		*  返回值：图片加载是否成功
		*/
		bool setImage(LPCTSTR ImageFile, int x, int y, int width, int height);
		/**
		*  从资源文件获取图像，不支持 png
		*  返回值：图片加载是否成功
		*/
		bool setImageFromRes(LPCTSTR pResName);
		/**
		*  从资源文件获取图像，不支持 png
		*  参数：资源名称，图片裁剪坐标，图片裁剪宽度和高度
		*  返回值：图片加载是否成功
		*/
		bool setImageFromRes(LPCTSTR pResName, int x, int y, int width, int height);
		// 裁剪图片（裁剪后会恢复 stretch 拉伸）
		void crop(int x, int y, int width, int height);
		// 将图片拉伸到固定宽高
		void stretch(int width, int height);
		// 按比例拉伸图片
		void setScale(float scaleX, float scaleY);
		// 设置透明度，范围 0~1.0f（只对 png 图片生效）
		void setOpacity(float value);
		// 设置透明色
		void setTransparentColor(COLORREF value);
		// 重置图片属性
		void reset();
		// 保存游戏截图
		static void saveScreenshot();

	protected:
		CImage	m_Image;
		CRect	m_SrcRect;
		BYTE	m_nAlpha;
		float	m_fScaleX;
		float	m_fScaleY;

	protected:
		virtual void _onDraw() override;
	};


	class Sprite :
		public RectNode
	{
		friend class BatchSprite;
	public:
		Sprite();
		Sprite(Image * image);
		Sprite(LPCTSTR imageFileName);
		virtual ~Sprite();

		// 判断是否和另一个精灵碰撞
		bool isCollisionWith(Sprite * sprite);
		// 修改精灵图片
		virtual void setImage(Image * image);
		// 添加动作
		virtual void addAction(Action * action);
		// 暂停所有动作
		virtual void pauseAllAction();
		// 继续所有动作
		virtual void resumeAllAction();
		// 停止所有动作
		virtual void stopAllAction();

		virtual float getScaleX() const;
		virtual float getScaleY() const;
		virtual float getOpacity() const;

		virtual void setScale(float scaleX, float scaleY);
		virtual void setOpacity(float opacity);

	protected:
		float	m_fScaleX;
		float	m_fScaleY;
		BYTE	m_nAlpha;
		Image *	m_pImage;

	protected:
		bool _exec(bool active) override;
		void _onDraw() override;
	};


	class BatchSprite :
		public Node
	{
	public:
		BatchSprite();
		virtual ~BatchSprite();

		// 添加精灵
		void addSprite(Sprite * sprite, int z_Order = 0);
		// 删除精灵
		bool delSprite(Sprite * child);
		// 删除所有精灵
		void clearAllSprites();
		// 判断是否有精灵产生碰撞
		// 返回值：若有碰撞，返回第一个产生碰撞的精灵，否则返回空指针
		Sprite * isCollisionWith(Sprite * sprite);
		// 判断点是否在精灵内部
		// 返回值：若这个点在任意一个精灵内部，返回这个精灵，否则返回空指针
		Sprite * isPointIn(CPoint point);
		// 同时修改所有精灵的图片
		virtual void setImage(Image * image);

		virtual float getScaleX() const;
		virtual float getScaleY() const;
		virtual float getOpacity() const;

		virtual void setScale(float scaleX, float scaleY);
		virtual void setOpacity(float opacity);

	protected:
		std::vector<Sprite*> m_vSprites;
		float	m_fScaleX;
		float	m_fScaleY;
		BYTE	m_nAlpha;

	protected:
		bool _exec(bool active) override;
		void _onDraw() override;
	};


	class MouseNode :
		public RectNode
	{
	public:
		MouseNode();
		virtual ~MouseNode();

		// 鼠标是否移入
		virtual bool isMouseIn();
		// 鼠标是否选中
		virtual bool isSelected();
		// 设置回调函数
		virtual void setClickedCallback(const CLICK_CALLBACK & callback);
		// 设置回调函数
		virtual void setMouseInCallback(const CLICK_CALLBACK & callback);
		// 设置回调函数
		virtual void setMouseOutCallback(const CLICK_CALLBACK & callback);
		// 设置回调函数
		virtual void setSelectCallback(const CLICK_CALLBACK & callback);
		// 设置回调函数
		virtual void setUnselectCallback(const CLICK_CALLBACK & callback);
		// 重置状态
		virtual void reset();
		// 设置节点是否阻塞鼠标消息
		void setBlock(bool block);

	protected:
		bool m_bTarget;
		bool m_bBlock;
		enum Status { NORMAL, MOUSEIN, SELECTED } m_eStatus;
		CLICK_CALLBACK m_OnMouseInCallback;
		CLICK_CALLBACK m_OnMouseOutCallback;
		CLICK_CALLBACK m_OnSelectCallback;
		CLICK_CALLBACK m_OnUnselectCallback;
		CLICK_CALLBACK m_ClickCallback;

	protected:
		virtual bool _exec(bool active) override;
		virtual void _onDraw() override;

		// 重写这个方法可以自定义按钮的判断方法
		virtual bool _isMouseIn();
		// 切换状态
		virtual void _setStatus(Status status);
		// 正常状态
		virtual void _onNormal() = 0;
		// 鼠标移入时
		virtual void _onMouseIn() = 0;
		// 鼠标选中时
		virtual void _onSelected() = 0;
	};


	class Button :
		public MouseNode
	{
	public:
		Button();
		virtual ~Button();

		// 按钮是否启用
		virtual bool isEnable();
		// 设置是否启用
		virtual void setEnable(bool enable);

		// 设置按钮横坐标
		virtual void setX(int x) override;
		// 设置按钮纵坐标
		virtual void setY(int y) override;
		// 设置按钮坐标
		virtual void setPos(int x, int y) override;
		// 设置按钮坐标
		virtual void setPos(CPoint p) override;
		// 移动按钮
		virtual void move(int x, int y) override;
		// 移动按钮
		virtual void move(CVector v) override;

	protected:
		bool m_bEnable;

	protected:
		virtual bool _exec(bool active) override;
		virtual void _onDraw() override;
		virtual void _resetPosition() = 0;

		virtual void _onNormal() = 0;
		virtual void _onMouseIn() = 0;
		virtual void _onSelected() = 0;
		virtual void _onDisable() = 0;
	};


	class TextButton :
		public Button
	{
	public:
		TextButton();
		TextButton(tstring text);
		TextButton(Text * text);
		virtual ~TextButton();

		// 设置按钮文字
		void setNormal(Text * text);
		// 设置鼠标移入时的按钮文字
		void setMouseIn(Text * text);
		// 设置鼠标选中时的按钮文字
		void setSelected(Text * text);
		// 设置按钮禁用时的按钮文字
		void setUnable(Text * text);

	protected:
		Text * m_pNormalText;
		Text * m_pMouseInText;
		Text * m_pSelectedText;
		Text * m_pUnableText;

	protected:
		virtual void _resetPosition() override;

		virtual void _setStatus(Status status) override;
		virtual void _onNormal() override;
		virtual void _onMouseIn() override;
		virtual void _onSelected() override;
		virtual void _onDisable() override;
	};


	class ImageButton :
		public Button
	{
	public:
		ImageButton();
		ImageButton(LPCTSTR image);
		ImageButton(Image * image);
		virtual ~ImageButton();

		// 设置按钮图片
		void setNormal(Image * image);
		// 设置鼠标移入时的按钮图片
		void setMouseIn(Image * image);
		// 设置鼠标选中时的按钮图片
		void setSelected(Image * image);
		// 设置按钮禁用时的按钮图片
		void setUnable(Image * image);

	protected:
		Image * m_pNormalImage;
		Image * m_pMouseInImage;
		Image * m_pSelectedImage;
		Image * m_pUnableImage;

	protected:
		virtual void _resetPosition() override;
		virtual void _setStatus(Status status) override;
		virtual void _onNormal() override;
		virtual void _onMouseIn() override;
		virtual void _onSelected() override;
		virtual void _onDisable() override;
	};


	class Shape :
		public Node
	{
	public:
		Shape();
		virtual ~Shape();

		// 形状填充样式
		enum STYLE { ROUND, SOLID, FILL } m_eStyle;

		// 获取形状的填充颜色
		COLORREF getFillColor() const;
		// 获取形状的线条颜色
		COLORREF getLineColor() const;
		// 设置填充颜色
		void setFillColor(COLORREF color);
		// 设置线条颜色
		void setLineColor(COLORREF color);
		// 设置填充样式
		void setStyle(STYLE style);

	protected:
		COLORREF fillColor;
		COLORREF lineColor;

	protected:
		virtual void _onDraw() override;
		virtual void solidShape() = 0;
		virtual void fillShape() = 0;
		virtual void roundShape() = 0;
	};


	class Rect :
		public Shape
	{
	public:
		Rect();
		Rect(int x, int y, int width, int height);
		virtual ~Rect();

		// 获取矩形宽度
		int getWidth() const;
		// 获取矩形高度
		int getHeight() const;
		// 设置矩形宽度
		void setWidth(int width);
		// 设置矩形高度
		void setHeight(int height);
		// 设置矩形大小
		void setSize(int width, int height);

	protected:
		CSize m_Size;

	protected:
		virtual void solidShape() override;
		virtual void fillShape() override;
		virtual void roundShape() override;
	};


	class Circle :
		public Shape
	{
	public:
		Circle();
		Circle(int x, int y, int radius);
		virtual ~Circle();

		// 获取圆形半径
		int getRadius() const;
		// 设置圆形半径
		void setRadius(int m_nRadius);

	protected:
		int m_nRadius;

	protected:
		virtual void solidShape() override;
		virtual void fillShape() override;
		virtual void roundShape() override;
	};

}	// End of easy2d namespace