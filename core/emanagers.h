#pragma once
#include "emacros.h"
#include "ecommon.h"

namespace e2d
{

class Game;
class Renderer;
class Object;
class Scene;
class Node;
class Timer;
class Action;
class Music;
class Shape;
class Transition;

// 对象管理器
class ObjectManager
{
	friend Game;

public:
	// 将一个对象放入内存池
	static void add(
		Object * nptr
	);

	// 释放垃圾对象的内存空间
	static void clear();

private:
	// 更新对象管理器
	static void __update();

	// 清空所有对象
	static void __uninit();
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
	// 等待一段时间后执行指定函数
	static void start(
		double timeOut,	/* 等待的时长（秒） */
		Function func	/* 执行的函数 */
	);

	// 启动具有相同名称的定时器
	static void start(
		String&name
	);

	// 停止具有相同名称的定时器
	static void stop(
		String&name
	);

	// 删除具有相同名称的定时器
	static void clear(
		String&name
	);

	// 获取名称相同的定时器
	static std::vector<Timer*> get(
		String& name
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

	// 添加一个定时器
	static void __add(
		Timer * pTimer
	);

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
		String& strActionName
	);

	// 暂停名称相同的所有动作
	static void pause(
		String& strActionName
	);

	// 停止名称相同的所有动作
	static void stop(
		String& strActionName
	);

	// 继续所有动作
	static void resumeAll();

	// 暂停所有动作
	static void pauseAll();

	// 停止所有动作
	static void stopAll();

	// 获取所有名称相同的动作
	static std::vector<Action *> get(
		String& strActionName
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
};


// 音乐管理工具
class MusicManager
{
	friend Game;

public:
	// 预加载音乐资源
	static bool preload(
		String& strFilePath	/* 音乐文件路径 */
	);

	// 播放音乐
	static bool play(
		String& strFilePath,	/* 音乐文件路径 */
		int nLoopCount = 0			/* 重复播放次数，设置 -1 为循环播放 */
	);

	// 暂停音乐
	static void pause(
		String& strFilePath	/* 音乐文件路径 */
	);

	// 继续播放音乐
	static void resume(
		String& strFilePath	/* 音乐文件路径 */
	);

	// 停止音乐
	static void stop(
		String& strFilePath	/* 音乐文件路径 */
	);

	// 获取指定音乐的 Music 对象
	static Music * get(
		String& strFilePath	/* 音乐文件路径 */
	);

	// 暂停所有音乐
	static void pauseAll();

	// 继续播放所有音乐
	static void resumeAll();

	// 停止所有音乐
	static void stopAll();

	// 获取 IXAudio2 对象
	static IXAudio2 * getIXAudio2();

	// 获取 IXAudio2MasteringVoice 对象
	static IXAudio2MasteringVoice * getIXAudio2MasteringVoice();

private:
	// 初始化 XAudio2
	static bool __init();

	// 回收相关资源
	static void __uninit();
};


class ShapeManager
{
	friend Game;
	friend Node;
	friend Shape;

public:
	// 开启或禁用碰撞触发 onCollide 函数
	static void setCollisionEnable(
		bool bEnable
	);

private:
	// 更新形状
	static void __updateShape(
		Shape * pActiveShape
	);

	// 添加形状
	static void __add(
		Shape * pShape
	);

	// 删除已绑定的形状
	static void __remove(
		Shape * pShape
	);
};

}