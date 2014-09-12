#include "include/Commands/QuitCommand.hpp"
#include "ApplicationBase.hpp"
#include <Athena/Utility.hpp>

QuitCommand::QuitCommand()
    : IConsoleCommand("Quit")
{
}

std::string QuitCommand::usage() const
{
    return "Quits the game";
}

void QuitCommand::execute(std::vector<std::string> args)
{
    UNUSED(args);
    orApplicationRef.close();
}


REGISTER_COMMAND(QuitCommand);
