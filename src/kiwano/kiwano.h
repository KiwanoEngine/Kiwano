// Copyright (c) 2016-2018 Kiwano - Nomango
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
// Website: https://www.kiwanoengine.com
// Source Code: https://github.com/KiwanoEngine/Kiwano
//


#pragma once

#include "macros.h"


//
// core
//

#include "core/vector.hpp"
#include "core/string.hpp"
#include "core/core.h"
#include "core/Function.hpp"
#include "core/intrusive_list.hpp"
#include "core/intrusive_ptr.hpp"
#include "core/noncopyable.hpp"
#include "core/singleton.hpp"
#include "core/basic_json.hpp"


//
// math
//

#include "math/constants.h"
#include "math/scalar.h"
#include "math/ease.h"
#include "math/Vec2.hpp"
#include "math/rand.h"
#include "math/Matrix.hpp"


//
// renderer
//

#include "renderer/Color.h"
#include "renderer/Font.h"
#include "renderer/Texture.h"
#include "renderer/GifImage.h"
#include "renderer/TextLayout.h"
#include "renderer/Geometry.h"
#include "renderer/LayerArea.h"
#include "renderer/TextureCache.h"
#include "renderer/Renderer.h"


//
// base
//

#include "base/time.h"
#include "base/Window.h"
#include "base/input.h"
#include "base/Director.h"
#include "base/Logger.h"
#include "base/SmartPtr.hpp"
#include "base/ObjectBase.h"
#include "base/Event.hpp"
#include "base/EventListener.h"
#include "base/EventDispatcher.h"
#include "base/Timer.h"
#include "base/TimerManager.h"
#include "base/AsyncTask.h"
#include "base/Resource.h"


//
// 2d
//


#include "2d/Transform.h"
#include "2d/TextStyle.hpp"

#include "2d/Frame.h"
#include "2d/FrameSequence.h"

#include "2d/action/Action.h"
#include "2d/action/ActionGroup.h"
#include "2d/action/ActionTween.h"
#include "2d/action/ActionWalk.h"
#include "2d/action/ActionDelay.h"
#include "2d/action/Animation.h"
#include "2d/action/ActionHelper.h"
#include "2d/action/ActionManager.h"

#include "2d/Transition.h"

#include "2d/Actor.h"
#include "2d/Stage.h"
#include "2d/Layer.h"
#include "2d/Sprite.h"
#include "2d/GifSprite.h"
#include "2d/Text.h"
#include "2d/Canvas.h"
#include "2d/ShapeActor.h"
#include "2d/DebugActor.h"


//
// platform
//

#include "platform/modules.h"
#include "platform/Application.h"


//
// utils
//

#include "utils/Path.h"
#include "utils/DataUtil.h"
#include "utils/FileUtil.h"
#include "utils/ResourceCache.h"


//
// ui
//

#include "ui/Button.h"
#include "ui/Menu.h"
