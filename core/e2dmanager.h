#pragma once
#include "e2dmacros.h"
#include "e2dcommon.h"

namespace e2d
{

class Game;
class Input;
class Renderer;
class Node;
class Timer;
class Action;
class Music;
class Collider;
class Transition;
class InputListener;
class CollisionListener;

// 对象管理器
class ObjectManager
{
	friend Game;
	friend Object;

public:
	// 释放垃圾对象的内存空间
	static void flush();

private:
	// 将对象放入内存池进行管理
	static void __add(
		Object * pObject
	);

	// 更新对象管理器
	static void __update();

	// 清空所有对象
	static void __clear();
};


// 场景管理器
class SceneManager
{
	friend Game;
	friend Renderer;

public:
	// 切换场景
	static void enter(
		Scene * scene,						/* 下一个场景的指针 */
		Transition * transition = nullptr,	/* 场景切换动画 */
		bool saveCurrentScene = true		/* 是否保存当前场景 */
	);

	// 返回上一场景
	static void back(
		Transition * transition = nullptr	/* 场景切换动画 */
	);

	// 清空保存的所有场景
	static void clear();

	// 获取当前场景
	static Scene * getCurrentScene();

	// 获取场景栈
	static std::stack<Scene*> getSceneStack();

	// 是否正在进行转场动画
	static bool isTransitioning();

private:
	// 更新场景内容
	static void __update();

	// 渲染场景画面
	static void __render();

	// 初始化场景
	static bool __init();

	// 回收场景资源
	static void __uninit();
};


// 定时器管理器
class TimerManager
{
	friend Game;
	friend Node;
	friend Timer;

public:
	// 启动一个新任务
	static Timer* start(
		String name,				/* 任务名称 */
		Function func,				/* 执行函数 */
		double interval = 0,		/* 时间间隔（秒） */
		int times = -1,				/* 执行次数（设 -1 为永久执行） */
		bool atOnce = false,		/* 是否立即执行 */
		bool autoRelease = false	/* 自动清除 */
	);

	// 启动一个新任务：等待一段时间后执行指定函数
	static Timer* start(
		double timeOut,	/* 等待的时长（秒） */
		Function func	/* 执行的函数 */
	);

	// 启动具有相同名称的定时器
	static void start(
		String name
	);

	// 停止具有相同名称的定时器
	static void stop(
		String name
	);

	// 删除具有相同名称的定时器
	static void clear(
		String name
	);

	// 添加定时器
	static void add(
		Timer * pTimer
	);

	// 获取名称相同的定时器
	static std::vector<Timer*> get(
		String name
	);

	// 启动所有定时器
	static void startAll();

	// 停止所有定时器
	static void stopAll();

	// 停止并清除所有定时器
	static void stopAndClearAll();

	// 获取所有定时器
	static std::vector<Timer*> getAll();

private:
	// 更新定时器
	static void __update();

	// 重置定时器状态
	static void __resetAllTimers();

	// 清空定时器
	static void __uninit();
};


// 动作管理器
class ActionManager
{
	friend Game;
	friend Node;
	friend Action;

public:
	// 继续名称相同的所有动作
	static void resume(
		String strActionName
	);

	// 暂停名称相同的所有动作
	static void pause(
		String strActionName
	);

	// 停止名称相同的所有动作
	static void stop(
		String strActionName
	);

	// 继续所有动作
	static void resumeAll();

	// 暂停所有动作
	static void pauseAll();

	// 停止所有动作
	static void stopAll();

	// 获取所有名称相同的动作
	static std::vector<Action *> get(
		String strActionName
	);

	// 获取所有动作
	static std::vector<Action*> getAll();

private:
	// 更新动画状态
	static void __update();

	// 添加动作
	static void __add(
		Action * pAction
	);

	// 删除动作
	static void __remove(
		Action * pAction
	);

	// 执行动作
	static void __startAction(
		Action * pAction,
		Node * pTargetNode
	);

	// 继续绑定在节点上的所有动作
	static void __resumeAllBindedWith(
		Node * pTargetNode
	);

	// 暂停绑定在节点上的所有动作
	static void __pauseAllBindedWith(
		Node * pTargetNode
	);

	// 停止绑定在节点上的所有动作
	static void __stopAllBindedWith(
		Node * pTargetNode
	);

	// 清空绑定在节点上的所有动作
	static void __clearAllBindedWith(
		Node * pTargetNode
	);

