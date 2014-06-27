#ifndef ORION_CONFIG_HPP
#define ORION_CONFIG_HPP

#ifndef TEXTURERESOURCE_IMPL
#include <SDLTexture.hpp>
#define TEXTURERESOURCE_IMPL SDLTextureResource
#endif

#ifndef APPLICATION_IMPL
#include <SDLApplication.hpp>
#define APPLICATION_IMPL SDLApplication
#endif

#ifndef WINDOW_IMPL
#include <SDLWindow.hpp>
#define WINDOW_IMPL SDLWindow
#endif

#ifndef KEYBOARDMANAGER_IMPL
#include <SDLKeyboardManager.hpp>
#define KEYBOARDMANAGER_IMPL SDLKeyboardManager
#endif

#endif // ORION_CONFIG_HPP
