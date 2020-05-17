include_directories(..)

set(SOURCE_FILES
        AudioModule.cpp
        AudioModule.h
        kiwano-audio.h
        libraries.cpp
        libraries.h
        Sound.cpp
        Sound.h
        SoundPlayer.cpp
        SoundPlayer.h
        Transcoder.cpp
        Transcoder.h)

add_library(libkiwanoaudio ${SOURCE_FILES})
