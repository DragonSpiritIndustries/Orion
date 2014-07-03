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
