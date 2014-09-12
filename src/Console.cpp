#include "Console.hpp"
#include "ApplicationBase.hpp"
#include "ScriptEngine.hpp"
#include "ITextureResource.hpp"
#include "IFontResource.hpp"
#include "CVarManager.hpp"
#include "CVar.hpp"
#include "IConsoleCommand.hpp"
#include <Athena/Utility.hpp>
#include <ctime>

extern CVar* com_windowWidth;
extern CVar* com_windowHeight;
extern CVar* com_developer;
CVar* con_key = new CVar("con_key", CVar::Binding(Key::TILDE), "Toggles the console", CVar::Archive | CVar::System);
CVar* com_player1Controller = new CVar("com_player1Controller", "0", "Specifies the first player controller, The first player has sole access to the debug console", CVar::Integer,
                                       CVar::Archive | CVar::System);
CVar* con_height     = new CVar("con_height", "242", "Console Height", CVar::Integer, CVar::System);
CVar* con_speed      = new CVar("con_speed", "500.f", "Specifies how fast the console opens or closes", CVar::Float, CVar::System);
CVar* con_color      = new CVar("con_color", Colorb(0, 12, 64, 240), "Console color", CVar::System);
CVar* con_textcolor  = new CVar("con_textcolor", Colorb(128, 128, 128), "Console text color", CVar::System);
CVar* con_sndopen    = new CVar("con_sndopen", "sounds/con_open.wav", "Console opening sound effect", CVar::Literal, CVar::System);
CVar* con_sndclose   = new CVar("con_sndopen", "sounds/con_close.wav", "Console opening sound effect", CVar::Literal, CVar::System);

Console::Console(const std::string& logfile)
    : m_state(Closed),
      m_showCursor(false),
      m_cursorPosition(0),
      m_startString(0),
      m_currentCommand(0),
      m_isInitialized(false),
      m_overwrite(false),
      m_fullscreen(false),
      m_maxLines(10),
      m_conHeight(con_height->toInteger()),
      m_currentMaxLen(MAX_LEN),
      m_conY(-con_height->toInteger())
{
    m_log.open(logfile, std::ios_base::out | std::ios_base::app);
    orApplicationRef.updateSignal().connect  <Console, &Console::onUpdate>   (this);
    orApplicationRef.textSignal().connect    <Console, &Console::handleText> (this);
    orApplicationRef.keyboardSignal().connect<Console, &Console::handleInput>(this);
    orApplicationRef.mouseWheelSignal().connect<Console, &Console::handleMouseWheel>(this);
}

void Console::initialize()
{
    orConsoleRef.print(orConsoleRef.Info, "Console initialized");
    m_conBg1 = orResourceManagerRef.loadResource<ITextureResource>("console/bg1.png");
    if (m_conBg1)
        m_conBg1->setRepeat(true);
    m_conBg2 = orResourceManagerRef.loadResource<ITextureResource>("console/bg2.png");
    if (m_conBg2)
        m_conBg2->setRepeat(true);

    m_font = orResourceManagerRef.loadResource<IFontResource>("fonts/debug.ttf");
    if (!m_font)
        print(Fatal, "Unable to load fonts/debug.ttf");
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
    return m_state != Closed;
}

bool Console::isClosed() const
{
    return m_state == Closed;
}

void Console::handleMouseWheel(const Event& ev)
{
    if (m_state != Opened)
        return;

    MouseWheelEvent wheelEv = ev.eventData.mouseWheelEvent;

    Rectanglef bounds(0, 0, com_windowWidth->toInteger(), m_conHeight - 20);
    if (bounds.contains(wheelEv.x, wheelEv.y))
    {
        m_startString += wheelEv.vDelta * 4;
        if (m_startString < 0)
            m_startString = 0;
        else if (m_startString > (int)(m_history.size() - 1) - m_maxLines)
            m_startString = (m_history.size() - 1) - m_maxLines;
        return;
    }
    bounds = Rectanglef(0, m_conHeight - 20, com_windowWidth->toInteger(), 20);
    if (bounds.contains(wheelEv.x, wheelEv.y))
    {
        if (m_commandHistory.size() == 0)
            return;
        int oldCommand = m_currentCommand;
        m_currentCommand += wheelEv.vDelta;

        if (m_currentCommand > (int)(m_commandHistory.size() - 1))
            m_currentCommand = m_commandHistory.size() - 1;
        if (m_currentCommand < 0)
        {
            m_currentCommand = -1;
            if (oldCommand >= 0)
            {
                m_commandString.clear();
                m_cursorPosition = 0;
            }
            return;
        }

        m_commandString = m_commandHistory.at(m_currentCommand);
        m_cursorPosition = m_commandString.size();
    }
}

