/******************************************************
* Easy2D Game Engine (v1.0.0)
* http://www.easy2d.cn
* 
* Depends on EasyX (Ver:20170827(beta))
******************************************************/

#pragma once

#ifndef __cplusplus
	#error Easy2D is only for C++
#endif


// String macros

#ifdef UNICODE
	#define tstring std::wstring
#else
	#define tstring std::string
#endif


// Safe macros

#define SAFE_DELETE(p)			{ delete (p); (p) = nullptr; }
#define SAFE_DELETE_ARRAY(p)	{ if (p) { delete[] (p); (p) = nullptr; } }
#define SAFE_RELEASE(p)			{ if (p) p->release(); }


#include <windows.h>
#include <tchar.h>
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


// Class Declare

namespace easy2d {

// 基础类
class Application;
class Scene;
class KeyMsg;
class MouseMsg;
class FreePool;
// 样式类
class Color;
class FontStyle;
// 工具类
class Timer;
class MusicUtils;
class FileUtils;
// 对象
class Object;
class Node;
class BatchNode;
class MouseNode;
class Image;
class Text;
class Shape;
class Rect;
class Circle;
class Button;
class TextButton;
class ImageButton;


typedef unsigned int				VK_KEY;
typedef std::function<void()>		CLICK_CALLBACK;
typedef std::function<void()>		TIMER_CALLBACK;
typedef std::function<void(VK_KEY)>	KEY_CALLBACK;


class Application
{
protected:
	tstring				m_sTitle;
	Scene*				m_currentScene;
	Scene*				m_nextScene;
	std::stack<Scene*>	m_sceneStack;
	LARGE_INTEGER		m_nAnimationInterval;
	int					m_nWidth;
	int					m_nHeight;
	int					m_nWindowMode;
	bool				m_bRunning;
	bool				m_bPause;
	bool				m_bSaveScene;

protected:
	void _initGraph();
	void _mainLoop();
	void _enterNextScene();

public:
	Application();
	~Application();

	// 窗口可选模式
	enum { SHOW_CONSOLE = 1, NO_CLOSE = 2, NO_MINI_MIZE = 4 };
	// 获取程序实例
	static Application * get();
	// 设置坐标原点
	static void setOrigin(int originX, int originY);
	// 获取坐标原点的物理横坐标
	static int getOriginX();
	// 获取坐标原点的物理纵坐标
	static int getOriginY();
	// 启动程序
	int run();
	// 暂停程序
	void pause();
	// 终止程序
	void quit();
	// 终止程序
	void end();
	// 定义绘图窗口
	void createWindow(int width, int height, int mode = 0);
	// 定义绘图窗口
	void createWindow(tstring title, int width, int height, int mode = 0);
	// 修改窗口大小
	void setWindowSize(int width, int height);
	// 设置窗口标题
	void setWindowText(tstring title);
	// 关闭窗口
	void close();
	// 获取窗口宽度
	int getWidth() const;
	// 获取窗口高度
	int getHeight() const;
	// 切换场景
	void enterScene(Scene *scene, bool save = true);
	// 返回上一场景
	void backScene();
	// 游戏是否正在运行
	bool isRunning();
	// 设置帧率
	void setFPS(DWORD fps);
	// 重置绘图样式为默认值
	void reset();
	// 释放所有内存资源
	void free();
	// 销毁该对象
	void destory();
	// 获取当前场景
	Scene * getCurrentScene();
	// 获取 Easy2D 版本号
	LPCTSTR getVersion();
};

class FreePool
{
	friend class Application;

private:
	static void __flush();

public:
	// 将一个节点放入释放池
	static void add(Object * nptr);
};

class Scene
{
	friend class Application;
	friend class MouseMsg;

protected:
	std::vector<Node*> m_vChildren;

protected:
	void _exec();
	void _onDraw();

public:
	Scene();
	~Scene();

