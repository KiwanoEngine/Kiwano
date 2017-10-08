/******************************************************
* Easy2D Game Engine
* 
* Website: http://www.easy2d.cn
* Github:  https://github.com/Nomango/Easy2D
* Gitee:   https://gitee.com/werelone/Easy2D
******************************************************/

#pragma once

#ifndef __cplusplus
	#error Easy2D is only for C++
#endif

#if _MSC_VER < 1900
	#error Do Visual Studio 2015/2017 specific stuff
#endif


#include <Windows.h>
#include <tchar.h>
#include <atltypes.h>
#include <atlimage.h>
#include <vector>
#include <stack>
#include <functional>


#if defined(UNICODE) && (_DEBUG)
	#pragma comment(lib,"Easy2Ddw.lib")
#elif !defined(UNICODE) && (_DEBUG)
	#pragma comment(lib,"Easy2Dd.lib")
#elif defined(UNICODE)
	#pragma comment(lib,"Easy2Dw.lib")
#elif !defined(UNICODE)
	#pragma comment(lib,"Easy2D.lib")
#endif


// Type Declare

typedef CPoint						CVector;
typedef unsigned int				VK_KEY;
typedef std::function<void()>		CLICK_CALLBACK;
typedef std::function<void()>		TIMER_CALLBACK;
typedef std::function<void(VK_KEY)>	KEY_CALLBACK;
typedef std::function<void()>		MOUSE_CALLBACK;

#ifdef UNICODE
	typedef std::wstring TString;
#else
	typedef std::string TString;
#endif

// Classes Declare

namespace easy2d
{
	class App;
	class Scene;

	class Object;
	class Node;
	class FontStyle;
	class Color;
	class RectNode;
	class Text;
	class Image;
	class BatchNode;
	class MouseNode;
	class Button;
	class TextButton;
	class ImageButton;
	class Layer;
	class Sprite;
	class BatchSprite;
	class Shape;
	class Circle;
	class Rectangle;

	class Action;
	class Animation;
	class ActionMoveTo;
	class ActionMoveBy;
	class ActionScaleTo;
	class ActionScaleBy;
	class ActionOpacityTo;
	class ActionOpacityBy;
	class ActionFadeIn;
	class ActionFadeOut;
	class ActionFrames;
	class ActionDelay;
	class ActionCallback;
	class ActionTwo;
	class ActionSequence;
	class ActionNeverStop;

	class MouseMsg;
	class KeyMsg;

	class FreePool;
	class FileUtils;
	class MusicUtils;
	class ActionManager;
}


// Classes

namespace easy2d
{

class App
{
	friend Scene;
public:
	App();
	~App();

	// 窗口可选模式
	enum MODE { SHOW_CONSOLE = 1, NO_CLOSE = 2, NO_MINI_MIZE = 4 };

	// 定义绘图窗口
	void createWindow(int width, int height, int mode = 0);
	// 定义绘图窗口
	void createWindow(CSize size, int mode = 0);
	// 定义绘图窗口
	void createWindow(TString title, int width, int height, int mode = 0);
	// 定义绘图窗口
	void createWindow(TString title, CSize size, int mode = 0);
	// 启动程序
	int run();
	// 释放所有内存资源
	void free();

	// 获取程序实例
	static App * get();
	// 设置坐标原点
	static void setOrigin(int originX, int originY);
	// 获取坐标原点的物理横坐标
	static int getOriginX();
	// 获取坐标原点的物理纵坐标
	static int getOriginY();
	// 终止程序
	static void quit();
	// 终止程序
	static void end();
	// 修改窗口大小
	static void setWindowSize(int width, int height);
	// 修改窗口大小
	static void setWindowSize(CSize size);
	// 关闭窗口
	static void close();
	// 设置窗口标题
	static void setWindowTitle(TString title);
	// 获取窗口标题
	static TString getWindowTitle();
	// 获取窗口宽度
	static int getWidth();
	// 获取窗口高度
	static int getHeight();
	// 切换场景
	static void enterScene(Scene *scene, bool save = true);
	// 返回上一场景
	static void backScene();
	// 清空之前保存的所有场景
	static void clearScene();
	// 设置 AppName
	static void setAppName(TString appname);
	// 获取 AppName
	static TString getAppName();
	// 修改窗口背景色
	static void setBkColor(COLORREF color);
	// 设置帧率
	static void setFPS(DWORD fps);
	// 重置绘图样式为默认值
	static void reset();
	// 获取当前场景
	static Scene * getCurrentScene();
	// 获取正处于加载中的场景
	static Scene * getLoadingScene();

protected:
	TString				m_sTitle;
	TString				m_sAppName;
	Scene*				m_pCurrentScene;
	Scene*				m_pNextScene;
	Scene*				m_pLoadingScene;
	std::stack<Scene*>	m_SceneStack;
	LARGE_INTEGER		m_nAnimationInterval;
	CSize				m_Size;
	int					m_nWindowMode;
	bool				m_bRunning;
	bool				m_bSaveScene;

protected:
	void _initGraph();
	void _mainLoop();
	void _enterNextScene();
};

class FreePool
{
	friend App;
	friend Object;
private:
	// 刷新内存池
	static void __flush();
	// 将一个节点放入释放池
	static void __add(Object * nptr);
	// 删除所有节点
	static void __clearAllObjects();
};

class Scene
{
	friend App;
	friend MouseMsg;
public:
	Scene();
	~Scene();