void Console::draw()
{
    if (m_state == Closed)
        return;

    float glyphW =  2.f;
    if (m_commandString.size() > 0)
        glyphW = m_font->glyphAdvance(m_commandString[m_cursorPosition]) + 2;
    orApplicationRef.drawRectangle(com_windowWidth->toInteger(), m_conHeight, 0, m_conY, true, con_color->toColorb());
    orApplicationRef.drawRectangle(com_windowWidth->toInteger(), m_conHeight, 0, m_conY, false);
    orApplicationRef.drawRectangle(com_windowWidth->toInteger(), 20, 0, m_conY + m_conHeight - 20, false);
    drawHistory();
    if (m_showCursor)
        orApplicationRef.drawRectangle(2,
                                       16, glyphW + m_cursorX + 6, m_conY + m_conHeight - 18);
    m_font->draw(2, m_conY + m_conHeight - 20, "]" + m_commandString);
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
    char mbstr[100];
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

    if (level == Console::Fatal)
    {
        m_hadFatalError = true;
        m_state = Opened;
        m_conHeight = com_windowHeight->toInteger();
        m_conY = m_conHeight;
        //recalcMaxLines();
    }
}

void Console::clear()
{
}

void Console::toggleConsole()
{
    if (m_state == Closed || m_state == Closing)
        m_state = Opening;
    else
        m_state = Closing;

    m_commandString = "";
    m_cursorPosition = 0;
    m_cursorX = 0;
    m_currentCommand = 0;
    m_conHeight = (m_fullscreen ? com_windowHeight->toInteger() : con_height->toInteger());
    if (m_hadFatalError)
        orApplicationRef.close();
}

Colorb Console::consoleColor()
{
}

Colorb Console::textColor()
{
}

void Console::registerCommand(IConsoleCommand* command)
{
    std::string name = command->name();
    Athena::utility::tolower(name);
    if (m_commands.find(name) != m_commands.end())
    {
        delete command;
        return;
    }

    m_commands[name] = command;
}

