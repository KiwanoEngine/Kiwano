// Copyright (C) 2019 Nomango

#include <kiwano/common/common.h>
#include <kiwano/common/Function.hpp>
#include <kiwano/common/intrusive_ptr.hpp>
#include <kiwano/platform/Window.h>
#include <kiwano/platform/Input.h>
#include <kiwano/renderer/Renderer.h>
#include <kiwano-imgui/ImGuiModule.h>
#include <kiwano-imgui/imgui_impl.h>

#include <XInput.h>
#pragma comment(lib, "xinput")

// Allow compilation with old Windows SDK. MinGW doesn't have default _WIN32_WINNT/WINVER versions.
#ifndef WM_MOUSEHWHEEL
#	define WM_MOUSEHWHEEL 0x020E
#endif

#ifndef DBT_DEVNODES_CHANGED
#	define DBT_DEVNODES_CHANGED 0x0007
#endif

namespace kiwano
{
	namespace imgui
	{
		ImGuiModule::ImGuiModule()
			: has_gamepad_(false)
			, want_update_has_gamepad_(false)
			, target_window_(nullptr)
		{
		}

		void ImGuiModule::SetupComponent()
		{
			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();
			//ImGui::StyleColorsClassic();

			// Setup Platform/Renderer bindings
			Init(Window::GetInstance()->GetHandle());

			target_window_ = Renderer::GetInstance()->GetTargetWindow();
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
			io.KeyCtrl = Input::GetInstance()->IsDown(KeyCode::Ctrl);
			io.KeyShift = Input::GetInstance()->IsDown(KeyCode::Shift);
			io.KeyAlt = Input::GetInstance()->IsDown(KeyCode::Alt);
			io.KeySuper = false;
			// io.KeysDown[], io.MousePos, io.MouseDown[], io.MouseWheel: filled by the WndProc handler below.

			// Update OS mouse position
			UpdateMousePos();

			// Update OS mouse cursor with the cursor requested by imgui
			UpdateMouseCursor();

			// Update game controllers (if enabled and available)
			UpdateGamepads();
		}

		void ImGuiModule::Init(HWND hwnd)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
			io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)
			io.BackendPlatformName = "imgui_impl_win32";
			io.ImeWindowHandle = hwnd;

			// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array that we will update during the application lifetime.
			io.KeyMap[ImGuiKey_Tab] = KeyCode::Tab;
			io.KeyMap[ImGuiKey_LeftArrow] = KeyCode::Left;
			io.KeyMap[ImGuiKey_RightArrow] = KeyCode::Right;
			io.KeyMap[ImGuiKey_UpArrow] = KeyCode::Up;
			io.KeyMap[ImGuiKey_DownArrow] = KeyCode::Down;
			io.KeyMap[ImGuiKey_Delete] = KeyCode::Delete;
			io.KeyMap[ImGuiKey_Backspace] = KeyCode::Back;
			io.KeyMap[ImGuiKey_Space] = KeyCode::Space;
			io.KeyMap[ImGuiKey_Enter] = KeyCode::Enter;
			io.KeyMap[ImGuiKey_Escape] = KeyCode::Esc;
			io.KeyMap[ImGuiKey_A] = KeyCode::A;
			io.KeyMap[ImGuiKey_C] = KeyCode::C;
			io.KeyMap[ImGuiKey_V] = KeyCode::V;
			io.KeyMap[ImGuiKey_X] = KeyCode::X;
			io.KeyMap[ImGuiKey_Y] = KeyCode::Y;
			io.KeyMap[ImGuiKey_Z] = KeyCode::Z;