	// 重写这个函数，初始化这个场景
	virtual void init() = 0;
	// 重写这个函数，它将在进入这个场景时自动执行
	virtual void onEnter();
	// 重写这个函数，它将在离开这个场景时自动执行
	virtual void onExit();
	// 添加子成员到场景
	void add(Node * child, int zOrder = 0);
	// 删除子成员
	bool del(Node * child);
	// 清空所有子成员
	void clearAllChildren();

protected:
	std::vector<Node*> m_vChildren;

protected:
	void _exec();
	void _onDraw();
};

class Object
{
	friend FreePool;
public:
	Object();
	virtual ~Object();

	// 保留这个对象
	void retain();
	// 释放这个对象
	void release();
	// 让引擎自动释放这个对象
	void autoRelease();

protected:
	int m_nRefCount;
	bool m_bAutoRelease;
};

class MouseMsg
{
	friend App;
public:
	MouseMsg();
	MouseMsg(TString name, const MOUSE_CALLBACK& callback);
	~MouseMsg();

	enum MESSAGE 
	{
		MOVE = 0x0200,	// 鼠标移动
		LBUTTON_DOWN,	// 鼠标左键按下
		LBUTTON_UP,		// 鼠标左键抬起
		LBUTTON_DBLCLK,	// 鼠标左键双击
		RBUTTON_DOWN,	// 鼠标右键按下
		RBUTTON_UP,		// 鼠标右键抬起
		RBUTTON_DBLCLK,	// 鼠标右键双击
		MBUTTON_DOWN,	// 鼠标中键按下
		MBUTTON_UP,		// 鼠标中键抬起
		MBUTTON_DBLCLK,	// 鼠标中键双击
		WHEEL			// 滑动滚轮
	};

	// 启动监听
	void start();
	// 停止监听
	void stop();
	// 进入等待状态
	void wait();
	// 唤醒
	void notify();

	// 左键是否按下
	static bool isLButtonDown();
	// 右键是否按下
	static bool isRButtonDown();
	// 中键是否按下
	static bool isMButtonDown();
	// 获取鼠标X坐标
	static int getX();
	// 获取鼠标Y坐标
	static int getY();
	// 获取鼠标坐标
	static CPoint getPos();
	// 获取鼠标滚轮值
	static int getWheel();
	// 获取当前鼠标消息
	static MESSAGE getMsg();

	// 添加鼠标消息监听
	static void addListener(TString name, const MOUSE_CALLBACK& callback);
	// 启动鼠标消息监听
	static void startListener(TString name);
	// 停止鼠标消息监听
	static void stopListener(TString name);
	// 删除鼠标消息监听
	static void delListener(TString name);
	// 删除所有鼠标消息监听
	static void clearAllListeners();
	// 启动绑定在场景上的所有监听器
	static void notifyAllSceneListeners(Scene* scene);
	// 停止绑定在场景上的所有监听器
	static void waitAllSceneListeners(Scene* scene);
	// 清除绑定在场景上的所有监听器
	static void clearAllSceneListeners(Scene* scene);

private:
	static void __exec();

protected:
	bool			m_bRunning;
	bool			m_bWaiting;
	TString			m_sName;
	MOUSE_CALLBACK	m_callback;
	Scene *			m_pParentScene;

protected:
	// 执行回调函数
	void onMouseMsg();
};

class KeyMsg
{
	friend App;
public:
	KeyMsg(TString name, const KEY_CALLBACK& callback);
	~KeyMsg();