	// 添加子成员到场景
	void add(Node * child, int zOrder = 0);
	// 删除子成员
	bool del(Node * child);
	// 清空所有子成员
	void clearAllChildren();
};


class MouseMsg
{
	friend class Application;

private:
	static void __exec();

public:
	UINT uMsg;			// 当前鼠标消息
	bool mkLButton;		// 鼠标左键是否按下
	bool mkMButton;		// 鼠标中键是否按下
	bool mkRButton;		// 鼠标右键是否按下
	short x;			// 当前鼠标 x 坐标
	short y;			// 当前鼠标 y 坐标
	short wheel;		// 鼠标滚轮滚动值 (120 的倍数)

public:
	// 获取当前鼠标消息
	static MouseMsg getMsg();
	// 左键是否按下
	static bool getLButtonDown();
	// 右键是否按下
	static bool getRButtonDown();
	// 中键是否按下
	static bool getMButtonDown();
	// 获取鼠标X坐标
	static int getMouseX();
	// 获取鼠标Y坐标
	static int getMouseY();
	// 获取鼠标滚轮值
	static int getMouseWheel();
	// 鼠标移动消息
	static bool getMouseMovedMsg();
	// 左键双击消息
	static bool getLButtonDBClickedMsg();
	// 右键按下消息
	static bool getLButtonDownMsg();
	// 左键弹起消息
	static bool getLButtonUpMsg();
	// 右键双击消息
	static bool getRButtonDBClicked();
	// 右键按下消息
	static bool getRButtonDownMsg();
	// 右键弹起消息
	static bool getRButtonUpMsg();
	// 中键双击消息
	static bool getMButtonDBClicked();
	// 中键按下消息
	static bool getMButtonDownMsg();
	// 中键弹起消息
	static bool getMButtonUpMsg();
	// 鼠标滚轮拨动消息
	static bool getWheelMsg();
	// 清空鼠标消息
	static void resetMouseMsg();
};


class KeyMsg
{
	friend class Application;

public:
	// 字母键值
	static const VK_KEY A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z;
	// 数字键值
	static const VK_KEY NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, NUM_0;
	// 小数字键盘值
	static const VK_KEY NUMPAD_1, NUMPAD_2, NUMPAD_3, NUMPAD_4, NUMPAD_5, NUMPAD_6, NUMPAD_7, NUMPAD_8, NUMPAD_9, NUMPAD_0, Decimal;
	// 控制键值
	static const VK_KEY Enter, Space, Up, Down, Left, Right, Esc, Shift, LShift, RShift, Ctrl, LCtrl, RCtrl;
	// F 键值
	static const VK_KEY F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12;

private:
	static void __exec();

protected:
	tstring m_sName;
	KEY_CALLBACK m_callback;

public:
	KeyMsg(tstring name, const KEY_CALLBACK& callback);
	~KeyMsg();

	// 执行回调函数
	void onKbHit(VK_KEY key);

	// 添加键盘监听
	static void addListener(tstring name, const KEY_CALLBACK& callback);
	// 删除键盘监听
	static bool delListener(tstring name);
	// 删除所有键盘监听
	static void clearAllListener();
	// 判断键是否被按下，按下返回true
	static bool isKeyDown(VK_KEY key);
};


class FileUtils
{
public:
	// 得到文件扩展名（小写）
	static tstring getFileExtension(const tstring& filePath);
	/**
	*  打开保存文件对话框，得到有效保存路径返回 true
	*  参数：返回文件路径的字符串，窗口标题，设置扩展名过滤，设置默认扩展名
	*/
	static bool getSaveFilePath(tstring& path, LPCTSTR title = _T("保存到"), LPCTSTR defExt = NULL);
};


class MusicUtils
{
public:
	// 播放背景音乐
	static void playBackgroundMusic(tstring pszFilePath, bool bLoop = false);
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

