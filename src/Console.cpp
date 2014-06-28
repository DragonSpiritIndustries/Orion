#include "Console.hpp"
#include "ApplicationBase.hpp"
#include "ScriptEngine.hpp"
#include "ITextureResource.hpp"
#include "CVarManager.hpp"
#include "CVar.hpp"
#include <Athena/Utility.hpp>
#include <ctime>

extern CVar* com_windowWidth;
extern CVar* com_windowHeight;
CVar* con_key = new CVar("con_key", CVar::Binding(Key::TILDE), "Toggles the console", CVar::Archive | CVar::System);
CVar* com_player1Controller = new CVar("com_player1Controller", "0", "Specifies the first player controller, The first player has sole access to the debug console", CVar::Integer,
                                       CVar::Archive | CVar::System);
CVar* con_height     = new CVar("con_height", "234", "Console Height", CVar::Integer, CVar::System);
CVar* con_speed      = new CVar("con_speed", "500.f", "Specifies how fast the console opens or closes", CVar::Float, CVar::System);
CVar* con_color      = new CVar("con_color", Colorb::white, "Console color", CVar::System);
CVar* con_textcolor  = new CVar("con_textcolor", Colorb::white, "Console text color", CVar::System);
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
      m_maxLines(15),
      m_conHeight(234),
      m_currentMaxLen(MAX_LEN),
      m_conY(0.0f)
{
    m_log.open(logfile, std::ios_base::out | std::ios_base::app);
    orApplicationRef.updateSignal().connect  <Console, &Console::onUpdate>(this);
    orApplicationRef.textSignal().connect    <Console, &Console::handleText>(this);
    orApplicationRef.keyboardSignal().connect<Console, &Console::handleInput>(this);
}

void Console::initialize()
{
    orConsoleRef.print(orConsoleRef.Info, "Console initialized");
    m_conBg1 = orResourceManagerRef.loadResource<ITextureResource>("console/bg1.png");
    if (m_conBg1)
    {
        m_conBg1->setWrapH(true);
        m_conBg1->setWrapV(true);
    }
    m_conBg2 = orResourceManagerRef.loadResource<ITextureResource>("console/bg2.png");
    if (m_conBg2)
    {
        m_conBg2->setWrapH(true);
        m_conBg2->setWrapV(true);
    }
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

void Console::handleMouseWheel(int delta, int x, int y)
{
    orConsoleRef.print(orConsoleRef.Warning, "IMPLMENT ME: Console::draw");}

void Console::draw()
{
    if (m_conBg1)
        m_conBg1->draw(Rectanglef(0, 0, com_windowWidth->toInteger(), con_height->toInteger()), Rectanglef(1, 0, 64, 64), Vector2f(), false, false, 0.0f);
//    if (m_conBg2)
//        m_conBg2->draw(0, 0);
    orApplicationRef.drawRectangle(com_windowWidth->toInteger(), con_height->toInteger(), 0, 0, false);
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
    {
        m_state = Opening;
    }
    else
    {
        m_state = Closing;
    }

    m_commandString = "";
    m_cursorPosition = 0;
    m_cursorX = 0;
    m_currentCommand = 0;

    if (m_hadFatalError)
        orApplicationRef.close();
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

void Console::onUpdate(float)
{
    const int player1 = com_player1Controller->toInteger();
    if (orJoystickManagerRef.buttonReleased(player1, con_key->toBinding().Joysticks[player1].Button))
        toggleConsole();
}

void Console::handleText(const Event& e)
{
    if (e.eventData.textEvent.string[0] == '`' || e.eventData.textEvent.string[0] == '~' || m_state != Opened)
        return;

    m_commandString += e.eventData.textEvent.string;
    print(Info, "CommandString: %s", m_commandString.c_str());
}

void Console::handleInput(const Event& event)
{
    KeyboardEvent kbEvent = event.eventData.keyboardEvent;
    if (kbEvent.scanCode == con_key->toBinding().KeyVal && event.type == Event::EV_KEY_PRESSED)
        toggleConsole();
    if (kbEvent.scanCode == Key::BACKSPACE && event.type == Event::EV_KEY_PRESSED)
    {
        if (m_commandString.size() > 0)
            m_commandString.erase(m_commandString.end() - 1);
        print(Info, "CommandString: %s", m_commandString.c_str());
    }
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
    int r;
    r = engine->RegisterObjectType    ("Console", 0, asOBJ_REF | asOBJ_NOHANDLE);
    r = engine->RegisterObjectMethod  ("Console", "void print(int level, const string& in)", asMETHOD(Console, print), asCALL_THISCALL);
    r = engine->RegisterObjectMethod  ("Console", "bool isOpen()", asMETHOD(Console, isOpen), asCALL_THISCALL);
    r = engine->RegisterGlobalProperty("Console orConsole", orConsolePtr);

    orConsoleRef.print(Console::Info, "Registered Console");
}

REGISTER_SCRIPT_FUNCTION(Console, registerConsole);
