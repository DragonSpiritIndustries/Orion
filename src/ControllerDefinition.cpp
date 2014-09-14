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