			ImGui_Impl_Init(Renderer::GetInstance());
		}

		void ImGuiModule::BeforeRender()
		{
			NewFrame();
		}

		void ImGuiModule::AfterRender()
		{
			Render();
		}

		void ImGuiModule::HandleMessage(HWND hwnd, UINT32 msg, WPARAM wparam, LPARAM lparam)
		{
			if (ImGui::GetCurrentContext() == NULL)
				return;

			ImGuiIO& io = ImGui::GetIO();
			switch (msg)
			{
			case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
			case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
			case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
			case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK:
			{
				int button = 0;
				if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK) { button = 0; }
				if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK) { button = 1; }
				if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK) { button = 2; }
				if (msg == WM_XBUTTONDOWN || msg == WM_XBUTTONDBLCLK) { button = (GET_XBUTTON_WPARAM(wparam) == XBUTTON1) ? 3 : 4; }
				if (!ImGui::IsAnyMouseDown() && ::GetCapture() == NULL)
					::SetCapture(hwnd);

				io.MouseDown[button] = true;
				break;
			}
			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_MBUTTONUP:
			case WM_XBUTTONUP:
			{
				int button = 0;
				if (msg == WM_LBUTTONUP) { button = 0; }
				if (msg == WM_RBUTTONUP) { button = 1; }
				if (msg == WM_MBUTTONUP) { button = 2; }
				if (msg == WM_XBUTTONUP) { button = (GET_XBUTTON_WPARAM(wparam) == XBUTTON1) ? 3 : 4; }
				io.MouseDown[button] = false;
				if (!ImGui::IsAnyMouseDown() && ::GetCapture() == hwnd)
					::ReleaseCapture();
				break;
			}
			case WM_MOUSEWHEEL:
			{
				io.MouseWheel += (float)GET_WHEEL_DELTA_WPARAM(wparam) / (float)WHEEL_DELTA;
				break;
			}
			case WM_MOUSEHWHEEL:
			{
				io.MouseWheelH += (float)GET_WHEEL_DELTA_WPARAM(wparam) / (float)WHEEL_DELTA;
				break;
			}
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			{
				if (wparam < 256)
					io.KeysDown[wparam] = 1;
				break;
			}
			case WM_KEYUP:
			case WM_SYSKEYUP:
			{
				if (wparam < 256)
					io.KeysDown[wparam] = 0;
				break;
			}
			case WM_CHAR:
			{
				// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
				io.AddInputCharacter((uint32_t)wparam);
				break;
			}
			case WM_SETCURSOR:
			{
				if (LOWORD(lparam) == HTCLIENT)
				{
					UpdateMouseCursor();
				}
				break;
			}
			case WM_DEVICECHANGE:
			{
				if ((uint32_t)wparam == DBT_DEVNODES_CHANGED)
					want_update_has_gamepad_ = true;
				break;
			}
			}
		}

		void ImGuiModule::NewFrame()
		{
			ImGui_Impl_NewFrame();

			ImGuiIO& io = ImGui::GetIO();
			KGE_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built!");

			// Setup display size (every frame to accommodate for window resizing)
			Size display_size = Renderer::GetInstance()->GetOutputSize();
			io.DisplaySize = ImVec2(display_size.x, display_size.y);

			ImGui::NewFrame();
		}

		void ImGuiModule::Render()
		{
			ImGui::Render();

			ImGui_Impl_RenderDrawData(ImGui::GetDrawData());
		}

		void ImGuiModule::UpdateMousePos()
		{
			ImGuiIO& io = ImGui::GetIO();

			// Set OS mouse position if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
			if (io.WantSetMousePos)
			{
				POINT pos = { (int)io.MousePos.x, (int)io.MousePos.y };
				::ClientToScreen(target_window_, &pos);
				::SetCursorPos(pos.x, pos.y);
			}

			Point pos = Input::GetInstance()->GetMousePos();
			io.MousePos = ImVec2(pos.x, pos.y);
		}

		void ImGuiModule::UpdateMouseCursor()
		{
			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
				return;

			CursorType cursor = CursorType::Arrow;
			switch (ImGui::GetMouseCursor())
			{
			case ImGuiMouseCursor_Arrow:        cursor = CursorType::Arrow; break;
			case ImGuiMouseCursor_TextInput:    cursor = CursorType::TextInput; break;
			case ImGuiMouseCursor_ResizeAll:    cursor = CursorType::SizeAll; break;
			case ImGuiMouseCursor_ResizeEW:     cursor = CursorType::SizeWE; break;
			case ImGuiMouseCursor_ResizeNS:     cursor = CursorType::SizeNS; break;
			case ImGuiMouseCursor_ResizeNESW:   cursor = CursorType::SizeNESW; break;
			case ImGuiMouseCursor_ResizeNWSE:   cursor = CursorType::SizeNWSE; break;
			case ImGuiMouseCursor_Hand:         cursor = CursorType::Hand; break;
			}

			Window::GetInstance()->SetCursor(cursor);
		}
		void ImGuiModule::UpdateGamepads()
		{
			ImGuiIO& io = ImGui::GetIO();
			memset(io.NavInputs, 0, sizeof(io.NavInputs));
			if ((io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) == 0)
				return;

			// Calling XInputGetState() every frame on disconnected gamepads is unfortunately too slow.
			// Instead we refresh gamepad availability by calling XInputGetCapabilities() _only_ after receiving WM_DEVICECHANGE.
			if (want_update_has_gamepad_)
			{
				XINPUT_CAPABILITIES caps;
				has_gamepad_ = (XInputGetCapabilities(0, XINPUT_FLAG_GAMEPAD, &caps) == ERROR_SUCCESS);
				want_update_has_gamepad_ = false;
			}

			XINPUT_STATE xinput_state;
			io.BackendFlags &= ~ImGuiBackendFlags_HasGamepad;
			if (has_gamepad_ && XInputGetState(0, &xinput_state) == ERROR_SUCCESS)
			{
				const XINPUT_GAMEPAD& gamepad = xinput_state.Gamepad;
				io.BackendFlags |= ImGuiBackendFlags_HasGamepad;

#define MAP_BUTTON(NAV_NO, BUTTON_ENUM)     { io.NavInputs[NAV_NO] = (gamepad.wButtons & BUTTON_ENUM) ? 1.0f : 0.0f; }
#define MAP_ANALOG(NAV_NO, VALUE, V0, V1)   { float vn = (float)(VALUE - V0) / (float)(V1 - V0); if (vn > 1.0f) vn = 1.0f; if (vn > 0.0f && io.NavInputs[NAV_NO] < vn) io.NavInputs[NAV_NO] = vn; }
				MAP_BUTTON(ImGuiNavInput_Activate, XINPUT_GAMEPAD_A);               // Cross / A
				MAP_BUTTON(ImGuiNavInput_Cancel, XINPUT_GAMEPAD_B);                 // Circle / B
				MAP_BUTTON(ImGuiNavInput_Menu, XINPUT_GAMEPAD_X);                   // Square / X
				MAP_BUTTON(ImGuiNavInput_Input, XINPUT_GAMEPAD_Y);                  // Triangle / Y
				MAP_BUTTON(ImGuiNavInput_DpadLeft, XINPUT_GAMEPAD_DPAD_LEFT);       // D-Pad Left
				MAP_BUTTON(ImGuiNavInput_DpadRight, XINPUT_GAMEPAD_DPAD_RIGHT);     // D-Pad Right
				MAP_BUTTON(ImGuiNavInput_DpadUp, XINPUT_GAMEPAD_DPAD_UP);           // D-Pad Up
				MAP_BUTTON(ImGuiNavInput_DpadDown, XINPUT_GAMEPAD_DPAD_DOWN);       // D-Pad Down
				MAP_BUTTON(ImGuiNavInput_FocusPrev, XINPUT_GAMEPAD_LEFT_SHOULDER);  // L1 / LB
				MAP_BUTTON(ImGuiNavInput_FocusNext, XINPUT_GAMEPAD_RIGHT_SHOULDER); // R1 / RB
				MAP_BUTTON(ImGuiNavInput_TweakSlow, XINPUT_GAMEPAD_LEFT_SHOULDER);  // L1 / LB
				MAP_BUTTON(ImGuiNavInput_TweakFast, XINPUT_GAMEPAD_RIGHT_SHOULDER); // R1 / RB
				MAP_ANALOG(ImGuiNavInput_LStickLeft, gamepad.sThumbLX, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, -32768);
				MAP_ANALOG(ImGuiNavInput_LStickRight, gamepad.sThumbLX, +XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, +32767);
				MAP_ANALOG(ImGuiNavInput_LStickUp, gamepad.sThumbLY, +XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, +32767);
				MAP_ANALOG(ImGuiNavInput_LStickDown, gamepad.sThumbLY, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, -32767);
#undef MAP_BUTTON
#undef MAP_ANALOG
			}
		}
	}
}
