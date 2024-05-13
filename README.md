![Kiwano Logo](./logo/logo_text_h.png)

# Kiwano Game Engine

[![Build status](https://ci.appveyor.com/api/projects/status/frqh09om9ldaklr9/branch/master?svg=true)](https://ci.appveyor.com/project/Nomango/kiwano/branch/master)
[![GitHub release](https://img.shields.io/github/release/nomango/kiwano)](https://github.com/Nomango/Kiwano/releases/latest)
[![GitHub license](https://img.shields.io/github/license/nomango/kiwano)](https://github.com/Nomango/Kiwano/blob/master/LICENSE)

English | [简体中文](./README-zh.md)

## Introduction
Kiwano is a open-source 2D C++ game engine, only support win32 platform.

Kiwano-Core is a game irrelevant lib that provide a lot useful tools can be used in other projects. I made a lot wheels to make cpp simpler to use.

It's still under heavy developing. I write it and use it to developy my own tiny games.

You can move to [Kiwano Demos](https://github.com/kiwanogame/KiwanoDemos) repository to see how to use Kiwano to implement simple games.

More docs and examples will be added later.

## Features
* Scene management
* Transitions between scenes
* Action behaviours
* Buttons and menus
* Texture atlas support
* Audio support
* Custom data storage
* Physical engine (based on Box2D)
* GUI system (based on ImGui)

## Install

### Dev Environment

- Win8 or Greater (Win10 recommended)
- Visual Studio 2015 or Greater

### Install with NuGet

1. Open your Visual Studio solution
2. In Solution Explorer, right-click `References` and choose `Manage NuGet Packages`
3. Select the `Browse` tab, search for `kiwano`, select that package in the list, and select Install
4. Have a good trip with Kiwano !

### Install with source code

1. Clone or download source code from Github repository
2. Open your Visual Studio solution, right-click your solution in Solution Explorer, select `Add` and then `Existing item`
3. Select `.vcxproj` files in /projects folder which you downloaded in 1st step
4. Right-click your project and choose `Properties`, select C\C++ => General, add the root directory of kiwano project to the `Additional include directory` field
5. Right-click `References` and choose `Add Reference`, select `kiwano` project
6. Now you can build your own applications based on Kiwano source code !

## Star History

[![Star History Chart](https://api.star-history.com/svg?repos=kiwanoengine/kiwano&type=Date)](https://star-history.com/#kiwanoengine/kiwano&Date)

## Next plan
* Cross-platform
* Particle system

## Contact
* Website: [kiwanoengine.com](https://kiwanoengine.com)
* QQ Group: 608406540
