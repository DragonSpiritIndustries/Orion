TEMPLATE = app
CONFIG += thread
CONFIG -= console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11
win32:QMAKE_LFLAGS += -mwindows

DEFINES += \
    ATHENA_NO_ZQUEST \
    ATHENA_NO_SAVES

win32:DEFINES += \
    SDL_MAIN_NEEDED

include(Athena/Athena.pri)

win32:LIBS += \
    -lmingw32

LIBS += \
    -lSDL2 \
    -lSDL2_ttf \
    -lSDL2_image \
    -lz \
    -lphysfs \
    -ltinyxml \

unix:LIBS += \
    -lGL \
    -lGLU
win32:LIBS += \
    -lSDL2main

INCLUDEPATH += \
    include

SOURCES += \
    src/ScriptEngine.cpp \
    src/Console.cpp \
    src/Vector2.cpp \
    src/Vector3.cpp \
    src/Rectangle.cpp \
    src/Color.cpp \
    src/CVarManager.cpp \
    src/CVar.cpp \
    src/ApplicationBase.cpp \
    src/ComponentFactory.cpp \
    src/IComponent.cpp \
    src/TransformableComponent.cpp \
    src/IMouseManager.cpp \
    src/physfsrwops.c \
    src/IJoystickManager.cpp \
    src/IResource.cpp \
    src/ITextureResource.cpp \
    src/ScriptResource.cpp \
    src/ResourceManager.cpp \
    src/ObjectManager.cpp \
    src/Object.cpp \
    src/angelscript/as_atomic.cpp \
    src/angelscript/as_builder.cpp \
    src/angelscript/as_bytecode.cpp \
    src/angelscript/as_callfunc.cpp \
    src/angelscript/as_callfunc_arm.cpp \
    src/angelscript/as_callfunc_mips.cpp \
    src/angelscript/as_callfunc_ppc.cpp \
    src/angelscript/as_callfunc_ppc_64.cpp \
    src/angelscript/as_callfunc_sh4.cpp \
    src/angelscript/as_callfunc_x64_gcc.cpp \
    src/angelscript/as_callfunc_x64_mingw.cpp \
    src/angelscript/as_callfunc_x64_msvc.cpp \
    src/angelscript/as_callfunc_x86.cpp \
    src/angelscript/as_callfunc_xenon.cpp \
    src/angelscript/as_compiler.cpp \
    src/angelscript/as_configgroup.cpp \
    src/angelscript/as_context.cpp \
    src/angelscript/as_datatype.cpp \
    src/angelscript/as_gc.cpp \
    src/angelscript/as_generic.cpp \
    src/angelscript/as_globalproperty.cpp \
    src/angelscript/as_memory.cpp \
    src/angelscript/as_module.cpp \
    src/angelscript/as_objecttype.cpp \
    src/angelscript/as_outputbuffer.cpp \
    src/angelscript/as_parser.cpp \
    src/angelscript/as_restore.cpp \
    src/angelscript/as_scriptcode.cpp \
    src/angelscript/as_scriptengine.cpp \
    src/angelscript/as_scriptfunction.cpp \
    src/angelscript/as_scriptnode.cpp \
    src/angelscript/as_scriptobject.cpp \
    src/angelscript/as_string.cpp \
    src/angelscript/as_string_util.cpp \
    src/angelscript/as_thread.cpp \
    src/angelscript/as_tokenizer.cpp \
    src/angelscript/as_typeinfo.cpp \
    src/angelscript/as_variablescope.cpp \
    src/angelscript/addons/scriptarray.cpp \
    src/angelscript/addons/scriptbuilder.cpp \
    src/angelscript/addons/scriptmath.cpp \
    src/angelscript/addons/scriptmathcomplex.cpp \
    src/angelscript/addons/scriptstdstring.cpp \
    src/angelscript/addons/scriptstdstring_utils.cpp \
    src/angelscript/addons/scripthandle.cpp \
    src/angelscript/addons/weakref.cpp \
    src/SDLWindow.cpp \
    src/SDLRenderer.cpp \
    src/SDLApplication.cpp \
    src/TestObject.cpp \
    src/SDLKeyboardManager.cpp \
    src/IKeyboardManager.cpp \
    src/SDLJoystickManager.cpp \
    src/SDLMouseManager.cpp \
    include/TileObj.cpp \
    src/SDLTexture.cpp \
    src/main.cpp \
    src/ByteCodeStream.cpp

