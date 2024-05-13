![Kiwano Logo](./logo/logo_text_h.png)

# Kiwano 游戏引擎

[![Build status](https://ci.appveyor.com/api/projects/status/frqh09om9ldaklr9/branch/master?svg=true)](https://ci.appveyor.com/project/Nomango/kiwano/branch/master)
[![GitHub release](https://img.shields.io/github/release/nomango/kiwano)](https://github.com/Nomango/Kiwano/releases/latest)
[![GitHub license](https://img.shields.io/github/license/nomango/kiwano)](https://github.com/Nomango/Kiwano/blob/master/LICENSE)

[English](./README.md) | 简体中文

## 介绍

Kiwano 是一个使用 C++ 开发的 2D 游戏引擎，目前仅支持 Windows 平台。

Kiwano-Core 是一个提供了一系列实用工具的游戏无关库，它的目的是简化 C++ 开发过程。

这个仓库仍处于开发过程中，我创建这个仓库用来学习游戏引擎知识和开发自己的小游戏。

你可以到 [Kiwano Demos](https://github.com/kiwanogame/KiwanoDemos) 仓库查看和学习如何使用 Kiwano 引擎实现小游戏。

欢迎您任何形式的贡献。

## 功能

* 舞台和角色管理
* 舞台过渡动画
* 动作行为
* 按钮等简易UI元素
* 音频支持
* 网络通信支持
* 数据持久化
* 物理引擎 (基于 Box2D)
* GUI 引擎 (基于 ImGui)

## 安装

### 开发环境

- Win8 或更高 (推荐 Win10)
- Visual Studio 2015 或更高

### 通过 NuGet 安装

1. 打开你的 Visual Studio 解决方案
2. 在解决方案资源管理器, 右击 `引用` 并选择 `管理 NuGet 程序包`
3. 选择 `浏览` 选项卡, 搜索 `kiwano`, 选中列表中的包然后点击 `安装`
4. 开始使用 Kiwano 进行开发吧!

### 通过源代码安装

1. 从 Github 仓库克隆或下载源代码
2. 打开你的 Visual Studio 解决方案, 在解决方案资源管理器中右键你的解决方案, 选择 `添加` => `现有项`
3. 选中源代码目录下 /projects 文件夹中所有的 `.vcxproj` 文件，并确认添加
4. 右键你的项目，打开 `属性`, 选中 C\C++ => 常规, 并将源代码文件夹下的 src 目录添加到 `附加包含目录` 中
5. 右键你的项目 `引用` 并选择 `添加引用`, 选中 `kiwano` 项目和其他你需要的项目
6. 开始使用 Kiwano 进行开发吧!

## Star History

[![Star History Chart](https://api.star-history.com/svg?repos=kiwanoengine/kiwano&type=Date)](https://star-history.com/#kiwanoengine/kiwano&Date)

## 开发计划

* 跨平台支持
* 粒子系统

## 社交媒体

* 网站: [kiwanoengine.com](https://kiwanoengine.com)
* QQ群: 608406540