void Console::exec(const std::string& command)
{
    // only except commands when console is closed
    if (m_state != Closed)
        return;

    m_commandString = command;
    parseCommand();
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

void Console::onUpdate(float delta)
{
    if (m_state == Closed)
        return;

    m_cursorTime += 1.f*delta;
    if (m_state == Closing)
    {
        if (m_fullscreen || m_wasFullscreen)
            m_conY = -(m_conHeight + 1); // Offset by one to trigger further code
        else
            m_conY -= con_speed->toFloat()*delta;
    }
    else if (m_state == Opening)
    {
        if (m_fullscreen)
            m_conY = 1; // Offset by one to trigger further code
        else
            m_conY += con_speed->toFloat()*delta;
    }

    m_maxLines = (m_conHeight - 42) / 20;
    if (m_conY < -m_conHeight)
    {
        m_conY = -m_conHeight;
        m_state = Closed;
    }
    else if (m_conY > 0)
    {
        m_conY = 0;
        m_state = Opened;
    }

    if (m_cursorTime > .5f)
    {
        m_showCursor ^= 1;
        m_cursorTime = 0;
    }

    m_bgOffX += 1.f*delta;
    m_bgOffY += 1.f*delta;

    m_cursorX = 0;
    for (int i = 0; i < m_cursorPosition; ++i)
        m_cursorX += m_font->glyphAdvance(m_commandString.at(i));
}

void Console::handleText(const Event& e)
{
    if (e.eventData.textEvent.string[0] == '`' || e.eventData.textEvent.string[0] == '~' || m_state != Opened)
        return;

    m_commandString += e.eventData.textEvent.string;
    m_cursorPosition++;
}

void Console::handleInput(const Event& event)
{
    KeyboardEvent kbEvent = event.eventData.keyboardEvent;
    if (kbEvent.scanCode == con_key->toBinding().KeyVal && event.type == Event::EV_KEY_PRESSED)
    {
        if (kbEvent.modifier & (int)KeyModifier::ALT)
        {
            m_wasFullscreen = m_fullscreen;
            m_fullscreen = true;
        }
        else
        {
            m_wasFullscreen = m_fullscreen;
            m_fullscreen = false;
        }
        toggleConsole();
    }

    const int player1 = com_player1Controller->toInteger();
    if (orJoystickManagerRef.buttonReleased(player1, con_key->toBinding().Joysticks[player1].Button))
        toggleConsole();

    if (m_state != Opened)
        return;

    if (event.type == Event::EV_KEY_PRESSED)
    {
        switch (kbEvent.scanCode)
        {
            case Key::BACKSPACE:
            {
                if (m_commandString.size() > 0)
                {
                    if (kbEvent.modifier & (int)KeyModifier::CONTROL)
                    {
                        int index = m_commandString.rfind(' ', m_cursorPosition - 1);

                        if (index == (int)std::string::npos)
                        {
                            m_commandString.clear();
                            m_cursorPosition = 0;
                        }
                        else
                        {
                            m_commandString.erase(index, (index - m_commandString.size()));
                            m_cursorPosition = index;
                        }
                        break;
                    }
                    m_commandString.erase(m_cursorPosition--, 1);
                }
            }
                break;
            case Key::DELETE:
            {
                if (m_commandString.size() > 0)
                {
                    // Don't try to delete if the cursor is at the end of the line
                    if (m_cursorPosition > (int)m_commandString.size())
                        break;
                    m_commandString.erase(m_cursorPosition  + 1, 1);
                }
            }
                break;
            case Key::PAGEUP:
            {
                if (m_startString < (int)(m_history.size() - m_maxLines) - 1)
                    m_startString++;
            }
                break;
            case Key::PAGEDOWN:
            {
                if (m_startString > 0)
                    m_startString--;
            }
                break;
            case Key::RETURN:
                parseCommand();
                break;
            case Key::LEFT:
            {
                if (m_cursorPosition < 0)
                    break;

                m_cursorPosition--;
                m_showCursor = true;
                m_cursorTime = 0;
            }
                break;
            case Key::RIGHT:
            {
                if (m_cursorPosition > (int)m_commandString.size() - 1)
                    break;
                //                if (m_cursorPosition >= (int)currentMaxLen())
                //                    break;

                m_cursorPosition++;
                m_showCursor = true;
                m_cursorTime = 0;
            }
                break;
            case Key::UP:
            {
                if (m_commandHistory.size() == 0)
                    break;

                m_currentCommand++;

                if (m_currentCommand > (int)m_commandHistory.size() - 1)
                    m_currentCommand = (int)m_commandHistory.size() - 1;

                m_commandString = m_commandHistory[m_currentCommand];
                m_cursorPosition = m_commandString.size();
            }
                break;
            case Key::DOWN:
            {
                if (m_commandHistory.size() == 0)
                    break;
                m_currentCommand--;
                if (m_currentCommand >= 0)
                {
                    m_commandString = m_commandHistory[m_currentCommand];
                }
                else if (m_currentCommand <= -1)
                {
                    m_currentCommand = -1;
                    m_commandString.clear();
                }
                m_cursorPosition = m_commandString.size();
            }
                break;
            default:
                break;
        }
    }

    if (m_cursorPosition > (int)m_commandString.size() - 1)
        m_cursorPosition = (int)m_commandString.size() - 1;
    if (m_cursorPosition < 0)
        m_cursorPosition = 0;

    if (m_startString > (int)m_history.size() - 1)
        m_startString = (int)m_history.size() - 1;
    if (m_startString < 0)
        m_startString = 0;
}

void Console::doAutoComplete()
{
}

void Console::drawHistory()
{
    int posY = (m_conY + m_conHeight) - 42;
    std::vector<LogEntry>::reverse_iterator iter = m_history.rbegin() + m_startString;

    int line = 0;
    for (; iter != m_history.rend(); ++iter)
    {
        if (line > m_maxLines)
            break;

        orApplicationRef.drawDebugText("> "  + ((LogEntry)*iter).message, Vector2f(2, posY), con_textcolor->toColorb());
        posY -= 20;
        line++;
    }
}

void Console::drawSeparator()
{
}

void Console::drawVersion()
{

}

void Console::parseCommand()
{
    m_currentCommand = 0;

    if (m_commandString.empty())
        return;

    m_commandHistory.emplace(m_commandHistory.begin(), m_commandString);

    if (m_commandHistory.size() > 5)
        m_commandHistory.erase(m_commandHistory.end());

    print(Message, m_commandString);

    std::vector<std::string> commands = Athena::utility::split(m_commandString, ';');

    orForeach(std::string cmd : commands)
    {
        std::string tmpCmd(cmd);

        if (tmpCmd[0] == ' ')
            tmpCmd.erase(tmpCmd.begin(), tmpCmd.begin() + 1);

        std::vector<std::string> args = Athena::utility::split(tmpCmd, ' ');

        if (args.size() > 0)
        {
            std::string command = args[0];

            Athena::utility::tolower(command);

            args.erase(args.begin());

            if (!command.compare("list"))
            {
                std::string secondary = args[0];
                Athena::utility::tolower(secondary);
                if (!secondary.compare("cmds") || !secondary.compare("all"))
                {
                    for (std::pair<std::string, IConsoleCommand*> cmd : m_commands)
                        print(Message, "%s: %s", cmd.first.c_str(), cmd.second->usage().c_str());

                    print(Message, "set");
                }

                if (!secondary.compare("cvars") || !secondary.compare("all"))
                {
                    std::string tertiary;
                    if (args.size() >= 2)
                        tertiary = args[1];

                    Athena::utility::tolower(tertiary);

                    for (CVar* cvar : orCVarManagerRef.cvars())
                    {
                        if (cvar->isHidden() && !com_developer->toBoolean())
                            continue;

                        if ((tertiary.compare("readonly") && tertiary.compare("ro")) && cvar->isReadOnly())
                            continue;

                        print(Message, "%s: %s", cvar->name().c_str(), cvar->help().c_str());
                    }
                }

                resetCursor();
                return;
            }

            if (m_commands.find(command) != m_commands.end())
                m_commands[command]->execute(args);
            else
                parseCVars(command, args);
        }
    }

    resetCursor();
}

void Console::parseCVars(const std::string& command, std::vector<std::string> args)
{
    if (!command.compare("set"))
    {
        if (args.size() == 0)
        {
            resetCursor();
            return;
        }

        std::string setting = std::string(args[0]);
        Athena::utility::tolower(setting);
        CVar* tmp = orCVarManagerRef.findCVar(setting);
        if (tmp == nullptr)
        {
            resetCursor();
            return;
        }

        // Not a command?
        // It's probably a setting
        if (args.size() >= 2)
        {
            args.erase(args.begin());
            std::stringstream ss;
            switch(tmp->type())
            {
                case CVar::Boolean:
                    tmp->fromBoolean(Athena::utility::parseBool(std::string(args[0])));
                    break;
                case CVar::Integer:
                {
                    int val;
                    ss << args[0];
                    ss >> val;
                    tmp->fromInteger(val);
                }
                    break;
                case CVar::Float:
                {
                    float val;
                    ss << args[1];
                    ss >> val;
                    tmp->fromFloat(val);
                }
                    break;
                case CVar::Literal:
                {
                    std::string val = "";
                    for (std::string s : args)
                        val += s + " ";
                    if (val.find_last_of(" ") != std::string::npos)
                        val.erase(val.find_last_of(" "));

                    std::cout << val << std::endl;

                    tmp->fromLiteral(val);
                }
                    break;
                case CVar::Color:
                {
                    int r, g, b, a;
                    std::stringstream ss;
                    int i = 0;
                    for (; i < (int)args.size(); i++)
                        ss << args[i] << " ";

                    while ((i++) < 3)
                        ss << 0;

                    ss >> r >> g >> b >> a;

                    tmp->fromColorb(Colorb(r, g, b, a));
                }
                    break;
                default: break;
            }
        }
    }
    else
    {
        CVar* tmp = orCVarManagerRef.findCVar(command);
        if (tmp == nullptr)
        {
            resetCursor();
            return;
        }

        print(Info, "%s -> %s", tmp->name().c_str(), tmp->help().c_str());

        switch(tmp->type())
        {
            case CVar::Boolean:
                print(Info, "Current: %i", tmp->toBoolean());
                break;
            case CVar::Integer:
                print(Info, "Current: %i", tmp->toInteger());
                break;
            case CVar::Float:
                print(Info, "Current: %f", tmp->toFloat());
                break;
            case CVar::Literal:
                print(Info, "Current: %s", tmp->toLiteral().c_str());
                break;
            case CVar::Color:
            {
                Colori color = tmp->toColori();
                print(Info, "Current: %i %i %i %i", color.r, color.g, color.b, color.a);

            }
                break;
            default: break;
        }
    }
}

void Console::resetCursor()
{
    m_commandString.clear();
    m_cursorX = 0.0f;
    m_cursorPosition = 0;
    m_currentCommand = -1;
}

void Console::addEntry(const Console::Level level, const std::string& message, const std::string& timestamp, const std::string& label)
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
    engine->SetDefaultNamespace("Console");
    engine->RegisterEnum("Level");
    engine->RegisterEnumValue("Level", "Message", Console::Message);
    engine->RegisterEnumValue("Level", "Info",    Console::Info);
    engine->RegisterEnumValue("Level", "Warning", Console::Warning);
    engine->RegisterEnumValue("Level", "Error",   Console::Error);
    engine->RegisterEnumValue("Level", "Fatal",   Console::Fatal);
    engine->RegisterObjectType("Binding", sizeof(CVar::Binding), asOBJ_VALUE | asOBJ_POD);
    engine->SetDefaultNamespace("");
    int r;
    r = engine->RegisterObjectType    ("Console", 0, asOBJ_REF | asOBJ_NOHANDLE);
    r = engine->RegisterObjectMethod  ("Console", "void print(int level, const string& in)", asMETHOD(Console, print), asCALL_THISCALL);
    r = engine->RegisterObjectMethod  ("Console", "bool isOpen()", asMETHOD(Console, isOpen), asCALL_THISCALL);
    r = engine->RegisterGlobalProperty("Console orConsole", orConsolePtr);

    orConsoleRef.print(Console::Info, "Registered Console");
}

REGISTER_SCRIPT_FUNCTION(Console, registerConsole);
