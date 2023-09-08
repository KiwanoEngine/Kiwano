include_directories(..)

set(SOURCE_FILES
        imconfig.h
        imgui.cpp
        imgui.h
        imgui_draw.cpp
        imgui_internal.h
        imgui_widgets.cpp
        imstb_rectpack.h
        imstb_textedit.h
        imstb_truetype.h
        imgui_stdlib.h
        imgui_stdlib.cpp)

add_library(libimgui ${SOURCE_FILES})
