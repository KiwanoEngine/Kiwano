#pragma once
#include <Windows.h>
#include <tchar.h>
#include <atltypes.h>
#include <vector>
#include <stack>
#include <functional>

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


// Type Declare

typedef CPoint						CVector;
typedef unsigned int				VK_KEY;
typedef std::function<void()>		CLICK_CALLBACK;
typedef std::function<void()>		TIMER_CALLBACK;
typedef std::function<void(VK_KEY)>	KEY_CALLBACK;
typedef std::function<void()>		MOUSE_CALLBACK;


// Classes Declare

namespace easy2d
{
	class Scene;
	class Node;

	class App
	{
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
		void createWindow(tstring title, int width, int height, int mode = 0);
		// 定义绘图窗口
		void createWindow(tstring title, CSize size, int mode = 0);
		// 启动程序
		int run();
		// 释放所有内存资源
		void free();
		// 销毁该对象
		void destory();

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
		static void setWindowTitle(tstring title);
		// 获取窗口标题
		static tstring getWindowTitle();
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
		static void setAppName(tstring appname);
		// 获取 AppName
		static tstring getAppName();
		// 修改窗口背景色
		static void setBkColor(COLORREF color);
		// 设置帧率
		static void setFPS(DWORD fps);
		// 重置绘图样式为默认值
		static void reset();
		// 获取当前场景
		static Scene * getCurrentScene();

	protected:
		tstring				m_sTitle;
		tstring				m_sAppName;
		Scene*				m_CurrentScene;
		Scene*				m_NextScene;
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
		friend class App;
		friend class Object;

	private:
		// 刷新内存池
		static void __flush();
		// 将一个节点放入释放池
		static void __add(Object * nptr);
	};

	class Scene
	{
		friend class App;
		friend class MouseMsg;

	public:
		Scene();
		~Scene();

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
		friend class FreePool;

	public:
		Object();
		virtual ~Object();

		void retain();
		void release();

	protected:
		int m_nRef;
	};

}	// End of easy2d namespace