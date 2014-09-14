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

#define ORION_DEVELOPMENT 1

#if ORION_DEVELOPMENT != 1
#ifndef APPLICATION_IMPL
#error APPLICATION_IMPL_HEADER not set
#error APPLICATION_IMPL not set
#endif

#ifndef WINDOW_IMPL
#error WINDOW_IMPL_HEADER not set
#error WINDOW_IMPL SDLWindow
#endif

#ifndef RENDERER_IMPL
#error RENDERER_IMPL_HEADER not set
#error RENDERER_IMPL not set
#endif

#ifndef KEYBOARDMANAGER_IMPL
#error KEYBOARDMANAGER_IMPL_HEADER not set
#error KEYBOARDMANAGER_IMPL not set
#endif

#ifndef JOYSTICKMANAGER_IMPL
#error JOYSTICKMANAGER_IMPL_HEADER not set
#error JOYSTICKMANAGER_IMPL not set
#endif

#ifndef MAX_JOYSTICKS
#error MAX_JOYSTICKS  not set
#endif

#ifndef MOUSEMANAGER_IMPL
#error MOUSEMANAGER_IMPL_HEADER not set
#error MOUSEMANAGER_IMPL not set
#endif

#ifndef TEXTURERESOURCE_IMPL
#define TEXTURERESOURCE_IMPL_HEADER not set
#define TEXTURERESOURCE_IMPL not set
#endif

#ifndef FONTRESOURCE_IMPL
#error FONTRESOURCE_IMPL_HEADER not set
#error FONTRESOURCE_IMPL not set
#endif

#else

// Set these to some dummy development files
// This is so Orion compiles fine by itself,
// without having to rely on specific implementaions
#ifndef APPLICATION_IMPL
#define APPLICATION_IMPL_HEADER "DummyApplication.hpp"
#define APPLICATION_IMPL DummyApplication
#endif

#ifndef WINDOW_IMPL
#define WINDOW_IMPL_HEADER "DummyWindow.hpp"
#define WINDOW_IMPL DummyWindow
#endif

#ifndef RENDERER_IMPL
#define RENDERER_IMPL_HEADER "DummyRenderer.hpp"
#define RENDERER_IMPL DummyRenderer
#endif

#ifndef KEYBOARDMANAGER_IMPL
#define KEYBOARDMANAGER_IMPL_HEADER "DummyKeyboardManager.hpp"
#define KEYBOARDMANAGER_IMPL DummyKeyboardManager
#endif

#ifndef JOYSTICKMANAGER_IMPL
#define JOYSTICKMANAGER_IMPL_HEADER "DummyJoystickManager.hpp"
#define JOYSTICKMANAGER_IMPL DummyJoystickManager
#endif

#ifndef MAX_JOYSTICKS
#define MAX_JOYSTICKS  0
#endif

#ifndef MOUSEMANAGER_IMPL
#define MOUSEMANAGER_IMPL_HEADER "DummyMouseManager.hpp"
#define MOUSEMANAGER_IMPL DummyMouseManager
#endif

#ifndef TEXTURERESOURCE_IMPL
#define TEXTURERESOURCE_IMPL_HEADER "DummyTextureResource.hpp"
#define TEXTURERESOURCE_IMPL DummyTextureResource
#endif

#ifndef FONTRESOURCE_IMPL
#define FONTRESOURCE_IMPL_HEADER "DummyFontResource.hpp"
#define FONTRESOURCE_IMPL DummyFontResource
#endif

#endif // ORION_DEVELOPMENT

#endif // ORION_CONFIG_HPP
