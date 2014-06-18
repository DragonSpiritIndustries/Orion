#include <iostream>
#include <SDLApplication.hpp>
#include <ObjectManager.hpp>
#include <TestObject.hpp>
#include <memory>

void testDraw1(IApplication* app)
{
    app->drawRectangle(32, 32, 100, 200);
}

void testDraw2(IApplication* app)
{
    app->drawRectangle(32, 64, 16, 200, true);
}

void resizeEvent(Event resize)
{
    std::cout << resize.eventData.resizeEvent.width << "x" << resize.eventData.resizeEvent.height << std::endl;
}

void keyboardEvent(Event key)
{
    std::cout << std::hex << (int)key.eventData.keyboardEvent.keyCode << " " << (int)key.eventData.keyboardEvent.scanCode << " Modifier Flags = " << std::hex << (int)key.eventData.keyboardEvent.modifier << std::dec << std::endl;
}

void joyAxisEvent(Event joy)
{
    if (joy.type == Event::EV_JOY_AXIS)
        std::cout << "Joy " << joy.eventData.joystickEvent.id << " " << (int)joy.eventData.joystickEvent.axis << " " << joy.eventData.joystickEvent.position << std::endl;
    else
        std::cout << "Joy " << joy.eventData.joystickEvent.id << " " << (int)joy.eventData.joystickEvent.button << std::endl;
}

void textEvent(Event text)
{
    std::cout << text.eventData.textEvent.string << std::endl;
}

void focusEvent(Event focus)
{
    std::cout << focus.eventData.focusEvent.focus << std::endl;
}

void mouseButtonEvent(Event button)
{
    std::cout << button.eventData.mouseButtonEvent.x << " " << button.eventData.mouseButtonEvent.y  << " "
              << (int)button.eventData.mouseButtonEvent.button
              << std::endl;
}

void mouseWheelEvent(Event wheel)
{
    std::cout << wheel.eventData.mouseWheelEvent.x      << " " << wheel.eventData.mouseWheelEvent.y  << " "
              << wheel.eventData.mouseWheelEvent.hDelta << " " << wheel.eventData.mouseWheelEvent.vDelta
              << std::endl;
}

void mouseMoveEvent(Event move)
{
    std::cout << move.eventData.mouseMoveEvent.x << " " << move.eventData.mouseMoveEvent.y << std::endl;
}
int main(int argc, char* argv[])
{
    orCreateApplication(new SDLApplication);
    orApplicationPtr->init(argc, argv);
    orApplicationPtr->setClearColor(Colorb::cyan);
    //orApplicationRef.setTitle("SDLTest");
//    orApplicationPtr->drawSignal().connect<testDraw1>();
//    orApplicationRef.drawSignal().connect<testDraw2>();
//    orApplicationRef.resizeSignal().connect<resizeEvent>();
    orApplicationRef.keyboardSignal().connect<keyboardEvent>();
//    orApplicationRef.joystickSignal().connect<joyAxisEvent>();
//    orApplicationRef.textSignal().connect<textEvent>();
//    orApplicationRef.focusChangedSignal().connect<focusEvent>();
//    orApplicationRef.mouseButtonSignal().connect<mouseButtonEvent>();
//    orApplicationRef.mouseWheelSignal().connect<mouseWheelEvent>();
//    orApplicationRef.mouseMoveSignal().connect<mouseMoveEvent>();
    orObjectManagerPtr->addObject(new TestObject);
    return IApplication::instanceRef().exec();
}
