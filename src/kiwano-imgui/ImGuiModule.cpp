// Copyright (C) 2019 Nomango


#include <kiwano/core/Common.h>
#include <kiwano/core/event/KeyEvent.h>
#include <kiwano/core/event/MouseEvent.h>
#include <kiwano/platform/Input.h>
#include <kiwano/platform/win32/WindowImpl.h>
#include <kiwano/render/Renderer.h>
#include <kiwano-imgui/ImGuiModule.h>
#include <kiwano-imgui/imgui_impl/imgui_impl.h>

namespace kiwano
{
namespace imgui
{
ImGuiModule::ImGuiModule()
{
}

void ImGuiModule::SetupComponent()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;  // We can honor GetMouseCursor() values (optional)
    io.BackendFlags |=
        ImGuiBackendFlags_HasSetMousePos;  // We can honor io.WantSetMousePos requests (optional, rarely used)
    io.BackendPlatformName = "imgui_impl_win32";
    io.ImeWindowHandle     = WindowImpl::GetInstance().GetHandle();

    // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array that we will update during
    // the application lifetime.
    io.KeyMap[ImGuiKey_Tab]        = (int)KeyCode::Tab;
    io.KeyMap[ImGuiKey_LeftArrow]  = (int)KeyCode::Left;
    io.KeyMap[ImGuiKey_RightArrow] = (int)KeyCode::Right;
    io.KeyMap[ImGuiKey_UpArrow]    = (int)KeyCode::Up;
    io.KeyMap[ImGuiKey_DownArrow]  = (int)KeyCode::Down;
    io.KeyMap[ImGuiKey_Delete]     = (int)KeyCode::Delete;
    io.KeyMap[ImGuiKey_Backspace]  = (int)KeyCode::Back;
    io.KeyMap[ImGuiKey_Space]      = (int)KeyCode::Space;
    io.KeyMap[ImGuiKey_Enter]      = (int)KeyCode::Enter;
    io.KeyMap[ImGuiKey_Escape]     = (int)KeyCode::Esc;
    io.KeyMap[ImGuiKey_A]          = (int)KeyCode::A;
    io.KeyMap[ImGuiKey_C]          = (int)KeyCode::C;
    io.KeyMap[ImGuiKey_V]          = (int)KeyCode::V;
    io.KeyMap[ImGuiKey_X]          = (int)KeyCode::X;
    io.KeyMap[ImGuiKey_Y]          = (int)KeyCode::Y;
    io.KeyMap[ImGuiKey_Z]          = (int)KeyCode::Z;

    ImGui_Impl_Init();
}

void ImGuiModule::DestroyComponent()
{
    ImGui_Impl_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiModule::OnUpdate(Duration dt)
{
    ImGuiIO& io = ImGui::GetIO();

    // Setup time step
    io.DeltaTime = dt.Seconds();

    // Read keyboard modifiers inputs
    io.KeyCtrl  = Input::GetInstance().IsDown(KeyCode::Ctrl);
    io.KeyShift = Input::GetInstance().IsDown(KeyCode::Shift);
    io.KeyAlt   = Input::GetInstance().IsDown(KeyCode::Alt);
    io.KeySuper = Input::GetInstance().IsDown(KeyCode::Super);
    // io.KeysDown[], io.MousePos, io.MouseDown[], io.MouseWheel: filled by the HandleEvent function below.

    // Update OS mouse position
    UpdateMousePos();

    // Update OS mouse cursor with the cursor requested by imgui
    UpdateMouseCursor();
}

void ImGuiModule::BeforeRender()
{
    ImGui_Impl_NewFrame();

    ImGuiIO& io = ImGui::GetIO();
    KGE_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built!");

    // Setup display size (every frame to accommodate for window resizing)
    Size display_size = Renderer::GetInstance().GetOutputSize();
    io.DisplaySize    = ImVec2(display_size.x, display_size.y);

    ImGui::NewFrame();
}

void ImGuiModule::AfterRender()
{
    ImGui::Render();

    ImGui_Impl_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiModule::HandleEvent(Event* evt)
{
    if (ImGui::GetCurrentContext() == NULL)
        return;

    ImGuiIO& io = ImGui::GetIO();
    if (evt->IsType<MouseEvent>())
    {
        if (evt->IsType<MouseDownEvent>())
        {
            MouseButton button = dynamic_cast<MouseDownEvent*>(evt)->button;
            int         index  = 0;
            if (button == MouseButton::Left)
                index = 0;
            else if (button == MouseButton::Right)
                index = 1;
            else if (button == MouseButton::Middle)
                index = 2;
            io.MouseDown[index] = true;
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
            io.MouseDown[index] = false;
        }
        else if (evt->IsType<MouseWheelEvent>())
        {
            float wheel = dynamic_cast<MouseWheelEvent*>(evt)->wheel;
            io.MouseWheel += wheel;
        }
    }
    else if (evt->IsType<KeyEvent>())
    {
        if (evt->IsType<KeyDownEvent>())
        {
            KeyCode key           = dynamic_cast<KeyDownEvent*>(evt)->code;
            io.KeysDown[(int)key] = true;
        }
        else if (evt->IsType<KeyUpEvent>())
        {
            KeyCode key           = dynamic_cast<KeyUpEvent*>(evt)->code;
            io.KeysDown[(int)key] = false;
        }
        else if (evt->IsType<KeyCharEvent>())
        {
            // You can also use ToAscii()+GetKeyboardState() to retrieve characters.
            char ch = dynamic_cast<KeyCharEvent*>(evt)->value;
            io.AddInputCharacter(static_cast<ImWchar>(ch));
        }
    }
}

void ImGuiModule::UpdateMousePos()
{
    ImGuiIO& io = ImGui::GetIO();

    // Set OS mouse position if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by
    // user)
    if (io.WantSetMousePos)
    {
        POINT pos = { (int)io.MousePos.x, (int)io.MousePos.y };
        ::ClientToScreen(WindowImpl::GetInstance().GetHandle(), &pos);
        ::SetCursorPos(pos.x, pos.y);
    }

    Point pos   = Input::GetInstance().GetMousePos();
    io.MousePos = ImVec2(pos.x, pos.y);
}

void ImGuiModule::UpdateMouseCursor()
{
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
        return;

    CursorType cursor = CursorType::Arrow;
    switch (ImGui::GetMouseCursor())
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
    }

    Window::GetInstance().SetCursor(cursor);
}

}  // namespace imgui
}  // namespace kiwano
