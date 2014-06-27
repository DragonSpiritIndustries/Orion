#include "common.as"

void onInitialized()
{
    orConsole.print(Console::Message, "Application Initialized");
}

void onStart()
{
    orApplication.setTitle("Orion TechDemo INTERNAL PRE-ALPHA");
}

void onUpdate(float delta)
{
    if (orJoystickManager.buttonReleased(0, 16) || orKeyboardManager.keyReleased(Key::ESCAPE))
        orApplication.quit();
}

void onShutdown()
{
   orConsole.print(Console::Message, "Application Shutdown");
}

