#ifndef CONTROLLERDEFINITION_HPP
#define CONTROLLERDEFINITION_HPP
#include <string>

class ControllerDefinition
{
public:
    struct ButtonInfo
    {
        std::string name;
        std::string image;
        int         button;
    };

    ControllerDefinition();

    static ControllerDefinition* loadDefinition(const std::string& controllerName);
};

#endif // CONTROLLERDEFINITION_HPP
