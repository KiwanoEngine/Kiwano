// Copyright (c) 2016-2018 Easy2D - Nomango
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// Website: https://www.easy2d.cn
// Source Code: https://github.com/Nomango/Easy2D
//


#pragma once

#ifndef __cplusplus
#	error 仅能在 C++ 环境下使用 Easy2D
#endif

#ifndef _MSC_VER
#   error 仅支持在 MSVC 环境下编译
#elif _MSC_VER < 1700
#	error Easy2D 不支持 Visual Studio 2012 以下版本
#endif


//
// base
//

#include "base/macros.h"
#include "base/base.h"
#include "base/modules.h"
#include "base/render.h"
#include "base/window.h"
#include "base/input.h"
#include "base/audio.h"
#include "base/time.h"
#include "base/logs.h"

#include "base/Size.h"
#include "base/Rect.hpp"
#include "base/Font.h"
#include "base/Color.h"
#include "base/Resource.h"

#include "base/RefCounter.h"
#include "base/Image.h"
#include "base/Scene.h"
#include "base/Node.h"
#include "base/Sprite.h"
#include "base/Task.h"
#include "base/Text.h"
#include "base/Action.h"
#include "base/ActionCombined.h"
#include "base/ActionFiniteTime.h"
#include "base/Animation.h"
#include "base/CallFunc.h"
#include "base/Canvas.h"
#include "base/Transition.h"

#include "base/KeyEvent.h"
#include "base/MouseEvent.h"

#include "base/Game.h"


//
// math
//

#include "math/scalar.hpp"
#include "math/vector.hpp"
#include "math/Matrix.hpp"
#include "math/Transform.h"
#include "math/rand.h"


//
// utils
//

#include "utils/Path.h"
#include "utils/Data.h"
#include "utils/File.h"
#include "utils/Transcoder.h"
#include "utils/Music.h"
#include "utils/Player.h"


//
// ui
//

#include "ui/Button.h"
#include "ui/Menu.h"


#if defined(DEBUG) || defined(_DEBUG)
#	pragma comment(lib, "Easy2Ddw.lib")
#else
#	pragma comment(lib, "Easy2Dw.lib")
#endif
