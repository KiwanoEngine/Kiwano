#pragma once
#include "emacros.h"
#include "ecommon.h"

namespace e2d
{

class EGame;
class ERenderer;
class EObject;
class EScene;
class ENode;
class ETimer;
class EAction;
class EMusic;
class EShape;
class ETransition;
class EListenerPhysics;

// 对象管理器
class EObjectManager
{
	friend EGame;

public:
	// 将一个节点放入内存池
	static void add(
		e2d::EObject * nptr
	);

	// 通知内存池刷新
	static void notifyFlush();

private:
	// 刷新内存池
	static void __flush();
};


// 场景管理器
class ESceneManager
{
	friend EGame;
	friend ERenderer;

public:
	// 切换场景
	static void enterScene(
		EScene * scene,						/* 下一个场景的指针 */
		ETransition * transition = nullptr,	/* 场景切换动画 */
		bool saveCurrentScene = true		/* 是否保存当前场景 */
	);

	// 返回上一场景
	static void backScene(
		ETransition * transition = nullptr	/* 场景切换动画 */
	);

	// 清空保存的所有场景
	static void clearScene();

	// 获取当前场景
	static EScene * getCurrentScene();

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
class ETimerManager
{
	friend EGame;
	friend ENode;

public:
	// 绑定定时器到场景
	static void bindTimer(
		ETimer * timer,
		EScene * pParentScene
	);

	// 绑定定时器到节点
	static void bindTimer(
		ETimer * timer,
		ENode * pParentNode
	);

	// 启动具有相同名称的定时器
	static void startTimers(
		const EString &name
	);

	// 停止具有相同名称的定时器
	static void stopTimers(
		const EString &name
	);

	// 删除具有相同名称的定时器
	static void delTimers(
		const EString &name
	);

	// 启动绑定在场景及其子节点上的所有定时器
	static void startAllTimersBindedWith(
		EScene * pParentScene
	);

	// 停止绑定在场景及其子节点上的所有定时器
	static void stopAllTimersBindedWith(
		EScene * pParentScene
	);

	// 启动绑定在节点上的所有定时器
	static void startAllTimersBindedWith(
		ENode * pParentNode
	);

	// 停止绑定在节点上的所有定时器
	static void stopAllTimersBindedWith(
		ENode * pParentNode
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
		ENode * pParentNode
	);

	// 重置定时器状态
	static void __resetAllTimers();
};


// 动作管理器
class EActionManager
{
	friend EGame;
	friend ENode;

public:
	// 添加动作
	static void addAction(
		EAction * action
	);

	// 继续绑定在节点上的所有动作
	static void resumeAllActionsBindedWith(
		ENode * pTargetNode
	);

	// 暂停绑定在节点上的所有动作
	static void pauseAllActionsBindedWith(
		ENode * pTargetNode
	);

	// 停止绑定在节点上的所有动作
	static void stopAllActionsBindedWith(
		ENode * pTargetNode
	);

	// 继续所有动作
	static void resumeAllActions();

	// 暂停所有动作
	static void pauseAllActions();

	// 停止所有动作
	static void stopAllActions();

private:
	// 更新动画状态
	static void __update();

	// 清空绑定在节点上的所有动作
	static void __clearAllActionsBindedWith(
		ENode * pTargetNode
	);

	// 重置所有动作状态
	static void __resetAllActions();
};


// 音乐管理工具
class EMusicManager
{
	friend EGame;

public:
	// 添加音乐文件
	static bool add(
		const EString & strFilePath	/* 音乐文件路径 */
	);

	// 获取指定音乐的 EMusic 对象
	static EMusic * get(
		const EString & strFilePath	/* 音乐文件路径 */
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


class EShapeManager
{
	friend EGame;
	friend ENode;
	friend EShape;

private:
	// 更新形状
	static void __updateShape(
		EShape * pActiveShape
	);

	// 添加形状
	static void __addShape(
		EShape * pShape
	);

	// 删除已绑定的形状
	static void __delShape(
		EShape * pShape
	);
};

}