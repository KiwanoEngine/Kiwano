include_directories(..)

set(SOURCE_FILES
        HttpModule.cpp
        HttpModule.h
        HttpRequest.cpp
        HttpRequest.h
        HttpResponse.hpp
        kiwano-network.h)

add_library(libkiwanonetwork ${SOURCE_FILES})