	// 执行回调函数
	void onKbHit(VK_KEY key);
	// 启动监听
	void start();
	// 停止监听
	void stop();
	// 进入等待状态
	void wait();
	// 唤醒
	void notify();

	// 判断键是否被按下，按下返回true
	static bool isKeyDown(VK_KEY key);
	// 添加按键监听
	static void addListener(TString name, const KEY_CALLBACK& callback);
	// 启动按键监听
	static void startListener(TString name);
	// 停止按键监听
	static void stopListener(TString name);
	// 删除按键监听
	static void KeyMsg::delListener(TString name);
	// 启动绑定在场景上的所有监听器
	static void notifyAllSceneListeners(Scene* scene);
	// 停止绑定在场景上的所有监听器
	static void waitAllSceneListeners(Scene* scene);
	// 停止绑定在场景上的所有定时器
	static void clearAllSceneListeners(Scene* scene);
	// 删除所有按键监听
	static void clearAllListeners();

public:
	// 字母键值
	static const VK_KEY A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z;
	// 数字键值
	static const VK_KEY NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, NUM_0;
	// 小数字键盘值
	static const VK_KEY NUMPAD_1, NUMPAD_2, NUMPAD_3, NUMPAD_4, NUMPAD_5, NUMPAD_6, NUMPAD_7, NUMPAD_8, NUMPAD_9, NUMPAD_0;
	// 控制键值
	static const VK_KEY Enter, Space, Up, Down, Left, Right, Esc, Shift, LShift, RShift, Ctrl, LCtrl, RCtrl;
	// F 键值
	static const VK_KEY F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12;

private:
	static void __exec();

protected:
	bool			m_bRunning;
	bool			m_bWaiting;
	TString			m_sName;
	KEY_CALLBACK	m_callback;
	Scene *			m_pParentScene;
};

class FontStyle :
	public Object
{
	friend Text;
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

class Node :
	public Object
{
	friend Scene;
	friend BatchNode;
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
	friend TextButton;
public:
	Text();
	// 根据字符串、颜色和字体创建文字
	Text(TString text, COLORREF color = Color::white, FontStyle * font = FontStyle::getDefault());
	// 根据横纵坐标、字符串、颜色和字体创建文字
	Text(int x, int y, TString text, COLORREF color = Color::white, FontStyle * font = FontStyle::getDefault());
	virtual ~Text();

	// 获取当前颜色
	COLORREF getColor() const;
	// 获取当前文字
	TString getText() const;
	// 获取当前字体
	FontStyle * getFontStyle();
	// 文本是否为空
	bool isEmpty() const;

	// 设置文字
	void setText(TString text);
	// 设置文字颜色
	void setColor(COLORREF color);
	// 设置字体
	void setFontStyle(FontStyle * style);

protected:
	TString		m_sText;
	COLORREF	m_color;
	FontStyle *	m_pFontStyle;

protected:
	virtual void _onDraw() override;
};

class Image :
	public RectNode
{
	friend Sprite;
	friend ImageButton;
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

	// 预加载图片
	static bool preload(LPCTSTR fileName, bool fromRes = false);
	// 保存游戏截图
	static void saveScreenshot();

protected:
	CImage*	m_pCImage;
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
	friend BatchSprite;
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
	// 继续动作
	virtual void resumeAction(Action * action);
	// 暂停动作
	virtual void pauseAction(Action * action);
	// 停止动作
	virtual void stopAction(Action * action);
	// 暂停所有动作
	virtual void pauseAllActions();
	// 继续所有动作
	virtual void resumeAllActions();
	// 停止所有动作
	virtual void stopAllActions();

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
	public Sprite
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
	// 所有精灵同时执行一段动画
	virtual void addAction(Action * action) override;
	// 同时修改所有精灵的图片
	virtual void setImage(Image * image) override;

	virtual float getScaleX() const override;
	virtual float getScaleY() const override;
	virtual float getOpacity() const override;

	virtual void setScale(float scaleX, float scaleY) override;
	virtual void setOpacity(float opacity) override;

protected:
	std::vector<Sprite*> m_vSprites;

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
	TextButton(TString text);
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

class Action :
	public Object
{
	friend Sprite;
	friend ActionManager;
	friend ActionTwo;
	friend ActionNeverStop;
	friend ActionSequence;
public:
	Action();
	virtual ~Action();

	// 获取动作运行状态
	virtual bool isRunning();
	// 获取动作结束状态
	virtual bool isEnding();
	// 继续动作
	virtual void start();
	// 继续动作
	virtual void resume();
	// 暂停动作
	virtual void pause();
	// 停止动作
	virtual void stop();
	// 进入等待状态
	virtual void wait();
	// 唤醒
	virtual void notify();
	// 设置动作每一帧时间间隔
	virtual void setInterval(UINT ms);
	// 获取一个新的拷贝动作
	virtual Action * copy() const = 0;
	// 获取一个新的逆向动作
	virtual Action * reverse() const;
	// 获取执行该动作的目标
	virtual Sprite * getTarget();

protected:
	bool			m_bRunning;
	bool			m_bWaiting;
	bool			m_bEnding;
	bool			m_bInit;
	Sprite *		m_pTargetSprite;
	Scene *			m_pParentScene;
	UINT			m_nMilliSeconds;
	LARGE_INTEGER	m_nLast;
	LARGE_INTEGER	m_nAnimationInterval;

protected:
	virtual void _init();
	virtual void _exec(LARGE_INTEGER nNow) = 0;
	virtual void _reset();
};

class Animation :
	public Action
{
public:
	Animation(float duration);
	virtual ~Animation();

protected:
	UINT	m_nDuration;
	UINT	m_nTotalDuration;

protected:
	bool _isEnd() const;
	bool _isDelayEnough(LARGE_INTEGER nNow);
	virtual void _init() override;
	virtual void _reset() override;
};

class ActionMoveBy :
	public Animation
{
public:
	ActionMoveBy(float duration, CVector vec);
	virtual ~ActionMoveBy();

	virtual ActionMoveBy * copy() const override;
	virtual ActionMoveBy * reverse() const override;

protected:
	CPoint	m_BeginPos;
	CVector	m_MoveVector;

protected:
	virtual void _init() override;
	virtual void _exec(LARGE_INTEGER nNow) override;
	virtual void _reset() override;
};

class ActionMoveTo :
	public ActionMoveBy
{
public:
	ActionMoveTo(float duration, CPoint pos);
	virtual ~ActionMoveTo();

	virtual ActionMoveTo * copy() const override;

protected:
	CPoint m_EndPos;

protected:
	virtual void _init() override;
	virtual void _reset() override;
};

class ActionScaleBy :
	public Animation
{
public:
	ActionScaleBy(float duration, float scaleX, float scaleY);
	virtual ~ActionScaleBy();

	virtual ActionScaleBy * copy() const override;
	virtual ActionScaleBy * reverse() const override;

protected:
	float	m_nBeginScaleX;
	float	m_nBeginScaleY;
	float	m_nVariationX;
	float	m_nVariationY;

protected:
	virtual void _init() override;
	virtual void _exec(LARGE_INTEGER nNow) override;
	virtual void _reset() override;
};

class ActionScaleTo :
	public ActionScaleBy
{
public:
	ActionScaleTo(float duration, float scaleX, float scaleY);
	virtual ~ActionScaleTo();

	virtual ActionScaleTo * copy() const override;

protected:
	float	m_nEndScaleX;
	float	m_nEndScaleY;

protected:
	virtual void _init() override;
	virtual void _reset() override;
};

class ActionOpacityBy :
	public Animation
{
public:
	ActionOpacityBy(float duration, float opacity);
	virtual ~ActionOpacityBy();

	virtual ActionOpacityBy * copy() const override;
	virtual ActionOpacityBy * reverse() const override;

protected:
	float m_nBeginVal;
	float m_nVariation;

protected:
	virtual void _init() override;
	virtual void _exec(LARGE_INTEGER nNow) override;
	virtual void _reset() override;
};

class ActionOpacityTo :
	public ActionOpacityBy
{
public:
	ActionOpacityTo(float duration, float opacity);
	virtual ~ActionOpacityTo();

	virtual ActionOpacityTo * copy() const override;

protected:
	float m_nEndVal;

protected:
	virtual void _init() override;
	virtual void _reset() override;
};

class ActionFadeIn :
	public ActionOpacityTo
{
public:
	ActionFadeIn(float duration) : ActionOpacityTo(duration, 1) {}
};

class ActionFadeOut :
	public ActionOpacityTo
{
public:
	ActionFadeOut(float duration) : ActionOpacityTo(duration, 0) {}
};

class ActionTwo :
	public Action
{
public:
	ActionTwo(Action * actionFirst, Action * actionSecond);
	virtual ~ActionTwo();

	virtual ActionTwo * copy() const override;
	virtual ActionTwo * reverse(bool actionReverse = true) const;

protected:
	Action *	m_FirstAction;
	Action *	m_SecondAction;

protected:
	virtual void _init() override;
	virtual void _exec(LARGE_INTEGER nNow) override;
	virtual void _reset() override;
};

class ActionSequence :
	public Action
{
public:
	ActionSequence();
	ActionSequence(int number, Action * action1, ...);
	virtual ~ActionSequence();

	void addAction(Action * action);
	virtual ActionSequence * copy() const override;
	virtual ActionSequence * reverse(bool actionReverse = true) const;

protected:
	UINT					m_nActionIndex;
	std::vector<Action*>	m_vActions;

protected:
	virtual void _init() override;
	virtual void _exec(LARGE_INTEGER nNow) override;
	virtual void _reset() override;
};

class ActionDelay :
	public Action
{
public:
	ActionDelay(float duration);
	virtual ~ActionDelay();

	virtual ActionDelay * copy() const override;

protected:
	virtual void _init() override;
	virtual void _exec(LARGE_INTEGER nNow) override;
	virtual void _reset() override;
};

class ActionNeverStop :
	public Action
{
public:
	ActionNeverStop(Action * action);
	virtual ~ActionNeverStop();

	virtual ActionNeverStop * copy() const override;

protected:
	Action * m_Action;

protected:
	virtual void _init() override;
	virtual void _exec(LARGE_INTEGER nNow) override;
	virtual void _reset() override;
};

class ActionFrames :
	public Action
{
public:
	ActionFrames();
	ActionFrames(UINT frameDelay);
	~ActionFrames();

	void addFrame(Image * frame);
	virtual ActionFrames * copy() const override;
	virtual ActionFrames * reverse() const override;

protected:
	UINT				m_nFrameIndex;
	std::vector<Image*> m_vFrames;

protected:
	virtual void _init() override;
	virtual void _exec(LARGE_INTEGER nNow) override;
	virtual void _reset() override;
};

class ActionCallback :
	public Action
{
public:
	ActionCallback(const std::function<void()>& callback);
	~ActionCallback();

	virtual ActionCallback * copy() const override;

protected:
	std::function<void()> m_Callback;

protected:
	virtual void _init() override;
	virtual void _exec(LARGE_INTEGER nNow) override;
	virtual void _reset() override;
};

class FileUtils
{
public:
	// 获取系统的 AppData\Local 路径
	static TString getLocalAppDataPath();
	// 获取默认的保存路径
	static TString getDefaultSavePath();
	// 保存 int 型的值
	static void saveInt(LPCTSTR key, int value);
	// 保存 double 型的值
	static void saveDouble(LPCTSTR key, double value);
	// 保存 字符串 型的值（不要在 Unicode 字符集下保存中文字符）
	static void saveString(LPCTSTR key, TString value);
	// 获取 int 型的值（若不存在则返回 default 参数的值）
	static int getInt(LPCTSTR key, int default);
	// 获取 double 型的值（若不存在则返回 default 参数的值）
	static double getDouble(LPCTSTR key, double default);
	// 获取 字符串 型的值（若不存在则返回 default 参数的值）
	static TString geTString(LPCTSTR key, TString default);
	// 得到文件扩展名（小写）
	static TString getFileExtension(const TString& filePath);
	/**
	*  打开保存文件对话框，得到有效保存路径返回 true
	*  参数：返回文件路径的字符串，窗口标题，设置扩展名过滤，设置默认扩展名
	*/
	static bool getSaveFilePath(TString& path, LPCTSTR title = _T("保存到"), LPCTSTR defExt = NULL);
};

class MusicUtils
{
public:
	// 播放背景音乐
	static void playBackgroundMusic(TString pszFilePath, bool bLoop = true);
	// 停止背景音乐
	static void stopBackgroundMusic(bool bReleaseData = false);
	// 暂停背景音乐
	static void pauseBackgroundMusic();
	// 继续播放背景音乐
	static void resumeBackgroundMusic();
	// 从头播放背景音乐
	static void rewindBackgroundMusic();
	// 背景音乐是否正在播放
	static bool isBackgroundMusicPlaying();
	// 设置背景音乐音量，0 ~ 1.0f
	static void setBackgroundMusicVolume(float volume);

	// 播放音效
	static unsigned int playMusic(TString pszFilePath, bool loop = false);
	// 停止音效
	static void stopMusic(unsigned int nSoundId);
	// 预加载音效
	static void preloadMusic(TString pszFilePath);
	// 暂停音效
	static void pauseMusic(unsigned int nSoundId);
	// 继续播放音效
	static void resumeMusic(unsigned int nSoundId);
	// 卸载音效
	static void unloadMusic(LPCTSTR pszFilePath);
	// 设置特定音乐的音量，0 ~ 1.0f
	static void setVolume(TString pszFilePath, float volume);

	// 暂停所有音乐
	static void pauseAllMusics();
	// 继续播放所有音乐
	static void resumeAllMusics();
	// 停止所有音乐
	static void stopAllMusics();
	// 停止所有音乐，并释放内存
	static void end();
	// 设置总音量，0 ~ 1.0f
	static void setVolume(float volume);
};

class Timer
{
	friend App;
public:
	Timer(TString name, UINT ms, const TIMER_CALLBACK & callback);
	~Timer();

	// 启动定时器
	void start();
	// 停止定时器
	void stop();
	// 进入等待状态
	void wait();
	// 唤醒
	void notify();
	// 定时器是否正在运行
	bool isRunning();
	// 设置间隔时间
	void setInterval(UINT ms);
	// 设置回调函数
	void setCallback(const TIMER_CALLBACK& callback);
	// 设置定时器名称
	void setName(TString name);
	// 获取定时器间隔时间
	UINT getInterval() const;
	// 获取定时器名称
	TString getName() const;

	// 添加定时器
	static void addTimer(Timer * timer);
	// 添加定时器
	static void addTimer(TString name, UINT ms, const TIMER_CALLBACK & callback);
	// 启动特定定时器
	static void startTimer(TString name);
	// 停止特定定时器
	static void stopTimer(TString name);
	// 删除特定定时器
	static void delTimer(TString name);
	// 删除所有定时器
	static void clearAllTimers();

	// 继续绑定在场景上的所有定时器
	static void notifyAllSceneTimers(Scene* scene);
	// 停止绑定在场景上的所有定时器
	static void waitAllSceneTimers(Scene* scene);
	// 清除绑定在场景上的所有定时器
	static void clearAllSceneTimers(Scene* scene);

protected:
	bool			m_bRunning;
	bool			m_bWaiting;
	TString			m_sName;
	TIMER_CALLBACK	m_callback;
	LARGE_INTEGER	m_nLast;
	LARGE_INTEGER	m_nAnimationInterval;
	UINT			m_nMilliSeconds;
	Scene *			m_pParentScene;

private:
	static void __exec();
};

class ActionManager
{
	friend App;
	friend Sprite;
public:
	// 继续一个特定的动作
	static void startAction(Action * action);
	// 继续一个特定的动作
	static void resumeAction(Action * action);
	// 暂停一个特定的动作
	static void pauseAction(Action * action);
	// 停止一个特定的动作
	static void stopAction(Action * action);

	// 继续一个 Sprite 的所有动作
	static void startSpriteAllActions(Sprite * sprite);
	// 继续一个 Sprite 的所有动作
	static void resumeSpriteAllActions(Sprite * sprite);
	// 暂停一个 Sprite 的所有动作
	static void pauseSpriteAllActions(Sprite * sprite);
	// 停止一个 Sprite 的所有动作
	static void stopSpriteAllActions(Sprite * sprite);

	// 继续当前存在的所有动作
	static void startAllActions();
	// 继续当前存在的所有动作
	static void resumeAllActions();
	// 暂停当前存在的所有动作
	static void pauseAllActions();
	// 停止当前存在的所有动作
	static void stopAllActions();
	// 删除当前存在的所有动作
	static void clearAllActions();

	// 继续绑定在场景上的动作
	static void notifyAllSceneActions(Scene* scene);
	// 暂停绑定在场景上的动作
	static void waitAllSceneActions(Scene* scene);
	// 停止绑定在场景上的动作
	static void stopAllSceneActions(Scene* scene);

private:
	static void __exec();
	// 将一个动作添加进动作管理器
	static void addAction(Action * action);
};

}	// End of easy2d namespace


// Functions Declare

inline void SafeRelease(easy2d::Object * p) { if (p) p->release(); }
inline void SafeDelete(void * p) { if (p) delete p; }

using namespace easy2d;