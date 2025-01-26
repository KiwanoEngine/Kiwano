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
// actors
//

#include <kiwano/Actor/Actor.h>
#include <kiwano/Actor/Canvas.h>
#include <kiwano/Actor/GifSprite.h>
#include <kiwano/Actor/LayerActor.h>
#include <kiwano/Actor/ShapeActor.h>
#include <kiwano/Actor/SpriteFrame.h>
#include <kiwano/Actor/Sprite.h>
#include <kiwano/Actor/Stage.h>
#include <kiwano/Actor/TextActor.h>

//
// abilities
//

#include <kiwano/ability/Ability.h>
#include <kiwano/ability/ComponentManager.h>
#include <kiwano/ability/RenderAbility.h>
#include <kiwano/ability/MouseSensor.h>
#include <kiwano/ability/Button.h>

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
// animation
//

#include <kiwano/ability/animation/Animation.h>
#include <kiwano/ability/animation/DelayAnimation.h>
#include <kiwano/ability/animation/AnimationGroup.h>
#include <kiwano/ability/animation/TweenAnimation.h>
#include <kiwano/ability/animation/PathAnimation.h>
#include <kiwano/ability/animation/FrameSequence.h>
#include <kiwano/ability/animation/FrameAnimation.h>
#include <kiwano/ability/animation/Animator.h>
#include <kiwano/ability/animation/AnimationWrapper.h>

//
// transition
//

#include <kiwano/transition/Transition.h>
#include <kiwano/transition/BoxTransition.h>
#include <kiwano/transition/FadeTransition.h>
#include <kiwano/transition/MoveTransition.h>
#include <kiwano/transition/RotationTransition.h>

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
