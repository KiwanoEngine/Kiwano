include_directories(..)

set(SOURCE_FILES
        pugiconfig.hpp
        pugixml-impl.hpp
        pugixml.hpp)

add_library(libpugixml ${SOURCE_FILES})

set_target_properties(libpugixml PROPERTIES LINKER_LANGUAGE CXX)
