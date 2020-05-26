include_directories(..)

set(SOURCE_FILES
        imgui_impl/imgui_impl.h
        imgui_impl/imgui_impl_dx10.cpp
        imgui_impl/imgui_impl_dx10.h
        imgui_impl/imgui_impl_dx11.cpp
        imgui_impl/imgui_impl_dx11.h
        ImGuiLayer.cpp
        ImGuiLayer.h
        ImGuiModule.cpp
        ImGuiModule.h
        kiwano-imgui.h)

add_library(libkiwanoimgui ${SOURCE_FILES})
