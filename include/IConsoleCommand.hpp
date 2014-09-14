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

#ifndef ICONSOLECOMMAND_HPP
#define ICONSOLECOMMAND_HPP

#include <vector>
#include <string>

class IConsoleCommand
{
public:
    IConsoleCommand(const std::string& name)
        : m_name(name)
    {
    }

    /*!
     * \brief ~IConsoleCommand
     */
    virtual ~IConsoleCommand(){}
    /*!
     * \brief usage
     */
    virtual std::string usage() const= 0;
    /*!
     * \brief execute
     * \param args
     */
    virtual void execute(std::vector<std::string> args) = 0;

    virtual std::string name() const {return m_name;}
private:
    std::string m_name;
    std::string m_help;
};

#endif // ICONSOLECOMMAND_HPP
