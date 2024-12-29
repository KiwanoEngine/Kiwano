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
#include <kiwano/math/Interpolator.h>

//
// core
//

#include <kiwano/core/Common.h>
#include <kiwano/core/Defer.h>
#include <kiwano/core/Resource.h>
#include <kiwano/core/Time.h>
#include <kiwano/core/RefObject.hpp>
#include <kiwano/core/BaseObject.h>

//
// event
//

#include <kiwano/event/Event.h>
#include <kiwano/event/KeyEvent.h>
#include <kiwano/event/MouseEvent.h>
#include <kiwano/event/WindowEvent.h>
#include <kiwano/event/listener/EventListener.h>
#include <kiwano/event/listener/MouseEventListener.h>
#include <kiwano/event/listener/KeyEventListener.h>
#include <kiwano/event/EventDispatcher.h>

//
// modules
//

#include <kiwano/module/Module.h>
#include <kiwano/module/Director.h>

//
// components
//

#include <kiwano/component/Component.h>
#include <kiwano/component/ComponentManager.h>
#include <kiwano/component/RenderComponent.h>
#include <kiwano/component/MouseSensor.h>
#include <kiwano/component/Button.h>

//
// renderer
//

#include <kiwano/render/Color.h>
#include <kiwano/render/Font.h>
#include <kiwano/render/Shape.h>
#include <kiwano/render/ShapeMaker.h>
#include <kiwano/render/Texture.h>
#include <kiwano/render/GifImage.h>
#include <kiwano/render/Layer.h>
#include <kiwano/render/TextLayout.h>
#include <kiwano/render/TextureCache.h>
#include <kiwano/render/Renderer.h>

//
// 2d
//

#include <kiwano/2d/Actor.h>
#include <kiwano/2d/Canvas.h>
#include <kiwano/2d/GifSprite.h>
#include <kiwano/2d/LayerActor.h>
#include <kiwano/2d/ShapeActor.h>
#include <kiwano/2d/SpriteFrame.h>
#include <kiwano/2d/Sprite.h>
#include <kiwano/2d/Stage.h>
#include <kiwano/2d/TextActor.h>

//
// transition
//

#include <kiwano/2d/transition/Transition.h>
#include <kiwano/2d/transition/BoxTransition.h>
#include <kiwano/2d/transition/FadeTransition.h>
#include <kiwano/2d/transition/MoveTransition.h>
#include <kiwano/2d/transition/RotationTransition.h>

//
// animation
//

#include <kiwano/2d/animation/Animation.h>
#include <kiwano/2d/animation/DelayAnimation.h>
#include <kiwano/2d/animation/AnimationGroup.h>
#include <kiwano/2d/animation/TweenAnimation.h>
#include <kiwano/2d/animation/PathAnimation.h>
#include <kiwano/2d/animation/FrameSequence.h>
#include <kiwano/2d/animation/FrameAnimation.h>
#include <kiwano/2d/animation/Animator.h>
#include <kiwano/2d/animation/AnimationWrapper.h>

//
// platform
//

#include <kiwano/module/Window.h>
#include <kiwano/module/Runner.h>
#include <kiwano/platform/Application.h>
#include <kiwano/platform/FileSystem.h>
#include <kiwano/module/Input.h>

//
// utils
//

#include <kiwano/utils/Logger.h>
#include <kiwano/utils/ResourceCache.h>
#include <kiwano/utils/ResourceLoader.h>
#include <kiwano/utils/UserData.h>
#include <kiwano/utils/Timer.h>
#include <kiwano/utils/Ticker.h>
#include <kiwano/utils/EventTicker.h>
#include <kiwano/utils/Task.h>
#include <kiwano/utils/TaskScheduler.h>
#include <kiwano/utils/ConfigIni.h>
