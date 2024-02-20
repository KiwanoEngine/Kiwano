// Copyright (C) 2019 Nomango

#include <kiwano/core/Common.h>
#include <kiwano/event/Events.h>
#include <kiwano/platform/Input.h>
#include <kiwano/platform/Application.h>
#include <kiwano/render/Renderer.h>
#include <kiwano-imgui/ImGuiModule.h>
#include <kiwano-imgui/imgui_impl/imgui_impl.h>

namespace kiwano
{
namespace imgui
{

ImGuiKey KeyCodeToImGuiKey(const KeyCode code)
{
    switch (code)
    {
    case KeyCode::Tab:
        return ImGuiKey_Tab;
    case KeyCode::Left:
        return ImGuiKey_LeftArrow;
    case KeyCode::Right:
        return ImGuiKey_RightArrow;
    case KeyCode::Up:
        return ImGuiKey_UpArrow;
    case KeyCode::Down:
        return ImGuiKey_DownArrow;
    case KeyCode::Delete:
        return ImGuiKey_Delete;
    case KeyCode::Back:
        return ImGuiKey_Backspace;
    case KeyCode::Space:
        return ImGuiKey_Space;
    case KeyCode::Enter:
        return ImGuiKey_Enter;
    case KeyCode::Esc:
        return ImGuiKey_Escape;
    case KeyCode::Num0:
    case KeyCode::Num1:
    case KeyCode::Num2:
    case KeyCode::Num3:
    case KeyCode::Num4:
    case KeyCode::Num5:
    case KeyCode::Num6:
    case KeyCode::Num7:
    case KeyCode::Num8:
    case KeyCode::Num9:
        return ImGuiKey(ImGuiKey_0 + (code - KeyCode::Num0));
    case KeyCode::Numpad0:
    case KeyCode::Numpad1:
    case KeyCode::Numpad2:
    case KeyCode::Numpad3:
    case KeyCode::Numpad4:
    case KeyCode::Numpad5:
    case KeyCode::Numpad6:
    case KeyCode::Numpad7:
    case KeyCode::Numpad8:
    case KeyCode::Numpad9:
        return ImGuiKey(ImGuiKey_Keypad0 + (code - KeyCode::Numpad0));
    case KeyCode::A:
    case KeyCode::B:
    case KeyCode::C:
    case KeyCode::D:
    case KeyCode::E:
    case KeyCode::F:
    case KeyCode::G:
    case KeyCode::H:
    case KeyCode::I:
    case KeyCode::J:
    case KeyCode::K:
    case KeyCode::L:
    case KeyCode::M:
    case KeyCode::N:
    case KeyCode::O:
    case KeyCode::P:
    case KeyCode::Q:
    case KeyCode::R:
    case KeyCode::S:
    case KeyCode::T:
    case KeyCode::U:
    case KeyCode::V:
    case KeyCode::W:
    case KeyCode::X:
    case KeyCode::Y:
    case KeyCode::Z:
        return ImGuiKey(ImGuiKey_A + (code - KeyCode::A));
    case KeyCode::F1:
    case KeyCode::F2:
    case KeyCode::F3:
    case KeyCode::F4:
    case KeyCode::F5:
    case KeyCode::F6:
    case KeyCode::F7:
    case KeyCode::F8:
    case KeyCode::F9:
    case KeyCode::F10:
    case KeyCode::F11:
    case KeyCode::F12:
        return ImGuiKey(ImGuiKey_F1 + (code - KeyCode::F1));
    default:
        return ImGuiKey_None;
    }
}

ImGuiModule::ImGuiModule() {}

void ImGuiModule::SetupModule()
{
    window_ = Application::GetInstance().GetWindow();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    this->InitPlatform();
    ImGui_Impl_Init();
}

void ImGuiModule::InitPlatform()
{
    ImGuiIO& io = ImGui::GetIO();
    IM_ASSERT(io.BackendPlatformUserData == nullptr && "Already initialized a platform backend!");

    // Setup backend capabilities flags
    io.BackendPlatformUserData = (void*)this;
    io.BackendPlatformName     = "imgui_impl_win32";
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;

    // Set platform dependent data in viewport
    ImGui::GetMainViewport()->PlatformHandleRaw = (void*)window_->GetHandle();

    // Setup Platform/Renderer bindings
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    io.BackendPlatformName = "imgui_impl_win32";
    io.ImeWindowHandle     = window_->GetHandle();
}

void ImGuiModule::DestroyModule()
{
    ImGui_Impl_Shutdown();
    this->ShutdownPlatform();
    ImGui::DestroyContext();
}

void ImGuiModule::ShutdownPlatform()
{
    ImGuiIO& io                = ImGui::GetIO();
    io.BackendPlatformName     = nullptr;
    io.BackendPlatformUserData = nullptr;
}

void ImGuiModule::OnUpdate(UpdateModuleContext& ctx)
{
    ImGuiIO& io = ImGui::GetIO();

    // Setup time step
    io.DeltaTime = ctx.dt.GetSeconds();
    if (io.DeltaTime <= 0.f)
    {
        // Prevent from failed assertion
        io.DeltaTime = 0.0001f;
    }

    // Read keyboard modifiers inputs
    io.KeyCtrl  = Input::GetInstance().IsDown(KeyCode::Ctrl);
    io.KeyShift = Input::GetInstance().IsDown(KeyCode::Shift);
    io.KeyAlt   = Input::GetInstance().IsDown(KeyCode::Alt);
    io.KeySuper = Input::GetInstance().IsDown(KeyCode::Super);
    // io.KeysDown[], io.MousePos, io.MouseDown[], io.MouseWheel: filled by the HandleEvent function below.

    // Update OS mouse cursor with the cursor requested by imgui
    UpdateMouseCursor();

    ctx.Next();
}

void ImGuiModule::BeforeRender(RenderModuleContext& ctx)
{
    ImGui_Impl_NewFrame();

    ImGuiIO& io = ImGui::GetIO();

    // Setup display size (every frame to accommodate for window resizing)
    Size display_size = Renderer::GetInstance().GetOutputSize();
    io.DisplaySize    = ImVec2(display_size.x, display_size.y);

    ImGui::NewFrame();
}

void ImGuiModule::AfterRender(RenderModuleContext& ctx)
{
    ImGui::Render();

    ImGui_Impl_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiModule::HandleEvent(EventModuleContext& ctx)
{
    if (ImGui::GetCurrentContext() != NULL)
    {
        Event* evt = ctx.evt;

        ImGuiIO& io = ImGui::GetIO();
        if (evt->IsType<MouseEvent>())
        {
            if (evt->IsType<MouseMoveEvent>())
            {
                const auto& pos = dynamic_cast<MouseMoveEvent*>(evt)->pos;
                io.AddMousePosEvent(pos.x, pos.y);
            }
            else if (evt->IsType<MouseDownEvent>())
            {
                MouseButton button = dynamic_cast<MouseDownEvent*>(evt)->button;
                int         index  = 0;
                if (button == MouseButton::Left)
                    index = 0;
                else if (button == MouseButton::Right)
                    index = 1;
                else if (button == MouseButton::Middle)
                    index = 2;
                io.AddMouseButtonEvent(index, true);
            }
            else if (evt->IsType<MouseUpEvent>())
            {
                MouseButton button = dynamic_cast<MouseUpEvent*>(evt)->button;
                int         index  = 0;
                if (button == MouseButton::Left)
                    index = 0;
                else if (button == MouseButton::Right)
                    index = 1;
                else if (button == MouseButton::Middle)
                    index = 2;
                io.AddMouseButtonEvent(index, false);
            }
            else if (evt->IsType<MouseWheelEvent>())
            {
                float wheel = dynamic_cast<MouseWheelEvent*>(evt)->wheel;
                io.AddMouseWheelEvent(0.0f, wheel);
            }
        }
        else if (evt->IsType<KeyEvent>())
        {
            if (evt->IsType<KeyDownEvent>())
            {
                KeyCode key = dynamic_cast<KeyDownEvent*>(evt)->code;
                io.AddKeyEvent(KeyCodeToImGuiKey(key), true);
            }
            else if (evt->IsType<KeyUpEvent>())
            {
                KeyCode key = dynamic_cast<KeyUpEvent*>(evt)->code;
                io.AddKeyEvent(KeyCodeToImGuiKey(key), false);
            }
            else if (evt->IsType<KeyCharEvent>())
            {
                char ch = dynamic_cast<KeyCharEvent*>(evt)->value;
                io.AddInputCharacter(static_cast<ImWchar>(ch));
            }
            else if (evt->IsType<IMEInputEvent>())
            {
                const auto& str      = dynamic_cast<IMEInputEvent*>(evt)->value;
                const auto  utf8_str = strings::WideToUTF8(strings::NarrowToWide(str));
                io.AddInputCharactersUTF8(utf8_str.c_str());
            }
        }
        else if (evt->IsType<WindowEvent>())
        {
            if (evt->IsType<WindowFocusChangedEvent>())
            {
                bool focus = dynamic_cast<WindowFocusChangedEvent*>(evt)->focus;
                io.AddFocusEvent(focus);
            }
        }
    }

    ctx.Next();
}

void ImGuiModule::UpdateMouseCursor()
{
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
        return;

    static ImGuiMouseCursor last_cursor = ImGuiMouseCursor_None;
    if (ImGui::GetMouseCursor() == last_cursor)
        return;

    last_cursor = ImGui::GetMouseCursor();

    CursorType cursor = CursorType::Arrow;
    switch (last_cursor)
    {
    case ImGuiMouseCursor_Arrow:
        cursor = CursorType::Arrow;
        break;
    case ImGuiMouseCursor_TextInput:
        cursor = CursorType::TextInput;
        break;
    case ImGuiMouseCursor_ResizeAll:
        cursor = CursorType::SizeAll;
        break;
    case ImGuiMouseCursor_ResizeEW:
        cursor = CursorType::SizeWE;
        break;
    case ImGuiMouseCursor_ResizeNS:
        cursor = CursorType::SizeNS;
        break;
    case ImGuiMouseCursor_ResizeNESW:
        cursor = CursorType::SizeNESW;
        break;
    case ImGuiMouseCursor_ResizeNWSE:
        cursor = CursorType::SizeNWSE;
        break;
    case ImGuiMouseCursor_Hand:
        cursor = CursorType::Hand;
        break;
    case ImGuiMouseCursor_NotAllowed:
        cursor = CursorType::No;
        break;
    }

    window_->SetCursor(cursor);
}

}  // namespace imgui
}  // namespace kiwano
