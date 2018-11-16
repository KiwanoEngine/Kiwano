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


//
// base
//

#include "base/macros.h"
#include "base/base.hpp"
#include "base/modules.h"
#include "base/render.h"
#include "base/window.h"
#include "base/input.h"
#include "base/audio.h"
#include "base/time.h"
#include "base/logs.h"

#include "base/Point.hpp"
#include "base/Size.hpp"
#include "base/Rect.hpp"
#include "base/Font.hpp"
#include "base/Color.h"
#include "base/Resource.h"
#include "base/Transform.hpp"
#include "base/TextStyle.hpp"

#include "base/intrusive/SmartPointer.hpp"
#include "base/intrusive/List.hpp"

#include "base/RefCounter.hpp"
#include "base/Image.h"
#include "base/Node.h"
#include "base/Scene.h"
#include "base/Sprite.h"
#include "base/Text.h"
#include "base/Canvas.h"
#include "base/Music.h"
#include "base/Task.h"
#include "base/Action.hpp"
#include "base/ActionCombined.h"
#include "base/ActionFiniteTime.h"
#include "base/Animation.h"
#include "base/CallFunc.h"
#include "base/Transition.h"

#include "base/KeyEvent.h"
#include "base/MouseEvent.h"

#include "base/Game.h"


//
// math
//

#include "math/scalar.hpp"
#include "math/vector.hpp"
#include "math/rand.h"
#include "math/Matrix.hpp"


//
// utils
//

#include "utils/Path.h"
#include "utils/Data.h"
#include "utils/File.h"
#include "utils/Player.h"
#include "utils/Transcoder.h"


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
