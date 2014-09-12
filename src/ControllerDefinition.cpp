#include "ControllerDefinition.hpp"
#include <physfs.h>
#include <tinyxml.h>

ControllerDefinition::ControllerDefinition()
{
}

ControllerDefinition* ControllerDefinition::loadDefinition(const std::string& controllerName)
{
    PHYSFS_file* file = PHYSFS_openRead(controllerName.c_str());
    if (file)
    {
        PHYSFS_sint64 fileLength = PHYSFS_fileLength(file);
    }
}
