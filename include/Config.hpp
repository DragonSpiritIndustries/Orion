// This file is part of Orion.
//
// Orion is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Orion is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Orion.  If not, see <http://www.gnu.org/licenses/>

#ifndef ORION_CONFIG_HPP
#define ORION_CONFIG_HPP

#ifndef APPLICATION_IMPL
#define APPLICATION_IMPL_HEADER "SDLApplication.hpp"
#define APPLICATION_IMPL SDLApplication
#endif

#ifndef WINDOW_IMPL
#define WINDOW_IMPL_HEADER "SDLWindow.hpp"
#define WINDOW_IMPL SDLWindow
#endif

#ifndef RENDERER_IMPL
#define RENDERER_IMPL_HEADER "SDLRenderer.hpp"
#define RENDERER_IMPL SDLRenderer
#endif

#ifndef KEYBOARDMANAGER_IMPL
#define KEYBOARDMANAGER_IMPL_HEADER "SDLKeyboardManager.hpp"
#define KEYBOARDMANAGER_IMPL SDLKeyboardManager
#endif

#ifndef JOYSTICKMANAGER_IMPL
#define JOYSTICKMANAGER_IMPL_HEADER "SDLJoystickManager.hpp"
#define JOYSTICKMANAGER_IMPL SDLJoystickManager
#endif

#ifndef MAX_JOYSTICKS
#define MAX_JOYSTICKS 8
#endif

#ifndef MOUSEMANAGER_IMPL
#define MOUSEMANAGER_IMPL_HEADER "SDLMouseManager.hpp"
#define MOUSEMANAGER_IMPL SDLMouseManager
#endif

#ifndef TEXTURERESOURCE_IMPL
#define TEXTURERESOURCE_IMPL_HEADER "SDLTextureResource.hpp"
#define TEXTURERESOURCE_IMPL SDLTextureResource
#endif

#ifndef FONTRESOURCE_IMPL
#define FONTRESOURCE_IMPL_HEADER "SDLFontResource.hpp"
#define FONTRESOURCE_IMPL SDLFontResource
#endif

#endif // ORION_CONFIG_HPP
