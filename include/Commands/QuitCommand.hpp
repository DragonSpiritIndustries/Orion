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
