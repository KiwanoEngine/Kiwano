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

#include <kiwano/math/Vec2.hpp>
#include <kiwano/math/Rect.hpp>
#include <kiwano/math/Matrix.hpp>
#include <kiwano/math/Transform.hpp>
#include <kiwano/math/Constants.h>
#include <kiwano/math/EaseFunctions.h>
#include <kiwano/math/Random.h>
#include <kiwano/math/Scalar.h>

//
// core
//

#include <kiwano/core/Common.h>
#include <kiwano/core/Director.h>
#include <kiwano/core/EventDispatcher.h>
#include <kiwano/core/EventListener.h>
#include <kiwano/core/Logger.h>
#include <kiwano/core/ObjectBase.h>
#include <kiwano/core/Resource.h>
#include <kiwano/core/SmartPtr.hpp>
#include <kiwano/core/Time.h>
#include <kiwano/core/Timer.h>
#include <kiwano/core/TimerManager.h>
#include <kiwano/core/event/Event.h>
#include <kiwano/core/event/KeyEvent.h>
#include <kiwano/core/event/MouseEvent.h>
#include <kiwano/core/event/WindowEvent.h>

//
// renderer
//

#include <kiwano/render/Color.h>
#include <kiwano/render/Font.h>
#include <kiwano/render/Shape.h>
#include <kiwano/render/ShapeMaker.h>
#include <kiwano/render/Texture.h>
#include <kiwano/render/Frame.h>
#include <kiwano/render/FrameSequence.h>
#include <kiwano/render/GifImage.h>
#include <kiwano/render/Layer.h>
#include <kiwano/render/TextLayout.h>
#include <kiwano/render/TextureCache.h>
#include <kiwano/render/Renderer.h>

//
// 2d
//

#include <kiwano/2d/Actor.h>
#include <kiwano/2d/Button.h>
#include <kiwano/2d/Canvas.h>
#include <kiwano/2d/DebugActor.h>
#include <kiwano/2d/GifSprite.h>
#include <kiwano/2d/LayerActor.h>
#include <kiwano/2d/ShapeActor.h>
#include <kiwano/2d/Sprite.h>
#include <kiwano/2d/Stage.h>
#include <kiwano/2d/TextActor.h>
#include <kiwano/2d/Transition.h>
#include <kiwano/2d/action/Action.h>
#include <kiwano/2d/action/ActionDelay.h>
#include <kiwano/2d/action/ActionGroup.h>
#include <kiwano/2d/action/ActionHelper.h>
#include <kiwano/2d/action/ActionManager.h>
#include <kiwano/2d/action/ActionTween.h>
#include <kiwano/2d/action/ActionWalk.h>
#include <kiwano/2d/action/Animation.h>

//
// platform
//

#include <kiwano/platform/Window.h>
#include <kiwano/platform/Runner.h>
#include <kiwano/platform/Application.h>
#include <kiwano/platform/FileSystem.h>
#include <kiwano/platform/Input.h>

//
// utils
//

#include <kiwano/utils/LocalStorage.h>
#include <kiwano/utils/ResourceCache.h>
#include <kiwano/utils/UserData.h>
