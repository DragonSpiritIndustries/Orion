#ifndef ORION_CONFIG_HPP
#define ORION_CONFIG_HPP

#ifndef APPLICATION_IMPL
#define APPLICATION_IMPL_HEADER "SDLApplication.hpp"
#define APPLICATION_IMPL SDLApplication
#endif

#ifndef TEXTURERESOURCE_IMPL
#define TEXTURERESOURCE_IMPL_HEADER "SDLTexture.hpp"
#define TEXTURERESOURCE_IMPL SDLTextureResource
#endif

#ifndef WINDOW_IMPL
#define WINDOW_IMPL_HEADER "SDLWindow.hpp"
#define WINDOW_IMPL SDLWindow
#endif

#ifndef KEYBOARDMANAGER_IMPL
#define KEYBOARDMANAGER_IMPL_HEADER "SDLKeyboardManager.hpp"
#define KEYBOARDMANAGER_IMPL SDLKeyboardManager
#endif

#ifndef MAX_JOYSTICKS
#define MAX_JOYSTICKS 8
#endif

#endif // ORION_CONFIG_HPP
