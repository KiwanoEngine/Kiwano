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

#include <kiwano/macros.h>


//
// math
//

#include <kiwano/math/constants.h>
#include <kiwano/math/scalar.h>
#include <kiwano/math/ease.h>
#include <kiwano/math/Vec2.hpp>
#include <kiwano/math/rand.h>
#include <kiwano/math/Matrix.hpp>


//
// core
//

#include <kiwano/core/common.h>
#include <kiwano/core/time.h>
#include <kiwano/core/Logger.h>
#include <kiwano/core/SmartPtr.hpp>
#include <kiwano/core/ObjectBase.h>
#include <kiwano/core/event/Event.h>
#include <kiwano/core/event/MouseEvent.h>
#include <kiwano/core/event/KeyEvent.h>
#include <kiwano/core/event/WindowEvent.h>
#include <kiwano/core/EventListener.h>
#include <kiwano/core/EventDispatcher.h>
#include <kiwano/core/Timer.h>
#include <kiwano/core/TimerManager.h>
#include <kiwano/core/AsyncTask.h>
#include <kiwano/core/Resource.h>


//
// renderer
//

#include <kiwano/renderer/Color.h>
#include <kiwano/renderer/Font.h>
#include <kiwano/renderer/Texture.h>
#include <kiwano/renderer/GifImage.h>
#include <kiwano/renderer/TextLayout.h>
#include <kiwano/renderer/Geometry.h>
#include <kiwano/renderer/LayerArea.h>
#include <kiwano/renderer/TextureCache.h>
#include <kiwano/renderer/Renderer.h>


//
// 2d
//


#include <kiwano/2d/Transform.h>
#include <kiwano/2d/Frame.h>
#include <kiwano/2d/FrameSequence.h>
#include <kiwano/2d/action/Action.h>
#include <kiwano/2d/action/ActionGroup.h>
#include <kiwano/2d/action/ActionTween.h>
#include <kiwano/2d/action/ActionWalk.h>
#include <kiwano/2d/action/ActionDelay.h>
#include <kiwano/2d/action/Animation.h>
#include <kiwano/2d/action/ActionHelper.h>
#include <kiwano/2d/action/ActionManager.h>
#include <kiwano/2d/Transition.h>
#include <kiwano/2d/Actor.h>
#include <kiwano/2d/Stage.h>
#include <kiwano/2d/Layer.h>
#include <kiwano/2d/Sprite.h>
#include <kiwano/2d/GifSprite.h>
#include <kiwano/2d/TextActor.h>
#include <kiwano/2d/Canvas.h>
#include <kiwano/2d/ShapeActor.h>
#include <kiwano/2d/DebugActor.h>


//
// platform
//

#include <kiwano/platform/FileSystem.h>
#include <kiwano/platform/Input.h>
#include <kiwano/platform/Window.h>
#include <kiwano/platform/Director.h>
#include <kiwano/platform/Application.h>


//
// utils
//

#include <kiwano/utils/UserData.h>
#include <kiwano/utils/LocalStorage.h>
#include <kiwano/utils/ResourceCache.h>


//
// ui
//

#include <kiwano/ui/Button.h>
#include <kiwano/ui/Menu.h>
