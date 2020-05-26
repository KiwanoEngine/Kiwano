include_directories(..)

set(SOURCE_FILES
        json.hpp)

add_library(libnlohmann ${SOURCE_FILES})

set_target_properties(libnlohmann PROPERTIES LINKER_LANGUAGE CXX)

