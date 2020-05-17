include_directories(..)

set(SOURCE_FILES
        curl.h
        curlbuild.h
        curlrules.h
        curlver.h
        easy.h
        mprintf.h
        multi.h
        stdcheaders.h
        typecheck-gcc.h)

add_library(libcurl ${SOURCE_FILES})

target_link_libraries(libcurl ./libs/libcurl.lib)

set_target_properties(libcurl PROPERTIES LINKER_LANGUAGE CXX)