	// 播放音效
	static unsigned int playMusic(tstring pszFilePath, bool loop = false);
	// 停止音效
	static void stopMusic(unsigned int nSoundId);
	// 预加载音效
	static void preloadMusic(tstring pszFilePath);
	// 暂停音效
	static void pauseMusic(unsigned int nSoundId);
	// 继续播放音效
	static void resumeMusic(unsigned int nSoundId);
	// 卸载音效
	static void unloadMusic(LPCTSTR pszFilePath);

	// 暂停所有音乐
	static void pauseAllMusics();
	// 继续播放所有音乐
	static void resumeAllMusics();
	// 停止所有音乐
	static void stopAllMusics();
	// 停止所有音乐，并释放内存
	static void end();
};


class Timer
{
	friend class Application;

protected:
	bool			m_bRunning;
	tstring			m_sName;
	TIMER_CALLBACK	m_callback;
	LARGE_INTEGER	m_nLast;
	LARGE_INTEGER	m_nAnimationInterval;
	UINT			m_nMilliSeconds;

private:
	static void __exec();

public:
	Timer(tstring name, UINT ms, const TIMER_CALLBACK & callback);
	~Timer();

	// 启动定时器
	void start();
	// 停止定时器
	void stop();
	// 定时器是否正在运行
	bool isRunning();
	// 设置间隔时间
	void setInterval(UINT ms);
	// 设置回调函数
	void setCallback(const TIMER_CALLBACK& callback);
	// 设置定时器名称
	void setName(tstring name);

	// 获取定时器间隔时间
	UINT getInterval() const;
	// 获取定时器名称
	tstring getName() const;

	// 添加定时器
	static void addTimer(Timer * timer);
	// 添加定时器
	static void addTimer(tstring name, UINT ms, const TIMER_CALLBACK & callback);
	// 根据名称获取定时器
	static Timer * getTimer(tstring name);
	// 启动特定定时器
	static bool startTimer(tstring name);
	// 停止特定定时器
	static bool stopTimer(tstring name);
	// 删除特定定时器
	static bool deleteTimer(tstring name);
	// 删除所有定时器
	static void clearAllTimers();
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


class Object
{
	friend class FreePool;

protected:
	int m_nRef;

public:
	Object();
	virtual ~Object();

