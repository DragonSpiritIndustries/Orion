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

#ifndef QUITCOMMAND_HPP
#define QUITCOMMAND_HPP

#include "IConsoleCommand.hpp"
class QuitCommand : public IConsoleCommand
{
public:
    QuitCommand();
    std::string usage() const;
    void execute(std::vector<std::string> args);
};

#endif // QUITCOMMAND_HPP
