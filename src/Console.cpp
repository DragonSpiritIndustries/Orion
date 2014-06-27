#include "Console.hpp"
#include "ApplicationBase.hpp"
#include "ScriptEngine.hpp"
#include "CVarManager.hpp"
#include <Athena/Utility.hpp>
#include <ctime>

Console::Console(const std::string& logfile)
    : m_startString(0)
{
    m_log.open(logfile, std::ios_base::out | std::ios_base::app);
}

void Console::initialize()
{
    orConsoleRef.print(orConsoleRef.Info, "Console initialized");
}

void Console::shutdown()
{
    orConsoleRef.print(orConsoleRef.Warning, "IMPLMENT ME: Console::shutdown");
}

bool Console::isInitialized() const
{
    return true;
}

bool Console::isOpen() const
{
    orConsoleRef.print(orConsoleRef.Warning, "IMPLMENT ME: Console::draw");
}

bool Console::isClosed() const
{
    orConsoleRef.print(orConsoleRef.Warning, "IMPLMENT ME: Console::isClosed");
}

void Console::handleText(int utf8)
{
    orConsoleRef.print(orConsoleRef.Warning, "IMPLMENT ME: Console::handleText");
}

void Console::handleInput(Key key, bool alt, bool control, bool shift, bool system)
{
    orConsoleRef.print(orConsoleRef.Warning, "IMPLMENT ME: Console::handleMouseWheel");
}

void Console::handleMouseWheel(int delta, int x, int y)
{
    orConsoleRef.print(orConsoleRef.Warning, "IMPLMENT ME: Console::draw");}

void Console::draw()
{
    orConsoleRef.print(orConsoleRef.Warning, "IMPLMENT ME: Console::draw");
}

void Console::print(Console::Level level, const std::string& fmt, ...)
{
    int size = 100;
    std::string str;

    va_list ap;
    while (1)
    {
        str.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf((char *)str.c_str(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size)
        {
            str.resize(n);
            break;
        }
        if (n > -1)
            size = n + 1;
        else
            size *= 2;
    }

    std::string label;
    std::string timestamp;
    std::time_t t = std::time(NULL);
    char mbstr[9];
    if (std::strftime(mbstr, sizeof(mbstr), "%H:%M:%S ", std::localtime(&t)))
        timestamp = std::string(mbstr);

    switch(level)
    {
        case Message:
            break;
        case Info:
            label = "[Info   ] ";
            break;
        case Warning:
            label = "[Warning] ";
            break;
        case Error:
            label = "[Error  ] ";
            break;
        case Fatal:
            label = "[FATAL  ] ";
            break;
    }
    std::vector<std::string> entries = Athena::utility::split(str, '\n');

    for (std::string string : entries)
    {
        if (string == std::string())
            break;

        if (string.size() >= MAX_LEN)
        {
            bool hasSpaces = true;
            std::vector<std::string> parts = Athena::utility::split(string, ' ');
            if (parts.size() <= 1)
            {
                parts.clear();
                hasSpaces = false;
                for (char c : string)
                {
                    std::string tmp;
                    tmp.push_back(c);
                    parts.push_back(tmp);
                }
            }

            std::string merged = label;
            int mergeLen = 0;
            for (std::string part : parts)
            {
                if (part == std::string() || part.find(' ') == 0)
                    continue;

                mergeLen = merged.length();
                if (mergeLen >= MAX_LEN)
                {
                    addEntry(level, merged, timestamp);
                    merged = label;
                }

                merged += (hasSpaces ? std::string(" ") : std::string()) + part;
            }

            // Since compare returns 0 when both strings match, we can assume that there is more in merged
            if (merged.compare(label))
                addEntry(level, merged, timestamp);
        }
        else
        {
            addEntry(level, string, timestamp, label);
        }
    }

    //    if (level == Console::Fatal)
    //    {
    //        m_hadFatalError = true;
    //        m_state = Opened;
    //        m_conHeight = com_windowHeight->toInteger();
    //        m_conY = m_conHeight;
    //        recalcMaxLines();
    //    }
}

void Console::clear()
{
}

void Console::toggleConsole()
{
}

Colorb Console::consoleColor()
{
}

Colorb Console::textColor()
{
}

Console& Console::instanceRef()
{
    return *Console::instancePtr();
}

Console* Console::instancePtr()
{
    static std::shared_ptr<Console> instance = std::shared_ptr<Console>(new Console);
    return instance.get();
}

void Console::doAutoComplete()
{
}

void Console::drawHistory()
{
}

void Console::drawSeparator()
{
}

void Console::drawVersion()
{
}

void Console::parseCommand()
{
}

void Console::parseCVars()
{
}

void Console::addEntry(const Console::Level level, const std::string& message, const std::string& timestamp, const std::string label)
{
    LogEntry entry;
    entry.level = level;
    entry.message = label + message;
    entry.timestamp = timestamp;
    m_history.push_back(entry);
    m_log << timestamp << entry.message << std::endl;

    // Increment the start string to prevent
    // the console from scrolling
    if (m_startString > 0)
        m_startString++;

    // print to console just incase there is no font
    // and the user can't read it.
    std::cout << timestamp << entry.message << std::endl;
}

void registerConsole()
{
    asIScriptEngine* engine = orScriptEngineRef.handle();
    int r;
    r = engine->RegisterObjectType("Console", 0, asOBJ_REF | asOBJ_NOHANDLE);
    r = engine->RegisterObjectMethod("Console", "void print(int level, const string& in)", asMETHOD(Console, print), asCALL_THISCALL);
    r = engine->RegisterObjectMethod("Console", "bool isOpen()", asMETHOD(Console, isOpen), asCALL_THISCALL);
    r = engine->RegisterGlobalProperty("Console orConsole", orConsolePtr);


    orConsoleRef.print(Console::Info, "Registered Console");
}

REGISTER_SCRIPT_FUNCTION(Console, registerConsole);