	void retain();
	void release();
};


class FontStyle :
	public virtual Object
{
	friend class Text;

protected:
	LOGFONT m_font;

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


class Node :
	public virtual Object
{
	friend class Scene;
	friend class Layer;
	friend class BatchNode;

protected:
	int		m_nZOrder;
	bool	m_bDisplay;
	Scene*	m_pScene;
	int		m_nX;
	int		m_nY;

protected:
	virtual bool _exec(bool active);
	virtual void _onDraw() = 0;
	void setParentScene(Scene * scene);

public:
	Node();
	Node(int x, int y);
	virtual ~Node();

	// 获取节点横坐标
	const int getX() const;
	// 获取节点纵坐标
	const int getY() const;
	// 设置节点横坐标
	virtual void setX(int x);
	// 设置节点纵坐标
	virtual void setY(int y);
	// 设置节点横纵坐标
	virtual void setPos(int x, int y);
	// 移动节点
	virtual void move(int x, int y);
	// 节点是否显示
	bool display() const;
	// 设置节点是否显示
	void setDisplay(bool value);
	// 获取节点绘图顺序
	virtual int getZOrder() const;
	// 设置节点绘图顺序（0为最先绘制，显示在最底层）
	virtual void setZOrder(int z);
	// 获取节点所在场景
	Scene * getParentScene();
};


class BatchNode :
	public virtual Node
{
protected:
	std::vector<Node*> m_vChildren;

protected:
	virtual bool _exec(bool active) override;
	virtual void _onDraw() override;

public:
	BatchNode();
	virtual ~BatchNode();

	// 添加子节点
	void add(Node *child, int z_Order = 0);
	// 删除子节点
	bool del(Node * child);
	// 清空所有子节点
	void clearAllChildren();
};


class Image :
	public virtual Node
{
	friend class ImageButton;

protected:
	CImage m_Image;
	CRect m_rDest;
	CRect m_rSrc;
	float m_fScaleX;
	float m_fScaleY;

protected:
	virtual void _onDraw() override;

public:
	Image();
	/**
	*  从图片文件获取图像(png/bmp/jpg/gif/emf/wmf/ico)
	*  参数：图片文件名，图片裁剪坐标，图片裁剪宽度和高度
	*/
	Image(LPCTSTR ImageFile, int x = 0, int y = 0, int width = 0, int height = 0);
	virtual ~Image();

	// 获取图像宽度
	int getWidth() const;
	// 获取图像高度
	int getHeight() const;
	// 获取横向拉伸比例
	float getScaleX() const;
	// 获取纵向拉伸比例
	float getScaleY() const;

	/**
	*  从图片文件获取图像(png/bmp/jpg/gif/emf/wmf/ico)
	*  参数：图片文件名，图片裁剪坐标，图片裁剪宽度和高度
	*/
	void setImageFile(LPCTSTR ImageFile, int x = 0, int y = 0, int width = 0, int height = 0);
	/**
	*  从资源文件获取图像，不支持 png (bmp/jpg/gif/emf/wmf/ico)
	*  参数：资源名称，图片裁剪坐标，图片裁剪宽度和高度
	*/
	void setImageRes(LPCTSTR pResName, int x = 0, int y = 0, int width = 0, int height = 0);
	// 裁剪图片（裁剪后会恢复 stretch 拉伸）
	void crop(int x = 0, int y = 0, int width = 0, int height = 0);
	// 将图片拉伸到固定宽高
	void stretch(int width = 0, int height = 0);
	// 按比例拉伸图片（0 ~ 1.0f）
	void scale(float scaleX, float scaleY);
	// 设置图片位置
	void setPos(int x, int y) override;
	// 移动图片
	void move(int x, int y) override;
	// 设置图片横坐标
	void setX(int x) override;
	// 设置图片纵坐标
	void setY(int y) override;
	// 设置透明色
	void setTransparentColor(COLORREF value);
	// 保存到截图
	static void screenshot();
};


class Text :
	public virtual Node
{
	friend class TextButton;

protected:
	tstring		m_sText;
	COLORREF	m_color;
	FontStyle *	m_pFontStyle;

protected:
	virtual void _onDraw() override;

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
	// 获取文字宽度
	int getWidth();
	// 获取文字高度
	int getHeight();
	// 文本是否为空
	bool isEmpty() const;

	// 设置文字
	void setText(tstring text);
	// 设置文字颜色
	void setColor(COLORREF color);
	// 设置字体
	void setFontStyle(FontStyle * style);
};


class MouseNode :
	public virtual Node
{
private:
	bool m_bTarget;
	bool m_bBlock;
	enum { NORMAL, MOUSEIN, SELECTED } m_eStatus;
	CLICK_CALLBACK m_callback;

protected:
	virtual bool _exec(bool active) override;
	virtual void _onDraw() override;

	void _setNormal();
	void _setMouseIn();
	void _setSelected();

	// 重写该函数，实现鼠标位置的判定
	virtual void _judge() = 0;
	// 正常状态
	virtual void _onNormal() = 0;
	// 鼠标移入时
	virtual void _onMouseIn() = 0;
	// 鼠标选中时
	virtual void _onSelected() = 0;

public:
	MouseNode();
	virtual ~MouseNode();

	// 鼠标点击时
	virtual void onClicked();
	// 鼠标是否移入
	virtual bool isMouseIn();
	// 鼠标是否选中
	virtual bool isSelected();
	// 设置回调函数
	virtual void setOnMouseClicked(const CLICK_CALLBACK & callback);
	// 重置状态
	virtual void reset();
	// 设置节点是否阻塞鼠标消息
	void setBlock(bool block);
};


class Button :
	public virtual MouseNode
{
protected:
	int m_nWidth;
	int m_nHeight;
	bool m_bEnable;

protected:
	virtual bool _exec(bool active) override;
	virtual void _onDraw() override;
	virtual void _judge() override;

	virtual void _onNormal() = 0;
	virtual void _onMouseIn() = 0;
	virtual void _onSelected() = 0;
	virtual void _onDisable() = 0;

public:
	Button();
	virtual ~Button();

	// 按钮是否启用
	virtual bool isEnable();
	// 设置是否启用
	virtual void setEnable(bool enable);
};



class TextButton :
	public virtual Button
{
protected:
	Text * m_pNormalText;
	Text * m_pMouseInText;
	Text * m_pSelectedText;
	Text * m_pUnableText;

protected:
	// 重置文字位置（居中显示）
	void resetTextPosition();

	virtual void _onNormal() override;
	virtual void _onMouseIn() override;
	virtual void _onSelected() override;
	virtual void _onDisable() override;

public:
	TextButton();
	TextButton(Text * text);
	virtual ~TextButton();

	// 设置按钮文字
	void setNormalText(Text * text);
	// 设置鼠标移入时的按钮文字
	void setMouseInText(Text * text);
	// 设置鼠标选中时的按钮文字
	void setSelectedText(Text * text);
	// 设置按钮禁用时的按钮文字
	void setUnableText(Text * text);

	// 设置按钮横坐标
	virtual void setX(int x) override;
	// 设置按钮纵坐标
	virtual void setY(int y) override;
	// 设置按钮横纵坐标
	virtual void setPos(int x, int y) override;
};



class ImageButton :
	public virtual Button
{
protected:
	Image * m_pNormalImage;
	Image * m_pMouseInImage;
	Image * m_pSelectedImage;
	Image * m_pUnableImage;

protected:
	// 重置图片位置（居中显示）
	void resetImagePosition();

	virtual void _onNormal() override;
	virtual void _onMouseIn() override;
	virtual void _onSelected() override;
	virtual void _onDisable() override;

public:
	ImageButton();
	ImageButton(Image * image);
	virtual ~ImageButton();

	// 设置按钮图片
	void setNormalImage(Image * image);
	// 设置鼠标移入时的按钮图片
	void setMouseInImage(Image * image);
	// 设置鼠标选中时的按钮图片
	void setSelectedImage(Image * image);
	// 设置按钮禁用时的按钮图片
	void setUnableImage(Image * image);

	// 设置按钮横坐标
	virtual void setX(int x) override;
	// 设置按钮纵坐标
	virtual void setY(int y) override;
	// 设置按钮横纵坐标
	virtual void setPos(int x, int y) override;
};


class Shape :
	public virtual Node
{
protected:
	enum STYLE { round, solid, fill };	// 形状填充样式
	STYLE		_style;
	COLORREF	fillColor = 0;
	COLORREF	lineColor = 0;

protected:
	virtual void _onDraw() override;
	virtual void solidShape() = 0;
	virtual void fillShape() = 0;
	virtual void roundShape() = 0;

public:
	Shape();
	virtual ~Shape();

	// 获取形状的填充颜色
	COLORREF getFillColor() const;
	// 获取形状的线条颜色
	COLORREF getLineColor() const;
	// 设置填充颜色
	void setFillColor(COLORREF color);
	// 设置线条颜色
	void setLineColor(COLORREF color);
};


class Rect :
	public virtual Shape
{
protected:
	int m_nWidth;
	int m_nHeight;

protected:
	virtual void solidShape() override;
	virtual void fillShape() override;
	virtual void roundShape() override;

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
};


class Circle :
	public virtual Shape
{
protected:
	int m_nRadius;

protected:
	virtual void solidShape() override;
	virtual void fillShape() override;
	virtual void roundShape() override;

public:
	Circle();
	Circle(int x, int y, int radius);
	virtual ~Circle();

	// 获取圆形半径
	int getRadius() const;
	// 设置圆形半径
	void setRadius(int m_nRadius);
};

}	// End of easy2d namespace


// 默认使用 easy2d 命名空间
using namespace easy2d;