	// 重置所有动作状态
	static void __resetAllActions();

	// 回收资源
	static void __uninit();
};


// 音乐管理工具
class MusicManager
{
	friend Game;

public:
	// 预加载音乐资源
	static bool preload(
		String strFilePath	/* 音乐文件路径 */
	);

	// 播放音乐
	static bool play(
		String strFilePath,	/* 音乐文件路径 */
		int nLoopCount = 0	/* 重复播放次数，设置 -1 为循环播放 */
	);

	// 暂停音乐
	static void pause(
		String strFilePath	/* 音乐文件路径 */
	);

	// 继续播放音乐
	static void resume(
		String strFilePath	/* 音乐文件路径 */
	);

	// 停止音乐
	static void stop(
		String strFilePath	/* 音乐文件路径 */
	);

	// 获取指定音乐的 Music 对象
	static Music * get(
		String strFilePath	/* 音乐文件路径 */
	);

	// 暂停所有音乐
	static void pauseAll();

	// 继续播放所有音乐
	static void resumeAll();

	// 停止所有音乐
	static void stopAll();

#if HIGHER_THAN_VS2010

	// 获取 IXAudio2 对象
	static IXAudio2 * getIXAudio2();

	// 获取 IXAudio2MasteringVoice 对象
	static IXAudio2MasteringVoice * getIXAudio2MasteringVoice();

#else

	// 获取 HINSTANCE
	static HINSTANCE getHInstance();

#endif

private:
	// 初始化音乐播放器
	static bool __init();

	// 回收相关资源
	static void __uninit();
};


// 键盘和鼠标消息管理器
class InputManager
{
	friend Game;
	friend Input;
	friend InputListener;

public:
	// 添加输入监听
	static void add(
		Function func,		/* 监听到用户输入时的执行函数 */
		String name = L""	/* 监听器名称 */
	);

	// 启动输入监听
	static void start(
		String name
	);

	// 停止输入监听
	static void stop(
		String name
	);

	// 清除输入监听
	static void clear(
		String name
	);

	// 启动所有监听器
	static void startAll();

	// 停止所有监听器
	static void stopAll();

	// 清除所有监听器
	static void clearAll();

	// 获取监听器
	static std::vector<InputListener*> get(
		String name
	);

	// 获取全部监听器
	static std::vector<InputListener*> getAll();

private:
	// 添加输入监听
	static void __add(
		InputListener * pListener
	);

	// 更新监听器
	static void __update();

	// 回收资源
	static void __uninit();
};


// 碰撞管理器
class ColliderManager
{
	friend Game;
	friend Node;
	friend Collider;
	friend CollisionListener;

public:
	// 开启或关闭碰撞监听功能（默认关闭）
	static void setEnable(
		bool bEnable
	);

	// 添加碰撞监听
	static void add(
		Function func,		/* 监听到碰撞时的执行函数 */
		String name = L""	/* 监听器名称 */
	);

	// 启动碰撞监听
	static void start(
		String name
	);

	// 停止碰撞监听
	static void stop(
		String name
	);

	// 清除碰撞监听
	static void clear(
		String name
	);

	// 启动所有监听器
	static void startAll();

	// 停止所有监听器
	static void stopAll();

	// 清除所有监听器
	static void clearAll();

	// 获取监听器
	static std::vector<CollisionListener*> get(
		String name
	);

	// 获取全部监听器
	static std::vector<CollisionListener*> getAll();

	// 获取碰撞发生时的主动体
	static Node * getActiveNode();

	// 获取碰撞发生时的被动体
	static Node * getPassiveNode();

	// 判断碰撞是否由该节点引发（如果是，返回与其相撞的节点指针，否则返回空）
	static Node * isCausedBy(
		Node * pNode
	);

	// 判断发生碰撞的节点名称是否相同（若相同返回其指针，否则返回空）
	static Node * isCausedBy(
		String name
	);

private:
	// 添加碰撞监听
	static void __add(
		CollisionListener * pListener
	);

	// 更新监听器
	static void __update();

	// 更新碰撞体
	static void __updateCollider(
		Collider * pActiveCollider
	);

	// 添加碰撞体
	static void __addCollider(
		Collider * pCollider
	);

	// 删除已绑定的碰撞体
	static void __removeCollider(
		Collider * pCollider
	);

	// 回收资源
	static void __uninit();
};

}