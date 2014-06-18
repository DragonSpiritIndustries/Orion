TEMPLATE = app
CONFIG -= console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

#DEFINES += \
#    ATHENA_NO_ZQUEST \
#    ATHENA_NO_SAVES

include(Athena/Athena.pri)

LIBS += \
    -lSDL2

INCLUDEPATH += \
    include

SOURCES += \
    src/main.cpp \
    src/Object.cpp \
    src/SDLWindow.cpp \
    src/SDLRenderer.cpp \
    src/SDLApplication.cpp \
    src/IApplication.cpp \
    src/ObjectManager.cpp \
    src/TestObject.cpp \
    src/SDLKeyboardManager.cpp \
    src/IKeyboardManager.cpp \
    src/SDLJoystickManager.cpp

HEADERS += \
    include/IApplication.hpp \
    include/Global.hpp \
    include/Object.hpp \
    include/SDLEvent.hpp \
    include/IWindow.hpp \
    include/SDLWindow.hpp \
    include/IRenderer.hpp \
    include/SDLRenderer.hpp \
    include/SDLApplication.hpp \
    include/Color.hpp \
    include/Event.hpp \
    include/IComponent.hpp \
    include/Matrix3x3.hpp \
    include/Vector3.hpp \
    include/Vector4.hpp \
    include/Vector2.hpp \
    include/ObjectManager.hpp \
    include/TestObject.hpp \
    include/IKeyboardManager.hpp \
    include/SDLKeyboardManager.hpp \
    include/IJoystickManager.hpp \
    include/SDLJoystickManager.hpp