HEADERS += \
    include/Global.hpp \
    include/ApplicationBase.hpp \
    include/ScriptEngine.hpp \
    include/angelscript/addons.h \
    include/angelscript/addons/weakref.h \
    include/ScriptResource.hpp \
    include/angelscript/addons/scripthandle.h \
    include/EnumToString.hpp \
    include/ComponentFactory.hpp \
    include/TransformableComponent.hpp \
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
    include/Vector2.hpp \
    include/ObjectManager.hpp \
    include/TestObject.hpp \
    include/IKeyboardManager.hpp \
    include/SDLKeyboardManager.hpp \
    include/IJoystickManager.hpp \
    include/SDLJoystickManager.hpp \
    include/SDLMouseManager.hpp \
    include/TileObj.hpp \
    include/IMouseManager.hpp \
    include/ResourceManager.hpp \
    include/IResource.hpp \
    include/ITextureResource.hpp \
    include/Rectangle.hpp \
    include/SDLTexture.hpp \
    include/physfsrwops.h \
    include/Console.hpp \
    include/angelscript/angelscript.h \
    include/angelscript/as_array.h \
    include/angelscript/as_atomic.h \
    include/angelscript/as_builder.h \
    include/angelscript/as_bytecode.h \
    include/angelscript/as_callfunc.h \
    include/angelscript/as_compiler.h \
    include/angelscript/as_config.h \
    include/angelscript/as_configgroup.h \
    include/angelscript/as_context.h \
    include/angelscript/as_criticalsection.h \
    include/angelscript/as_datatype.h \
    include/angelscript/as_debug.h \
    include/angelscript/as_gc.h \
    include/angelscript/as_generic.h \
    include/angelscript/as_map.h \
    include/angelscript/as_memory.h \
    include/angelscript/as_module.h \
    include/angelscript/as_namespace.h \
    include/angelscript/as_objecttype.h \
    include/angelscript/as_outputbuffer.h \
    include/angelscript/as_parser.h \
    include/angelscript/as_property.h \
    include/angelscript/as_restore.h \
    include/angelscript/as_scriptcode.h \
    include/angelscript/as_scriptengine.h \
    include/angelscript/as_scriptfunction.h \
    include/angelscript/as_scriptnode.h \
    include/angelscript/as_scriptobject.h \
    include/angelscript/as_string.h \
    include/angelscript/as_string_util.h \
    include/angelscript/as_symboltable.h \
    include/angelscript/as_texts.h \
    include/angelscript/as_thread.h \
    include/angelscript/as_tokendef.h \
    include/angelscript/as_tokenizer.h \
    include/angelscript/as_typeinfo.h \
    include/angelscript/as_variablescope.h \
    include/angelscript/addons/scriptarray.h \
    include/angelscript/addons/scriptbuilder.h \
    include/angelscript/addons/scriptmath.h \
    include/angelscript/addons/scriptmathcomplex.h \
    include/angelscript/addons/scriptstdstring.h \
    include/Config.hpp \
    include/CVarManager.hpp \
    include/CVar.hpp \
    include/ByteCodeStream.hpp \

OTHER_FILES += \
    src/angelscript/as_callfunc_arm_gcc.S \
    src/angelscript/as_callfunc_arm_xcode.S \
    src/angelscript/as_callfunc_arm_msvc.asm \
    src/angelscript/as_callfunc_x64_msvc_asm.asm \
    data/scripts/common.as \
    data/scripts/main.as \
    data/scripts/test.as

