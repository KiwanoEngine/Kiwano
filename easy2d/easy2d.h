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
// Source Code: https://github.com/easy2d/easy2d
//


#pragma once

#include "core/macros.h"


//
// common
//

#include "core/common/Array.h"
#include "core/common/String.h"
#include "core/common/closure.hpp"
#include "core/common/IntrusiveList.hpp"
#include "core/common/IntrusivePtr.hpp"
#include "core/common/ComPtr.hpp"
#include "core/common/noncopyable.hpp"
#include "core/common/Singleton.hpp"
#include "core/common/helper.h"


//
// math
//

#include "core/math/constants.hpp"
#include "core/math/scalar.hpp"
#include "core/math/ease.hpp"
#include "core/math/Vec2.hpp"
#include "core/math/rand.h"
#include "core/math/Matrix.hpp"


//
// base
//

#include "core/base/window.h"
#include "core/base/input.h"
#include "core/base/time.h"
#include "core/base/logs.h"
#include "core/renderer/render.h"
#include "core/platform/modules.h"
#include "core/platform/Application.h"

#include "core/base/Event.hpp"
#include "core/base/EventListener.h"
#include "core/base/EventDispatcher.h"
#include "core/base/Task.h"
#include "core/base/TaskManager.h"

#include "core/2d/Font.hpp"
#include "core/2d/Color.h"
#include "core/2d/Transform.hpp"
#include "core/2d/TextStyle.hpp"
#include "core/base/Resource.h"

#include "core/base/Object.h"
#include "core/2d/Image.h"
#include "core/2d/Frames.h"
#include "core/2d/Geometry.h"
#include "core/2d/Action.h"
#include "core/2d/ActionGroup.h"
#include "core/2d/ActionTween.h"
#include "core/2d/ActionHelper.h"
#include "core/2d/Animation.h"
#include "core/2d/ActionManager.h"
#include "core/2d/Transition.h"

#include "core/2d/Node.h"
#include "core/2d/Scene.h"
#include "core/2d/Layer.h"
#include "core/2d/Sprite.h"
#include "core/2d/Text.h"
#include "core/2d/Canvas.h"
#include "core/2d/GeometryNode.h"
#include "core/2d/DebugNode.h"


//
// utils
//

#include "core/utils/Path.h"
#include "core/utils/Data.h"
#include "core/utils/File.h"
#include "core/utils/ResLoader.h"


//
// ui
//

#include "core/ui/Button.h"
#include "core/ui/Menu.h"
