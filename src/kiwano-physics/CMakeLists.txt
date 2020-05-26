include_directories(..)
include_directories(../3rd-party)

set(SOURCE_FILES
        Contact.cpp
        Contact.h
        ContactEdge.cpp
        ContactEdge.h
        ContactEvent.cpp
        ContactEvent.h
        Fixture.cpp
        Fixture.h
        Global.cpp
        Global.h
        Joint.cpp
        Joint.h
        kiwano-physics.h
        PhysicBody.cpp
        PhysicBody.h
        PhysicWorld.cpp
        PhysicWorld.h)

add_library(libkiwanophysics ${SOURCE_FILES})

target_link_libraries(libkiwanophysics libbox2d)
