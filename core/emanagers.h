#pragma once
#include "emacros.h"
#include "ecommon.h"

namespace e2d
{

class Game;
class Renderer;
class Obj;
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
	// 将一个节点放入内存池
	static void preload(
		e2d::Obj * nptr
	);

	// 通知内存池刷新
	static void notifyFlush();

private:
	// 刷新内存池
	static void __flush();
};


// 场景管理器
class SceneManager
{
	friend Game;
	friend Renderer;

public:
	// 切换场景
	static void enterScene(
		Scene * scene,						/* 下一个场景的指针 */
		Transition * transition = nullptr,	/* 场景切换动画 */
		bool saveCurrentScene = true		/* 是否保存当前场景 */
	);

	// 返回上一场景
	static void backScene(
		Transition * transition = nullptr	/* 场景切换动画 */
	);

	// 清空保存的所有场景
	static void clearScene();

	// 获取当前场景
	static Scene * getCurrentScene();

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

public:
	// 绑定定时器到场景
	static void preload(
		Timer * pTimer,
		Scene * pParentScene
	);

	// 绑定定时器到节点
	static void preload(
		Timer * pTimer,
		Node * pParentNode
	);

	// 启动具有相同名称的定时器
	static void startTimers(
		const String &name
	);

	// 停止具有相同名称的定时器
	static void stopTimers(
		const String &name
	);

	// 删除具有相同名称的定时器
	static void deleteTimers(
		const String &name
	);

	// 启动绑定在场景及其子节点上的所有定时器
	static void startAllTimersBindedWith(
		Scene * pParentScene
	);

	// 停止绑定在场景及其子节点上的所有定时器
	static void stopAllTimersBindedWith(
		Scene * pParentScene
	);

	// 启动绑定在节点上的所有定时器
	static void startAllTimersBindedWith(
		Node * pParentNode
	);

	// 停止绑定在节点上的所有定时器
	static void stopAllTimersBindedWith(
		Node * pParentNode
	);

	// 启动所有定时器
	static void startAllTimers();

	// 停止所有定时器
	static void stopAllTimers();

private:
	// 更新定时器
	static void __update();

	// 清空绑定在节点上的所有定时器
	static void __clearAllTimersBindedWith(
		Node * pParentNode
	);

	// 重置定时器状态
	static void __resetAllTimers();
};


// 动作管理器
class ActionManager
{
	friend Game;
	friend Node;
	friend Action;

public:
	// 继续绑定在节点上的所有动作
	static void resumeAllActionsBindedWith(
		Node * pTargetNode
	);

	// 暂停绑定在节点上的所有动作
	static void pauseAllActionsBindedWith(
		Node * pTargetNode
	);

	// 停止绑定在节点上的所有动作
	static void stopAllActionsBindedWith(
		Node * pTargetNode
	);

	// 继续所有动作
	static void resumeAllActions();

	// 暂停所有动作
	static void pauseAllActions();

	// 停止所有动作
	static void stopAllActions();

private:
	// 添加动作
	static void _add(
		Action * pAction,
		Node * pTargetNode
	);

	// 更新动画状态
	static void __update();

	// 清空绑定在节点上的所有动作
	static void __clearAllActionsBindedWith(
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
		const String & strFilePath	/* 音乐文件路径 */
	);

	// 播放音乐
	static bool play(
		const String & strFilePath,	/* 音乐文件路径 */
		int nLoopCount = 0			/* 重复播放次数，设置 -1 为循环播放 */
	);

	// 暂停音乐
	static void pause(
		const String & strFilePath	/* 音乐文件路径 */
	);

	// 继续播放音乐
	static void resume(
		const String & strFilePath	/* 音乐文件路径 */
	);

	// 停止音乐
	static void stop(
		const String & strFilePath	/* 音乐文件路径 */
	);

	// 获取指定音乐的 Music 对象
	static Music * get(
		const String & strFilePath	/* 音乐文件路径 */
	);

	// 暂停所有音乐
	static void pauseAllMusics();

	// 继续播放所有音乐
	static void resumeAllMusics();

	// 停止所有音乐
	static void stopAllMusics();

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

private:
	// 更新形状
	static void __updateShape(
		Shape * pActiveShape
	);

	// 添加形状
	static void __addShape(
		Shape * pShape
	);

	// 删除已绑定的形状
	static void __delShape(
		Shape * pShape
	);
};

}