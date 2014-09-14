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

#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include "Color.hpp"
#include "IKeyboardManager.hpp"
#include <vector>
#include <map>
#include <fstream>

class ITextureResource;
class IFontResource;
class ApplicationBase;
class IConsoleCommand;
class Console
{
public:
    static const char ColorChar = '^';
    /*!
     * \enum State
     * \brief The State enumerator is used to determine the Console's current state.
     */
    enum State
    {
        Opened,  //!< The Console is open and accepting input
        Closed,  //!< The Console is closed and not drawn
        Opening, //!< The Console is in the process of opening, but not accepting input
        Closing  //!< The Console is in the process of closing, and is still drawn
    };

    /*!
     * \brief The Level enum
     */
    enum Level
    {
        Message, //!< The lowest level, it is not prepended to the text, usually for chat text, or simple messages that don't need attention
        Info,    //!< Usually information about resources, may also contain information relevant to the games state.
        Warning, //!< Usually used when the engine can't find a resource, which isn't usually a big deal
        Error,   //!< Used when the engine encounters a non-fatal error, like a packet drop
        Fatal    //!< Used when the engine encounters a fatal error, like a required resource isn't available, e.g fonts/debug.ttf
    };
    enum
    {
        MAX_LEN = 92 //! The maximum length the command text can be.
    };

    /*!
     * \brief The LogEntry struct
     */
    struct LogEntry
    {
        /*!
         * \brief level
         */
        Level level;
        /*!
         * \brief timestamp
         */
        std::string timestamp;
        /*!
         * \brief message
         */
        std::string message;
    };

    /*!
     * \brief Console
     * \param logfile
     */
    Console(const std::string& logfile="log.txt");

    virtual void initialize();
    virtual void shutdown();
    virtual bool isInitialized() const;
    virtual bool isOpen() const;
    virtual bool isClosed() const;
    virtual void draw();
    virtual void print(Level level, const std::string &fmt, ...);
    virtual void clear();
    virtual void toggleConsole();
    virtual Colorb consoleColor();
    virtual Colorb textColor();

    virtual void registerCommand(IConsoleCommand* command);
    virtual void exec(const std::string& command);
    static Console& instanceRef();
    static Console* instancePtr();
protected:
    void recalcMaxLines();
    virtual void onUpdate(float);
    virtual void handleText(const Event&);
    virtual void handleInput(const Event&);
    virtual void handleMouseWheel(const Event& ev);
    virtual void doAutoComplete();
    virtual void drawHistory();
    virtual void drawSeparator();
    virtual void drawVersion();
    virtual void parseCommand();
    virtual void parseCVars(const std::string& command, std::vector<std::string> args);
    virtual void resetCursor();
    virtual void addEntry(const Level level, const std::string& message, const std::string& timestamp, const std::string& label = std::string());
    ITextureResource*     m_conBg1;
    ITextureResource*     m_conBg2;
    IFontResource*        m_font;
    std::ofstream         m_log;
    std::vector<LogEntry> m_history;
    std::vector<std::string> m_commandHistory;
    std::map<std::string, IConsoleCommand*> m_commands;
    State                 m_state;
    bool                  m_showCursor;
    int                   m_cursorPosition;
    int                   m_startString;
    int                   m_currentCommand;
    bool                  m_isInitialized;
    bool                  m_overwrite;
    bool                  m_hadFatalError;
    bool                  m_fullscreen;
    bool                  m_wasFullscreen;
    int                   m_maxLines;
    int                   m_conHeight;
    int                   m_conWidth;
    int                   m_currentMaxLen;
    int                   m_commandStart;
    std::string           m_commandString;

    float                 m_bgOffX;
    float                 m_bgOffY;
    float                 m_cursorX;
    float                 m_conY;
    float                 m_cursorTime;
};


#define orConsoleRef     Console::instanceRef()
#define orConsolePtr     Console::instancePtr()

#ifndef REGISTER_COMMAND
#define REGISTER_COMMAND(Class) \
struct hidden_commandRegistration##Class \
{ \
    hidden_commandRegistration##Class() \
    { \
        orConsoleRef.registerCommand(new Class); \
    }\
};\
static hidden_commandRegistration##Class hidden_commandRegistration##Class
#endif

#endif // CONSOLE_HPP
