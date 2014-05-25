TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

include(Athena/Athena.pri)

LIBS += \
    -lSDL2

INCLUDEPATH += \
    include

SOURCES += \
    main.cpp \
    Object.cpp \
    SDLEvent.cpp \
    SDLWindow.cpp \
    SDLRenderer.cpp \
    SDLApplication.cpp \
    IApplication.cpp

HEADERS += \
    IApplication.hpp \
    Global.hpp \
    Object.hpp \
    IEvent.hpp \
    IDriverInfo.hpp \
    IGraphicsDriverInfo.hpp \
    SDLEvent.hpp \
    IWindow.hpp \
    SDLWindow.hpp \
    IRenderer.hpp \
    SDLRenderer.hpp \
    SDLApplication.hpp

